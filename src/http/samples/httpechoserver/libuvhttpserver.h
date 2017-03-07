#include <signal.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#ifdef __unix__
#include <unistd.h> // _SC_NPROCESSORS_ONLN on OS X
#endif
#include "uv.h"
#include "http_parser.h"

#include <string>
#include <sstream>
#include <string>
#include <iostream>


#ifdef DEBUG
#define CHECK(status, msg) \
  if (status != 0) { \
    fprintf(stderr, "%s: %s\n", msg, uv_err_name(status)); \
    exit(1); \
  }
#define UVERR(err, msg) fprintf(stderr, "%s: %s\n", msg, uv_err_name(err))
#define LOG_ERROR(msg) puts(msg);
#define LOG(msg) puts(msg);
#define LOGF(...) printf(__VA_ARGS__);
#else
#define CHECK(status, msg)
#define UVERR(err, msg)
#define LOG_ERROR(msg)
#define LOG(msg)
#define LOGF(...)
#endif


namespace scy {
namespace libuv {


static int request_num = 1;
static uv_loop_t* uv_loop;
static uv_tcp_t server;
static http_parser_settings parser_settings;

struct client_t {
    uv_tcp_t handle;
    http_parser parser;
    uv_write_t write_req;
    int request_num;
    std::string path;
};

void on_close(uv_handle_t* handle) {
    client_t* client = (client_t*)handle->data;
    LOGF("[ %5d ] connection closed\n\n", client->request_num);
    delete client;
}

void alloc_cb(uv_handle_t * /*handle*/, size_t suggested_size, uv_buf_t* buf) {
    *buf = uv_buf_init((char*)malloc(suggested_size), suggested_size);
}

void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t * buf) {
    ssize_t parsed;
    LOGF("on read: %ld\n", nread);
    client_t* client = (client_t*)tcp->data;
    if (nread >= 0) {
        parsed = (ssize_t)http_parser_execute(
            &client->parser, &parser_settings, buf->base, nread);
        if (client->parser.upgrade) {
            LOG_ERROR("parse error: cannot handle http upgrade");
            uv_close((uv_handle_t*)&client->handle, on_close);
        }
        else if (parsed < nread) {
            LOG_ERROR("parse error");
            uv_close((uv_handle_t*)&client->handle, on_close);
        }
    }
    else {
        if (nread != UV_EOF) {
            UVERR(nread, "read");
        }
        uv_close((uv_handle_t*)&client->handle, on_close);
    }
    free(buf->base);
}

struct render_baton {
    render_baton(client_t * _client) :
        client(_client),
        request(),
        result(),
        response_code("200 OK"),
        content_type("text/plain"),
        error(false) {
        request.data = this;
    }
    client_t* client;
    uv_work_t request;
    std::string result;
    std::string response_code;
    std::string content_type;
    bool error;
};

void after_write(uv_write_t* req, int status) {
    CHECK(status, "write");
    if (!uv_is_closing((uv_handle_t*)req->handle))
    {
        render_baton *closure = static_cast<render_baton *>(req->data);
        delete closure;
        uv_close((uv_handle_t*)req->handle, on_close);
    }
}

bool endswith(std::string const& value, std::string const& search)
{
    if (value.length() >= search.length()) {
        return (0 == value.compare(value.length() - search.length(), search.length(), search));
    }
    else {
        return false;
    }
}

void render(uv_work_t* req) {
    render_baton *closure = static_cast<render_baton *>(req->data);
    client_t* client = (client_t*)closure->client;
    LOGF("[ %5d ] render\n", client->request_num);

    //closure->result = "hello universe";
    closure->response_code = "200 OK";

#if 0
    std::string filepath(".");
    filepath += client->path;
    std::string index_path = (filepath + "index.html");
    bool has_index = (access(index_path.c_str(), R_OK) != -1);
    if (/*!has_index &&*/ filepath[filepath.size() - 1] == '/') {
       uv_fs_t scandir_req;
       int r = uv_fs_scandir(uv_loop, &scandir_req, filepath.c_str(), 0, NULL);
       uv_dirent_t dent;
       closure->content_type = "text/html";
       closure->result = "<html><body><ul>";
       while (UV_EOF != uv_fs_scandir_next(&scandir_req, &dent)) {
           std::string name = dent.name;
           if (dent.type == UV_DIRENT_DIR) {
               name += "/";
           }
           closure->result += "<li><a href='";
           closure->result += name;
           closure->result += "'>";
           closure->result += name;
           closure->result += "</a></li>";
           closure->result += "\n";
       }
       closure->result += "</ul></body></html>";
       uv_fs_req_cleanup(&scandir_req);
    }
    else {
       std::string file_to_open = filepath;
       if (has_index) {
           file_to_open = index_path;
       }
       bool exists = (access(file_to_open.c_str(), R_OK) != -1);
       if (!exists) {
           closure->result = "no access";
           closure->response_code = "404 Not Found";
           return;
       }
       FILE * f = fopen(file_to_open.c_str(), "rb");
       if (f) {
           std::fseek(f, 0, SEEK_END);
           unsigned size = std::ftell(f);
           std::fseek(f, 0, SEEK_SET);
           closure->result.resize(size);
           std::fread(&closure->result[0], size, 1, f);
           fclose(f);
           if (endswith(file_to_open, "html")) {
               closure->content_type = "text/html";
           }
           else if (endswith(file_to_open, "css")) {
               closure->content_type = "text/css";
           }
           else if (endswith(file_to_open, "js")) {
               closure->content_type = "application/javascript";
           }
       }
       else {
           closure->result = "failed to open";
           closure->response_code = "404 Not Found";
       }
    }
#endif
}

void after_render(uv_work_t* req) {
    render_baton *closure = static_cast<render_baton *>(req->data);
    client_t* client = (client_t*)closure->client;

    LOGF("[ %5d ] after render\n", client->request_num);

    std::ostringstream rep;
    rep << "HTTP/1.1 " << closure->response_code << "\r\n"
        << "Content-Type: " << closure->content_type << "\r\n"
        //<< "Connection: keep-alive\r\n"
        << "Connection: close\r\n"
        << "Content-Length: " << closure->result.size() << "\r\n"
        << "Access-Control-Allow-Origin: *" << "\r\n"
        << "\r\n";
    rep << closure->result;
    std::string res = rep.str();
    uv_buf_t resbuf;
    resbuf.base = (char *)res.c_str();
    resbuf.len = res.size();

    client->write_req.data = closure;

    // https://github.com/joyent/libuv/issues/344
    int r = uv_write(&client->write_req,
        (uv_stream_t*)&client->handle,
        &resbuf, 1, after_write);
    CHECK(r, "write buff");
}

int on_message_begin(http_parser* /*parser*/) {
    LOGF("\n***MESSAGE BEGIN***\n");
    return 0;
}

int on_headers_complete(http_parser* /*parser*/) {
    LOGF("\n***HEADERS COMPLETE***\n");
    return 0;
}

int on_url(http_parser* parser, const char* url, size_t length) {
    client_t* client = (client_t*)parser->data;
    LOGF("[ %5d ] on_url\n", client->request_num);
    LOGF("Url: %.*s\n", (int)length, url);
    // TODO - use https://github.com/bnoordhuis/uriparser2 instead?
    struct http_parser_url u;
    int result = http_parser_parse_url(url, length, 0, &u);
    if (result) {
        fprintf(stderr, "\n\n*** failed to parse URL %s ***\n\n", url);
        return -1;
    }
    else {
        if ((u.field_set & (1 << UF_PATH))) {
            const char * data = url + u.field_data[UF_PATH].off;
            client->path = std::string(data, u.field_data[UF_PATH].len);
        }
        else {
            fprintf(stderr, "\n\n*** failed to parse PATH in URL %s ***\n\n", url);
            return -1;
        }
    }
    return 0;
}

int on_header_field(http_parser* /*parser*/, const char* at, size_t length) {
    LOGF("Header field: %.*s\n", (int)length, at);
    return 0;
}

int on_header_value(http_parser* /*parser*/, const char* at, size_t length) {
    LOGF("Header value: %.*s\n", (int)length, at);
    return 0;
}

int on_body(http_parser* /*parser*/, const char* at, size_t length) {
    LOGF("Body: %.*s\n", (int)length, at);
    return 0;
}

int on_message_complete(http_parser* parser) {
    client_t* client = (client_t*)parser->data;
    LOGF("[ %5d ] on_message_complete\n", client->request_num);
    render_baton *closure = new render_baton(client);
    int status = uv_queue_work(uv_loop,
        &closure->request,
        render,
        (uv_after_work_cb)after_render);
    CHECK(status, "uv_queue_work");
    assert(status == 0);

    return 0;
}

void on_connect(uv_stream_t* server_handle, int status) {
    CHECK(status, "connect");
    assert((uv_tcp_t*)server_handle == &server);

    client_t* client = new client_t();
    client->request_num = request_num;
    request_num++;

    LOGF("[ %5d ] new connection\n", request_num);

    uv_tcp_init(uv_loop, &client->handle);
    http_parser_init(&client->parser, HTTP_REQUEST);

    client->parser.data = client;
    client->handle.data = client;

    int r = uv_accept(server_handle, (uv_stream_t*)&client->handle);
    CHECK(r, "accept");

    uv_read_start((uv_stream_t*)&client->handle, alloc_cb, on_read);
}

#define MAX_WRITE_HANDLES 1000

void raiseBenchmarkServer() {
    parser_settings.on_url = on_url;
    // notification callbacks
    parser_settings.on_message_begin = on_message_begin;
    parser_settings.on_headers_complete = on_headers_complete;
    parser_settings.on_message_complete = on_message_complete;
    // data callbacks
    parser_settings.on_header_field = on_header_field;
    parser_settings.on_header_value = on_header_value;
    parser_settings.on_body = on_body;
    uv_loop = uv_default_loop();
    int r = uv_tcp_init(uv_loop, &server);
    CHECK(r, "tcp_init");
    r = uv_tcp_keepalive(&server, 1, 60);
    CHECK(r, "tcp_keepalive");
    struct sockaddr_in address;
    r = uv_ip4_addr("0.0.0.0", 1337, &address);
    CHECK(r, "ip4_addr");
    r = uv_tcp_bind(&server, (const struct sockaddr*)&address, 0);
    CHECK(r, "tcp_bind");
    r = uv_listen((uv_stream_t*)&server, MAX_WRITE_HANDLES, on_connect);
    CHECK(r, "uv_listen");
    LOG("listening on port 1337");
    uv_run(uv_loop, UV_RUN_DEFAULT);
}


} } // namespace scy::libuv
