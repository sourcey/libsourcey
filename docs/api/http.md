{#httpmodule}

# http

HTTP client/server, WebSocket support, form handling, cookies, URL parsing.

### Namespaces

| Name | Description |
|------|-------------|
| [`http`](#http) | HTTP request/response types, parsers, and server/client helpers. |
| [`ws`](#ws) | [WebSocket](#websocket) framing, handshakes, and connection helpers. |

{#http}

# http

HTTP request/response types, parsers, and server/client helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Authenticator`](#authenticator) | Maintains HTTP Basic or Digest authentication state for outbound requests. |
| [`BasicAuthenticator`](#basicauthenticator) | Encodes and decodes HTTP Basic authentication credentials. |
| [`ChunkedAdapter`](#chunkedadapter) | HTTP chunked transfer encoding adapter for streaming responses. |
| [`Client`](#client) | HTTP client for creating and managing outgoing connections. |
| [`ClientConnection`](#clientconnection-1) | HTTP client connection for managing request/response lifecycle. |
| [`Connection`](#connection-1) | Base HTTP connection managing socket I/O and message lifecycle |
| [`ConnectionAdapter`](#connectionadapter-1) | Default HTTP socket adapter for reading and writing HTTP messages |
| [`ConnectionPool`](#connectionpool) | LIFO connection pool for reusing [ServerConnection](#serverconnection-1) objects. Avoids per-request heap allocation by resetting and reusing connections instead of destroying and recreating them. |
| [`ConnectionStream`](#connectionstream-1) | Packet stream wrapper for an HTTP connection. |
| [`Cookie`](#cookie) | HTTP cookie value plus its response/header attributes. |
| [`FilePart`](#filepart) | Form part backed by a file on disk. |
| [`FormPart`](#formpart) | An implementation of [FormPart](#formpart). |
| [`FormWriter`](#formwriter) | [FormWriter](#formwriter) is an HTTP client connection adapter for writing HTML forms. |
| [`Message`](#message) | The base class for [Request](#request-4) and [Response](#response-1). |
| [`MultipartAdapter`](#multipartadapter) | HTTP multipart encoding adapter for multipart/x-mixed-replace streaming. |
| [`Parser`](#parser-1) | HTTP request/response parser using the llhttp library. |
| [`ParserObserver`](#parserobserver) | Abstract observer interface for HTTP parser events. |
| [`ProgressSignal`](#progresssignal) | HTTP progress signal for upload and download progress notifications. |
| [`Request`](#request-4) | HTTP request message with method, URI, headers, and optional body. |
| [`Response`](#response-1) | HTTP response message with status, reason phrase, headers, and body metadata. |
| [`Server`](#server) | HTTP server implementation. |
| [`ServerConnection`](#serverconnection-1) | HTTP server connection. |
| [`ServerConnectionFactory`](#serverconnectionfactory) | Factory for creating per-socket `[ServerConnection](#serverconnection-1)` and per-request `[ServerResponder](#serverresponder)` objects. |
| [`ServerResponder`](#serverresponder) | Base responder interface for handling one HTTP request on a server connection. Derived classes typically override `[onRequest()](#onrequest)` and optionally the streaming hooks. |
| [`StringPart`](#stringpart-1) | Form part backed by an in-memory string payload. |
| [`URL`](#url) | An RFC 3986 based [URL](#url) parser. Constructors and assignment operators will throw a SyntaxException if the [URL](#url) is invalid. |
| [`DateCache`](#datecache) | Caches the formatted Date header, updated once per second. Avoids per-request time formatting and string allocation. |
| [`Method`](#method) | HTTP request methods. |
| [`StaticFileInfo`](#staticfileinfo) | Metadata needed to serve a static file with HTTP validators. |

### Enumerations

| Name | Description |
|------|-------------|
| [`StatusCode`](#statuscode)  | HTTP [Response](#response-1) Status Codes. |
| [`ServerConnectionState`](#serverconnectionstate)  | HTTP server-side lifecycle phases used by the keep-alive state machine. |
| [`ServerConnectionMode`](#serverconnectionmode)  | Transport mode for server connections before and after protocol upgrade. |

---

{#statuscode}

#### StatusCode

```cpp
enum StatusCode
```

HTTP [Response](#response-1) Status Codes.

| Value | Description |
|-------|-------------|
| `Continue` |  |
| `SwitchingProtocols` |  |
| `OK` |  |
| `Created` |  |
| `Accepted` |  |
| `NonAuthoritative` |  |
| `NoContent` |  |
| `ResetContent` |  |
| `PartialContent` |  |
| `MultipleChoices` |  |
| `MovedPermanently` |  |
| `Found` |  |
| `SeeOther` |  |
| `NotModified` |  |
| `UseProxy` |  |
| `TemporaryRedirect` |  |
| `BadRequest` |  |
| `Unauthorized` |  |
| `PaymentRequired` |  |
| `Forbidden` |  |
| `NotFound` |  |
| `MethodNotAllowed` |  |
| `NotAcceptable` |  |
| `ProxyAuthRequired` |  |
| `RequestTimeout` |  |
| `Conflict` |  |
| `Gone` |  |
| `LengthRequired` |  |
| `PreconditionFailed` |  |
| `EntityTooLarge` |  |
| `UriTooLong` |  |
| `UnsupportedMediaType` |  |
| `RangeNotSatisfiable` |  |
| `ExpectationFailed` |  |
| `UnprocessableEntity` |  |
| `UpgradeRequired` |  |
| `InternalServerError` |  |
| `NotImplemented` |  |
| `BadGateway` |  |
| `Unavailable` |  |
| `GatewayTimeout` |  |
| `VersionNotSupported` |  |

---

{#serverconnectionstate}

#### ServerConnectionState

```cpp
enum ServerConnectionState
```

HTTP server-side lifecycle phases used by the keep-alive state machine.

| Value | Description |
|-------|-------------|
| `ReceivingHeaders` | Parsing request headers. |
| `ReceivingBody` | Receiving request body bytes. |
| `DispatchingOrSending` | Dispatching the responder or sending a normal response. |
| `Streaming` | Sending a long-lived streaming response. |
| `Upgraded` | Upgraded to a non-HTTP protocol such as WebSocket. |
| `Closing` | Close has been requested and teardown is in progress. |
| `Closed` | [Connection](#connection-1) has fully closed. |

---

{#serverconnectionmode}

#### ServerConnectionMode

```cpp
enum ServerConnectionMode
```

Transport mode for server connections before and after protocol upgrade.

| Value | Description |
|-------|-------------|
| `Http` | Standard HTTP request/response mode. |
| `Upgraded` | Upgraded transport mode owned by another protocol adapter. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< ClientConnection::Ptr >` | [`ClientConnectionPtrVec`](#clientconnectionptrvec)  | List of owned client connections tracked by an HTTP client. |

---

{#clientconnectionptrvec}

#### ClientConnectionPtrVec

```cpp
using ClientConnectionPtrVec = std::vector< ClientConnection::Ptr >
```

List of owned client connections tracked by an HTTP client.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`isBasicCredentials`](#isbasiccredentials) `nodiscard` | Returns true if the given Authorization header value uses HTTP Basic authentication. |
| `bool` | [`isDigestCredentials`](#isdigestcredentials) `nodiscard` | Returns true if the given Authorization header value uses HTTP Digest authentication. |
| `bool` | [`hasBasicCredentials`](#hasbasiccredentials) `nodiscard` | Returns true if the request contains a Basic Authorization header. |
| `bool` | [`hasDigestCredentials`](#hasdigestcredentials) `nodiscard` | Returns true if the request contains a Digest Authorization header. |
| `bool` | [`hasProxyBasicCredentials`](#hasproxybasiccredentials) `nodiscard` | Returns true if the request contains a Basic Proxy-Authorization header. |
| `bool` | [`hasProxyDigestCredentials`](#hasproxydigestcredentials) `nodiscard` | Returns true if the request contains a Digest Proxy-Authorization header. |
| `void` | [`extractCredentials`](#extractcredentials)  | Splits a "user:password" user-info string into separate username and password strings. If no ':' is present, the entire string is treated as the username and password is empty. |
| `void` | [`extractCredentials`](#extractcredentials-1)  | Extracts username and password from the user-info component of a [URL](#url). Does nothing if the [URL](#url) has no user-info part. |
| `ClientConnection::Ptr` | [`createConnectionT`](#createconnectiont) `inline` | Creates a [ClientConnection](#clientconnection-1) (or subtype) for the given [URL](#url) without registering it with a [Client](#client) instance. The socket and adapter are chosen based on the [URL](#url) scheme: |
| `ClientConnection::Ptr` | [`createConnection`](#createconnection) `inline` | Creates a [ClientConnection](#clientconnection-1) for the given [URL](#url) and optionally registers it with a [Client](#client). Equivalent to calling [Client::createConnection()](#createconnection-1) when `client` is non-null. |
| `const char *` | [`getStatusCodeReason`](#getstatuscodereason) `nodiscard` | Returns the standard reason phrase for the given HTTP status code (e.g. "OK" for [StatusCode::OK](#namespaceicy_1_1http_1aa73f8ae30b4882be20ce0a7e16adc1a4ae0aa021e21dddbd6d8cecec71e9cf564), "Not Found" for [StatusCode::NotFound](#namespaceicy_1_1http_1aa73f8ae30b4882be20ce0a7e16adc1a4a38c300f4fc9ce8a77aad4a30de05cad8)). |
| `const char *` | [`getStatusCodeString`](#getstatuscodestring) `nodiscard` | Returns a combined "NNN Reason" string for the given HTTP status code (e.g. "200 OK"). |
| `bool` | [`statStaticFile`](#statstaticfile)  | Read static-file metadata from disk. |
| `bool` | [`prepareStaticFileResponse`](#preparestaticfileresponse)  | Apply static-file headers and request conditionals to the response. |
| `std::string` | [`parseURI`](#parseuri)  | Extracts the URI (path and query) from a raw HTTP request line. |
| `bool` | [`matchURL`](#matchurl)  | Tests whether a URI matches a glob-style expression. |
| `std::string` | [`parseCookieItem`](#parsecookieitem)  | Extracts a named attribute from a [Cookie](#cookie) header value. |
| `bool` | [`splitURIParameters`](#splituriparameters)  | Parses [URL](#url) query parameters from a URI into key-value pairs. Handles percent-decoding of names and values. |
| `void` | [`splitParameters`](#splitparameters)  | Splits a header-style parameter string into a primary value and named attributes. Attributes are separated by ';'. Enclosing quotes are stripped from values. |
| `void` | [`splitParameters`](#splitparameters-1)  | Splits a substring (defined by iterators) into named attributes. Attributes are separated by ';'. Enclosing quotes are stripped from values. |

---

{#isbasiccredentials}

#### isBasicCredentials

`nodiscard`

```cpp
[[nodiscard]] bool isBasicCredentials(std::string_view header)
```

Returns true if the given Authorization header value uses HTTP Basic authentication. 
#### Parameters
* `header` Value of the Authorization or WWW-Authenticate header.

---

{#isdigestcredentials}

#### isDigestCredentials

`nodiscard`

```cpp
[[nodiscard]] bool isDigestCredentials(std::string_view header)
```

Returns true if the given Authorization header value uses HTTP Digest authentication. 
#### Parameters
* `header` Value of the Authorization or WWW-Authenticate header.

---

{#hasbasiccredentials}

#### hasBasicCredentials

`nodiscard`

```cpp
[[nodiscard]] bool hasBasicCredentials(const http::Request & request)
```

Returns true if the request contains a Basic Authorization header. 
#### Parameters
* `request` HTTP request to inspect.

---

{#hasdigestcredentials}

#### hasDigestCredentials

`nodiscard`

```cpp
[[nodiscard]] bool hasDigestCredentials(const http::Request & request)
```

Returns true if the request contains a Digest Authorization header. 
#### Parameters
* `request` HTTP request to inspect.

---

{#hasproxybasiccredentials}

#### hasProxyBasicCredentials

`nodiscard`

```cpp
[[nodiscard]] bool hasProxyBasicCredentials(const http::Request & request)
```

Returns true if the request contains a Basic Proxy-Authorization header. 
#### Parameters
* `request` HTTP request to inspect.

---

{#hasproxydigestcredentials}

#### hasProxyDigestCredentials

`nodiscard`

```cpp
[[nodiscard]] bool hasProxyDigestCredentials(const http::Request & request)
```

Returns true if the request contains a Digest Proxy-Authorization header. 
#### Parameters
* `request` HTTP request to inspect.

---

{#extractcredentials}

#### extractCredentials

```cpp
void extractCredentials(std::string_view userInfo, std::string & username, std::string & password)
```

Splits a "user:password" user-info string into separate username and password strings. If no ':' is present, the entire string is treated as the username and password is empty. 
#### Parameters
* `userInfo` Input string in the form "user:password". 

* `username` Receives the extracted username. 

* `password` Receives the extracted password.

---

{#extractcredentials-1}

#### extractCredentials

```cpp
void extractCredentials(const http::URL & uri, std::string & username, std::string & password)
```

Extracts username and password from the user-info component of a [URL](#url). Does nothing if the [URL](#url) has no user-info part. 
#### Parameters
* `uri` [URL](#url) to extract credentials from. 

* `username` Receives the extracted username. 

* `password` Receives the extracted password.

---

{#createconnectiont}

#### createConnectionT

`inline`

```cpp
template<class ConnectionT> inline ClientConnection::Ptr createConnectionT(const URL & url, uv::Loop * loop = uv::defaultLoop())
```

Creates a [ClientConnection](#clientconnection-1) (or subtype) for the given [URL](#url) without registering it with a [Client](#client) instance. The socket and adapter are chosen based on the [URL](#url) scheme:

* "http" -> TCPSocket

* "https" -> SSLSocket

* "ws" -> TCPSocket + WebSocket adapter

* "wss" -> SSLSocket + WebSocket adapter

#### Parameters
* `ConnectionT` Concrete connection type derived from [ClientConnection](#clientconnection-1). 

#### Parameters
* `url` Target [URL](#url). Must have a recognised scheme. 

* `loop` Event loop to use. Defaults to the default libuv loop. 

#### Returns
Shared pointer to the created connection. 

#### Exceptions
* `std::runtime_error` if the [URL](#url) scheme is not recognised.

---

{#createconnection}

#### createConnection

`inline`

```cpp
inline ClientConnection::Ptr createConnection(const URL & url, http::Client * client = nullptr, uv::Loop * loop = uv::defaultLoop())
```

Creates a [ClientConnection](#clientconnection-1) for the given [URL](#url) and optionally registers it with a [Client](#client). Equivalent to calling [Client::createConnection()](#createconnection-1) when `client` is non-null. 
#### Parameters
* `url` Target [URL](#url). The scheme determines the socket and adapter type. 

* `client` Optional [Client](#client) instance to register the connection with. 

* `loop` Event loop to use. Defaults to the default libuv loop. 

#### Returns
Shared pointer to the created connection.

---

{#getstatuscodereason}

#### getStatusCodeReason

`nodiscard`

```cpp
[[nodiscard]] const char * getStatusCodeReason(StatusCode status)
```

Returns the standard reason phrase for the given HTTP status code (e.g. "OK" for [StatusCode::OK](#namespaceicy_1_1http_1aa73f8ae30b4882be20ce0a7e16adc1a4ae0aa021e21dddbd6d8cecec71e9cf564), "Not Found" for [StatusCode::NotFound](#namespaceicy_1_1http_1aa73f8ae30b4882be20ce0a7e16adc1a4a38c300f4fc9ce8a77aad4a30de05cad8)). 
#### Parameters
* `status` HTTP status code. 

#### Returns
Null-terminated reason phrase string.

---

{#getstatuscodestring}

#### getStatusCodeString

`nodiscard`

```cpp
[[nodiscard]] const char * getStatusCodeString(StatusCode status)
```

Returns a combined "NNN Reason" string for the given HTTP status code (e.g. "200 OK"). 
#### Parameters
* `status` HTTP status code. 

#### Returns
Null-terminated status code string.

---

{#statstaticfile}

#### statStaticFile

```cpp
bool statStaticFile(std::string_view path, StaticFileInfo & info)
```

Read static-file metadata from disk. 
#### Parameters
* `path` File path on disk. 

* `info` Receives the file size, weak ETag, and HTTP-normalized last-modified time. 

#### Returns
True if the path exists and is a regular file.

---

{#preparestaticfileresponse}

#### prepareStaticFileResponse

```cpp
bool prepareStaticFileResponse(const Request & request, Response & response, const StaticFileInfo & info)
```

Apply static-file headers and request conditionals to the response.

Sets `Content-Length`, `ETag`, and `Last-Modified` from `info`, then evaluates `If-None-Match` and `If-Modified-Since` for the current request.

#### Parameters
* `request` Incoming HTTP request. 

* `response` Outgoing HTTP response to update. 

* `info` Precomputed static-file metadata. 

#### Returns
True if the response should not send a body (`304` or `412`).

---

{#parseuri}

#### parseURI

```cpp
std::string parseURI(std::string_view request)
```

Extracts the URI (path and query) from a raw HTTP request line. 
#### Parameters
* `request` Raw HTTP request line (e.g. "GET /path?q=1 HTTP/1.1"). 

#### Returns
The URI portion (e.g. "/path?q=1").

---

{#matchurl}

#### matchURL

```cpp
bool matchURL(std::string_view uri, std::string_view expression)
```

Tests whether a URI matches a glob-style expression. 
#### Parameters
* `uri` The URI to test. 

* `expression` Pattern to match against. '*' matches any sequence of characters. 

#### Returns
true if the URI matches the expression.

---

{#parsecookieitem}

#### parseCookieItem

```cpp
std::string parseCookieItem(std::string_view cookie, std::string_view item)
```

Extracts a named attribute from a [Cookie](#cookie) header value. 
#### Parameters
* `cookie` Full [Cookie](#cookie) header value (e.g. "name=value; Path=/; HttpOnly"). 

* `item` Attribute name to find (e.g. "Path"). 

#### Returns
The value of the named attribute, or an empty string if not found.

---

{#splituriparameters}

#### splitURIParameters

```cpp
bool splitURIParameters(std::string_view uri, NVCollection & out)
```

Parses [URL](#url) query parameters from a URI into key-value pairs. Handles percent-decoding of names and values. 
#### Parameters
* `uri` URI string optionally containing a '?' query component. 

* `out` Collection to populate with parsed parameters. 

#### Returns
true if at least one parameter was parsed; false otherwise.

---

{#splitparameters}

#### splitParameters

```cpp
void splitParameters(const std::string & s, std::string & value, NVCollection & parameters)
```

Splits a header-style parameter string into a primary value and named attributes. Attributes are separated by ';'. Enclosing quotes are stripped from values.

Example input: "multipart/mixed; boundary=\"boundary-01234567\"" Output value: "multipart/mixed", parameters: { "boundary" -> "boundary-01234567" }

#### Parameters
* `s` Input string to split. 

* `value` Receives the primary value before the first ';'. 

* `parameters` Receives the parsed attribute key-value pairs.

---

{#splitparameters-1}

#### splitParameters

```cpp
void splitParameters(const std::string::const_iterator & begin, const std::string::const_iterator & end, NVCollection & parameters)
```

Splits a substring (defined by iterators) into named attributes. Attributes are separated by ';'. Enclosing quotes are stripped from values. 
#### Parameters
* `begin` Iterator to the start of the string to parse. 

* `end` Iterator past the end of the string to parse. 

* `parameters` Receives the parsed attribute key-value pairs.

{#authenticator}

## Authenticator

```cpp
#include <icy/http/authenticator.h>
```

```cpp
class Authenticator
```

Defined in src/http/include/icy/http/authenticator.h:32

Maintains HTTP Basic or Digest authentication state for outbound requests.

Note: Do not forget to read the entire response stream from the 401 response before sending the authenticated request, otherwise there may be problems if a persistent connection is used.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Authenticator`](#authenticator-1)  | Creates an empty authenticator. |
|  | [`Authenticator`](#authenticator-2)  | Creates an authenticator with the given username and password. |
|  | [`~Authenticator`](#authenticator-3)  | Destroys the authenticator. |
| `void` | [`fromUserInfo`](#fromuserinfo)  | Parses username:password std::string and sets username and password of the credentials object. Throws SyntaxException on invalid user information. |
| `void` | [`fromURI`](#fromuri)  | Extracts username and password from the given URI and sets username and password of the credentials object. Does nothing if URI has no user info part. |
| `void` | [`setUsername`](#setusername)  | Sets the username. |
| `const std::string &` | [`username`](#username) `const` `nodiscard` | Returns the username. |
| `void` | [`setPassword`](#setpassword)  | Sets the password. |
| `const std::string &` | [`password`](#password) `const` `nodiscard` | Returns the password. |
| `void` | [`authenticate`](#authenticate)  | Inspects WWW-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given http::Request. |
| `void` | [`updateAuthInfo`](#updateauthinfo)  | Updates internal state (in case of digest authentication) and replaces authentication information in the request accordingly. |
| `void` | [`proxyAuthenticate`](#proxyauthenticate)  | Inspects Proxy-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given http::Request. |
| `void` | [`updateProxyAuthInfo`](#updateproxyauthinfo)  | Updates internal state (in case of digest authentication) and replaces proxy authentication information in the request accordingly. |

---

{#authenticator-1}

#### Authenticator

```cpp
Authenticator()
```

Defined in src/http/include/icy/http/authenticator.h:36

Creates an empty authenticator.

---

{#authenticator-2}

#### Authenticator

```cpp
Authenticator(const std::string & username, const std::string & password)
```

Defined in src/http/include/icy/http/authenticator.h:39

Creates an authenticator with the given username and password.

---

{#authenticator-3}

#### ~Authenticator

```cpp
~Authenticator()
```

Defined in src/http/include/icy/http/authenticator.h:42

Destroys the authenticator.

---

{#fromuserinfo}

#### fromUserInfo

```cpp
void fromUserInfo(std::string_view userInfo)
```

Defined in src/http/include/icy/http/authenticator.h:47

Parses username:password std::string and sets username and password of the credentials object. Throws SyntaxException on invalid user information.

---

{#fromuri}

#### fromURI

```cpp
void fromURI(const http::URL & uri)
```

Defined in src/http/include/icy/http/authenticator.h:52

Extracts username and password from the given URI and sets username and password of the credentials object. Does nothing if URI has no user info part.

---

{#setusername}

#### setUsername

```cpp
void setUsername(const std::string & username)
```

Defined in src/http/include/icy/http/authenticator.h:55

Sets the username.

---

{#username}

#### username

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & username() const
```

Defined in src/http/include/icy/http/authenticator.h:58

Returns the username.

---

{#setpassword}

#### setPassword

```cpp
void setPassword(const std::string & password)
```

Defined in src/http/include/icy/http/authenticator.h:61

Sets the password.

---

{#password}

#### password

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & password() const
```

Defined in src/http/include/icy/http/authenticator.h:64

Returns the password.

---

{#authenticate}

#### authenticate

```cpp
void authenticate(http::Request & request, const http::Response & response)
```

Defined in src/http/include/icy/http/authenticator.h:69

Inspects WWW-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given http::Request.

---

{#updateauthinfo}

#### updateAuthInfo

```cpp
void updateAuthInfo(http::Request & request)
```

Defined in src/http/include/icy/http/authenticator.h:73

Updates internal state (in case of digest authentication) and replaces authentication information in the request accordingly.

---

{#proxyauthenticate}

#### proxyAuthenticate

```cpp
void proxyAuthenticate(http::Request & request, const http::Response & response)
```

Defined in src/http/include/icy/http/authenticator.h:78

Inspects Proxy-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given http::Request.

---

{#updateproxyauthinfo}

#### updateProxyAuthInfo

```cpp
void updateProxyAuthInfo(http::Request & request)
```

Defined in src/http/include/icy/http/authenticator.h:83

Updates internal state (in case of digest authentication) and replaces proxy authentication information in the request accordingly.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_username`](#_username)  |  |
| `std::string` | [`_password`](#_password)  |  |

---

{#_username}

#### _username

```cpp
std::string _username
```

Defined in src/http/include/icy/http/authenticator.h:89

---

{#_password}

#### _password

```cpp
std::string _password
```

Defined in src/http/include/icy/http/authenticator.h:90

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Authenticator`](#authenticator-4)  | Deleted constructor. |

---

{#authenticator-4}

#### Authenticator

```cpp
Authenticator(const Authenticator &) = delete
```

Defined in src/http/include/icy/http/authenticator.h:86

Deleted constructor.

{#basicauthenticator}

## BasicAuthenticator

```cpp
#include <icy/http/authenticator.h>
```

```cpp
class BasicAuthenticator
```

Defined in src/http/include/icy/http/authenticator.h:99

Encodes and decodes HTTP Basic authentication credentials.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`BasicAuthenticator`](#basicauthenticator-1)  | Creates an empty basic authenticator. |
|  | [`BasicAuthenticator`](#basicauthenticator-2)  | Creates a basic authenticator with the given username and password. |
|  | [`BasicAuthenticator`](#basicauthenticator-3) `explicit` | Extracts basic authentication credentials from the given request. |
|  | [`BasicAuthenticator`](#basicauthenticator-4) `explicit` | Parses a raw Basic authentication payload string. The value can be extracted from a request via `[Request::getCredentials()](#getcredentials)`. |
|  | [`~BasicAuthenticator`](#basicauthenticator-5)  | Destroys the basic authenticator. |
| `void` | [`setUsername`](#setusername-1)  | Sets the username. |
| `const std::string &` | [`username`](#username-1) `const` `nodiscard` | Returns the username. |
| `void` | [`setPassword`](#setpassword-1)  | Sets the password. |
| `const std::string &` | [`password`](#password-1) `const` `nodiscard` | Returns the password. |
| `void` | [`authenticate`](#authenticate-1) `const` | Adds authentication information to the given http::Request. |
| `void` | [`proxyAuthenticate`](#proxyauthenticate-1) `const` | Adds proxy authentication information to the given http::Request. |

---

{#basicauthenticator-1}

#### BasicAuthenticator

```cpp
BasicAuthenticator()
```

Defined in src/http/include/icy/http/authenticator.h:103

Creates an empty basic authenticator.

---

{#basicauthenticator-2}

#### BasicAuthenticator

```cpp
BasicAuthenticator(const std::string & username, const std::string & password)
```

Defined in src/http/include/icy/http/authenticator.h:106

Creates a basic authenticator with the given username and password.

---

{#basicauthenticator-3}

#### BasicAuthenticator

`explicit`

```cpp
explicit BasicAuthenticator(const http::Request & request)
```

Defined in src/http/include/icy/http/authenticator.h:113

Extracts basic authentication credentials from the given request.

Throws a NotAuthenticatedException if the request does not contain basic authentication information.

---

{#basicauthenticator-4}

#### BasicAuthenticator

`explicit`

```cpp
explicit BasicAuthenticator(const std::string & authInfo)
```

Defined in src/http/include/icy/http/authenticator.h:117

Parses a raw Basic authentication payload string. The value can be extracted from a request via `[Request::getCredentials()](#getcredentials)`.

---

{#basicauthenticator-5}

#### ~BasicAuthenticator

```cpp
~BasicAuthenticator()
```

Defined in src/http/include/icy/http/authenticator.h:120

Destroys the basic authenticator.

---

{#setusername-1}

#### setUsername

```cpp
void setUsername(const std::string & username)
```

Defined in src/http/include/icy/http/authenticator.h:123

Sets the username.

---

{#username-1}

#### username

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & username() const
```

Defined in src/http/include/icy/http/authenticator.h:126

Returns the username.

---

{#setpassword-1}

#### setPassword

```cpp
void setPassword(const std::string & password)
```

Defined in src/http/include/icy/http/authenticator.h:129

Sets the password.

---

{#password-1}

#### password

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & password() const
```

Defined in src/http/include/icy/http/authenticator.h:132

Returns the password.

---

{#authenticate-1}

#### authenticate

`const`

```cpp
void authenticate(http::Request & request) const
```

Defined in src/http/include/icy/http/authenticator.h:135

Adds authentication information to the given http::Request.

---

{#proxyauthenticate-1}

#### proxyAuthenticate

`const`

```cpp
void proxyAuthenticate(http::Request & request) const
```

Defined in src/http/include/icy/http/authenticator.h:138

Adds proxy authentication information to the given http::Request.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`parseAuthInfo`](#parseauthinfo)  | Extracts username and password from Basic authentication info by base64-decoding authInfo and splitting the resulting std::string at the ':' delimiter. |

---

{#parseauthinfo}

#### parseAuthInfo

```cpp
void parseAuthInfo(std::string_view authInfo)
```

Defined in src/http/include/icy/http/authenticator.h:144

Extracts username and password from Basic authentication info by base64-decoding authInfo and splitting the resulting std::string at the ':' delimiter.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_username`](#_username-1)  |  |
| `std::string` | [`_password`](#_password-1)  |  |

---

{#_username-1}

#### _username

```cpp
std::string _username
```

Defined in src/http/include/icy/http/authenticator.h:150

---

{#_password-1}

#### _password

```cpp
std::string _password
```

Defined in src/http/include/icy/http/authenticator.h:151

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`BasicAuthenticator`](#basicauthenticator-6)  | Deleted constructor. |

---

{#basicauthenticator-6}

#### BasicAuthenticator

```cpp
BasicAuthenticator(const BasicAuthenticator &) = delete
```

Defined in src/http/include/icy/http/authenticator.h:147

Deleted constructor.

{#chunkedadapter}

## ChunkedAdapter

```cpp
#include <icy/http/packetizers.h>
```

```cpp
class ChunkedAdapter
```

Defined in src/http/include/icy/http/packetizers.h:29

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

HTTP chunked transfer encoding adapter for streaming responses.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Connection::Ptr` | [`connection`](#connection)  | HTTP connection to send the initial response header through. |
| `std::string` | [`contentType`](#contenttype)  | Content-Type value for the chunked response. |
| `std::string` | [`frameSeparator`](#frameseparator)  | Optional separator written before each chunk payload. |
| `bool` | [`initial`](#initial)  | True until the first chunk is processed and the header emitted. |
| `bool` | [`nocopy`](#nocopy)  | If true, header/data/footer are emitted as separate packets. |
| `PacketSignal` | [`emitter`](#emitter-5)  |  |

---

{#connection}

#### connection

```cpp
Connection::Ptr connection
```

Defined in src/http/include/icy/http/packetizers.h:32

HTTP connection to send the initial response header through.

---

{#contenttype}

#### contentType

```cpp
std::string contentType
```

Defined in src/http/include/icy/http/packetizers.h:33

Content-Type value for the chunked response.

---

{#frameseparator}

#### frameSeparator

```cpp
std::string frameSeparator
```

Defined in src/http/include/icy/http/packetizers.h:34

Optional separator written before each chunk payload.

---

{#initial}

#### initial

```cpp
bool initial
```

Defined in src/http/include/icy/http/packetizers.h:35

True until the first chunk is processed and the header emitted.

---

{#nocopy}

#### nocopy

```cpp
bool nocopy
```

Defined in src/http/include/icy/http/packetizers.h:36

If true, header/data/footer are emitted as separate packets.

---

{#emitter-5}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/http/include/icy/http/packetizers.h:148

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ChunkedAdapter`](#chunkedadapter-1) `inline` | Creates a [ChunkedAdapter](#chunkedadapter) that sends its initial response header through the given connection. The content type is read from the connection's outgoing header. |
|  | [`ChunkedAdapter`](#chunkedadapter-2) `inline` | Creates a [ChunkedAdapter](#chunkedadapter) that emits its own raw HTTP/1.1 200 response header. Use this when no [Connection](#connection-1) object is available. |
| `void` | [`emitHeader`](#emitheader) `virtual` `inline` | Emits the initial HTTP/1.1 200 OK response headers with chunked transfer encoding. If a connection is set, headers are written through it; otherwise a raw response string is emitted via the packet signal. |
| `void` | [`process`](#process-5) `virtual` `inline` `override` | Encodes an incoming packet as a chunked transfer encoding chunk and emits it. Emits the HTTP response headers on the first call. |

---

{#chunkedadapter-1}

#### ChunkedAdapter

`inline`

```cpp
inline ChunkedAdapter(Connection::Ptr connection = nullptr, const std::string & frameSeparator = "", bool nocopy = true)
```

Defined in src/http/include/icy/http/packetizers.h:43

Creates a [ChunkedAdapter](#chunkedadapter) that sends its initial response header through the given connection. The content type is read from the connection's outgoing header. 
#### Parameters
* `connection` HTTP connection to use. May be nullptr to emit a raw response instead. 

* `frameSeparator` Optional data prepended to each chunk payload. 

* `nocopy` If true, header and payload are emitted as separate packets (avoids copies).

---

{#chunkedadapter-2}

#### ChunkedAdapter

`inline`

```cpp
inline ChunkedAdapter(const std::string & contentType, const std::string & frameSeparator = "", bool nocopy = true)
```

Defined in src/http/include/icy/http/packetizers.h:56

Creates a [ChunkedAdapter](#chunkedadapter) that emits its own raw HTTP/1.1 200 response header. Use this when no [Connection](#connection-1) object is available. 
#### Parameters
* `contentType` Content-Type value for the response. 

* `frameSeparator` Optional data prepended to each chunk payload. 

* `nocopy` If true, header and payload are emitted as separate packets.

---

{#emitheader}

#### emitHeader

`virtual` `inline`

```cpp
virtual inline void emitHeader()
```

Defined in src/http/include/icy/http/packetizers.h:71

Emits the initial HTTP/1.1 200 OK response headers with chunked transfer encoding. If a connection is set, headers are written through it; otherwise a raw response string is emitted via the packet signal.

---

{#process-5}

#### process

`virtual` `inline` `override`

```cpp
virtual inline void process(IPacket & packet) override
```

Defined in src/http/include/icy/http/packetizers.h:110

Encodes an incoming packet as a chunked transfer encoding chunk and emits it. Emits the HTTP response headers on the first call. 
#### Parameters
* `packet` Packet containing the raw payload data. 

#### Exceptions
* `std::invalid_argument` if the packet does not carry data.

{#client}

## Client

```cpp
#include <icy/http/client.h>
```

```cpp
class Client
```

Defined in src/http/include/icy/http/client.h:192

HTTP client for creating and managing outgoing connections.

### Friends

| Name | Description |
|------|-------------|
| [`ClientConnection`](#clientconnection)  |  |

---

{#clientconnection}

#### ClientConnection

```cpp
friend class ClientConnection
```

Defined in src/http/include/icy/http/client.h:252

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`Shutdown`](#shutdown-6)  |  |

---

{#shutdown-6}

#### Shutdown

```cpp
NullSignal Shutdown
```

Defined in src/http/include/icy/http/client.h:247

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Client`](#client-1)  |  |
| `void` | [`stop`](#stop-7)  | Stop the [Client](#client) and close all connections. |
| `ClientConnection::Ptr` | [`createConnectionT`](#createconnectiont-1) `inline` | Creates and registers a typed client connection for the given [URL](#url). The connection type is inferred from the [URL](#url) scheme (http, https, ws, wss). |
| `ClientConnection::Ptr` | [`createConnection`](#createconnection-1) `inline` | Creates and registers a [ClientConnection](#clientconnection-1) for the given [URL](#url). The socket type is chosen based on the [URL](#url) scheme (http/https/ws/wss). |
| `void` | [`addConnection`](#addconnection) `virtual` | Registers a connection with this client so it is tracked and cleaned up on [stop()](#stop-7). |
| `void` | [`removeConnection`](#removeconnection) `virtual` | Removes a previously registered connection from the client. |

---

{#client-1}

#### Client

```cpp
Client()
```

Defined in src/http/include/icy/http/client.h:195

---

{#stop-7}

#### stop

```cpp
void stop()
```

Defined in src/http/include/icy/http/client.h:205

Stop the [Client](#client) and close all connections.

---

{#createconnectiont-1}

#### createConnectionT

`inline`

```cpp
template<class ConnectionT> inline ClientConnection::Ptr createConnectionT(const URL & url, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/http/include/icy/http/client.h:215

Creates and registers a typed client connection for the given [URL](#url). The connection type is inferred from the [URL](#url) scheme (http, https, ws, wss). 
#### Parameters
* `ConnectionT` Concrete connection type derived from [ClientConnection](#clientconnection-1). 

#### Parameters
* `url` Target [URL](#url). The scheme determines the socket and adapter type. 

* `loop` Event loop to use. Defaults to the default libuv loop. 

#### Returns
Shared pointer to the created connection.

---

{#createconnection-1}

#### createConnection

`inline`

```cpp
inline ClientConnection::Ptr createConnection(const URL & url, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/http/include/icy/http/client.h:229

Creates and registers a [ClientConnection](#clientconnection-1) for the given [URL](#url). The socket type is chosen based on the [URL](#url) scheme (http/https/ws/wss). 
#### Parameters
* `url` Target [URL](#url). 

* `loop` Event loop to use. Defaults to the default libuv loop. 

#### Returns
Shared pointer to the created connection.

---

{#addconnection}

#### addConnection

`virtual`

```cpp
virtual void addConnection(ClientConnection::Ptr conn)
```

Defined in src/http/include/icy/http/client.h:240

Registers a connection with this client so it is tracked and cleaned up on [stop()](#stop-7). 
#### Parameters
* `conn` The connection to add.

---

{#removeconnection}

#### removeConnection

`virtual`

```cpp
virtual void removeConnection(ClientConnection * conn)
```

Defined in src/http/include/icy/http/client.h:245

Removes a previously registered connection from the client. 
#### Parameters
* `conn` Raw pointer to the connection to remove. 

#### Exceptions
* `std::logic_error` if the connection is not tracked by this client.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Client &` | [`instance`](#instance-3) `static` | Return the default HTTP [Client](#client) singleton. |
| `void` | [`destroy`](#destroy-1) `static` | Destroys the default HTTP [Client](#client) singleton. |

---

{#instance-3}

#### instance

`static`

```cpp
static Client & instance()
```

Defined in src/http/include/icy/http/client.h:199

Return the default HTTP [Client](#client) singleton.

---

{#destroy-1}

#### destroy

`static`

```cpp
static void destroy()
```

Defined in src/http/include/icy/http/client.h:202

Destroys the default HTTP [Client](#client) singleton.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ClientConnectionPtrVec` | [`_connections`](#_connections)  |  |

---

{#_connections}

#### _connections

```cpp
ClientConnectionPtrVec _connections
```

Defined in src/http/include/icy/http/client.h:254

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onConnectionClose`](#onconnectionclose)  |  |

---

{#onconnectionclose}

#### onConnectionClose

```cpp
void onConnectionClose(Connection & conn)
```

Defined in src/http/include/icy/http/client.h:250

{#clientconnection-1}

## ClientConnection

```cpp
#include <icy/http/client.h>
```

```cpp
class ClientConnection
```

Defined in src/http/include/icy/http/client.h:33

> **Inherits:** [`Connection`](#connection-1)

HTTP client connection for managing request/response lifecycle.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `void *` | [`opaque`](#opaque-1)  | Optional unmanaged client data pointer. Not used by the connection internally. |
| `NullSignal` | [`Connect`](#connect-11)  | Status signals. |
| `Signal< void(Response &)>` | [`Headers`](#headers)  | Signals when the response HTTP header has been received. |
| `Signal< void(const MutableBuffer &)>` | [`Payload`](#payload)  | Signals when raw data is received. |
| `Signal< void(const Response &)>` | [`Complete`](#complete)  | Signals when the HTTP transaction is complete. |
| `Signal< void(const icy::Error &)>` | [`Error`](#error-9)  | Signals when the underlying transport reports an error. |
| `Signal< void(Connection &)>` | [`Close`](#close-19)  | Signals when the connection is closed. |
| `ProgressSignal` | [`IncomingProgress`](#incomingprogress)  | Signals download progress (0-100%). |

---

{#opaque-1}

#### opaque

```cpp
void * opaque
```

Defined in src/http/include/icy/http/client.h:91

Optional unmanaged client data pointer. Not used by the connection internally.

---

{#connect-11}

#### Connect

```cpp
NullSignal Connect
```

Defined in src/http/include/icy/http/client.h:110

Status signals.

Signals when the client socket is connected and data can flow

---

{#headers}

#### Headers

```cpp
Signal< void(Response &)> Headers
```

Defined in src/http/include/icy/http/client.h:111

Signals when the response HTTP header has been received.

---

{#payload}

#### Payload

```cpp
Signal< void(const MutableBuffer &)> Payload
```

Defined in src/http/include/icy/http/client.h:112

Signals when raw data is received.

---

{#complete}

#### Complete

```cpp
Signal< void(const Response &)> Complete
```

Defined in src/http/include/icy/http/client.h:113

Signals when the HTTP transaction is complete.

---

{#error-9}

#### Error

```cpp
Signal< void(const icy::Error &)> Error
```

Defined in src/http/include/icy/http/client.h:114

Signals when the underlying transport reports an error.

---

{#close-19}

#### Close

```cpp
Signal< void(Connection &)> Close
```

Defined in src/http/include/icy/http/client.h:115

Signals when the connection is closed.

---

{#incomingprogress}

#### IncomingProgress

```cpp
ProgressSignal IncomingProgress
```

Defined in src/http/include/icy/http/client.h:116

Signals download progress (0-100%).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ClientConnection`](#clientconnection-2)  | Creates a [ClientConnection](#clientconnection-1) to the given [URL](#url), pre-populating the request URI and Host header. The response status is initialised to 502 Bad Gateway until a real response is received. |
| `void` | [`start`](#start-8) `virtual` | Starts the internal HTTP request. |
| `void` | [`start`](#start-9) `virtual` | Starts the given HTTP request, replacing the internal request object. |
| `ssize_t` | [`send`](#send-10) `virtual` `override` | Sends raw data to the peer, initiating a connection first if needed. Data is buffered internally until the connection is established. |
| `void` | [`setReadStream`](#setreadstream) `virtual` | Sets the output stream to which incoming response body data is written. The stream pointer is owned by the connection and freed with it. Must be called before [start()](#start-8). |
| `StreamT &` | [`readStream`](#readstream) `inline` | Returns a reference to the read stream cast to the specified type. |

---

{#clientconnection-2}

#### ClientConnection

```cpp
ClientConnection(const URL & url, const net::TCPSocket::Ptr & socket = std::make_shared< net::TCPSocket >())
```

Defined in src/http/include/icy/http/client.h:42

Creates a [ClientConnection](#clientconnection-1) to the given [URL](#url), pre-populating the request URI and Host header. The response status is initialised to 502 Bad Gateway until a real response is received. 
#### Parameters
* `url` Target [URL](#url). Scheme, host, port and path are extracted automatically. 

* `socket` TCP socket to use. Defaults to a plain TCPSocket; pass an SSLSocket for HTTPS.

---

{#start-8}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/http/include/icy/http/client.h:51

Starts the internal HTTP request.

Calls [connect()](#connect-12) internally if the socket is not already connecting or connected. The actual request will be sent when the socket is connected. 
#### Exceptions
* `std::runtime_error` if already connecting.

---

{#start-9}

#### start

`virtual`

```cpp
virtual void start(http::Request & req)
```

Defined in src/http/include/icy/http/client.h:59

Starts the given HTTP request, replacing the internal request object.

Calls [connect()](#connect-12) internally if the socket is not already connecting or connected. The actual request will be sent when the socket is connected. 
#### Parameters
* `req` The HTTP request to send. Replaces the internal request. 

#### Exceptions
* `std::runtime_error` if already connecting.

---

{#send-10}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/http/include/icy/http/client.h:67

Sends raw data to the peer, initiating a connection first if needed. Data is buffered internally until the connection is established. 
#### Parameters
* `data` Pointer to the data buffer. 

* `len` Number of bytes to send. 

* `flags` Socket send flags (unused for HTTP). 

#### Returns
Number of bytes sent or buffered.

---

{#setreadstream}

#### setReadStream

`virtual`

```cpp
virtual void setReadStream(std::ostream * os)
```

Defined in src/http/include/icy/http/client.h:74

Sets the output stream to which incoming response body data is written. The stream pointer is owned by the connection and freed with it. Must be called before [start()](#start-8). 
#### Parameters
* `os` Pointer to the output stream. Takes ownership. 

#### Exceptions
* `std::runtime_error` if already connecting.

---

{#readstream}

#### readStream

`inline`

```cpp
template<class StreamT> inline StreamT & readStream()
```

Defined in src/http/include/icy/http/client.h:82

Returns a reference to the read stream cast to the specified type. 
#### Parameters
* `StreamT` The concrete stream type to cast to. 

#### Returns
Reference to the stream. 

#### Exceptions
* `std::runtime_error` if no read stream has been set. 

* `std::bad_cast` if the stream is not of type StreamT.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `URL` | [`_url`](#_url)  |  |
| `bool` | [`_connect`](#_connect)  |  |
| `bool` | [`_active`](#_active)  |  |
| `bool` | [`_complete`](#_complete)  |  |
| `std::vector< PendingWrite >` | [`_outgoingBuffer`](#_outgoingbuffer)  |  |
| `std::unique_ptr< std::ostream >` | [`_readStream`](#_readstream)  |  |

---

{#_url}

#### _url

```cpp
URL _url
```

Defined in src/http/include/icy/http/client.h:135

---

{#_connect}

#### _connect

```cpp
bool _connect
```

Defined in src/http/include/icy/http/client.h:136

---

{#_active}

#### _active

```cpp
bool _active
```

Defined in src/http/include/icy/http/client.h:137

---

{#_complete}

#### _complete

```cpp
bool _complete
```

Defined in src/http/include/icy/http/client.h:138

---

{#_outgoingbuffer}

#### _outgoingBuffer

```cpp
std::vector< PendingWrite > _outgoingBuffer
```

Defined in src/http/include/icy/http/client.h:139

---

{#_readstream}

#### _readStream

```cpp
std::unique_ptr< std::ostream > _readStream
```

Defined in src/http/include/icy/http/client.h:140

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`connect`](#connect-12) `virtual` | Connects to the server endpoint. All sent data is buffered until the connection is made. |
| `http::Message *` | [`incomingHeader`](#incomingheader) `virtual` `override` | Returns the incoming HTTP message header (request or response depending on role). |
| `http::Message *` | [`outgoingHeader`](#outgoingheader) `virtual` `override` | Returns the outgoing HTTP message header (request or response depending on role). |
| `bool` | [`onSocketConnect`](#onsocketconnect-2) `virtual` `override` | [net::SocketAdapter](net.md#socketadapter) interface |

---

{#connect-12}

#### connect

`virtual`

```cpp
virtual void connect()
```

Defined in src/http/include/icy/http/client.h:121

Connects to the server endpoint. All sent data is buffered until the connection is made.

---

{#incomingheader}

#### incomingHeader

`virtual` `override`

```cpp
virtual http::Message * incomingHeader() override
```

Defined in src/http/include/icy/http/client.h:123

Returns the incoming HTTP message header (request or response depending on role).

---

{#outgoingheader}

#### outgoingHeader

`virtual` `override`

```cpp
virtual http::Message * outgoingHeader() override
```

Defined in src/http/include/icy/http/client.h:124

Returns the outgoing HTTP message header (request or response depending on role).

---

{#onsocketconnect-2}

#### onSocketConnect

`virtual` `override`

```cpp
virtual bool onSocketConnect(net::Socket & socket) override
```

Defined in src/http/include/icy/http/client.h:126

[net::SocketAdapter](net.md#socketadapter) interface

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-11)  |  |

---

{#ptr-11}

#### Ptr

```cpp
using Ptr = std::shared_ptr< ClientConnection >
```

Defined in src/http/include/icy/http/client.h:36

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onHeaders`](#onheaders) `virtual` `override` | [Connection](#connection-1) interface. |
| `void` | [`onPayload`](#onpayload) `virtual` `override` | Called for each chunk of incoming response body data. |
| `void` | [`onComplete`](#oncomplete) `virtual` `override` | Called when the full HTTP response has been received. |
| `void` | [`onClose`](#onclose-3) `virtual` `override` | Called when the connection is closed. |
| `bool` | [`onSocketError`](#onsocketerror-2) `virtual` `override` | Called when the underlying transport encounters an error. |

---

{#onheaders}

#### onHeaders

`virtual` `override`

```cpp
virtual void onHeaders() override
```

Defined in src/http/include/icy/http/client.h:97

[Connection](#connection-1) interface.

Called when the response headers have been parsed.

---

{#onpayload}

#### onPayload

`virtual` `override`

```cpp
virtual void onPayload(const MutableBuffer & buffer) override
```

Defined in src/http/include/icy/http/client.h:99

Called for each chunk of incoming response body data.

---

{#oncomplete}

#### onComplete

`virtual` `override`

```cpp
virtual void onComplete() override
```

Defined in src/http/include/icy/http/client.h:101

Called when the full HTTP response has been received.

---

{#onclose-3}

#### onClose

`virtual` `override`

```cpp
virtual void onClose() override
```

Defined in src/http/include/icy/http/client.h:103

Called when the connection is closed.

---

{#onsocketerror-2}

#### onSocketError

`virtual` `override`

```cpp
virtual bool onSocketError(net::Socket & socket, const icy::Error & error) override
```

Defined in src/http/include/icy/http/client.h:105

Called when the underlying transport encounters an error.

{#pendingwrite}

## PendingWrite

```cpp
#include <icy/http/client.h>
```

```cpp
struct PendingWrite
```

Defined in src/http/include/icy/http/client.h:129

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Buffer` | [`data`](#data-1)  |  |
| `int` | [`flags`](#flags)  |  |

---

{#data-1}

#### data

```cpp
Buffer data
```

Defined in src/http/include/icy/http/client.h:131

---

{#flags}

#### flags

```cpp
int flags = 0
```

Defined in src/http/include/icy/http/client.h:132

{#connection-1}

## Connection

```cpp
#include <icy/http/connection.h>
```

```cpp
class Connection
```

Defined in src/http/include/icy/http/connection.h:34

> **Inherits:** [`SocketAdapter`](net.md#socketadapter)
> **Subclassed by:** [`ClientConnection`](#clientconnection-1), [`ServerConnection`](#serverconnection-1)

Base HTTP connection managing socket I/O and message lifecycle

### Friends

| Name | Description |
|------|-------------|
| [`ConnectionStream`](#connectionstream)  |  |
| [`ConnectionAdapter`](#connectionadapter)  |  |

---

{#connectionstream}

#### ConnectionStream

```cpp
friend class ConnectionStream
```

Defined in src/http/include/icy/http/connection.h:148

---

{#connectionadapter}

#### ConnectionAdapter

```cpp
friend class ConnectionAdapter
```

Defined in src/http/include/icy/http/connection.h:149

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Connection`](#connection-2)  | Creates a [Connection](#connection-1) using the given TCP socket. |
| `void` | [`onHeaders`](#onheaders-1) `virtual` | Called when the incoming HTTP headers have been fully parsed. |
| `void` | [`onPayload`](#onpayload-1) `virtual` | Called for each chunk of incoming body data after headers are complete. |
| `void` | [`onComplete`](#oncomplete-1) `virtual` | Called when the incoming HTTP message is fully received. |
| `void` | [`onClose`](#onclose-4) `virtual` | Called when the connection is closed. |
| `ssize_t` | [`send`](#send-11) `virtual` `override` | Send raw data to the peer. |
| `ssize_t` | [`sendOwned`](#sendowned-10) `virtual` `override` | Send an owned payload buffer to the peer. |
| `ssize_t` | [`sendHeader`](#sendheader) `virtual` | Send the outdoing HTTP header. |
| `void` | [`close`](#close-20) `virtual` | Close the connection and schedule the object for deferred deletion. |
| `void` | [`markActive`](#markactive) `virtual` `inline` | Marks the connection as active. [Server](#server) connections override this to refresh the idle timer. |
| `void` | [`beginStreaming`](#beginstreaming) `virtual` `inline` | Explicitly enter long-lived streaming mode. Base connections ignore this; server connections use it to disable keep-alive idle reaping while a response stream is active. |
| `void` | [`endStreaming`](#endstreaming) `virtual` `inline` | Exit long-lived streaming mode. |
| `bool` | [`closed`](#closed-4) `const` `nodiscard` | Return true if the connection is closed. |
| `icy::Error` | [`error`](#error-10) `const` `nodiscard` | Return the error object if any. |
| `bool` | [`headerAutoSendEnabled`](#headerautosendenabled) `const` `nodiscard` | Return true if headers should be automatically sent. |
| `void` | [`setHeaderAutoSendEnabled`](#setheaderautosendenabled)  | Enable or disable automatic header emission for the next outgoing send path. |
| `void` | [`replaceAdapter`](#replaceadapter) `virtual` | Assign the new [ConnectionAdapter](#connectionadapter-1) and setup the chain. The flow is: [Connection](#connection-1) <-> [ConnectionAdapter](#connectionadapter-1) <-> Socket. Takes ownership of the adapter (deferred deletion via uv loop). |
| `void` | [`replaceAdapter`](#replaceadapter-1) `virtual` | Overload for nullptr (used in destructor to clear adapter). |
| `bool` | [`secure`](#secure) `const` `nodiscard` | Return true if the connection uses TLS/SSL. |
| `net::TCPSocket::Ptr &` | [`socket`](#socket-5) `nodiscard` | Return the underlying socket pointer. |
| `net::SocketAdapter *` | [`adapter`](#adapter) `const` `nodiscard` | Return the underlying adapter pointer. |
| `Request &` | [`request`](#request-3) `nodiscard` | The HTTP request headers. |
| `Response &` | [`response`](#response) `nodiscard` | The HTTP response headers. |
| `http::Message *` | [`incomingHeader`](#incomingheader-1) `virtual` | Returns the incoming HTTP message header (request or response depending on role). |
| `http::Message *` | [`outgoingHeader`](#outgoingheader-1) `virtual` | Returns the outgoing HTTP message header (request or response depending on role). |

---

{#connection-2}

#### Connection

```cpp
Connection(const net::TCPSocket::Ptr & socket = std::make_shared< net::TCPSocket >())
```

Defined in src/http/include/icy/http/connection.h:41

Creates a [Connection](#connection-1) using the given TCP socket. 
#### Parameters
* `socket` The TCP socket to use for I/O. Defaults to a new TCPSocket.

---

{#onheaders-1}

#### onHeaders

`virtual`

```cpp
virtual void onHeaders()
```

Defined in src/http/include/icy/http/connection.h:45

Called when the incoming HTTP headers have been fully parsed.

---

{#onpayload-1}

#### onPayload

`virtual`

```cpp
virtual void onPayload(const MutableBuffer & buffer)
```

Defined in src/http/include/icy/http/connection.h:49

Called for each chunk of incoming body data after headers are complete. 
#### Parameters
* `buffer` [Buffer](base.md#buffer-2) containing the received data chunk.

---

{#oncomplete-1}

#### onComplete

`virtual`

```cpp
virtual void onComplete()
```

Defined in src/http/include/icy/http/connection.h:52

Called when the incoming HTTP message is fully received.

---

{#onclose-4}

#### onClose

`virtual`

```cpp
virtual void onClose()
```

Defined in src/http/include/icy/http/connection.h:55

Called when the connection is closed.

---

{#send-11}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/http/include/icy/http/connection.h:61

Send raw data to the peer.

This is the zero-copy fast path. The caller retains ownership of the payload until the underlying async write completes.

---

{#sendowned-10}

#### sendOwned

`virtual` `override`

```cpp
virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/http/include/icy/http/connection.h:66

Send an owned payload buffer to the peer.

Use this when the payload does not naturally outlive the current scope.

---

{#sendheader}

#### sendHeader

`virtual`

```cpp
virtual ssize_t sendHeader()
```

Defined in src/http/include/icy/http/connection.h:69

Send the outdoing HTTP header.

---

{#close-20}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/http/include/icy/http/connection.h:73

Close the connection and schedule the object for deferred deletion.

---

{#markactive}

#### markActive

`virtual` `inline`

```cpp
virtual inline void markActive()
```

Defined in src/http/include/icy/http/connection.h:77

Marks the connection as active. [Server](#server) connections override this to refresh the idle timer.

---

{#beginstreaming}

#### beginStreaming

`virtual` `inline`

```cpp
virtual inline void beginStreaming()
```

Defined in src/http/include/icy/http/connection.h:82

Explicitly enter long-lived streaming mode. Base connections ignore this; server connections use it to disable keep-alive idle reaping while a response stream is active.

---

{#endstreaming}

#### endStreaming

`virtual` `inline`

```cpp
virtual inline void endStreaming()
```

Defined in src/http/include/icy/http/connection.h:85

Exit long-lived streaming mode.

---

{#closed-4}

#### closed

`const` `nodiscard`

```cpp
[[nodiscard]] bool closed() const
```

Defined in src/http/include/icy/http/connection.h:88

Return true if the connection is closed.

---

{#error-10}

#### error

`const` `nodiscard`

```cpp
[[nodiscard]] icy::Error error() const
```

Defined in src/http/include/icy/http/connection.h:91

Return the error object if any.

---

{#headerautosendenabled}

#### headerAutoSendEnabled

`const` `nodiscard`

```cpp
[[nodiscard]] bool headerAutoSendEnabled() const
```

Defined in src/http/include/icy/http/connection.h:94

Return true if headers should be automatically sent.

---

{#setheaderautosendenabled}

#### setHeaderAutoSendEnabled

```cpp
void setHeaderAutoSendEnabled(bool enabled)
```

Defined in src/http/include/icy/http/connection.h:97

Enable or disable automatic header emission for the next outgoing send path.

---

{#replaceadapter}

#### replaceAdapter

`virtual`

```cpp
virtual void replaceAdapter(std::unique_ptr< net::SocketAdapter > adapter)
```

Defined in src/http/include/icy/http/connection.h:102

Assign the new [ConnectionAdapter](#connectionadapter-1) and setup the chain. The flow is: [Connection](#connection-1) <-> [ConnectionAdapter](#connectionadapter-1) <-> Socket. Takes ownership of the adapter (deferred deletion via uv loop).

---

{#replaceadapter-1}

#### replaceAdapter

`virtual`

```cpp
virtual void replaceAdapter(std::nullptr_t)
```

Defined in src/http/include/icy/http/connection.h:105

Overload for nullptr (used in destructor to clear adapter).

---

{#secure}

#### secure

`const` `nodiscard`

```cpp
[[nodiscard]] bool secure() const
```

Defined in src/http/include/icy/http/connection.h:108

Return true if the connection uses TLS/SSL.

---

{#socket-5}

#### socket

`nodiscard`

```cpp
[[nodiscard]] net::TCPSocket::Ptr & socket()
```

Defined in src/http/include/icy/http/connection.h:111

Return the underlying socket pointer.

---

{#adapter}

#### adapter

`const` `nodiscard`

```cpp
[[nodiscard]] net::SocketAdapter * adapter() const
```

Defined in src/http/include/icy/http/connection.h:114

Return the underlying adapter pointer.

---

{#request-3}

#### request

`nodiscard`

```cpp
[[nodiscard]] Request & request()
```

Defined in src/http/include/icy/http/connection.h:117

The HTTP request headers.

---

{#response}

#### response

`nodiscard`

```cpp
[[nodiscard]] Response & response()
```

Defined in src/http/include/icy/http/connection.h:120

The HTTP response headers.

---

{#incomingheader-1}

#### incomingHeader

`virtual`

```cpp
virtual http::Message * incomingHeader()
```

Defined in src/http/include/icy/http/connection.h:123

Returns the incoming HTTP message header (request or response depending on role).

---

{#outgoingheader-1}

#### outgoingHeader

`virtual`

```cpp
virtual http::Message * outgoingHeader()
```

Defined in src/http/include/icy/http/connection.h:126

Returns the outgoing HTTP message header (request or response depending on role).

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::TCPSocket::Ptr` | [`_socket`](#_socket-1)  |  |
| `net::SocketAdapter *` | [`_adapter`](#_adapter)  |  |
| `Request` | [`_request`](#_request)  |  |
| `Response` | [`_response`](#_response)  |  |
| `icy::Error` | [`_error`](#_error-2)  |  |
| `bool` | [`_closed`](#_closed)  |  |
| `bool` | [`_shouldSendHeader`](#_shouldsendheader)  |  |

---

{#_socket-1}

#### _socket

```cpp
net::TCPSocket::Ptr _socket
```

Defined in src/http/include/icy/http/connection.h:140

---

{#_adapter}

#### _adapter

```cpp
net::SocketAdapter * _adapter
```

Defined in src/http/include/icy/http/connection.h:141

---

{#_request}

#### _request

```cpp
Request _request
```

Defined in src/http/include/icy/http/connection.h:142

---

{#_response}

#### _response

```cpp
Response _response
```

Defined in src/http/include/icy/http/connection.h:143

---

{#_error-2}

#### _error

```cpp
icy::Error _error
```

Defined in src/http/include/icy/http/connection.h:144

---

{#_closed}

#### _closed

```cpp
bool _closed
```

Defined in src/http/include/icy/http/connection.h:145

---

{#_shouldsendheader}

#### _shouldSendHeader

```cpp
bool _shouldSendHeader
```

Defined in src/http/include/icy/http/connection.h:146

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`setError`](#seterror-4) `virtual` | Set the internal error. Note: Setting the error does not `[close()](#close-20)` the connection. |
| `bool` | [`onSocketConnect`](#onsocketconnect-3) `virtual` `override` | [net::SocketAdapter](net.md#socketadapter) interface |
| `bool` | [`onSocketRecv`](#onsocketrecv-3) `virtual` `override` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `bool` | [`onSocketError`](#onsocketerror-3) `virtual` `override` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `bool` | [`onSocketClose`](#onsocketclose-2) `virtual` `override` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |

---

{#seterror-4}

#### setError

`virtual`

```cpp
virtual void setError(const icy::Error & err)
```

Defined in src/http/include/icy/http/connection.h:131

Set the internal error. Note: Setting the error does not `[close()](#close-20)` the connection.

---

{#onsocketconnect-3}

#### onSocketConnect

`virtual` `override`

```cpp
virtual bool onSocketConnect(net::Socket & socket) override
```

Defined in src/http/include/icy/http/connection.h:134

[net::SocketAdapter](net.md#socketadapter) interface

---

{#onsocketrecv-3}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress) override
```

Defined in src/http/include/icy/http/connection.h:135

Called when data is received from the socket. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that received data. 

* `buffer` The received data buffer. 

* `peerAddress` Address of the sender. 

#### Returns
true to stop propagation to subsequent receivers.

---

{#onsocketerror-3}

#### onSocketError

`virtual` `override`

```cpp
virtual bool onSocketError(net::Socket & socket, const icy::Error & error) override
```

Defined in src/http/include/icy/http/connection.h:136

Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that encountered the error. 

* `error` [Error](base.md#error) details. 

#### Returns
true to stop propagation to subsequent receivers.

---

{#onsocketclose-2}

#### onSocketClose

`virtual` `override`

```cpp
virtual bool onSocketClose(net::Socket & socket) override
```

Defined in src/http/include/icy/http/connection.h:137

Called when the socket is closed. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that was closed. 

#### Returns
true to stop propagation to subsequent receivers.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-12)  |  |

---

{#ptr-12}

#### Ptr

```cpp
using Ptr = std::shared_ptr< Connection >
```

Defined in src/http/include/icy/http/connection.h:37

{#connectionadapter-1}

## ConnectionAdapter

```cpp
#include <icy/http/connection.h>
```

```cpp
class ConnectionAdapter
```

Defined in src/http/include/icy/http/connection.h:160

> **Inherits:** [`ParserObserver`](#parserobserver), [`SocketAdapter`](net.md#socketadapter)

Default HTTP socket adapter for reading and writing HTTP messages

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ConnectionAdapter`](#connectionadapter-2)  | Creates a [ConnectionAdapter](#connectionadapter-1) for the given connection. |
| `ssize_t` | [`send`](#send-12) `virtual` `override` | Sends data to the peer, flushing the outgoing HTTP header first if needed. |
| `ssize_t` | [`sendOwned`](#sendowned-11) `virtual` `override` | Sends an owned payload buffer to the connected peer. |
| `void` | [`removeReceiver`](#removereceiver-2) `override` | Remove the given receiver. |
| `Parser &` | [`parser`](#parser) `nodiscard` | Returns the HTTP parser instance. |
| `Connection *` | [`connection`](#connection-3) `nodiscard` | Returns the owning [Connection](#connection-1) pointer, or nullptr if detached. |
| `void` | [`reset`](#reset-7)  | Resets the adapter for reuse with a new socket and request. Clears the parser state and re-wires the sender without reallocating. Used by the connection pool to avoid destroying/recreating adapters. |

---

{#connectionadapter-2}

#### ConnectionAdapter

```cpp
ConnectionAdapter(Connection * connection, llhttp_type_t type)
```

Defined in src/http/include/icy/http/connection.h:167

Creates a [ConnectionAdapter](#connectionadapter-1) for the given connection. 
#### Parameters
* `connection` Owning HTTP connection. 

* `type` [Parser](#parser-1) type: HTTP_REQUEST for server side, HTTP_RESPONSE for client side.

---

{#send-12}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/http/include/icy/http/connection.h:175

Sends data to the peer, flushing the outgoing HTTP header first if needed. 
#### Parameters
* `data` Pointer to the data buffer. 

* `len` Number of bytes to send. 

* `flags` Send flags (unused for HTTP, used for WebSocket frame type). 

#### Returns
Number of bytes sent, or -1 on error.

---

{#sendowned-11}

#### sendOwned

`virtual` `override`

```cpp
virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/http/include/icy/http/connection.h:176

Sends an owned payload buffer to the connected peer.

The buffer is moved through the adapter chain and retained by the transport layer until async write completion.

---

{#removereceiver-2}

#### removeReceiver

`override`

```cpp
void removeReceiver(SocketAdapter * adapter) override
```

Defined in src/http/include/icy/http/connection.h:182

Remove the given receiver.

By default this function does nothing unless the given receiver matches the current receiver.

---

{#parser}

#### parser

`nodiscard`

```cpp
[[nodiscard]] Parser & parser()
```

Defined in src/http/include/icy/http/connection.h:185

Returns the HTTP parser instance.

---

{#connection-3}

#### connection

`nodiscard`

```cpp
[[nodiscard]] Connection * connection()
```

Defined in src/http/include/icy/http/connection.h:188

Returns the owning [Connection](#connection-1) pointer, or nullptr if detached.

---

{#reset-7}

#### reset

```cpp
void reset(net::SocketAdapter * sender, http::Request * request)
```

Defined in src/http/include/icy/http/connection.h:195

Resets the adapter for reuse with a new socket and request. Clears the parser state and re-wires the sender without reallocating. Used by the connection pool to avoid destroying/recreating adapters. 
#### Parameters
* `sender` New socket adapter to send data through. 

* `request` New HTTP request object for the parser to populate.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Connection *` | [`_connection`](#_connection)  |  |
| `Parser` | [`_parser`](#_parser)  |  |

---

{#_connection}

#### _connection

```cpp
Connection * _connection
```

Defined in src/http/include/icy/http/connection.h:208

---

{#_parser}

#### _parser

```cpp
Parser _parser
```

Defined in src/http/include/icy/http/connection.h:209

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`onSocketRecv`](#onsocketrecv-4) `virtual` `override` | SocketAdapter interface. |
| `void` | [`onParserHeader`](#onparserheader) `virtual` `override` | HTTP [Parser](#parser-1) interface. |
| `void` | [`onParserHeadersEnd`](#onparserheadersend) `virtual` `override` | Called when all HTTP headers have been parsed. |
| `void` | [`onParserChunk`](#onparserchunk) `virtual` `override` | Called for each chunk of body data received. |
| `void` | [`onParserError`](#onparsererror) `virtual` `override` | Called when a parse error occurs. |
| `void` | [`onParserEnd`](#onparserend) `virtual` `override` | Called when the HTTP message is fully parsed. |

---

{#onsocketrecv-4}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress) override
```

Defined in src/http/include/icy/http/connection.h:199

SocketAdapter interface.

---

{#onparserheader}

#### onParserHeader

`virtual` `override`

```cpp
virtual void onParserHeader(const std::string & name, const std::string & value) override
```

Defined in src/http/include/icy/http/connection.h:202

HTTP [Parser](#parser-1) interface.

---

{#onparserheadersend}

#### onParserHeadersEnd

`virtual` `override`

```cpp
virtual void onParserHeadersEnd(bool upgrade) override
```

Defined in src/http/include/icy/http/connection.h:203

Called when all HTTP headers have been parsed. 
#### Parameters
* `upgrade` True if the connection should be upgraded (e.g. to WebSocket).

---

{#onparserchunk}

#### onParserChunk

`virtual` `override`

```cpp
virtual void onParserChunk(const char * data, size_t len) override
```

Defined in src/http/include/icy/http/connection.h:204

Called for each chunk of body data received. 
#### Parameters
* `data` Pointer to the body data chunk. 

* `len` Length of the chunk in bytes.

---

{#onparsererror}

#### onParserError

`virtual` `override`

```cpp
virtual void onParserError(const icy::Error & err) override
```

Defined in src/http/include/icy/http/connection.h:205

Called when a parse error occurs. 
#### Parameters
* `err` [Error](base.md#error) details from llhttp.

---

{#onparserend}

#### onParserEnd

`virtual` `override`

```cpp
virtual void onParserEnd() override
```

Defined in src/http/include/icy/http/connection.h:206

Called when the HTTP message is fully parsed.

{#connectionpool}

## ConnectionPool

```cpp
#include <icy/http/server.h>
```

```cpp
class ConnectionPool
```

Defined in src/http/include/icy/http/server.h:291

LIFO connection pool for reusing [ServerConnection](#serverconnection-1) objects. Avoids per-request heap allocation by resetting and reusing connections instead of destroying and recreating them.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `ServerConnection::Ptr` | [`acquire`](#acquire) `inline` | Takes a connection from the pool for reuse. |
| `bool` | [`release`](#release) `inline` | Returns a connection to the pool after use. |
| `void` | [`setMaxSize`](#setmaxsize) `inline` | Sets the maximum number of connections the pool will hold. |
| `size_t` | [`size`](#size) `const` `inline` `nodiscard` | Returns the current number of connections held in the pool. |

---

{#acquire}

#### acquire

`inline`

```cpp
inline ServerConnection::Ptr acquire()
```

Defined in src/http/include/icy/http/server.h:296

Takes a connection from the pool for reuse. 
#### Returns
A pooled connection, or nullptr if the pool is empty.

---

{#release}

#### release

`inline`

```cpp
inline bool release(ServerConnection::Ptr & conn)
```

Defined in src/http/include/icy/http/server.h:307

Returns a connection to the pool after use. 
#### Parameters
* `conn` The connection to return. 

#### Returns
true if accepted into the pool; false if the pool is full.

---

{#setmaxsize}

#### setMaxSize

`inline`

```cpp
inline void setMaxSize(size_t n)
```

Defined in src/http/include/icy/http/server.h:316

Sets the maximum number of connections the pool will hold. 
#### Parameters
* `n` Maximum pool capacity.

---

{#size}

#### size

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t size() const
```

Defined in src/http/include/icy/http/server.h:319

Returns the current number of connections held in the pool.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< ServerConnection::Ptr >` | [`_pool`](#_pool)  |  |
| `size_t` | [`_maxSize`](#_maxsize)  |  |

---

{#_pool}

#### _pool

```cpp
std::vector< ServerConnection::Ptr > _pool
```

Defined in src/http/include/icy/http/server.h:322

---

{#_maxsize}

#### _maxSize

```cpp
size_t _maxSize = 128
```

Defined in src/http/include/icy/http/server.h:323

{#connectionstream-1}

## ConnectionStream

```cpp
#include <icy/http/connection.h>
```

```cpp
class ConnectionStream
```

Defined in src/http/include/icy/http/connection.h:261

> **Inherits:** [`SocketAdapter`](net.md#socketadapter)

Packet stream wrapper for an HTTP connection.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketStream` | [`Outgoing`](#outgoing)  | The Outgoing stream is responsible for packetizing raw application data into the agreed upon HTTP format and sending it to the peer. |
| `PacketStream` | [`Incoming`](#incoming)  | The Incoming stream emits incoming HTTP packets for processing by the application. |
| `ProgressSignal` | [`IncomingProgress`](#incomingprogress-1)  | Fired on download progress. |
| `ProgressSignal` | [`OutgoingProgress`](#outgoingprogress)  | Fired on upload progress. |

---

{#outgoing}

#### Outgoing

```cpp
PacketStream Outgoing
```

Defined in src/http/include/icy/http/connection.h:280

The Outgoing stream is responsible for packetizing raw application data into the agreed upon HTTP format and sending it to the peer.

---

{#incoming}

#### Incoming

```cpp
PacketStream Incoming
```

Defined in src/http/include/icy/http/connection.h:286

The Incoming stream emits incoming HTTP packets for processing by the application.

This is useful for example when writing incoming data to a file.

---

{#incomingprogress-1}

#### IncomingProgress

```cpp
ProgressSignal IncomingProgress
```

Defined in src/http/include/icy/http/connection.h:288

Fired on download progress.

---

{#outgoingprogress}

#### OutgoingProgress

```cpp
ProgressSignal OutgoingProgress
```

Defined in src/http/include/icy/http/connection.h:289

Fired on upload progress.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ConnectionStream`](#connectionstream-2)  | Creates a [ConnectionStream](#connectionstream-1) wrapping the given HTTP connection. Wires the Outgoing stream emitter to the connection adapter and registers this stream to receive incoming data from the adapter. |
| `ssize_t` | [`send`](#send-13) `virtual` `override` | Send data via the Outgoing stream. |
| `Connection::Ptr` | [`connection`](#connection-4) `nodiscard` | Return a reference to the underlying connection. |

---

{#connectionstream-2}

#### ConnectionStream

```cpp
ConnectionStream(Connection::Ptr connection)
```

Defined in src/http/include/icy/http/connection.h:268

Creates a [ConnectionStream](#connectionstream-1) wrapping the given HTTP connection. Wires the Outgoing stream emitter to the connection adapter and registers this stream to receive incoming data from the adapter. 
#### Parameters
* `connection` The HTTP connection to wrap.

---

{#send-13}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/http/include/icy/http/connection.h:272

Send data via the Outgoing stream.

---

{#connection-4}

#### connection

`nodiscard`

```cpp
[[nodiscard]] Connection::Ptr connection()
```

Defined in src/http/include/icy/http/connection.h:275

Return a reference to the underlying connection.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Connection::Ptr` | [`_connection`](#_connection-1)  |  |

---

{#_connection-1}

#### _connection

```cpp
Connection::Ptr _connection
```

Defined in src/http/include/icy/http/connection.h:294

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`onSocketRecv`](#onsocketrecv-5) `virtual` `override` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |

---

{#onsocketrecv-5}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress) override
```

Defined in src/http/include/icy/http/connection.h:292

Called when data is received from the socket. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that received data. 

* `buffer` The received data buffer. 

* `peerAddress` Address of the sender. 

#### Returns
true to stop propagation to subsequent receivers.

{#cookie}

## Cookie

```cpp
#include <icy/http/cookie.h>
```

```cpp
class Cookie
```

Defined in src/http/include/icy/http/cookie.h:39

HTTP cookie value plus its response/header attributes.

A cookie is a small amount of information sent by a Web server to a Web browser, saved by the browser, and later sent back to the server. A cookie's value can uniquely identify a client, so cookies are commonly used for session management.

A cookie has a name, a single value, and optional attributes such as a comment, path and domain qualifiers, a maximum age, and a version number.

This class supports both the Version 0 (by Netscape) and Version 1 (by RFC 2109) cookie specifications. By default, cookies are created using Version 0 to ensure the best interoperability.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Cookie`](#cookie-1)  | Creates an empty [Cookie](#cookie). |
|  | [`Cookie`](#cookie-2) `explicit` | Creates a cookie with the given name. The cookie never expires. |
|  | [`Cookie`](#cookie-3) `explicit` | Creates a cookie from the given [NVCollection](base.md#nvcollection). |
|  | [`Cookie`](#cookie-4)  | Creates a cookie with the given name and value. The cookie never expires. |
|  | [`Cookie`](#cookie-5)  | Creates the [Cookie](#cookie) by copying another one. |
|  | [`~Cookie`](#cookie-6)  | Destroys the [Cookie](#cookie). |
| `Cookie &` | [`operator=`](#operator-16)  | Assigns a cookie. |
| `void` | [`setVersion`](#setversion)  | Sets the version of the cookie. |
| `int` | [`getVersion`](#getversion) `const` `inline` `nodiscard` | Returns the version of the cookie, which is either 0 or 1. |
| `void` | [`setName`](#setname)  | Sets the name of the cookie. |
| `const std::string &` | [`getName`](#getname) `const` `inline` `nodiscard` | Returns the name of the cookie. |
| `void` | [`setValue`](#setvalue)  | Sets the value of the cookie. |
| `const std::string &` | [`getValue`](#getvalue) `const` `inline` `nodiscard` | Returns the value of the cookie. |
| `void` | [`setComment`](#setcomment)  | Sets the comment for the cookie. |
| `const std::string &` | [`getComment`](#getcomment) `const` `inline` `nodiscard` | Returns the comment for the cookie. |
| `void` | [`setDomain`](#setdomain)  | Sets the domain for the cookie. |
| `const std::string &` | [`getDomain`](#getdomain) `const` `inline` `nodiscard` | Returns the domain for the cookie. |
| `void` | [`setPath`](#setpath)  | Sets the path for the cookie. |
| `const std::string &` | [`getPath`](#getpath) `const` `inline` `nodiscard` | Returns the path for the cookie. |
| `void` | [`setSecure`](#setsecure)  | Sets the value of the secure flag for the cookie. |
| `bool` | [`getSecure`](#getsecure) `const` `inline` `nodiscard` | Returns the value of the secure flag for the cookie. |
| `void` | [`setMaxAge`](#setmaxage)  | Sets the maximum age in seconds for the cookie. |
| `int` | [`getMaxAge`](#getmaxage) `const` `inline` `nodiscard` | Returns the maximum age in seconds for the cookie. |
| `void` | [`setHttpOnly`](#sethttponly)  | Sets the HttpOnly flag for the cookie. |
| `bool` | [`getHttpOnly`](#gethttponly) `const` `inline` `nodiscard` | Returns true if the cookie's HttpOnly flag is set. |
| `std::string` | [`toString`](#tostring-5) `const` `nodiscard` | Returns a std::string representation of the cookie, suitable for use in a Set-Cookie header. |

---

{#cookie-1}

#### Cookie

```cpp
Cookie()
```

Defined in src/http/include/icy/http/cookie.h:43

Creates an empty [Cookie](#cookie).

---

{#cookie-2}

#### Cookie

`explicit`

```cpp
explicit Cookie(const std::string & name)
```

Defined in src/http/include/icy/http/cookie.h:47

Creates a cookie with the given name. The cookie never expires.

---

{#cookie-3}

#### Cookie

`explicit`

```cpp
explicit Cookie(const NVCollection & nvc)
```

Defined in src/http/include/icy/http/cookie.h:50

Creates a cookie from the given [NVCollection](base.md#nvcollection).

---

{#cookie-4}

#### Cookie

```cpp
Cookie(const std::string & name, const std::string & value)
```

Defined in src/http/include/icy/http/cookie.h:58

Creates a cookie with the given name and value. The cookie never expires.

Note: If value contains whitespace or non-alphanumeric characters, the value should be escaped by calling [escape()](#escape) before passing it to the constructor.

---

{#cookie-5}

#### Cookie

```cpp
Cookie(const Cookie & cookie)
```

Defined in src/http/include/icy/http/cookie.h:61

Creates the [Cookie](#cookie) by copying another one.

---

{#cookie-6}

#### ~Cookie

```cpp
~Cookie()
```

Defined in src/http/include/icy/http/cookie.h:64

Destroys the [Cookie](#cookie).

---

{#operator-16}

#### operator=

```cpp
Cookie & operator=(const Cookie & cookie)
```

Defined in src/http/include/icy/http/cookie.h:67

Assigns a cookie.

---

{#setversion}

#### setVersion

```cpp
void setVersion(int version)
```

Defined in src/http/include/icy/http/cookie.h:73

Sets the version of the cookie.

Version must be either 0 (denoting a Netscape cookie) or 1 (denoting a RFC 2109 cookie).

---

{#getversion}

#### getVersion

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline int getVersion() const
```

Defined in src/http/include/icy/http/cookie.h:77

Returns the version of the cookie, which is either 0 or 1.

---

{#setname}

#### setName

```cpp
void setName(const std::string & name)
```

Defined in src/http/include/icy/http/cookie.h:80

Sets the name of the cookie.

---

{#getname}

#### getName

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & getName() const
```

Defined in src/http/include/icy/http/cookie.h:83

Returns the name of the cookie.

---

{#setvalue}

#### setValue

```cpp
void setValue(const std::string & value)
```

Defined in src/http/include/icy/http/cookie.h:93

Sets the value of the cookie.

According to the cookie specification, the size of the value should not exceed 4 Kbytes.

Note: If value contains whitespace or non-alphanumeric characters, the value should be escaped by calling [escape()](#escape) prior to passing it to [setName()](#setname).

---

{#getvalue}

#### getValue

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & getValue() const
```

Defined in src/http/include/icy/http/cookie.h:96

Returns the value of the cookie.

---

{#setcomment}

#### setComment

```cpp
void setComment(const std::string & comment)
```

Defined in src/http/include/icy/http/cookie.h:101

Sets the comment for the cookie.

Comments are only supported for version 1 cookies.

---

{#getcomment}

#### getComment

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & getComment() const
```

Defined in src/http/include/icy/http/cookie.h:104

Returns the comment for the cookie.

---

{#setdomain}

#### setDomain

```cpp
void setDomain(const std::string & domain)
```

Defined in src/http/include/icy/http/cookie.h:107

Sets the domain for the cookie.

---

{#getdomain}

#### getDomain

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & getDomain() const
```

Defined in src/http/include/icy/http/cookie.h:110

Returns the domain for the cookie.

---

{#setpath}

#### setPath

```cpp
void setPath(const std::string & path)
```

Defined in src/http/include/icy/http/cookie.h:113

Sets the path for the cookie.

---

{#getpath}

#### getPath

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & getPath() const
```

Defined in src/http/include/icy/http/cookie.h:116

Returns the path for the cookie.

---

{#setsecure}

#### setSecure

```cpp
void setSecure(bool secure)
```

Defined in src/http/include/icy/http/cookie.h:120

Sets the value of the secure flag for the cookie.

---

{#getsecure}

#### getSecure

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool getSecure() const
```

Defined in src/http/include/icy/http/cookie.h:124

Returns the value of the secure flag for the cookie.

---

{#setmaxage}

#### setMaxAge

```cpp
void setMaxAge(int maxAge)
```

Defined in src/http/include/icy/http/cookie.h:134

Sets the maximum age in seconds for the cookie.

A value of -1 causes the cookie to never expire on the client.

A value of 0 deletes the cookie on the client.

---

{#getmaxage}

#### getMaxAge

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline int getMaxAge() const
```

Defined in src/http/include/icy/http/cookie.h:138

Returns the maximum age in seconds for the cookie.

---

{#sethttponly}

#### setHttpOnly

```cpp
void setHttpOnly(bool flag = true)
```

Defined in src/http/include/icy/http/cookie.h:141

Sets the HttpOnly flag for the cookie.

---

{#gethttponly}

#### getHttpOnly

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool getHttpOnly() const
```

Defined in src/http/include/icy/http/cookie.h:144

Returns true if the cookie's HttpOnly flag is set.

---

{#tostring-5}

#### toString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toString() const
```

Defined in src/http/include/icy/http/cookie.h:148

Returns a std::string representation of the cookie, suitable for use in a Set-Cookie header.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`escape`](#escape) `static` | Escapes the given std::string by replacing all non-alphanumeric characters with escape sequences in the form xx, where xx is the hexadecimal character code. |
| `std::string` | [`unescape`](#unescape) `static` | Unescapes the given std::string by replacing all escape sequences in the form xx with the respective characters. |

---

{#escape}

#### escape

`static`

```cpp
static std::string escape(std::string_view str)
```

Defined in src/http/include/icy/http/cookie.h:154

Escapes the given std::string by replacing all non-alphanumeric characters with escape sequences in the form xx, where xx is the hexadecimal character code.

---

{#unescape}

#### unescape

`static`

```cpp
static std::string unescape(std::string_view str)
```

Defined in src/http/include/icy/http/cookie.h:159

Unescapes the given std::string by replacing all escape sequences in the form xx with the respective characters.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_version`](#_version)  |  |
| `std::string` | [`_name`](#_name)  |  |
| `std::string` | [`_value`](#_value)  |  |
| `std::string` | [`_comment`](#_comment)  |  |
| `std::string` | [`_domain`](#_domain)  |  |
| `std::string` | [`_path`](#_path)  |  |
| `bool` | [`_secure`](#_secure)  |  |
| `int` | [`_maxAge`](#_maxage)  |  |
| `bool` | [`_httpOnly`](#_httponly)  |  |

---

{#_version}

#### _version

```cpp
int _version
```

Defined in src/http/include/icy/http/cookie.h:162

---

{#_name}

#### _name

```cpp
std::string _name
```

Defined in src/http/include/icy/http/cookie.h:163

---

{#_value}

#### _value

```cpp
std::string _value
```

Defined in src/http/include/icy/http/cookie.h:164

---

{#_comment}

#### _comment

```cpp
std::string _comment
```

Defined in src/http/include/icy/http/cookie.h:165

---

{#_domain}

#### _domain

```cpp
std::string _domain
```

Defined in src/http/include/icy/http/cookie.h:166

---

{#_path}

#### _path

```cpp
std::string _path
```

Defined in src/http/include/icy/http/cookie.h:167

---

{#_secure}

#### _secure

```cpp
bool _secure
```

Defined in src/http/include/icy/http/cookie.h:168

---

{#_maxage}

#### _maxAge

```cpp
int _maxAge
```

Defined in src/http/include/icy/http/cookie.h:169

---

{#_httponly}

#### _httpOnly

```cpp
bool _httpOnly
```

Defined in src/http/include/icy/http/cookie.h:170

{#filepart}

## FilePart

```cpp
#include <icy/http/form.h>
```

```cpp
class FilePart
```

Defined in src/http/include/icy/http/form.h:236

> **Inherits:** [`FormPart`](#formpart)

Form part backed by a file on disk.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FilePart`](#filepart-1)  | Creates the [FilePart](#filepart) for the given path. |
|  | [`FilePart`](#filepart-2)  | Creates the [FilePart](#filepart) for the given path and MIME type. |
|  | [`FilePart`](#filepart-3)  | Creates the [FilePart](#filepart) for the given path and MIME type. The given filename is used as part filename (see [filename()](#filename-1)) only. |
|  | [`~FilePart`](#filepart-4) `virtual` | Destroys the [FilePart](#filepart). |
| `void` | [`open`](#open-4) `virtual` | Opens the file for reading. |
| `void` | [`reset`](#reset-8) `virtual` `override` | Resets the file stream to the beginning and clears initial-write state. |
| `bool` | [`writeChunk`](#writechunk) `virtual` `override` | Writes the next chunk of the file to the [FormWriter](#formwriter). |
| `void` | [`write`](#write-2) `virtual` `override` | Writes the entire file content to the [FormWriter](#formwriter). |
| `void` | [`write`](#write-3) `virtual` `override` | Writes the entire file content to an output stream (used for content-length calculation). |
| `const std::string &` | [`filename`](#filename-1) `const` `nodiscard` | Returns the filename component of the file path (not the full path). |
| `std::ifstream &` | [`stream`](#stream-3) `nodiscard` | Returns a reference to the underlying file input stream. |
| `uint64_t` | [`length`](#length-1) `virtual` `const` `nodiscard` `override` | Returns the total file size in bytes. |

---

{#filepart-1}

#### FilePart

```cpp
FilePart(const std::string & path)
```

Defined in src/http/include/icy/http/form.h:244

Creates the [FilePart](#filepart) for the given path.

The MIME type is set to application/octet-stream.

Throws an FileException if the file cannot be opened.

---

{#filepart-2}

#### FilePart

```cpp
FilePart(const std::string & path, const std::string & contentType)
```

Defined in src/http/include/icy/http/form.h:250

Creates the [FilePart](#filepart) for the given path and MIME type.

Throws an FileException if the file cannot be opened.

---

{#filepart-3}

#### FilePart

```cpp
FilePart(const std::string & path, const std::string & filename, const std::string & contentType)
```

Defined in src/http/include/icy/http/form.h:257

Creates the [FilePart](#filepart) for the given path and MIME type. The given filename is used as part filename (see [filename()](#filename-1)) only.

Throws an FileException if the file cannot be opened.

---

{#filepart-4}

#### ~FilePart

`virtual`

```cpp
virtual ~FilePart()
```

Defined in src/http/include/icy/http/form.h:261

Destroys the [FilePart](#filepart).

---

{#open-4}

#### open

`virtual`

```cpp
virtual void open()
```

Defined in src/http/include/icy/http/form.h:265

Opens the file for reading. 
#### Exceptions
* `std::runtime_error` if the file cannot be opened.

---

{#reset-8}

#### reset

`virtual` `override`

```cpp
virtual void reset() override
```

Defined in src/http/include/icy/http/form.h:268

Resets the file stream to the beginning and clears initial-write state.

---

{#writechunk}

#### writeChunk

`virtual` `override`

```cpp
virtual bool writeChunk(FormWriter & writer) override
```

Defined in src/http/include/icy/http/form.h:273

Writes the next chunk of the file to the [FormWriter](#formwriter). 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send the chunk through. 

#### Returns
true if more data remains; false when the file is fully sent.

---

{#write-2}

#### write

`virtual` `override`

```cpp
virtual void write(FormWriter & writer) override
```

Defined in src/http/include/icy/http/form.h:277

Writes the entire file content to the [FormWriter](#formwriter). 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send data through.

---

{#write-3}

#### write

`virtual` `override`

```cpp
virtual void write(std::ostream & ostr) override
```

Defined in src/http/include/icy/http/form.h:281

Writes the entire file content to an output stream (used for content-length calculation). 
#### Parameters
* `ostr` Output stream to write to.

---

{#filename-1}

#### filename

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & filename() const
```

Defined in src/http/include/icy/http/form.h:284

Returns the filename component of the file path (not the full path).

---

{#stream-3}

#### stream

`nodiscard`

```cpp
[[nodiscard]] std::ifstream & stream()
```

Defined in src/http/include/icy/http/form.h:287

Returns a reference to the underlying file input stream.

---

{#length-1}

#### length

`virtual` `const` `nodiscard` `override`

```cpp
[[nodiscard]] virtual uint64_t length() const override
```

Defined in src/http/include/icy/http/form.h:290

Returns the total file size in bytes.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_path`](#_path-1)  |  |
| `std::string` | [`_filename`](#_filename-1)  |  |
| `std::ifstream` | [`_istr`](#_istr)  |  |
| `uint64_t` | [`_fileSize`](#_filesize)  |  |

---

{#_path-1}

#### _path

```cpp
std::string _path
```

Defined in src/http/include/icy/http/form.h:293

---

{#_filename-1}

#### _filename

```cpp
std::string _filename
```

Defined in src/http/include/icy/http/form.h:294

---

{#_istr}

#### _istr

```cpp
std::ifstream _istr
```

Defined in src/http/include/icy/http/form.h:295

---

{#_filesize}

#### _fileSize

```cpp
uint64_t _fileSize
```

Defined in src/http/include/icy/http/form.h:296

{#formpart}

## FormPart

```cpp
#include <icy/http/form.h>
```

```cpp
class FormPart
```

Defined in src/http/include/icy/http/form.h:183

> **Subclassed by:** [`FilePart`](#filepart), [`StringPart`](#stringpart-1)

An implementation of [FormPart](#formpart).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormPart`](#formpart-1)  | Creates the [FormPart](#formpart) with the given MIME content type. |
|  | [`~FormPart`](#formpart-2) `virtual` | Destroys the [FormPart](#formpart). |
| `void` | [`reset`](#reset-9) `virtual` | Resets the internal state and write position to the beginning. Called by [FormWriter](#formwriter) when retrying or recalculating content length. |
| `bool` | [`writeChunk`](#writechunk-1) `virtual` | Writes the next chunk of data to the [FormWriter](#formwriter). |
| `void` | [`write`](#write-4) `virtual` | Writes the entire part data to the [FormWriter](#formwriter) in one call. |
| `void` | [`write`](#write-5) `virtual` | Writes the entire part data to an output stream (used for content-length calculation). |
| `NVCollection &` | [`headers`](#headers-1) `nodiscard` | Returns the extra MIME headers for this part (e.g. Content-Disposition). |
| `bool` | [`initialWrite`](#initialwrite) `virtual` `const` `nodiscard` | Returns true if this is the first write call since construction or [reset()](#reset-9). |
| `const std::string &` | [`contentType`](#contenttype-1) `const` `nodiscard` | Returns the MIME content type for this part. |
| `uint64_t` | [`length`](#length-2) `virtual` `const` `nodiscard` | Returns the total byte length of the part data. |

---

{#formpart-1}

#### FormPart

```cpp
FormPart(const std::string & contentType = "application/octet-stream")
```

Defined in src/http/include/icy/http/form.h:188

Creates the [FormPart](#formpart) with the given MIME content type. 
#### Parameters
* `contentType` MIME type for this part (default: "application/octet-stream").

---

{#formpart-2}

#### ~FormPart

`virtual`

```cpp
virtual ~FormPart()
```

Defined in src/http/include/icy/http/form.h:191

Destroys the [FormPart](#formpart).

---

{#reset-9}

#### reset

`virtual`

```cpp
virtual void reset()
```

Defined in src/http/include/icy/http/form.h:195

Resets the internal state and write position to the beginning. Called by [FormWriter](#formwriter) when retrying or recalculating content length.

---

{#writechunk-1}

#### writeChunk

`virtual`

```cpp
virtual bool writeChunk(FormWriter & writer)
```

Defined in src/http/include/icy/http/form.h:200

Writes the next chunk of data to the [FormWriter](#formwriter). 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send the chunk through. 

#### Returns
true if more data remains to be written; false when complete.

---

{#write-4}

#### write

`virtual`

```cpp
virtual void write(FormWriter & writer)
```

Defined in src/http/include/icy/http/form.h:204

Writes the entire part data to the [FormWriter](#formwriter) in one call. 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send data through.

---

{#write-5}

#### write

`virtual`

```cpp
virtual void write(std::ostream & ostr)
```

Defined in src/http/include/icy/http/form.h:208

Writes the entire part data to an output stream (used for content-length calculation). 
#### Parameters
* `ostr` Output stream to write to.

---

{#headers-1}

#### headers

`nodiscard`

```cpp
[[nodiscard]] NVCollection & headers()
```

Defined in src/http/include/icy/http/form.h:211

Returns the extra MIME headers for this part (e.g. Content-Disposition).

---

{#initialwrite}

#### initialWrite

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual bool initialWrite() const
```

Defined in src/http/include/icy/http/form.h:214

Returns true if this is the first write call since construction or [reset()](#reset-9).

---

{#contenttype-1}

#### contentType

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & contentType() const
```

Defined in src/http/include/icy/http/form.h:217

Returns the MIME content type for this part.

---

{#length-2}

#### length

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual uint64_t length() const
```

Defined in src/http/include/icy/http/form.h:220

Returns the total byte length of the part data.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_contentType`](#_contenttype)  |  |
| `uint64_t` | [`_length`](#_length)  |  |
| `NVCollection` | [`_headers`](#_headers)  |  |
| `bool` | [`_initialWrite`](#_initialwrite)  |  |

---

{#_contenttype}

#### _contentType

```cpp
std::string _contentType
```

Defined in src/http/include/icy/http/form.h:223

---

{#_length}

#### _length

```cpp
uint64_t _length
```

Defined in src/http/include/icy/http/form.h:224

---

{#_headers}

#### _headers

```cpp
NVCollection _headers
```

Defined in src/http/include/icy/http/form.h:225

---

{#_initialwrite}

#### _initialWrite

```cpp
bool _initialWrite
```

Defined in src/http/include/icy/http/form.h:226

{#formwriter}

## FormWriter

```cpp
#include <icy/http/form.h>
```

```cpp
class FormWriter
```

Defined in src/http/include/icy/http/form.h:42

> **Inherits:** [`NVCollection`](base.md#nvcollection), [`PacketStreamAdapter`](base.md#packetstreamadapter), [`Startable`](base.md#startable)

[FormWriter](#formwriter) is an HTTP client connection adapter for writing HTML forms.

This class runs in its own thread so as not to block the event loop while uploading big files. Class members are not synchronized hence they should not be accessed while the form is sending, not that there would be any reason to do so.

### Friends

| Name | Description |
|------|-------------|
| [`FormPart`](#formpart-3)  |  |
| [`FilePart`](#filepart-5)  |  |
| [`StringPart`](#stringpart)  |  |

---

{#formpart-3}

#### FormPart

```cpp
friend class FormPart
```

Defined in src/http/include/icy/http/form.h:152

---

{#filepart-5}

#### FilePart

```cpp
friend class FilePart
```

Defined in src/http/include/icy/http/form.h:153

---

{#stringpart}

#### StringPart

```cpp
friend class StringPart
```

Defined in src/http/include/icy/http/form.h:154

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-6)  | The outgoing packet emitter. |

---

{#emitter-6}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/http/include/icy/http/form.h:122

The outgoing packet emitter.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`~FormWriter`](#formwriter-1) `virtual` | Destroys the [FormWriter](#formwriter). |
| `void` | [`addPart`](#addpart)  | Adds a part or file attachment to the multipart form. |
| `void` | [`start`](#start-10) `virtual` | Starts the sending thread. |
| `void` | [`stop`](#stop-8) `virtual` | Stops the sending thread. |
| `bool` | [`complete`](#complete-1) `const` `nodiscard` | Returns true if the request is complete. |
| `bool` | [`cancelled`](#cancelled-1) `const` `nodiscard` | Returns true if the request is cancelled. |
| `void` | [`prepareSubmit`](#preparesubmit)  | Prepares the outgoing HTTP request object for submitting the form. |
| `uint64_t` | [`calculateMultipartContentLength`](#calculatemultipartcontentlength)  | Processes the entire form body and computes its total byte length. Only meaningful for multipart/form-data when not using chunked encoding. |
| `void` | [`writeUrl`](#writeurl)  | Writes the complete "application/x-www-form-urlencoded" encoded body to `ostr`. All key-value pairs from the [NVCollection](base.md#nvcollection) base are percent-encoded and joined with '&'. |
| `void` | [`writeMultipartChunk`](#writemultipartchunk)  | Writes the next pending multipart chunk to the connection. Non-blocking; intended to be called repeatedly from the event loop until all parts have been sent. |
| `void` | [`writeAsync`](#writeasync)  | Writes the next message chunk from the background runner thread. Called by the [Runner](base.md#runner); do not call directly. |
| `void` | [`setEncoding`](#setencoding)  | Sets the MIME encoding used for submitting the form. Must be set before [prepareSubmit()](#preparesubmit) is called. |
| `const std::string &` | [`encoding`](#encoding) `const` `nodiscard` | Returns the encoding used for posting the form. |
| `void` | [`setBoundary`](#setboundary)  | Sets the MIME boundary string used to delimit multipart form parts. If not set, a random boundary is generated by [prepareSubmit()](#preparesubmit). Must be set before [prepareSubmit()](#preparesubmit) is called. |
| `const std::string &` | [`boundary`](#boundary) `const` `nodiscard` | Returns the MIME boundary used for writing multipart form data. |
| `ConnectionStream &` | [`connection`](#connection-5) `nodiscard` | The associated HTTP client connection. |

---

{#formwriter-1}

#### ~FormWriter

`virtual`

```cpp
virtual ~FormWriter()
```

Defined in src/http/include/icy/http/form.h:57

Destroys the [FormWriter](#formwriter).

---

{#addpart}

#### addPart

```cpp
void addPart(const std::string & name, FormPart * part)
```

Defined in src/http/include/icy/http/form.h:65

Adds a part or file attachment to the multipart form.

The [FormWriter](#formwriter) takes ownership of `part` and deletes it when done. Parts are only sent when the encoding is "multipart/form-data". 
#### Parameters
* `name` Form field name for this part. 

* `part` [Part](#part) to add. Ownership is transferred.

---

{#start-10}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/http/include/icy/http/form.h:68

Starts the sending thread.

---

{#stop-8}

#### stop

`virtual`

```cpp
virtual void stop()
```

Defined in src/http/include/icy/http/form.h:71

Stops the sending thread.

---

{#complete-1}

#### complete

`const` `nodiscard`

```cpp
[[nodiscard]] bool complete() const
```

Defined in src/http/include/icy/http/form.h:74

Returns true if the request is complete.

---

{#cancelled-1}

#### cancelled

`const` `nodiscard`

```cpp
[[nodiscard]] bool cancelled() const
```

Defined in src/http/include/icy/http/form.h:77

Returns true if the request is cancelled.

---

{#preparesubmit}

#### prepareSubmit

```cpp
void prepareSubmit()
```

Defined in src/http/include/icy/http/form.h:80

Prepares the outgoing HTTP request object for submitting the form.

---

{#calculatemultipartcontentlength}

#### calculateMultipartContentLength

```cpp
uint64_t calculateMultipartContentLength()
```

Defined in src/http/include/icy/http/form.h:85

Processes the entire form body and computes its total byte length. Only meaningful for multipart/form-data when not using chunked encoding. 
#### Returns
Total content length in bytes.

---

{#writeurl}

#### writeUrl

```cpp
void writeUrl(std::ostream & ostr)
```

Defined in src/http/include/icy/http/form.h:90

Writes the complete "application/x-www-form-urlencoded" encoded body to `ostr`. All key-value pairs from the [NVCollection](base.md#nvcollection) base are percent-encoded and joined with '&'. 
#### Parameters
* `ostr` Output stream to write to.

---

{#writemultipartchunk}

#### writeMultipartChunk

```cpp
void writeMultipartChunk()
```

Defined in src/http/include/icy/http/form.h:95

Writes the next pending multipart chunk to the connection. Non-blocking; intended to be called repeatedly from the event loop until all parts have been sent.

---

{#writeasync}

#### writeAsync

```cpp
void writeAsync()
```

Defined in src/http/include/icy/http/form.h:99

Writes the next message chunk from the background runner thread. Called by the [Runner](base.md#runner); do not call directly.

---

{#setencoding}

#### setEncoding

```cpp
void setEncoding(const std::string & encoding)
```

Defined in src/http/include/icy/http/form.h:104

Sets the MIME encoding used for submitting the form. Must be set before [prepareSubmit()](#preparesubmit) is called. 
#### Parameters
* `encoding` MIME type: ENCODING_URL or ENCODING_MULTIPART_FORM.

---

{#encoding}

#### encoding

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & encoding() const
```

Defined in src/http/include/icy/http/form.h:107

Returns the encoding used for posting the form.

---

{#setboundary}

#### setBoundary

```cpp
void setBoundary(const std::string & boundary)
```

Defined in src/http/include/icy/http/form.h:113

Sets the MIME boundary string used to delimit multipart form parts. If not set, a random boundary is generated by [prepareSubmit()](#preparesubmit). Must be set before [prepareSubmit()](#preparesubmit) is called. 
#### Parameters
* `boundary` Boundary string (without leading "--").

---

{#boundary}

#### boundary

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & boundary() const
```

Defined in src/http/include/icy/http/form.h:116

Returns the MIME boundary used for writing multipart form data.

---

{#connection-5}

#### connection

`nodiscard`

```cpp
[[nodiscard]] ConnectionStream & connection()
```

Defined in src/http/include/icy/http/form.h:119

The associated HTTP client connection.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const char *` | [`ENCODING_URL`](#encoding_url) `static` | "application/x-www-form-urlencoded" |
| `const char *` | [`ENCODING_MULTIPART_FORM`](#encoding_multipart_form) `static` | "multipart/form-data" |
| `const char *` | [`ENCODING_MULTIPART_RELATED`](#encoding_multipart_related) `static` | "multipart/related" [http://tools.ietf.org/html/rfc2387](http://tools.ietf.org/html/rfc2387) |

---

{#encoding_url}

#### ENCODING_URL

`static`

```cpp
const char * ENCODING_URL
```

Defined in src/http/include/icy/http/form.h:124

"application/x-www-form-urlencoded"

---

{#encoding_multipart_form}

#### ENCODING_MULTIPART_FORM

`static`

```cpp
const char * ENCODING_MULTIPART_FORM
```

Defined in src/http/include/icy/http/form.h:125

"multipart/form-data"

---

{#encoding_multipart_related}

#### ENCODING_MULTIPART_RELATED

`static`

```cpp
const char * ENCODING_MULTIPART_RELATED
```

Defined in src/http/include/icy/http/form.h:126

"multipart/related" [http://tools.ietf.org/html/rfc2387](http://tools.ietf.org/html/rfc2387)

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `FormWriter *` | [`create`](#create-7) `static` | Creates a [FormWriter](#formwriter) for the given connection and encoding. |

---

{#create-7}

#### create

`static`

```cpp
static FormWriter * create(ConnectionStream & conn, const std::string & encoding = FormWriter::ENCODING_URL)
```

Defined in src/http/include/icy/http/form.h:54

Creates a [FormWriter](#formwriter) for the given connection and encoding.

Encoding must be either "application/x-www-form-urlencoded" (which is the default) or "multipart/form-data". 
#### Parameters
* `conn` The HTTP connection stream to write form data to. 

* `encoding` MIME encoding type. 

#### Returns
Heap-allocated [FormWriter](#formwriter). The caller owns the returned pointer.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ConnectionStream &` | [`_stream`](#_stream)  |  |
| `std::shared_ptr< Runner >` | [`_runner`](#_runner)  |  |
| `std::string` | [`_encoding`](#_encoding)  |  |
| `std::string` | [`_boundary`](#_boundary)  |  |
| `PartQueue` | [`_parts`](#_parts)  |  |
| `uint64_t` | [`_filesLength`](#_fileslength)  |  |
| `int` | [`_writeState`](#_writestate)  |  |
| `bool` | [`_initial`](#_initial)  |  |
| `bool` | [`_complete`](#_complete-1)  |  |

---

{#_stream}

#### _stream

```cpp
ConnectionStream & _stream
```

Defined in src/http/include/icy/http/form.h:165

---

{#_runner}

#### _runner

```cpp
std::shared_ptr< Runner > _runner
```

Defined in src/http/include/icy/http/form.h:166

---

{#_encoding}

#### _encoding

```cpp
std::string _encoding
```

Defined in src/http/include/icy/http/form.h:167

---

{#_boundary}

#### _boundary

```cpp
std::string _boundary
```

Defined in src/http/include/icy/http/form.h:168

---

{#_parts}

#### _parts

```cpp
PartQueue _parts
```

Defined in src/http/include/icy/http/form.h:169

---

{#_fileslength}

#### _filesLength

```cpp
uint64_t _filesLength
```

Defined in src/http/include/icy/http/form.h:170

---

{#_writestate}

#### _writeState

```cpp
int _writeState
```

Defined in src/http/include/icy/http/form.h:171

---

{#_initial}

#### _initial

```cpp
bool _initial
```

Defined in src/http/include/icy/http/form.h:172

---

{#_complete-1}

#### _complete

```cpp
bool _complete
```

Defined in src/http/include/icy/http/form.h:173

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormWriter`](#formwriter-2)  | Creates the [FormWriter](#formwriter) that uses the given encoding. |
|  | [`FormWriter`](#formwriter-3)  | Deleted constructor. |
|  | [`FormWriter`](#formwriter-4)  | Deleted constructor. |
| `void` | [`writePartHeader`](#writepartheader)  | Writes the message boundary std::string, followed by the message header to the output stream. |
| `void` | [`writeEnd`](#writeend)  | Writes the final boundary std::string to the output stream. |
| `void` | [`updateProgress`](#updateprogress) `virtual` | Updates the upload progress via the associated [ConnectionStream](#connectionstream-1) object. |

---

{#formwriter-2}

#### FormWriter

```cpp
FormWriter(ConnectionStream & conn, std::shared_ptr< Runner > runner, const std::string & encoding = FormWriter::ENCODING_URL)
```

Defined in src/http/include/icy/http/form.h:130

Creates the [FormWriter](#formwriter) that uses the given encoding.

---

{#formwriter-3}

#### FormWriter

```cpp
FormWriter(const FormWriter &) = delete
```

Defined in src/http/include/icy/http/form.h:133

Deleted constructor.

---

{#formwriter-4}

#### FormWriter

```cpp
FormWriter(FormWriter &&) = delete
```

Defined in src/http/include/icy/http/form.h:135

Deleted constructor.

---

{#writepartheader}

#### writePartHeader

```cpp
void writePartHeader(const NVCollection & header, std::ostream & ostr)
```

Defined in src/http/include/icy/http/form.h:140

Writes the message boundary std::string, followed by the message header to the output stream.

---

{#writeend}

#### writeEnd

```cpp
void writeEnd(std::ostream & ostr)
```

Defined in src/http/include/icy/http/form.h:143

Writes the final boundary std::string to the output stream.

---

{#updateprogress}

#### updateProgress

`virtual`

```cpp
virtual void updateProgress(int nread)
```

Defined in src/http/include/icy/http/form.h:150

Updates the upload progress via the associated [ConnectionStream](#connectionstream-1) object.

{#part}

## Part

```cpp
#include <icy/http/form.h>
```

```cpp
struct Part
```

Defined in src/http/include/icy/http/form.h:157

Individual part within a multipart form submission.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name-5)  |  |
| `std::unique_ptr< FormPart >` | [`part`](#part-1)  |  |

---

{#name-5}

#### name

```cpp
std::string name
```

Defined in src/http/include/icy/http/form.h:159

---

{#part-1}

#### part

```cpp
std::unique_ptr< FormPart > part
```

Defined in src/http/include/icy/http/form.h:160

{#message}

## Message

```cpp
#include <icy/http/message.h>
```

```cpp
class Message
```

Defined in src/http/include/icy/http/message.h:28

> **Inherits:** [`NVCollection`](base.md#nvcollection)
> **Subclassed by:** [`Request`](#request-4), [`Response`](#response-1)

The base class for [Request](#request-4) and [Response](#response-1).

Defines the common properties of all HTTP messages. These are version, content length, content type and transfer encoding.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`setVersion`](#setversion-1)  | Sets the HTTP version for this message. |
| `const std::string &` | [`getVersion`](#getversion-1) `const` `nodiscard` | Returns the HTTP version for this message. |
| `void` | [`setContentLength`](#setcontentlength)  | Sets the Content-Length header. |
| `uint64_t` | [`getContentLength`](#getcontentlength) `const` `nodiscard` | Returns the content length for this message, which may be UNKNOWN_CONTENT_LENGTH if no Content-Length header is present. |
| `bool` | [`hasContentLength`](#hascontentlength) `const` `nodiscard` | Returns true if a Content-Length header is present. |
| `void` | [`setTransferEncoding`](#settransferencoding)  | Sets the transfer encoding for this message. |
| `const std::string &` | [`getTransferEncoding`](#gettransferencoding) `const` `nodiscard` | Returns the transfer encoding used for this message. |
| `void` | [`setChunkedTransferEncoding`](#setchunkedtransferencoding)  | If flag is true, sets the Transfer-Encoding header to chunked. Otherwise, removes the Transfer-Encoding header. |
| `bool` | [`isChunkedTransferEncoding`](#ischunkedtransferencoding) `const` `nodiscard` | Returns true if the Transfer-Encoding header is set and its value is chunked. |
| `void` | [`setContentType`](#setcontenttype)  | Sets the content type for this message. |
| `const std::string &` | [`getContentType`](#getcontenttype) `const` `nodiscard` | Returns the content type for this message. |
| `void` | [`setKeepAlive`](#setkeepalive-1)  | Sets the value of the [Connection](#connection-1) header field. |
| `bool` | [`getKeepAlive`](#getkeepalive) `const` `nodiscard` | Returns true if |
| `void` | [`write`](#write-6) `virtual` `const` | Writes the message header to the given output stream. |
| `void` | [`write`](#write-7) `virtual` `const` | Writes the message header to the given output string. |
| `void` | [`write`](#write-8) `virtual` `const` | Writes the message header directly into a byte buffer. |

---

{#setversion-1}

#### setVersion

```cpp
void setVersion(const std::string & version)
```

Defined in src/http/include/icy/http/message.h:32

Sets the HTTP version for this message.

---

{#getversion-1}

#### getVersion

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getVersion() const
```

Defined in src/http/include/icy/http/message.h:35

Returns the HTTP version for this message.

---

{#setcontentlength}

#### setContentLength

```cpp
void setContentLength(uint64_t length)
```

Defined in src/http/include/icy/http/message.h:41

Sets the Content-Length header.

If length is UNKNOWN_CONTENT_LENGTH, removes the Content-Length header.

---

{#getcontentlength}

#### getContentLength

`const` `nodiscard`

```cpp
[[nodiscard]] uint64_t getContentLength() const
```

Defined in src/http/include/icy/http/message.h:46

Returns the content length for this message, which may be UNKNOWN_CONTENT_LENGTH if no Content-Length header is present.

---

{#hascontentlength}

#### hasContentLength

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasContentLength() const
```

Defined in src/http/include/icy/http/message.h:49

Returns true if a Content-Length header is present.

---

{#settransferencoding}

#### setTransferEncoding

```cpp
void setTransferEncoding(const std::string & transferEncoding)
```

Defined in src/http/include/icy/http/message.h:55

Sets the transfer encoding for this message.

The value should be either IDENTITY_TRANSFER_CODING or CHUNKED_TRANSFER_CODING.

---

{#gettransferencoding}

#### getTransferEncoding

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getTransferEncoding() const
```

Defined in src/http/include/icy/http/message.h:62

Returns the transfer encoding used for this message.

Normally, this is the value of the Transfer-Encoding header field. If no such field is present, returns IDENTITY_TRANSFER_CODING.

---

{#setchunkedtransferencoding}

#### setChunkedTransferEncoding

```cpp
void setChunkedTransferEncoding(bool flag)
```

Defined in src/http/include/icy/http/message.h:66

If flag is true, sets the Transfer-Encoding header to chunked. Otherwise, removes the Transfer-Encoding header.

---

{#ischunkedtransferencoding}

#### isChunkedTransferEncoding

`const` `nodiscard`

```cpp
[[nodiscard]] bool isChunkedTransferEncoding() const
```

Defined in src/http/include/icy/http/message.h:70

Returns true if the Transfer-Encoding header is set and its value is chunked.

---

{#setcontenttype}

#### setContentType

```cpp
void setContentType(const std::string & contentType)
```

Defined in src/http/include/icy/http/message.h:76

Sets the content type for this message.

Specify NO_CONTENT_TYPE to remove the Content-Type header.

---

{#getcontenttype}

#### getContentType

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getContentType() const
```

Defined in src/http/include/icy/http/message.h:82

Returns the content type for this message.

If no Content-Type header is present, returns UNKNOWN_CONTENT_TYPE.

---

{#setkeepalive-1}

#### setKeepAlive

```cpp
void setKeepAlive(bool keepAlive)
```

Defined in src/http/include/icy/http/message.h:88

Sets the value of the [Connection](#connection-1) header field.

The value is set to "Keep-Alive" if keepAlive is true, or to "Close" otherwise.

---

{#getkeepalive}

#### getKeepAlive

`const` `nodiscard`

```cpp
[[nodiscard]] bool getKeepAlive() const
```

Defined in src/http/include/icy/http/message.h:94

Returns true if

* the message has a [Connection](#connection-1) header field and its value is "Keep-Alive"

* the message is an HTTP/1.1 message and no [Connection](#connection-1) header is set Returns false otherwise.

---

{#write-6}

#### write

`virtual` `const`

```cpp
virtual void write(std::ostream & ostr) const
```

Defined in src/http/include/icy/http/message.h:102

Writes the message header to the given output stream.

The format is one name-value pair per line, with name and value separated by a colon and lines delimited by a carriage return and a linefeed character. See RFC 2822 for details.

---

{#write-7}

#### write

`virtual` `const`

```cpp
virtual void write(std::string & str) const
```

Defined in src/http/include/icy/http/message.h:105

Writes the message header to the given output string.

---

{#write-8}

#### write

`virtual` `const`

```cpp
virtual void write(Buffer & buf) const
```

Defined in src/http/include/icy/http/message.h:108

Writes the message header directly into a byte buffer.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const std::string` | [`HTTP_1_0`](#http_1_0) `static` |  |
| `const std::string` | [`HTTP_1_1`](#http_1_1) `static` |  |
| `const std::string` | [`IDENTITY_TRANSFER_ENCODING`](#identity_transfer_encoding) `static` |  |
| `const std::string` | [`CHUNKED_TRANSFER_ENCODING`](#chunked_transfer_encoding) `static` |  |
| `const int` | [`UNKNOWN_CONTENT_LENGTH`](#unknown_content_length) `static` |  |
| `const std::string` | [`UNKNOWN_CONTENT_TYPE`](#unknown_content_type) `static` |  |
| `const std::string` | [`CONTENT_LENGTH`](#content_length) `static` |  |
| `const std::string` | [`CONTENT_TYPE`](#content_type) `static` |  |
| `const std::string` | [`TRANSFER_ENCODING`](#transfer_encoding) `static` |  |
| `const std::string` | [`CONNECTION`](#connection-6) `static` |  |
| `const std::string` | [`CONNECTION_KEEP_ALIVE`](#connection_keep_alive) `static` |  |
| `const std::string` | [`CONNECTION_CLOSE`](#connection_close) `static` |  |
| `const std::string` | [`EMPTY`](#empty) `static` |  |

---

{#http_1_0}

#### HTTP_1_0

`static`

```cpp
const std::string HTTP_1_0
```

Defined in src/http/include/icy/http/message.h:110

---

{#http_1_1}

#### HTTP_1_1

`static`

```cpp
const std::string HTTP_1_1
```

Defined in src/http/include/icy/http/message.h:111

---

{#identity_transfer_encoding}

#### IDENTITY_TRANSFER_ENCODING

`static`

```cpp
const std::string IDENTITY_TRANSFER_ENCODING
```

Defined in src/http/include/icy/http/message.h:113

---

{#chunked_transfer_encoding}

#### CHUNKED_TRANSFER_ENCODING

`static`

```cpp
const std::string CHUNKED_TRANSFER_ENCODING
```

Defined in src/http/include/icy/http/message.h:114

---

{#unknown_content_length}

#### UNKNOWN_CONTENT_LENGTH

`static`

```cpp
const int UNKNOWN_CONTENT_LENGTH
```

Defined in src/http/include/icy/http/message.h:116

---

{#unknown_content_type}

#### UNKNOWN_CONTENT_TYPE

`static`

```cpp
const std::string UNKNOWN_CONTENT_TYPE
```

Defined in src/http/include/icy/http/message.h:117

---

{#content_length}

#### CONTENT_LENGTH

`static`

```cpp
const std::string CONTENT_LENGTH
```

Defined in src/http/include/icy/http/message.h:119

---

{#content_type}

#### CONTENT_TYPE

`static`

```cpp
const std::string CONTENT_TYPE
```

Defined in src/http/include/icy/http/message.h:120

---

{#transfer_encoding}

#### TRANSFER_ENCODING

`static`

```cpp
const std::string TRANSFER_ENCODING
```

Defined in src/http/include/icy/http/message.h:121

---

{#connection-6}

#### CONNECTION

`static`

```cpp
const std::string CONNECTION
```

Defined in src/http/include/icy/http/message.h:122

---

{#connection_keep_alive}

#### CONNECTION_KEEP_ALIVE

`static`

```cpp
const std::string CONNECTION_KEEP_ALIVE
```

Defined in src/http/include/icy/http/message.h:124

---

{#connection_close}

#### CONNECTION_CLOSE

`static`

```cpp
const std::string CONNECTION_CLOSE
```

Defined in src/http/include/icy/http/message.h:125

---

{#empty}

#### EMPTY

`static`

```cpp
const std::string EMPTY
```

Defined in src/http/include/icy/http/message.h:127

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_version`](#_version-1)  |  |

---

{#_version-1}

#### _version

```cpp
std::string _version
```

Defined in src/http/include/icy/http/message.h:130

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Message`](#message-1)  | Creates the [Message](#message) with version HTTP/1.0. |
|  | [`Message`](#message-2)  | Creates the [Message](#message) and sets the version. |
|  | [`~Message`](#message-3) `virtual` | Destroys the [Message](#message). |

---

{#message-1}

#### Message

```cpp
Message()
```

Defined in src/http/include/icy/http/message.h:133

Creates the [Message](#message) with version HTTP/1.0.

---

{#message-2}

#### Message

```cpp
Message(const std::string & version)
```

Defined in src/http/include/icy/http/message.h:137

Creates the [Message](#message) and sets the version.

---

{#message-3}

#### ~Message

`virtual`

```cpp
virtual ~Message()
```

Defined in src/http/include/icy/http/message.h:140

Destroys the [Message](#message).

{#multipartadapter}

## MultipartAdapter

```cpp
#include <icy/http/packetizers.h>
```

```cpp
class MultipartAdapter
```

Defined in src/http/include/icy/http/packetizers.h:159

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

HTTP multipart encoding adapter for multipart/x-mixed-replace streaming.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Connection::Ptr` | [`connection`](#connection-7)  | HTTP connection for sending the initial response header. |
| `std::string` | [`contentType`](#contenttype-2)  | Content-Type of each part (e.g. "image/jpeg"). |
| `bool` | [`isBase64`](#isbase64)  | If true, adds "Content-Transfer-Encoding: base64" to each part. |
| `bool` | [`initial`](#initial-1)  | True until the first chunk is processed and the boundary header emitted. |
| `PacketSignal` | [`emitter`](#emitter-7)  |  |

---

{#connection-7}

#### connection

```cpp
Connection::Ptr connection
```

Defined in src/http/include/icy/http/packetizers.h:162

HTTP connection for sending the initial response header.

---

{#contenttype-2}

#### contentType

```cpp
std::string contentType
```

Defined in src/http/include/icy/http/packetizers.h:163

Content-Type of each part (e.g. "image/jpeg").

---

{#isbase64}

#### isBase64

```cpp
bool isBase64
```

Defined in src/http/include/icy/http/packetizers.h:164

If true, adds "Content-Transfer-Encoding: base64" to each part.

---

{#initial-1}

#### initial

```cpp
bool initial
```

Defined in src/http/include/icy/http/packetizers.h:165

True until the first chunk is processed and the boundary header emitted.

---

{#emitter-7}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/http/include/icy/http/packetizers.h:264

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MultipartAdapter`](#multipartadapter-1) `inline` | Creates a [MultipartAdapter](#multipartadapter) that sends headers through the given connection. The per-part content type is read from the connection's outgoing header. |
|  | [`MultipartAdapter`](#multipartadapter-2) `inline` | Creates a [MultipartAdapter](#multipartadapter) that emits its own raw HTTP/1.1 200 response header. Use this when no [Connection](#connection-1) object is available. |
| `void` | [`emitHeader`](#emitheader-1) `virtual` `inline` | Emits the initial HTTP/1.1 200 OK response with Content-Type multipart/x-mixed-replace. If a connection is set, headers are written through it; otherwise a raw response string is emitted. |
| `void` | [`emitChunkHeader`](#emitchunkheader) `virtual` `inline` | Emits the MIME boundary and per-part headers (Content-Type, optionally Content-Transfer-Encoding) for the next multipart chunk. |
| `void` | [`process`](#process-6) `virtual` `inline` | Wraps the incoming packet as a multipart chunk and emits it downstream. Emits the multipart HTTP response headers on the first call. |

---

{#multipartadapter-1}

#### MultipartAdapter

`inline`

```cpp
inline MultipartAdapter(Connection::Ptr connection, bool base64 = false)
```

Defined in src/http/include/icy/http/packetizers.h:171

Creates a [MultipartAdapter](#multipartadapter) that sends headers through the given connection. The per-part content type is read from the connection's outgoing header. 
#### Parameters
* `connection` HTTP connection to use for sending the initial multipart header. 

* `base64` If true, indicates parts are base64-encoded.

---

{#multipartadapter-2}

#### MultipartAdapter

`inline`

```cpp
inline MultipartAdapter(const std::string & contentType, bool base64 = false)
```

Defined in src/http/include/icy/http/packetizers.h:184

Creates a [MultipartAdapter](#multipartadapter) that emits its own raw HTTP/1.1 200 response header. Use this when no [Connection](#connection-1) object is available. 
#### Parameters
* `contentType` Content-Type for each multipart part. 

* `base64` If true, indicates parts are base64-encoded.

---

{#emitheader-1}

#### emitHeader

`virtual` `inline`

```cpp
virtual inline void emitHeader()
```

Defined in src/http/include/icy/http/packetizers.h:197

Emits the initial HTTP/1.1 200 OK response with Content-Type multipart/x-mixed-replace. If a connection is set, headers are written through it; otherwise a raw response string is emitted.

---

{#emitchunkheader}

#### emitChunkHeader

`virtual` `inline`

```cpp
virtual inline void emitChunkHeader()
```

Defined in src/http/include/icy/http/packetizers.h:231

Emits the MIME boundary and per-part headers (Content-Type, optionally Content-Transfer-Encoding) for the next multipart chunk.

---

{#process-6}

#### process

`virtual` `inline`

```cpp
virtual inline void process(IPacket & packet)
```

Defined in src/http/include/icy/http/packetizers.h:248

Wraps the incoming packet as a multipart chunk and emits it downstream. Emits the multipart HTTP response headers on the first call. 
#### Parameters
* `packet` Packet containing the raw payload data.

{#parser-1}

## Parser

```cpp
#include <icy/http/parser.h>
```

```cpp
class Parser
```

Defined in src/http/include/icy/http/parser.h:55

HTTP request/response parser using the llhttp library.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Parser`](#parser-2)  | Creates a response parser. The response object is populated as data is parsed. |
|  | [`Parser`](#parser-3)  | Creates a request parser. The request object is populated as data is parsed. |
|  | [`Parser`](#parser-4)  | Creates a parser of the given type without binding a message object. |
|  | [`Parser`](#parser-5)  | Deleted constructor. |
|  | [`Parser`](#parser-6)  | Deleted constructor. |
| `ParseResult` | [`parse`](#parse)  | Feeds a buffer of raw HTTP data into the parser. |
| `void` | [`reset`](#reset-10)  | Reset the internal state (reinitialises llhttp). Safe to call externally, NOT from inside llhttp callbacks. |
| `void` | [`resetState`](#resetstate)  | Reset internal flags without reinitialising llhttp. Safe to call from inside llhttp callbacks (e.g. on_message_begin). |
| `bool` | [`complete`](#complete-2) `const` `nodiscard` | Returns true if parsing is complete, either in success or error. |
| `bool` | [`upgrade`](#upgrade) `const` `nodiscard` | Returns true if the connection should be upgraded. |
| `llhttp_type_t` | [`type`](#type-9) `const` `inline` `nodiscard` | Returns the parser type (HTTP_REQUEST or HTTP_RESPONSE). |
| `void` | [`setRequest`](#setrequest)  | Binds an HTTP request object to populate during parsing. Must only be called when no message is currently set and type is HTTP_REQUEST. |
| `void` | [`setResponse`](#setresponse)  | Binds an HTTP response object to populate during parsing. Must only be called when no message is currently set and type is HTTP_RESPONSE. |
| `void` | [`setObserver`](#setobserver)  | Sets the observer that receives parser events. |
| `void` | [`clearMessage`](#clearmessage)  | Clear request/response pointers so they can be re-set. Used when resetting a pooled connection for reuse. |
| `http::Message *` | [`message`](#message-4) `nodiscard` | Returns the currently bound message (request or response), or nullptr. |
| `ParserObserver *` | [`observer`](#observer) `const` `nodiscard` | Returns the current parser observer, or nullptr if none is set. |

---

{#parser-2}

#### Parser

```cpp
Parser(http::Response * response)
```

Defined in src/http/include/icy/http/parser.h:70

Creates a response parser. The response object is populated as data is parsed. 
#### Parameters
* `response` HTTP response object to populate.

---

{#parser-3}

#### Parser

```cpp
Parser(http::Request * request)
```

Defined in src/http/include/icy/http/parser.h:74

Creates a request parser. The request object is populated as data is parsed. 
#### Parameters
* `request` HTTP request object to populate.

---

{#parser-4}

#### Parser

```cpp
Parser(llhttp_type_t type)
```

Defined in src/http/include/icy/http/parser.h:78

Creates a parser of the given type without binding a message object. 
#### Parameters
* `type` Either HTTP_REQUEST or HTTP_RESPONSE.

---

{#parser-5}

#### Parser

```cpp
Parser(const Parser &) = delete
```

Defined in src/http/include/icy/http/parser.h:81

Deleted constructor.

---

{#parser-6}

#### Parser

```cpp
Parser(Parser &&) = delete
```

Defined in src/http/include/icy/http/parser.h:83

Deleted constructor.

---

{#parse}

#### parse

```cpp
ParseResult parse(const char * data, size_t length)
```

Defined in src/http/include/icy/http/parser.h:93

Feeds a buffer of raw HTTP data into the parser.

May be called multiple times for streaming data. The parser state persists between calls. On completion or error, the observer is notified. 
#### Parameters
* `data` Pointer to the input data buffer. 

* `length` Number of bytes in the buffer. 

#### Returns
Structured parse result including bytes consumed and terminal state.

---

{#reset-10}

#### reset

```cpp
void reset()
```

Defined in src/http/include/icy/http/parser.h:97

Reset the internal state (reinitialises llhttp). Safe to call externally, NOT from inside llhttp callbacks.

---

{#resetstate}

#### resetState

```cpp
void resetState()
```

Defined in src/http/include/icy/http/parser.h:101

Reset internal flags without reinitialising llhttp. Safe to call from inside llhttp callbacks (e.g. on_message_begin).

---

{#complete-2}

#### complete

`const` `nodiscard`

```cpp
[[nodiscard]] bool complete() const
```

Defined in src/http/include/icy/http/parser.h:105

Returns true if parsing is complete, either in success or error.

---

{#upgrade}

#### upgrade

`const` `nodiscard`

```cpp
[[nodiscard]] bool upgrade() const
```

Defined in src/http/include/icy/http/parser.h:108

Returns true if the connection should be upgraded.

---

{#type-9}

#### type

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline llhttp_type_t type() const
```

Defined in src/http/include/icy/http/parser.h:111

Returns the parser type (HTTP_REQUEST or HTTP_RESPONSE).

---

{#setrequest}

#### setRequest

```cpp
void setRequest(http::Request * request)
```

Defined in src/http/include/icy/http/parser.h:116

Binds an HTTP request object to populate during parsing. Must only be called when no message is currently set and type is HTTP_REQUEST. 
#### Parameters
* `request` The request object to populate.

---

{#setresponse}

#### setResponse

```cpp
void setResponse(http::Response * response)
```

Defined in src/http/include/icy/http/parser.h:121

Binds an HTTP response object to populate during parsing. Must only be called when no message is currently set and type is HTTP_RESPONSE. 
#### Parameters
* `response` The response object to populate.

---

{#setobserver}

#### setObserver

```cpp
void setObserver(ParserObserver * observer)
```

Defined in src/http/include/icy/http/parser.h:125

Sets the observer that receives parser events. 
#### Parameters
* `observer` Observer to notify. May be nullptr to clear.

---

{#clearmessage}

#### clearMessage

```cpp
void clearMessage()
```

Defined in src/http/include/icy/http/parser.h:129

Clear request/response pointers so they can be re-set. Used when resetting a pooled connection for reuse.

---

{#message-4}

#### message

`nodiscard`

```cpp
[[nodiscard]] http::Message * message()
```

Defined in src/http/include/icy/http/parser.h:132

Returns the currently bound message (request or response), or nullptr.

---

{#observer}

#### observer

`const` `nodiscard`

```cpp
[[nodiscard]] ParserObserver * observer() const
```

Defined in src/http/include/icy/http/parser.h:135

Returns the current parser observer, or nullptr if none is set.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ParserObserver *` | [`_observer`](#_observer)  |  |
| `http::Request *` | [`_request`](#_request-1)  |  |
| `http::Response *` | [`_response`](#_response-1)  |  |
| `http::Message *` | [`_message`](#_message)  |  |
| `llhttp_t` | [`_parser`](#_parser-1)  |  |
| `llhttp_settings_t` | [`_settings`](#_settings)  |  |
| `llhttp_type_t` | [`_type`](#_type)  |  |
| `bool` | [`_wasHeaderValue`](#_washeadervalue)  |  |
| `std::string` | [`_lastHeaderField`](#_lastheaderfield)  |  |
| `std::string` | [`_lastHeaderValue`](#_lastheadervalue)  |  |
| `bool` | [`_complete`](#_complete-2)  |  |
| `bool` | [`_upgrade`](#_upgrade)  |  |
| `Error` | [`_error`](#_error-3)  |  |
| `ParseResult` | [`_lastResult`](#_lastresult)  |  |
| `MessageScratch` | [`_scratch`](#_scratch)  |  |

---

{#_observer}

#### _observer

```cpp
ParserObserver * _observer
```

Defined in src/http/include/icy/http/parser.h:181

---

{#_request-1}

#### _request

```cpp
http::Request * _request
```

Defined in src/http/include/icy/http/parser.h:182

---

{#_response-1}

#### _response

```cpp
http::Response * _response
```

Defined in src/http/include/icy/http/parser.h:183

---

{#_message}

#### _message

```cpp
http::Message * _message
```

Defined in src/http/include/icy/http/parser.h:184

---

{#_parser-1}

#### _parser

```cpp
llhttp_t _parser
```

Defined in src/http/include/icy/http/parser.h:186

---

{#_settings}

#### _settings

```cpp
llhttp_settings_t _settings
```

Defined in src/http/include/icy/http/parser.h:187

---

{#_type}

#### _type

```cpp
llhttp_type_t _type
```

Defined in src/http/include/icy/http/parser.h:188

---

{#_washeadervalue}

#### _wasHeaderValue

```cpp
bool _wasHeaderValue
```

Defined in src/http/include/icy/http/parser.h:190

---

{#_lastheaderfield}

#### _lastHeaderField

```cpp
std::string _lastHeaderField
```

Defined in src/http/include/icy/http/parser.h:191

---

{#_lastheadervalue}

#### _lastHeaderValue

```cpp
std::string _lastHeaderValue
```

Defined in src/http/include/icy/http/parser.h:192

---

{#_complete-2}

#### _complete

```cpp
bool _complete
```

Defined in src/http/include/icy/http/parser.h:194

---

{#_upgrade}

#### _upgrade

```cpp
bool _upgrade
```

Defined in src/http/include/icy/http/parser.h:195

---

{#_error-3}

#### _error

```cpp
Error _error
```

Defined in src/http/include/icy/http/parser.h:197

---

{#_lastresult}

#### _lastResult

```cpp
ParseResult _lastResult
```

Defined in src/http/include/icy/http/parser.h:198

---

{#_scratch}

#### _scratch

```cpp
MessageScratch _scratch
```

Defined in src/http/include/icy/http/parser.h:199

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-8)  |  |
| `void` | [`clearBoundMessage`](#clearboundmessage)  |  |
| `void` | [`storeHeader`](#storeheader)  |  |
| `void` | [`applyScratchToBoundMessage`](#applyscratchtoboundmessage)  |  |
| `void` | [`onHeader`](#onheader)  | Callbacks. |
| `void` | [`onHeadersEnd`](#onheadersend)  |  |
| `void` | [`onBody`](#onbody)  |  |
| `void` | [`onMessageEnd`](#onmessageend)  |  |
| `void` | [`onError`](#onerror-3)  |  |

---

{#init-8}

#### init

```cpp
void init()
```

Defined in src/http/include/icy/http/parser.h:158

---

{#clearboundmessage}

#### clearBoundMessage

```cpp
void clearBoundMessage()
```

Defined in src/http/include/icy/http/parser.h:159

---

{#storeheader}

#### storeHeader

```cpp
void storeHeader(std::string name, std::string value)
```

Defined in src/http/include/icy/http/parser.h:160

---

{#applyscratchtoboundmessage}

#### applyScratchToBoundMessage

```cpp
void applyScratchToBoundMessage()
```

Defined in src/http/include/icy/http/parser.h:161

---

{#onheader}

#### onHeader

```cpp
void onHeader(std::string name, std::string value)
```

Defined in src/http/include/icy/http/parser.h:164

Callbacks.

---

{#onheadersend}

#### onHeadersEnd

```cpp
void onHeadersEnd()
```

Defined in src/http/include/icy/http/parser.h:165

---

{#onbody}

#### onBody

```cpp
void onBody(const char * buf, size_t len)
```

Defined in src/http/include/icy/http/parser.h:166

---

{#onmessageend}

#### onMessageEnd

```cpp
void onMessageEnd()
```

Defined in src/http/include/icy/http/parser.h:167

---

{#onerror-3}

#### onError

```cpp
void onError(llhttp_errno_t errnum, const std::string & message = "")
```

Defined in src/http/include/icy/http/parser.h:168

{#parseresult}

## ParseResult

```cpp
#include <icy/http/parser.h>
```

```cpp
struct ParseResult
```

Defined in src/http/include/icy/http/parser.h:58

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`bytesConsumed`](#bytesconsumed)  |  |
| `bool` | [`messageComplete`](#messagecomplete)  |  |
| `bool` | [`upgrade`](#upgrade-1)  |  |
| `Error` | [`error`](#error-11)  |  |

---

{#bytesconsumed}

#### bytesConsumed

```cpp
size_t bytesConsumed = 0
```

Defined in src/http/include/icy/http/parser.h:60

---

{#messagecomplete}

#### messageComplete

```cpp
bool messageComplete = false
```

Defined in src/http/include/icy/http/parser.h:61

---

{#upgrade-1}

#### upgrade

```cpp
bool upgrade = false
```

Defined in src/http/include/icy/http/parser.h:62

---

{#error-11}

#### error

```cpp
Error error
```

Defined in src/http/include/icy/http/parser.h:63

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`ok`](#ok) `const` `inline` `nodiscard` |  |

---

{#ok}

#### ok

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool ok() const
```

Defined in src/http/include/icy/http/parser.h:65

{#messagescratch}

## MessageScratch

```cpp
#include <icy/http/parser.h>
```

```cpp
struct MessageScratch
```

Defined in src/http/include/icy/http/parser.h:138

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`version`](#version-2)  |  |
| `std::string` | [`method`](#method-1)  |  |
| `std::string` | [`uri`](#uri)  |  |
| `std::string` | [`reason`](#reason)  |  |
| `http::StatusCode` | [`status`](#status-2)  |  |
| `std::vector< std::pair< std::string, std::string > >` | [`headers`](#headers-2)  |  |

---

{#version-2}

#### version

```cpp
std::string version
```

Defined in src/http/include/icy/http/parser.h:140

---

{#method-1}

#### method

```cpp
std::string method
```

Defined in src/http/include/icy/http/parser.h:141

---

{#uri}

#### uri

```cpp
std::string uri
```

Defined in src/http/include/icy/http/parser.h:142

---

{#reason}

#### reason

```cpp
std::string reason
```

Defined in src/http/include/icy/http/parser.h:143

---

{#status-2}

#### status

```cpp
http::StatusCode status = 
```

Defined in src/http/include/icy/http/parser.h:144

---

{#headers-2}

#### headers

```cpp
std::vector< std::pair< std::string, std::string > > headers
```

Defined in src/http/include/icy/http/parser.h:145

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`reset`](#reset-11) `inline` |  |

---

{#reset-11}

#### reset

`inline`

```cpp
inline void reset()
```

Defined in src/http/include/icy/http/parser.h:147

{#parserobserver}

## ParserObserver

```cpp
#include <icy/http/parser.h>
```

```cpp
class ParserObserver
```

Defined in src/http/include/icy/http/parser.h:27

> **Subclassed by:** [`ConnectionAdapter`](#connectionadapter-1)

Abstract observer interface for HTTP parser events.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onParserHeader`](#onparserheader-1) `virtual` | Called for each parsed HTTP header name/value pair. |
| `void` | [`onParserHeadersEnd`](#onparserheadersend-1) `virtual` | Called when all HTTP headers have been parsed. |
| `void` | [`onParserChunk`](#onparserchunk-1) `virtual` | Called for each chunk of body data received. |
| `void` | [`onParserEnd`](#onparserend-1) `virtual` | Called when the HTTP message is fully parsed. |
| `void` | [`onParserError`](#onparsererror-1) `virtual` | Called when a parse error occurs. |

---

{#onparserheader-1}

#### onParserHeader

`virtual`

```cpp
virtual void onParserHeader(const std::string & name, const std::string & value)
```

Defined in src/http/include/icy/http/parser.h:33

Called for each parsed HTTP header name/value pair. 
#### Parameters
* `name` Header field name. 

* `value` Header field value.

---

{#onparserheadersend-1}

#### onParserHeadersEnd

`virtual`

```cpp
virtual void onParserHeadersEnd(bool upgrade)
```

Defined in src/http/include/icy/http/parser.h:37

Called when all HTTP headers have been parsed. 
#### Parameters
* `upgrade` True if the connection should be upgraded (e.g. to WebSocket).

---

{#onparserchunk-1}

#### onParserChunk

`virtual`

```cpp
virtual void onParserChunk(const char * data, size_t len)
```

Defined in src/http/include/icy/http/parser.h:42

Called for each chunk of body data received. 
#### Parameters
* `data` Pointer to the body data chunk. 

* `len` Length of the chunk in bytes.

---

{#onparserend-1}

#### onParserEnd

`virtual`

```cpp
virtual void onParserEnd()
```

Defined in src/http/include/icy/http/parser.h:45

Called when the HTTP message is fully parsed.

---

{#onparsererror-1}

#### onParserError

`virtual`

```cpp
virtual void onParserError(const Error & err)
```

Defined in src/http/include/icy/http/parser.h:49

Called when a parse error occurs. 
#### Parameters
* `err` [Error](base.md#error) details from llhttp.

{#progresssignal}

## ProgressSignal

```cpp
#include <icy/http/connection.h>
```

```cpp
class ProgressSignal
```

Defined in src/http/include/icy/http/connection.h:223

> **Inherits:** [`Signal< void(const double &)>`](base.md#signal)

HTTP progress signal for upload and download progress notifications.

Emits a double in the range [0, 100] as data is transferred. Set `[total](#total-1)` to the expected byte count before calling `[update()](#update-1)`.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `void *` | [`sender`](#sender-1)  | Optional context pointer identifying the sender. |
| `uint64_t` | [`current`](#current-1)  | Bytes transferred so far. |
| `uint64_t` | [`total`](#total-1)  | Total expected bytes (from Content-Length). |

---

{#sender-1}

#### sender

```cpp
void * sender
```

Defined in src/http/include/icy/http/connection.h:226

Optional context pointer identifying the sender.

---

{#current-1}

#### current

```cpp
uint64_t current
```

Defined in src/http/include/icy/http/connection.h:227

Bytes transferred so far.

---

{#total-1}

#### total

```cpp
uint64_t total
```

Defined in src/http/include/icy/http/connection.h:228

Total expected bytes (from Content-Length).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ProgressSignal`](#progresssignal-1) `inline` |  |
| `double` | [`progress`](#progress) `const` `inline` `nodiscard` | Returns the current transfer progress as a percentage (0-100). |
| `void` | [`update`](#update-1) `inline` | Advances the progress counter by `nread` bytes and emits the updated percentage. |

---

{#progresssignal-1}

#### ProgressSignal

`inline`

```cpp
inline ProgressSignal()
```

Defined in src/http/include/icy/http/connection.h:230

---

{#progress}

#### progress

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline double progress() const
```

Defined in src/http/include/icy/http/connection.h:238

Returns the current transfer progress as a percentage (0-100).

---

{#update-1}

#### update

`inline`

```cpp
inline void update(int nread)
```

Defined in src/http/include/icy/http/connection.h:243

Advances the progress counter by `nread` bytes and emits the updated percentage. 
#### Parameters
* `nread` Number of bytes just transferred. 

#### Exceptions
* `std::runtime_error` if current would exceed total.

{#request-4}

## Request

```cpp
#include <icy/http/request.h>
```

```cpp
class Request
```

Defined in src/http/include/icy/http/request.h:44

> **Inherits:** [`Message`](#message)

HTTP request message with method, URI, headers, and optional body.

In addition to the properties common to all HTTP messages, an HTTP request has a method (e.g. GET, HEAD, POST, etc.) and a request URI.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-17) `inline` |  |

---

{#operator-17}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const Request & req)
```

Defined in src/http/include/icy/http/request.h:146

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Request`](#request-5)  | Creates a GET / HTTP/1.1 HTTP request. |
|  | [`Request`](#request-6)  | Creates a GET / HTTP/1.x request with the given version (HTTP/1.0 or HTTP/1.1). |
|  | [`Request`](#request-7)  | Creates an HTTP/1.0 request with the given method and URI. |
|  | [`Request`](#request-8)  | Creates an HTTP request with the given method, URI and version. |
|  | [`~Request`](#request-9) `virtual` | Destroys the [Request](#request-4). |
| `void` | [`setMethod`](#setmethod)  | Sets the method. |
| `const std::string &` | [`getMethod`](#getmethod) `const` `nodiscard` | Returns the method. |
| `void` | [`setURI`](#seturi)  | Sets the request URI. |
| `void` | [`appendURI`](#appenduri)  | Appends a fragment to the request URI. Used by the parser when llhttp splits the [URL](#url) across callbacks. |
| `const std::string &` | [`getURI`](#geturi) `const` `nodiscard` | Returns the request URI. |
| `void` | [`setHost`](#sethost)  | Sets the value of the Host header field. |
| `void` | [`setHost`](#sethost-1)  | Sets the value of the Host header field. |
| `const std::string &` | [`getHost`](#gethost) `const` `nodiscard` | Returns the value of the Host header field. |
| `void` | [`setCookies`](#setcookies)  | Adds a [Cookie](#cookie) header with the names and values from cookies. |
| `void` | [`getCookies`](#getcookies) `const` | Fills cookies with the cookies extracted from the [Cookie](#cookie) headers in the request. |
| `void` | [`getURIParameters`](#geturiparameters) `const` | Returns the request URI parameters. |
| `bool` | [`hasCredentials`](#hascredentials) `const` `nodiscard` | Returns true if the request contains authentication information in the form of an Authorization header. |
| `void` | [`getCredentials`](#getcredentials) `const` | Returns the authentication scheme and additional authentication information contained in this request. |
| `void` | [`setCredentials`](#setcredentials)  | Sets the authentication scheme and information for this request. |
| `bool` | [`hasProxyCredentials`](#hasproxycredentials) `const` `nodiscard` | Returns true if the request contains proxy authentication information in the form of an Proxy-Authorization header. |
| `void` | [`getProxyCredentials`](#getproxycredentials) `const` | Returns the proxy authentication scheme and additional proxy authentication information contained in this request. |
| `void` | [`setProxyCredentials`](#setproxycredentials)  | Sets the proxy authentication scheme and information for this request. |
| `void` | [`write`](#write-9) `virtual` `const` | Writes the HTTP request to the given output stream. |
| `void` | [`write`](#write-10) `virtual` `const` | Writes the HTTP request to the given output string. |
| `void` | [`write`](#write-11) `virtual` `const` | Writes the HTTP request directly into a byte buffer. |

---

{#request-5}

#### Request

```cpp
Request()
```

Defined in src/http/include/icy/http/request.h:48

Creates a GET / HTTP/1.1 HTTP request.

---

{#request-6}

#### Request

```cpp
Request(const std::string & version)
```

Defined in src/http/include/icy/http/request.h:52

Creates a GET / HTTP/1.x request with the given version (HTTP/1.0 or HTTP/1.1).

---

{#request-7}

#### Request

```cpp
Request(const std::string & method, const std::string & uri)
```

Defined in src/http/include/icy/http/request.h:55

Creates an HTTP/1.0 request with the given method and URI.

---

{#request-8}

#### Request

```cpp
Request(const std::string & method, const std::string & uri, const std::string & version)
```

Defined in src/http/include/icy/http/request.h:58

Creates an HTTP request with the given method, URI and version.

---

{#request-9}

#### ~Request

`virtual`

```cpp
virtual ~Request()
```

Defined in src/http/include/icy/http/request.h:61

Destroys the [Request](#request-4).

---

{#setmethod}

#### setMethod

```cpp
void setMethod(const std::string & method)
```

Defined in src/http/include/icy/http/request.h:64

Sets the method.

---

{#getmethod}

#### getMethod

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getMethod() const
```

Defined in src/http/include/icy/http/request.h:67

Returns the method.

---

{#seturi}

#### setURI

```cpp
void setURI(std::string uri)
```

Defined in src/http/include/icy/http/request.h:70

Sets the request URI.

---

{#appenduri}

#### appendURI

```cpp
void appendURI(std::string_view uri)
```

Defined in src/http/include/icy/http/request.h:74

Appends a fragment to the request URI. Used by the parser when llhttp splits the [URL](#url) across callbacks.

---

{#geturi}

#### getURI

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getURI() const
```

Defined in src/http/include/icy/http/request.h:77

Returns the request URI.

---

{#sethost}

#### setHost

```cpp
void setHost(const std::string & host)
```

Defined in src/http/include/icy/http/request.h:80

Sets the value of the Host header field.

---

{#sethost-1}

#### setHost

```cpp
void setHost(const std::string & host, uint16_t port)
```

Defined in src/http/include/icy/http/request.h:87

Sets the value of the Host header field.

If the given port number is a non-standard port number (other than 80 or 443), it is included in the Host header field.

---

{#gethost}

#### getHost

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getHost() const
```

Defined in src/http/include/icy/http/request.h:93

Returns the value of the Host header field.

Throws a NotFoundException if the request does not have a Host header field.

---

{#setcookies}

#### setCookies

```cpp
void setCookies(const NVCollection & cookies)
```

Defined in src/http/include/icy/http/request.h:97

Adds a [Cookie](#cookie) header with the names and values from cookies.

---

{#getcookies}

#### getCookies

`const`

```cpp
void getCookies(NVCollection & cookies) const
```

Defined in src/http/include/icy/http/request.h:101

Fills cookies with the cookies extracted from the [Cookie](#cookie) headers in the request.

---

{#geturiparameters}

#### getURIParameters

`const`

```cpp
void getURIParameters(NVCollection & params) const
```

Defined in src/http/include/icy/http/request.h:104

Returns the request URI parameters.

---

{#hascredentials}

#### hasCredentials

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasCredentials() const
```

Defined in src/http/include/icy/http/request.h:108

Returns true if the request contains authentication information in the form of an Authorization header.

---

{#getcredentials}

#### getCredentials

`const`

```cpp
void getCredentials(std::string & scheme, std::string & authInfo) const
```

Defined in src/http/include/icy/http/request.h:115

Returns the authentication scheme and additional authentication information contained in this request.

Throws a std::exception if no authentication information is contained in the request.

---

{#setcredentials}

#### setCredentials

```cpp
void setCredentials(std::string_view scheme, std::string_view authInfo)
```

Defined in src/http/include/icy/http/request.h:119

Sets the authentication scheme and information for this request.

---

{#hasproxycredentials}

#### hasProxyCredentials

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasProxyCredentials() const
```

Defined in src/http/include/icy/http/request.h:123

Returns true if the request contains proxy authentication information in the form of an Proxy-Authorization header.

---

{#getproxycredentials}

#### getProxyCredentials

`const`

```cpp
void getProxyCredentials(std::string & scheme, std::string & authInfo) const
```

Defined in src/http/include/icy/http/request.h:131

Returns the proxy authentication scheme and additional proxy authentication information contained in this request.

Throws a std::exception if no proxy authentication information is contained in the request.

---

{#setproxycredentials}

#### setProxyCredentials

```cpp
void setProxyCredentials(std::string_view scheme, std::string_view authInfo)
```

Defined in src/http/include/icy/http/request.h:134

Sets the proxy authentication scheme and information for this request.

---

{#write-9}

#### write

`virtual` `const`

```cpp
virtual void write(std::ostream & ostr) const
```

Defined in src/http/include/icy/http/request.h:138

Writes the HTTP request to the given output stream.

---

{#write-10}

#### write

`virtual` `const`

```cpp
virtual void write(std::string & str) const
```

Defined in src/http/include/icy/http/request.h:141

Writes the HTTP request to the given output string.

---

{#write-11}

#### write

`virtual` `const`

```cpp
virtual void write(Buffer & buf) const
```

Defined in src/http/include/icy/http/request.h:144

Writes the HTTP request directly into a byte buffer.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`getCredentials`](#getcredentials-1) `const` | Returns the authentication scheme and additional authentication information contained in the given header of request. |
| `void` | [`setCredentials`](#setcredentials-1)  | Writes the authentication scheme and information for this request to the given header. |

---

{#getcredentials-1}

#### getCredentials

`const`

```cpp
void getCredentials(const std::string & header, std::string & scheme, std::string & authInfo) const
```

Defined in src/http/include/icy/http/request.h:158

Returns the authentication scheme and additional authentication information contained in the given header of request.

Throws a NotAuthenticatedException if no authentication information is contained in the request.

---

{#setcredentials-1}

#### setCredentials

```cpp
void setCredentials(const std::string & header, std::string_view scheme, std::string_view authInfo)
```

Defined in src/http/include/icy/http/request.h:162

Writes the authentication scheme and information for this request to the given header.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_method`](#_method)  |  |
| `std::string` | [`_uri`](#_uri)  |  |

---

{#_method}

#### _method

```cpp
std::string _method
```

Defined in src/http/include/icy/http/request.h:165

---

{#_uri}

#### _uri

```cpp
std::string _uri
```

Defined in src/http/include/icy/http/request.h:166

{#response-1}

## Response

```cpp
#include <icy/http/response.h>
```

```cpp
class Response
```

Defined in src/http/include/icy/http/response.h:78

> **Inherits:** [`Message`](#message)

HTTP response message with status, reason phrase, headers, and body metadata.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-18) `inline` |  |

---

{#operator-18}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const Response & res)
```

Defined in src/http/include/icy/http/response.h:145

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Response`](#response-2)  | Creates the [Response](#response-1) with OK status. |
|  | [`Response`](#response-3)  | Creates the [Response](#response-1) with the given status and reason phrase. |
|  | [`Response`](#response-4)  | Creates the [Response](#response-1) with the given version, status and reason phrase. |
|  | [`Response`](#response-5)  | Creates the [Response](#response-1) with the given status and an appropriate reason phrase. |
|  | [`Response`](#response-6)  | Creates the [Response](#response-1) with the given version, status and an appropriate reason phrase. |
|  | [`~Response`](#response-7) `virtual` | Destroys the [Response](#response-1). |
| `void` | [`setStatus`](#setstatus)  | Sets the HTTP status code. |
| `StatusCode` | [`getStatus`](#getstatus) `const` `nodiscard` | Returns the HTTP status code. |
| `void` | [`setReason`](#setreason)  | Sets the HTTP reason phrase. |
| `const std::string &` | [`getReason`](#getreason) `const` `nodiscard` | Returns the HTTP reason phrase. |
| `void` | [`setStatusAndReason`](#setstatusandreason)  | Sets the HTTP status code and reason phrase. |
| `void` | [`setDate`](#setdate)  | Sets the Date header to the given date/time value. |
| `Timestamp` | [`getDate`](#getdate) `const` `nodiscard` | Returns the value of the Date header. |
| `void` | [`addCookie`](#addcookie)  | Adds the cookie to the response by adding a Set-Cookie header. |
| `void` | [`getCookies`](#getcookies-1) `const` | Returns a vector with all the cookies set in the response header. |
| `void` | [`write`](#write-12) `virtual` `const` | Writes the HTTP response headers to the given output stream. |
| `void` | [`write`](#write-13) `virtual` `const` | Writes the HTTP response headers to the given output string. |
| `void` | [`write`](#write-14) `virtual` `const` | Writes the HTTP response headers directly into a byte buffer. |
| `bool` | [`success`](#success) `virtual` `const` `nodiscard` | Returns true if the HTTP response code was successful (< 400). |

---

{#response-2}

#### Response

```cpp
Response()
```

Defined in src/http/include/icy/http/response.h:82

Creates the [Response](#response-1) with OK status.

---

{#response-3}

#### Response

```cpp
Response(StatusCode status, const std::string & reason)
```

Defined in src/http/include/icy/http/response.h:85

Creates the [Response](#response-1) with the given status and reason phrase.

---

{#response-4}

#### Response

```cpp
Response(const std::string & version, StatusCode status, const std::string & reason)
```

Defined in src/http/include/icy/http/response.h:88

Creates the [Response](#response-1) with the given version, status and reason phrase.

---

{#response-5}

#### Response

```cpp
Response(StatusCode status)
```

Defined in src/http/include/icy/http/response.h:92

Creates the [Response](#response-1) with the given status and an appropriate reason phrase.

---

{#response-6}

#### Response

```cpp
Response(const std::string & version, StatusCode status)
```

Defined in src/http/include/icy/http/response.h:96

Creates the [Response](#response-1) with the given version, status and an appropriate reason phrase.

---

{#response-7}

#### ~Response

`virtual`

```cpp
virtual ~Response()
```

Defined in src/http/include/icy/http/response.h:99

Destroys the [Response](#response-1).

---

{#setstatus}

#### setStatus

```cpp
void setStatus(StatusCode status)
```

Defined in src/http/include/icy/http/response.h:104

Sets the HTTP status code.

The reason phrase is set according to the status code.

---

{#getstatus}

#### getStatus

`const` `nodiscard`

```cpp
[[nodiscard]] StatusCode getStatus() const
```

Defined in src/http/include/icy/http/response.h:107

Returns the HTTP status code.

---

{#setreason}

#### setReason

```cpp
void setReason(const std::string & reason)
```

Defined in src/http/include/icy/http/response.h:110

Sets the HTTP reason phrase.

---

{#getreason}

#### getReason

`const` `nodiscard`

```cpp
[[nodiscard]] const std::string & getReason() const
```

Defined in src/http/include/icy/http/response.h:113

Returns the HTTP reason phrase.

---

{#setstatusandreason}

#### setStatusAndReason

```cpp
void setStatusAndReason(StatusCode status, const std::string & reason)
```

Defined in src/http/include/icy/http/response.h:116

Sets the HTTP status code and reason phrase.

---

{#setdate}

#### setDate

```cpp
void setDate(const Timestamp & dateTime)
```

Defined in src/http/include/icy/http/response.h:119

Sets the Date header to the given date/time value.

---

{#getdate}

#### getDate

`const` `nodiscard`

```cpp
[[nodiscard]] Timestamp getDate() const
```

Defined in src/http/include/icy/http/response.h:122

Returns the value of the Date header.

---

{#addcookie}

#### addCookie

```cpp
void addCookie(const Cookie & cookie)
```

Defined in src/http/include/icy/http/response.h:126

Adds the cookie to the response by adding a Set-Cookie header.

---

{#getcookies-1}

#### getCookies

`const`

```cpp
void getCookies(std::vector< Cookie > & cookies) const
```

Defined in src/http/include/icy/http/response.h:131

Returns a vector with all the cookies set in the response header.

May throw an exception in case of a malformed Set-Cookie header.

---

{#write-12}

#### write

`virtual` `const`

```cpp
virtual void write(std::ostream & ostr) const
```

Defined in src/http/include/icy/http/response.h:134

Writes the HTTP response headers to the given output stream.

---

{#write-13}

#### write

`virtual` `const`

```cpp
virtual void write(std::string & str) const
```

Defined in src/http/include/icy/http/response.h:137

Writes the HTTP response headers to the given output string.

---

{#write-14}

#### write

`virtual` `const`

```cpp
virtual void write(Buffer & buf) const
```

Defined in src/http/include/icy/http/response.h:140

Writes the HTTP response headers directly into a byte buffer.

---

{#success}

#### success

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual bool success() const
```

Defined in src/http/include/icy/http/response.h:143

Returns true if the HTTP response code was successful (< 400).

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `StatusCode` | [`_status`](#_status)  |  |
| `std::string` | [`_reason`](#_reason)  |  |

---

{#_status}

#### _status

```cpp
StatusCode _status
```

Defined in src/http/include/icy/http/response.h:152

---

{#_reason}

#### _reason

```cpp
std::string _reason
```

Defined in src/http/include/icy/http/response.h:153

{#server}

## Server

```cpp
#include <icy/http/server.h>
```

```cpp
class Server
```

Defined in src/http/include/icy/http/server.h:332

> **Inherits:** [`SocketAdapter`](net.md#socketadapter)

HTTP server implementation.

This HTTP server is not strictly standards compliant. It was created to be a fast (nocopy where possible) solution for streaming media to web browsers.

### Friends

| Name | Description |
|------|-------------|
| [`ServerConnection`](#serverconnection)  |  |

---

{#serverconnection}

#### ServerConnection

```cpp
friend class ServerConnection
```

Defined in src/http/include/icy/http/server.h:435

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `LocalSignal< void(ServerConnection::Ptr)>` | [`Connection`](#connection-8)  | Signals when a new connection has been created. A reference to the new connection object is provided. |
| `LocalSignal< void()>` | [`Shutdown`](#shutdown-7)  | Signals when the server is shutting down. |

---

{#connection-8}

#### Connection

```cpp
LocalSignal< void(ServerConnection::Ptr)> Connection
```

Defined in src/http/include/icy/http/server.h:406

Signals when a new connection has been created. A reference to the new connection object is provided.

---

{#shutdown-7}

#### Shutdown

```cpp
LocalSignal< void()> Shutdown
```

Defined in src/http/include/icy/http/server.h:409

Signals when the server is shutting down.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Server`](#server-1)  | Constructs an HTTP server on the given host and port using an internally created TCP socket. |
|  | [`Server`](#server-2)  | Constructs an HTTP server on the given address using an internally created TCP socket. |
|  | [`Server`](#server-3)  | Constructs an HTTP server on the given host and port using a caller-supplied socket. Useful for HTTPS by passing an SSLSocket. The event loop is derived from the socket. |
|  | [`Server`](#server-4)  | Constructs an HTTP server on the given address using a caller-supplied socket. The event loop is derived from the socket. |
| `void` | [`start`](#start-11)  | Start the HTTP server. |
| `void` | [`stop`](#stop-9)  | Stop the HTTP server. |
| `void` | [`setReusePort`](#setreuseport-1) `inline` | Enable SO_REUSEPORT for multicore server instances. Must be called before [start()](#start-11). Allows multiple server instances to bind the same address:port with kernel-level load balancing (Linux 3.9+). |
| `void` | [`setMaxPooledConnections`](#setmaxpooledconnections) `inline` | Set the maximum number of pooled connections (default 128). Set to 0 to disable connection pooling entirely. |
| `void` | [`setKeepAliveTimeout`](#setkeepalivetimeout) `inline` | Set the keep-alive idle timeout in seconds (default 30). Connections idle longer than this are closed by the timer. Set to 0 to disable idle timeout. |
| `size_t` | [`connectionCount`](#connectioncount) `const` `inline` `nodiscard` | Return the number of active connections (all states). |
| `net::Address &` | [`address`](#address-11) `nodiscard` | Return the server bind address. |
| `const DateCache &` | [`dateCache`](#datecache-1) `const` `inline` `nodiscard` | Return the cached Date header for use in responses. |

---

{#server-1}

#### Server

```cpp
Server(const std::string & host, short port, uv::Loop * loop = uv::defaultLoop(), std::unique_ptr< ServerConnectionFactory > factory = std::make_unique< ServerConnectionFactory >())
```

Defined in src/http/include/icy/http/server.h:340

Constructs an HTTP server on the given host and port using an internally created TCP socket. 
#### Parameters
* `host` Bind address (e.g. "0.0.0.0" or "127.0.0.1"). 

* `port` TCP port to listen on. 

* `loop` Event loop to use. Defaults to the default libuv loop. 

* `factory` [Connection](#connection-1) and responder factory. Defaults to the base factory.

---

{#server-2}

#### Server

```cpp
Server(const net::Address & address, uv::Loop * loop = uv::defaultLoop(), std::unique_ptr< ServerConnectionFactory > factory = std::make_unique< ServerConnectionFactory >())
```

Defined in src/http/include/icy/http/server.h:348

Constructs an HTTP server on the given address using an internally created TCP socket. 
#### Parameters
* `address` Bind address and port. 

* `loop` Event loop to use. Defaults to the default libuv loop. 

* `factory` [Connection](#connection-1) and responder factory.

---

{#server-3}

#### Server

```cpp
Server(const std::string & host, short port, net::TCPSocket::Ptr socket, std::unique_ptr< ServerConnectionFactory > factory = std::make_unique< ServerConnectionFactory >())
```

Defined in src/http/include/icy/http/server.h:358

Constructs an HTTP server on the given host and port using a caller-supplied socket. Useful for HTTPS by passing an SSLSocket. The event loop is derived from the socket. 
#### Parameters
* `host` Bind address. 

* `port` TCP port to listen on. 

* `socket` Pre-created socket (e.g. SSLSocket for HTTPS). 

* `factory` [Connection](#connection-1) and responder factory.

---

{#server-4}

#### Server

```cpp
Server(const net::Address & address, net::TCPSocket::Ptr socket, std::unique_ptr< ServerConnectionFactory > factory = std::make_unique< ServerConnectionFactory >())
```

Defined in src/http/include/icy/http/server.h:367

Constructs an HTTP server on the given address using a caller-supplied socket. The event loop is derived from the socket. 
#### Parameters
* `address` Bind address and port. 

* `socket` Pre-created socket (e.g. SSLSocket for HTTPS). 

* `factory` [Connection](#connection-1) and responder factory.

---

{#start-11}

#### start

```cpp
void start()
```

Defined in src/http/include/icy/http/server.h:374

Start the HTTP server.

---

{#stop-9}

#### stop

```cpp
void stop()
```

Defined in src/http/include/icy/http/server.h:377

Stop the HTTP server.

---

{#setreuseport-1}

#### setReusePort

`inline`

```cpp
inline void setReusePort(bool enable = true)
```

Defined in src/http/include/icy/http/server.h:384

Enable SO_REUSEPORT for multicore server instances. Must be called before [start()](#start-11). Allows multiple server instances to bind the same address:port with kernel-level load balancing (Linux 3.9+).

---

{#setmaxpooledconnections}

#### setMaxPooledConnections

`inline`

```cpp
inline void setMaxPooledConnections(size_t n)
```

Defined in src/http/include/icy/http/server.h:388

Set the maximum number of pooled connections (default 128). Set to 0 to disable connection pooling entirely.

---

{#setkeepalivetimeout}

#### setKeepAliveTimeout

`inline`

```cpp
inline void setKeepAliveTimeout(int seconds)
```

Defined in src/http/include/icy/http/server.h:393

Set the keep-alive idle timeout in seconds (default 30). Connections idle longer than this are closed by the timer. Set to 0 to disable idle timeout.

---

{#connectioncount}

#### connectionCount

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t connectionCount() const
```

Defined in src/http/include/icy/http/server.h:396

Return the number of active connections (all states).

---

{#address-11}

#### address

`nodiscard`

```cpp
[[nodiscard]] net::Address & address()
```

Defined in src/http/include/icy/http/server.h:399

Return the server bind address.

---

{#datecache-1}

#### dateCache

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const DateCache & dateCache() const
```

Defined in src/http/include/icy/http/server.h:402

Return the cached Date header for use in responses.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Loop *` | [`_loop`](#_loop-1)  |  |
| `net::Address` | [`_address`](#_address)  |  |
| `net::TCPSocket::Ptr` | [`_socket`](#_socket-2)  |  |
| `Timer` | [`_timer`](#_timer)  |  |
| `std::unique_ptr< ServerConnectionFactory >` | [`_factory`](#_factory)  |  |
| `std::unordered_map< ServerConnection *, ServerConnection::Ptr >` | [`_connections`](#_connections-1)  |  |
| `ConnectionPool` | [`_pool`](#_pool-1)  |  |
| `DateCache` | [`_dateCache`](#_datecache)  |  |
| `int` | [`_keepAliveTimeout`](#_keepalivetimeout)  |  |
| `bool` | [`_reusePort`](#_reuseport)  |  |

---

{#_loop-1}

#### _loop

```cpp
uv::Loop * _loop
```

Defined in src/http/include/icy/http/server.h:424

---

{#_address}

#### _address

```cpp
net::Address _address
```

Defined in src/http/include/icy/http/server.h:425

---

{#_socket-2}

#### _socket

```cpp
net::TCPSocket::Ptr _socket
```

Defined in src/http/include/icy/http/server.h:426

---

{#_timer}

#### _timer

```cpp
Timer _timer
```

Defined in src/http/include/icy/http/server.h:427

---

{#_factory}

#### _factory

```cpp
std::unique_ptr< ServerConnectionFactory > _factory
```

Defined in src/http/include/icy/http/server.h:428

---

{#_connections-1}

#### _connections

```cpp
std::unordered_map< ServerConnection *, ServerConnection::Ptr > _connections
```

Defined in src/http/include/icy/http/server.h:429

---

{#_pool-1}

#### _pool

```cpp
ConnectionPool _pool
```

Defined in src/http/include/icy/http/server.h:430

---

{#_datecache}

#### _dateCache

```cpp
DateCache _dateCache
```

Defined in src/http/include/icy/http/server.h:431

---

{#_keepalivetimeout}

#### _keepAliveTimeout

```cpp
int _keepAliveTimeout {30}
```

Defined in src/http/include/icy/http/server.h:432

---

{#_reuseport}

#### _reusePort

```cpp
bool _reusePort {false}
```

Defined in src/http/include/icy/http/server.h:433

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< ServerResponder >` | [`createResponder`](#createresponder)  |  |
| `void` | [`onClientSocketAccept`](#onclientsocketaccept)  |  |
| `void` | [`onConnectionReady`](#onconnectionready)  |  |
| `void` | [`onConnectionClose`](#onconnectionclose-1)  |  |
| `bool` | [`onSocketClose`](#onsocketclose-3) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `void` | [`onTimer`](#ontimer)  |  |
| `uv::Loop *` | [`loop`](#loop-6) `const` `inline` `nodiscard` | Return the event loop this server runs on. |

---

{#createresponder}

#### createResponder

```cpp
std::unique_ptr< ServerResponder > createResponder(ServerConnection & conn)
```

Defined in src/http/include/icy/http/server.h:412

---

{#onclientsocketaccept}

#### onClientSocketAccept

```cpp
void onClientSocketAccept(const net::TCPSocket::Ptr & socket)
```

Defined in src/http/include/icy/http/server.h:414

---

{#onconnectionready}

#### onConnectionReady

```cpp
void onConnectionReady(ServerConnection & conn)
```

Defined in src/http/include/icy/http/server.h:415

---

{#onconnectionclose-1}

#### onConnectionClose

```cpp
void onConnectionClose(ServerConnection & conn)
```

Defined in src/http/include/icy/http/server.h:416

---

{#onsocketclose-3}

#### onSocketClose

`virtual`

```cpp
virtual bool onSocketClose(net::Socket & socket)
```

Defined in src/http/include/icy/http/server.h:417

Called when the socket is closed. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that was closed. 

#### Returns
true to stop propagation to subsequent receivers.

---

{#ontimer}

#### onTimer

```cpp
void onTimer()
```

Defined in src/http/include/icy/http/server.h:418

---

{#loop-6}

#### loop

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uv::Loop * loop() const
```

Defined in src/http/include/icy/http/server.h:421

Return the event loop this server runs on.

{#serverconnection-1}

## ServerConnection

```cpp
#include <icy/http/server.h>
```

```cpp
class ServerConnection
```

Defined in src/http/include/icy/http/server.h:59

> **Inherits:** [`Connection`](#connection-1)

HTTP server connection.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `LocalSignal< void(ServerConnection &, const MutableBuffer &)>` | [`Payload`](#payload-1)  | Signals when raw data is received. |
| `LocalSignal< void(ServerConnection &)>` | [`Close`](#close-21)  | Signals when the connection is closed. |

---

{#payload-1}

#### Payload

```cpp
LocalSignal< void(ServerConnection &, const MutableBuffer &)> Payload
```

Defined in src/http/include/icy/http/server.h:118

Signals when raw data is received.

---

{#close-21}

#### Close

```cpp
LocalSignal< void(ServerConnection &)> Close
```

Defined in src/http/include/icy/http/server.h:119

Signals when the connection is closed.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerConnection`](#serverconnection-2)  | Creates a [ServerConnection](#serverconnection-1) attached to the given server and socket. |
| `Server &` | [`server`](#server-5) `nodiscard` | Returns the owning [Server](#server) instance. |
| `ServerConnectionState` | [`state`](#state-1) `const` `inline` `nodiscard` | Returns the current server-side connection state. |
| `ServerConnectionMode` | [`mode`](#mode-1) `const` `inline` `nodiscard` | Returns the current transport mode. |
| `bool` | [`upgraded`](#upgraded) `const` `inline` `nodiscard` | Returns true if the connection has been upgraded (e.g. to WebSocket). |
| `bool` | [`streaming`](#streaming) `const` `inline` `nodiscard` | Returns true if the connection is in long-lived streaming mode. |
| `bool` | [`idleTimeoutEnabled`](#idletimeoutenabled) `const` `nodiscard` | Returns true if the server idle timer is allowed to reap this connection. |
| `bool` | [`reusableForPool`](#reusableforpool) `const` `nodiscard` | Returns true if the closed connection can be returned to the reuse pool. |
| `void` | [`markActive`](#markactive-1) `virtual` `inline` `override` | Refresh the idle timer. |
| `void` | [`reset`](#reset-12)  | Reset this connection for reuse with a new socket. Called by the connection pool to avoid allocating a new [ServerConnection](#serverconnection-1). |
| `void` | [`touch`](#touch) `inline` | Update the last activity timestamp. |
| `double` | [`idleSeconds`](#idleseconds) `const` `inline` `nodiscard` | Return seconds since last activity. |
| `void` | [`beginStreaming`](#beginstreaming-1) `virtual` `override` | Explicitly mark the response as long-lived streaming. Streaming connections are excluded from the keep-alive idle reaper. |
| `void` | [`endStreaming`](#endstreaming-1) `virtual` `override` | Exit streaming mode and return to the given HTTP state. |
| `void` | [`endStreaming`](#endstreaming-2)  |  |
| `ssize_t` | [`sendHeader`](#sendheader-1) `virtual` `override` | Send the outgoing HTTP header. |
| `void` | [`close`](#close-22) `virtual` `override` | Close the connection with an explicit terminal state transition. |

---

{#serverconnection-2}

#### ServerConnection

```cpp
ServerConnection(Server & server, net::TCPSocket::Ptr socket)
```

Defined in src/http/include/icy/http/server.h:67

Creates a [ServerConnection](#serverconnection-1) attached to the given server and socket. 
#### Parameters
* `server` The owning HTTP server instance. 

* `socket` The accepted TCP socket for this connection.

---

{#server-5}

#### server

`nodiscard`

```cpp
[[nodiscard]] Server & server()
```

Defined in src/http/include/icy/http/server.h:71

Returns the owning [Server](#server) instance.

---

{#state-1}

#### state

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline ServerConnectionState state() const
```

Defined in src/http/include/icy/http/server.h:74

Returns the current server-side connection state.

---

{#mode-1}

#### mode

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline ServerConnectionMode mode() const
```

Defined in src/http/include/icy/http/server.h:77

Returns the current transport mode.

---

{#upgraded}

#### upgraded

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool upgraded() const
```

Defined in src/http/include/icy/http/server.h:80

Returns true if the connection has been upgraded (e.g. to WebSocket).

---

{#streaming}

#### streaming

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool streaming() const
```

Defined in src/http/include/icy/http/server.h:83

Returns true if the connection is in long-lived streaming mode.

---

{#idletimeoutenabled}

#### idleTimeoutEnabled

`const` `nodiscard`

```cpp
[[nodiscard]] bool idleTimeoutEnabled() const
```

Defined in src/http/include/icy/http/server.h:86

Returns true if the server idle timer is allowed to reap this connection.

---

{#reusableforpool}

#### reusableForPool

`const` `nodiscard`

```cpp
[[nodiscard]] bool reusableForPool() const
```

Defined in src/http/include/icy/http/server.h:89

Returns true if the closed connection can be returned to the reuse pool.

---

{#markactive-1}

#### markActive

`virtual` `inline` `override`

```cpp
virtual inline void markActive() override
```

Defined in src/http/include/icy/http/server.h:92

Refresh the idle timer.

---

{#reset-12}

#### reset

```cpp
void reset(net::TCPSocket::Ptr socket)
```

Defined in src/http/include/icy/http/server.h:96

Reset this connection for reuse with a new socket. Called by the connection pool to avoid allocating a new [ServerConnection](#serverconnection-1).

---

{#touch}

#### touch

`inline`

```cpp
inline void touch()
```

Defined in src/http/include/icy/http/server.h:99

Update the last activity timestamp.

---

{#idleseconds}

#### idleSeconds

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline double idleSeconds() const
```

Defined in src/http/include/icy/http/server.h:102

Return seconds since last activity.

---

{#beginstreaming-1}

#### beginStreaming

`virtual` `override`

```cpp
virtual void beginStreaming() override
```

Defined in src/http/include/icy/http/server.h:106

Explicitly mark the response as long-lived streaming. Streaming connections are excluded from the keep-alive idle reaper.

---

{#endstreaming-1}

#### endStreaming

`virtual` `override`

```cpp
virtual void endStreaming() override
```

Defined in src/http/include/icy/http/server.h:109

Exit streaming mode and return to the given HTTP state.

---

{#endstreaming-2}

#### endStreaming

```cpp
void endStreaming(ServerConnectionState nextState)
```

Defined in src/http/include/icy/http/server.h:110

---

{#sendheader-1}

#### sendHeader

`virtual` `override`

```cpp
virtual ssize_t sendHeader() override
```

Defined in src/http/include/icy/http/server.h:113

Send the outgoing HTTP header.

---

{#close-22}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/http/include/icy/http/server.h:116

Close the connection with an explicit terminal state transition.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Server &` | [`_server`](#_server)  |  |
| `std::unique_ptr< ServerResponder >` | [`_responder`](#_responder)  |  |
| `std::time_t` | [`_lastActivity`](#_lastactivity)  |  |
| `ServerConnectionState` | [`_state`](#_state-2)  |  |
| `ServerConnectionMode` | [`_mode`](#_mode-2)  |  |

---

{#_server}

#### _server

```cpp
Server & _server
```

Defined in src/http/include/icy/http/server.h:135

---

{#_responder}

#### _responder

```cpp
std::unique_ptr< ServerResponder > _responder
```

Defined in src/http/include/icy/http/server.h:136

---

{#_lastactivity}

#### _lastActivity

```cpp
std::time_t _lastActivity {0}
```

Defined in src/http/include/icy/http/server.h:137

---

{#_state-2}

#### _state

```cpp
ServerConnectionState _state {}
```

Defined in src/http/include/icy/http/server.h:138

---

{#_mode-2}

#### _mode

```cpp
ServerConnectionMode _mode {}
```

Defined in src/http/include/icy/http/server.h:139

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onHeaders`](#onheaders-2) `virtual` `override` | Called when the incoming HTTP headers have been fully parsed. |
| `void` | [`onPayload`](#onpayload-2) `virtual` `override` | Called for each chunk of incoming body data after headers are complete. |
| `void` | [`onComplete`](#oncomplete-2) `virtual` `override` | Called when the incoming HTTP message is fully received. |
| `void` | [`onClose`](#onclose-5) `virtual` `override` | Called when the connection is closed. |
| `http::Message *` | [`incomingHeader`](#incomingheader-2) `virtual` `override` | Returns the incoming HTTP message header (request or response depending on role). |
| `http::Message *` | [`outgoingHeader`](#outgoingheader-2) `virtual` `override` | Returns the outgoing HTTP message header (request or response depending on role). |
| `void` | [`setState`](#setstate)  |  |
| `bool` | [`requestHasBody`](#requesthasbody) `const` `nodiscard` |  |
| `bool` | [`responseLooksStreaming`](#responselooksstreaming) `const` `nodiscard` |  |

---

{#onheaders-2}

#### onHeaders

`virtual` `override`

```cpp
virtual void onHeaders() override
```

Defined in src/http/include/icy/http/server.h:122

Called when the incoming HTTP headers have been fully parsed.

---

{#onpayload-2}

#### onPayload

`virtual` `override`

```cpp
virtual void onPayload(const MutableBuffer & buffer) override
```

Defined in src/http/include/icy/http/server.h:123

Called for each chunk of incoming body data after headers are complete. 
#### Parameters
* `buffer` [Buffer](base.md#buffer-2) containing the received data chunk.

---

{#oncomplete-2}

#### onComplete

`virtual` `override`

```cpp
virtual void onComplete() override
```

Defined in src/http/include/icy/http/server.h:124

Called when the incoming HTTP message is fully received.

---

{#onclose-5}

#### onClose

`virtual` `override`

```cpp
virtual void onClose() override
```

Defined in src/http/include/icy/http/server.h:125

Called when the connection is closed.

---

{#incomingheader-2}

#### incomingHeader

`virtual` `override`

```cpp
virtual http::Message * incomingHeader() override
```

Defined in src/http/include/icy/http/server.h:127

Returns the incoming HTTP message header (request or response depending on role).

---

{#outgoingheader-2}

#### outgoingHeader

`virtual` `override`

```cpp
virtual http::Message * outgoingHeader() override
```

Defined in src/http/include/icy/http/server.h:128

Returns the outgoing HTTP message header (request or response depending on role).

---

{#setstate}

#### setState

```cpp
void setState(ServerConnectionState state)
```

Defined in src/http/include/icy/http/server.h:130

---

{#requesthasbody}

#### requestHasBody

`const` `nodiscard`

```cpp
[[nodiscard]] bool requestHasBody() const
```

Defined in src/http/include/icy/http/server.h:131

---

{#responselooksstreaming}

#### responseLooksStreaming

`const` `nodiscard`

```cpp
[[nodiscard]] bool responseLooksStreaming() const
```

Defined in src/http/include/icy/http/server.h:132

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-13)  |  |

---

{#ptr-13}

#### Ptr

```cpp
using Ptr = std::shared_ptr< ServerConnection >
```

Defined in src/http/include/icy/http/server.h:62

{#serverconnectionfactory}

## ServerConnectionFactory

```cpp
#include <icy/http/server.h>
```

```cpp
class ServerConnectionFactory
```

Defined in src/http/include/icy/http/server.h:206

Factory for creating per-socket `[ServerConnection](#serverconnection-1)` and per-request `[ServerResponder](#serverresponder)` objects.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerConnectionFactory`](#serverconnectionfactory-1)  | Defaulted constructor. |
| `ServerConnection::Ptr` | [`createConnection`](#createconnection-2) `virtual` `inline` | Creates the `[ServerConnection](#serverconnection-1)` wrapper for an accepted TCP socket. |
| `std::unique_ptr< ServerResponder >` | [`createResponder`](#createresponder-1) `virtual` `inline` | Creates the responder for the current request on `connection`. |

---

{#serverconnectionfactory-1}

#### ServerConnectionFactory

```cpp
ServerConnectionFactory() = default
```

Defined in src/http/include/icy/http/server.h:209

Defaulted constructor.

---

{#createconnection-2}

#### createConnection

`virtual` `inline`

```cpp
virtual inline ServerConnection::Ptr createConnection(Server & server, const net::TCPSocket::Ptr & socket)
```

Defined in src/http/include/icy/http/server.h:213

Creates the `[ServerConnection](#serverconnection-1)` wrapper for an accepted TCP socket.

---

{#createresponder-1}

#### createResponder

`virtual` `inline`

```cpp
virtual inline std::unique_ptr< ServerResponder > createResponder(ServerConnection & connection)
```

Defined in src/http/include/icy/http/server.h:219

Creates the responder for the current request on `connection`.

{#serverresponder}

## ServerResponder

```cpp
#include <icy/http/server.h>
```

```cpp
class ServerResponder
```

Defined in src/http/include/icy/http/server.h:145

Base responder interface for handling one HTTP request on a server connection. Derived classes typically override `[onRequest()](#onrequest)` and optionally the streaming hooks.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerResponder`](#serverresponder-1) `inline` | Creates a [ServerResponder](#serverresponder) for the given connection. |
| `void` | [`onHeaders`](#onheaders-3) `virtual` `inline` | Called when the incoming request headers have been parsed. |
| `void` | [`onPayload`](#onpayload-3) `virtual` `inline` | Called for each chunk of incoming request body data. |
| `void` | [`onRequest`](#onrequest) `virtual` `inline` | Called when the complete HTTP request has been received. Derived classes should write their response here. |
| `void` | [`onClose`](#onclose-6) `virtual` `inline` | Called when the connection is closed. |
| `ServerConnection &` | [`connection`](#connection-9) `inline` `nodiscard` | Returns the underlying server connection. |
| `Request &` | [`request`](#request-10) `inline` `nodiscard` | Returns the current HTTP request from the underlying connection. |
| `Response &` | [`response`](#response-8) `inline` `nodiscard` | Returns the current HTTP response from the underlying connection. |

---

{#serverresponder-1}

#### ServerResponder

`inline`

```cpp
inline ServerResponder(ServerConnection & connection)
```

Defined in src/http/include/icy/http/server.h:150

Creates a [ServerResponder](#serverresponder) for the given connection. 
#### Parameters
* `connection` The server connection this responder handles.

---

{#onheaders-3}

#### onHeaders

`virtual` `inline`

```cpp
virtual inline void onHeaders(Request & request)
```

Defined in src/http/include/icy/http/server.h:159

Called when the incoming request headers have been parsed. 
#### Parameters
* `request` The parsed HTTP request with headers populated.

---

{#onpayload-3}

#### onPayload

`virtual` `inline`

```cpp
virtual inline void onPayload(const MutableBuffer & body)
```

Defined in src/http/include/icy/http/server.h:163

Called for each chunk of incoming request body data. 
#### Parameters
* `body` [Buffer](base.md#buffer-2) containing a chunk of the request body.

---

{#onrequest}

#### onRequest

`virtual` `inline`

```cpp
virtual inline void onRequest(Request & request, Response & response)
```

Defined in src/http/include/icy/http/server.h:169

Called when the complete HTTP request has been received. Derived classes should write their response here. 
#### Parameters
* `request` The fully received HTTP request. 

* `response` The HTTP response to populate and send.

---

{#onclose-6}

#### onClose

`virtual` `inline`

```cpp
virtual inline void onClose()
```

Defined in src/http/include/icy/http/server.h:176

Called when the connection is closed.

---

{#connection-9}

#### connection

`inline` `nodiscard`

```cpp
[[nodiscard]] inline ServerConnection & connection()
```

Defined in src/http/include/icy/http/server.h:179

Returns the underlying server connection.

---

{#request-10}

#### request

`inline` `nodiscard`

```cpp
[[nodiscard]] inline Request & request()
```

Defined in src/http/include/icy/http/server.h:185

Returns the current HTTP request from the underlying connection.

---

{#response-8}

#### response

`inline` `nodiscard`

```cpp
[[nodiscard]] inline Response & response()
```

Defined in src/http/include/icy/http/server.h:191

Returns the current HTTP response from the underlying connection.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ServerConnection &` | [`_connection`](#_connection-2)  |  |

---

{#_connection-2}

#### _connection

```cpp
ServerConnection & _connection
```

Defined in src/http/include/icy/http/server.h:197

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerResponder`](#serverresponder-2)  | Deleted constructor. |

---

{#serverresponder-2}

#### ServerResponder

```cpp
ServerResponder(const ServerResponder &) = delete
```

Defined in src/http/include/icy/http/server.h:200

Deleted constructor.

{#stringpart-1}

## StringPart

```cpp
#include <icy/http/form.h>
```

```cpp
class StringPart
```

Defined in src/http/include/icy/http/form.h:306

> **Inherits:** [`FormPart`](#formpart)

Form part backed by an in-memory string payload.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`StringPart`](#stringpart-2)  | Creates a [StringPart](#stringpart-1) with the given data and default content type "application/octet-stream". |
|  | [`StringPart`](#stringpart-3)  | Creates a [StringPart](#stringpart-1) with the given data and MIME content type. |
|  | [`~StringPart`](#stringpart-4) `virtual` | Destroys the [StringPart](#stringpart-1). |
| `bool` | [`writeChunk`](#writechunk-2) `virtual` `override` | Writes the string data as a single chunk to the [FormWriter](#formwriter). |
| `void` | [`write`](#write-15) `virtual` `override` | Writes the string data to the [FormWriter](#formwriter). |
| `void` | [`write`](#write-16) `virtual` `override` | Writes the string data to an output stream. |
| `uint64_t` | [`length`](#length-3) `virtual` `const` `nodiscard` `override` | Returns the byte length of the string data. |

---

{#stringpart-2}

#### StringPart

```cpp
StringPart(const std::string & data)
```

Defined in src/http/include/icy/http/form.h:311

Creates a [StringPart](#stringpart-1) with the given data and default content type "application/octet-stream". 
#### Parameters
* `data` String data to send as this part.

---

{#stringpart-3}

#### StringPart

```cpp
StringPart(const std::string & data, const std::string & contentType)
```

Defined in src/http/include/icy/http/form.h:316

Creates a [StringPart](#stringpart-1) with the given data and MIME content type. 
#### Parameters
* `data` String data to send as this part. 

* `contentType` MIME type for this part.

---

{#stringpart-4}

#### ~StringPart

`virtual`

```cpp
virtual ~StringPart()
```

Defined in src/http/include/icy/http/form.h:319

Destroys the [StringPart](#stringpart-1).

---

{#writechunk-2}

#### writeChunk

`virtual` `override`

```cpp
virtual bool writeChunk(FormWriter & writer) override
```

Defined in src/http/include/icy/http/form.h:324

Writes the string data as a single chunk to the [FormWriter](#formwriter). 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send data through. 

#### Returns
false always (string data is sent in a single chunk).

---

{#write-15}

#### write

`virtual` `override`

```cpp
virtual void write(FormWriter & writer) override
```

Defined in src/http/include/icy/http/form.h:328

Writes the string data to the [FormWriter](#formwriter). 
#### Parameters
* `writer` The [FormWriter](#formwriter) to send data through.

---

{#write-16}

#### write

`virtual` `override`

```cpp
virtual void write(std::ostream & ostr) override
```

Defined in src/http/include/icy/http/form.h:332

Writes the string data to an output stream. 
#### Parameters
* `ostr` Output stream to write to.

---

{#length-3}

#### length

`virtual` `const` `nodiscard` `override`

```cpp
[[nodiscard]] virtual uint64_t length() const override
```

Defined in src/http/include/icy/http/form.h:335

Returns the byte length of the string data.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_data`](#_data)  |  |

---

{#_data}

#### _data

```cpp
std::string _data
```

Defined in src/http/include/icy/http/form.h:338

{#url}

## URL

```cpp
#include <icy/http/url.h>
```

```cpp
class URL
```

Defined in src/http/include/icy/http/url.h:28

An RFC 3986 based [URL](#url) parser. Constructors and assignment operators will throw a SyntaxException if the [URL](#url) is invalid.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-19) `inline` |  |

---

{#operator-19}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const URL & url)
```

Defined in src/http/include/icy/http/url.h:163

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`URL`](#url-1)  | Creates an empty [URL](#url). |
|  | [`URL`](#url-2)  | Parses the [URL](#url) from a null-terminated string. |
|  | [`URL`](#url-3)  | Parses the [URL](#url) from a std::string. |
|  | [`URL`](#url-4)  | Constructs a [URL](#url) from scheme and authority components. |
|  | [`URL`](#url-5)  | Constructs a [URL](#url) from scheme, authority, and path+query+fragment. |
|  | [`URL`](#url-6)  | Constructs a [URL](#url) from individual components. |
|  | [`URL`](#url-7)  | Defaulted constructor. |
| `URL &` | [`operator=`](#operator-20)  | Assigns a [URL](#url) from another [URL](#url) instance. |
| `URL &` | [`operator=`](#operator-21)  | Assigns a [URL](#url) from a null-terminated string. |
| `URL &` | [`operator=`](#operator-22)  | Assigns a [URL](#url) from a std::string. |
| `bool` | [`parse`](#parse-1)  | Parses and assigns a [URL](#url) from the given string view, resetting all components first. |
| `std::string` | [`scheme`](#scheme) `const` `nodiscard` | Returns the [URL](#url) scheme (e.g. "http", "https", "ws"). Always lowercase. |
| `std::string` | [`userInfo`](#userinfo) `const` `nodiscard` | Returns the user info component (e.g. "user:pass" from "http://user:pass@host/"). Returns an empty string if not present. |
| `std::string` | [`host`](#host-1) `const` `nodiscard` | Returns the host component (e.g. "example.com"). Returns an empty string if not present. |
| `uint16_t` | [`port`](#port-1) `const` `nodiscard` | Returns the port number. If no explicit port was in the [URL](#url), returns the default port for the scheme (80 for http, 443 for https), or 0 if the scheme is unknown. |
| `std::string` | [`authority`](#authority) `const` `nodiscard` | Returns the authority component (`userinfo@[host](#host-1):[port](#port-1)`). Only includes components that are present. |
| `std::string` | [`path`](#path) `const` `nodiscard` | Returns the path component (e.g. "/index.html"). Returns an empty string if not present. |
| `std::string` | [`pathEtc`](#pathetc) `const` `nodiscard` | Returns the path, query and fragment combined (e.g. "/path?q=1#frag"). |
| `std::string` | [`query`](#query) `const` `nodiscard` | Returns the query string without the leading '?' (e.g. "key=value&foo=bar"). Returns an empty string if not present. |
| `std::string` | [`fragment`](#fragment) `const` `nodiscard` | Returns the fragment identifier without the leading '#'. Returns an empty string if not present. |
| `bool` | [`hasSchema`](#hasschema) `const` `nodiscard` | Returns true if the [URL](#url) has a scheme component. |
| `bool` | [`hasUserInfo`](#hasuserinfo) `const` `nodiscard` | Returns true if the [URL](#url) has a user info component. |
| `bool` | [`hasHost`](#hashost) `const` `nodiscard` | Returns true if the [URL](#url) has a host component. |
| `bool` | [`hasPort`](#hasport) `const` `nodiscard` | Returns true if an explicit port was specified in the [URL](#url). |
| `bool` | [`hasPath`](#haspath) `const` `nodiscard` | Returns true if the [URL](#url) has a path component. |
| `bool` | [`hasQuery`](#hasquery) `const` `nodiscard` | Returns true if the [URL](#url) has a query component. |
| `bool` | [`hasFragment`](#hasfragment) `const` `nodiscard` | Returns true if the [URL](#url) has a fragment component. |
| `bool` | [`valid`](#valid-1) `const` `nodiscard` | Returns true if the [URL](#url) is non-empty and was successfully parsed. |
| `std::string` | [`str`](#str-1) `const` `nodiscard` | Returns the original [URL](#url) string as parsed. |

---

{#url-1}

#### URL

```cpp
URL()
```

Defined in src/http/include/icy/http/url.h:32

Creates an empty [URL](#url).

---

{#url-2}

#### URL

```cpp
URL(const char * url)
```

Defined in src/http/include/icy/http/url.h:36

Parses the [URL](#url) from a null-terminated string. 
#### Parameters
* `url` Null-terminated [URL](#url) string to parse.

---

{#url-3}

#### URL

```cpp
URL(const std::string & url)
```

Defined in src/http/include/icy/http/url.h:40

Parses the [URL](#url) from a std::string. 
#### Parameters
* `url` [URL](#url) string to parse.

---

{#url-4}

#### URL

```cpp
URL(const std::string & scheme, const std::string & authority)
```

Defined in src/http/include/icy/http/url.h:45

Constructs a [URL](#url) from scheme and authority components. 
#### Parameters
* `scheme` [URL](#url) scheme (e.g. "http", "https"). 

* `authority` Host and optional port (e.g. "example.com:8080").

---

{#url-5}

#### URL

```cpp
URL(const std::string & scheme, const std::string & authority, const std::string & pathEtc)
```

Defined in src/http/include/icy/http/url.h:51

Constructs a [URL](#url) from scheme, authority, and path+query+fragment. 
#### Parameters
* `scheme` [URL](#url) scheme (e.g. "http"). 

* `authority` Host and optional port. 

* `pathEtc` Path, query and fragment combined (e.g. "/path?q=1#frag").

---

{#url-6}

#### URL

```cpp
URL(const std::string & scheme, const std::string & authority, const std::string & path, const std::string & query, const std::string & fragment = "")
```

Defined in src/http/include/icy/http/url.h:60

Constructs a [URL](#url) from individual components. 
#### Parameters
* `scheme` [URL](#url) scheme (e.g. "http"). 

* `authority` Host and optional port. 

* `path` [URL](#url) path (e.g. "/index.html"). 

* `query` Query string without leading '?' (e.g. "key=value"). 

* `fragment` Fragment identifier without leading '#'.

---

{#url-7}

#### URL

```cpp
URL(const URL &) = default
```

Defined in src/http/include/icy/http/url.h:64

Defaulted constructor.

---

{#operator-20}

#### operator=

```cpp
URL & operator=(const URL & uri)
```

Defined in src/http/include/icy/http/url.h:70

Assigns a [URL](#url) from another [URL](#url) instance. 
#### Parameters
* `uri` Source [URL](#url) to copy from. 

#### Returns
Reference to this [URL](#url).

---

{#operator-21}

#### operator=

```cpp
URL & operator=(const char * uri)
```

Defined in src/http/include/icy/http/url.h:75

Assigns a [URL](#url) from a null-terminated string. 
#### Parameters
* `uri` Null-terminated [URL](#url) string to parse. 

#### Returns
Reference to this [URL](#url).

---

{#operator-22}

#### operator=

```cpp
URL & operator=(const std::string & uri)
```

Defined in src/http/include/icy/http/url.h:80

Assigns a [URL](#url) from a std::string. 
#### Parameters
* `uri` [URL](#url) string to parse. 

#### Returns
Reference to this [URL](#url).

---

{#parse-1}

#### parse

```cpp
bool parse(std::string_view url, bool whiny = true)
```

Defined in src/http/include/icy/http/url.h:86

Parses and assigns a [URL](#url) from the given string view, resetting all components first. 
#### Parameters
* `url` [URL](#url) string to parse. 

* `whiny` If true, throws std::runtime_error on an invalid [URL](#url); otherwise returns false. 

#### Returns
true if the [URL](#url) was parsed successfully; false if invalid and whiny is false.

---

{#scheme}

#### scheme

`const` `nodiscard`

```cpp
[[nodiscard]] std::string scheme() const
```

Defined in src/http/include/icy/http/url.h:102

Returns the [URL](#url) scheme (e.g. "http", "https", "ws"). Always lowercase.

---

{#userinfo}

#### userInfo

`const` `nodiscard`

```cpp
[[nodiscard]] std::string userInfo() const
```

Defined in src/http/include/icy/http/url.h:106

Returns the user info component (e.g. "user:pass" from "http://user:pass@host/"). Returns an empty string if not present.

---

{#host-1}

#### host

`const` `nodiscard`

```cpp
[[nodiscard]] std::string host() const
```

Defined in src/http/include/icy/http/url.h:110

Returns the host component (e.g. "example.com"). Returns an empty string if not present.

---

{#port-1}

#### port

`const` `nodiscard`

```cpp
[[nodiscard]] uint16_t port() const
```

Defined in src/http/include/icy/http/url.h:115

Returns the port number. If no explicit port was in the [URL](#url), returns the default port for the scheme (80 for http, 443 for https), or 0 if the scheme is unknown.

---

{#authority}

#### authority

`const` `nodiscard`

```cpp
[[nodiscard]] std::string authority() const
```

Defined in src/http/include/icy/http/url.h:119

Returns the authority component (`userinfo@[host](#host-1):[port](#port-1)`). Only includes components that are present.

---

{#path}

#### path

`const` `nodiscard`

```cpp
[[nodiscard]] std::string path() const
```

Defined in src/http/include/icy/http/url.h:123

Returns the path component (e.g. "/index.html"). Returns an empty string if not present.

---

{#pathetc}

#### pathEtc

`const` `nodiscard`

```cpp
[[nodiscard]] std::string pathEtc() const
```

Defined in src/http/include/icy/http/url.h:126

Returns the path, query and fragment combined (e.g. "/path?q=1#frag").

---

{#query}

#### query

`const` `nodiscard`

```cpp
[[nodiscard]] std::string query() const
```

Defined in src/http/include/icy/http/url.h:130

Returns the query string without the leading '?' (e.g. "key=value&foo=bar"). Returns an empty string if not present.

---

{#fragment}

#### fragment

`const` `nodiscard`

```cpp
[[nodiscard]] std::string fragment() const
```

Defined in src/http/include/icy/http/url.h:134

Returns the fragment identifier without the leading '#'. Returns an empty string if not present.

---

{#hasschema}

#### hasSchema

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasSchema() const
```

Defined in src/http/include/icy/http/url.h:137

Returns true if the [URL](#url) has a scheme component.

---

{#hasuserinfo}

#### hasUserInfo

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasUserInfo() const
```

Defined in src/http/include/icy/http/url.h:140

Returns true if the [URL](#url) has a user info component.

---

{#hashost}

#### hasHost

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasHost() const
```

Defined in src/http/include/icy/http/url.h:143

Returns true if the [URL](#url) has a host component.

---

{#hasport}

#### hasPort

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasPort() const
```

Defined in src/http/include/icy/http/url.h:146

Returns true if an explicit port was specified in the [URL](#url).

---

{#haspath}

#### hasPath

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasPath() const
```

Defined in src/http/include/icy/http/url.h:149

Returns true if the [URL](#url) has a path component.

---

{#hasquery}

#### hasQuery

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasQuery() const
```

Defined in src/http/include/icy/http/url.h:152

Returns true if the [URL](#url) has a query component.

---

{#hasfragment}

#### hasFragment

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasFragment() const
```

Defined in src/http/include/icy/http/url.h:155

Returns true if the [URL](#url) has a fragment component.

---

{#valid-1}

#### valid

`const` `nodiscard`

```cpp
[[nodiscard]] bool valid() const
```

Defined in src/http/include/icy/http/url.h:158

Returns true if the [URL](#url) is non-empty and was successfully parsed.

---

{#str-1}

#### str

`const` `nodiscard`

```cpp
[[nodiscard]] std::string str() const
```

Defined in src/http/include/icy/http/url.h:161

Returns the original [URL](#url) string as parsed.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`encode`](#encode-21) `static` | Percent-encodes a string per RFC 3986, preserving unreserved characters (A-Z, a-z, 0-9, '-', '_', '.', '~'). Equivalent to JavaScript's encodeURIComponent(). |
| `std::string` | [`decode`](#decode-10) `static` | Decodes a percent-encoded string per RFC 3986. Equivalent to JavaScript's decodeURIComponent(). |

---

{#encode-21}

#### encode

`static`

```cpp
static std::string encode(std::string_view str)
```

Defined in src/http/include/icy/http/url.h:92

Percent-encodes a string per RFC 3986, preserving unreserved characters (A-Z, a-z, 0-9, '-', '_', '.', '~'). Equivalent to JavaScript's encodeURIComponent(). 
#### Parameters
* `str` Input string to encode. 

#### Returns
Percent-encoded string.

---

{#decode-10}

#### decode

`static`

```cpp
static std::string decode(std::string_view str)
```

Defined in src/http/include/icy/http/url.h:98

Decodes a percent-encoded string per RFC 3986. Equivalent to JavaScript's decodeURIComponent(). 
#### Parameters
* `str` Percent-encoded input string. 

#### Returns
Decoded string.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_buf`](#_buf)  |  |
| `std::string` | [`_scheme`](#_scheme)  |  |
| `std::string` | [`_userInfo`](#_userinfo)  |  |
| `std::string` | [`_host`](#_host)  |  |
| `uint16_t` | [`_port`](#_port)  |  |
| `std::string` | [`_path`](#_path-2)  |  |
| `std::string` | [`_query`](#_query)  |  |
| `std::string` | [`_fragment`](#_fragment)  |  |
| `bool` | [`_hasPort`](#_hasport)  |  |

---

{#_buf}

#### _buf

```cpp
std::string _buf
```

Defined in src/http/include/icy/http/url.h:170

---

{#_scheme}

#### _scheme

```cpp
std::string _scheme
```

Defined in src/http/include/icy/http/url.h:171

---

{#_userinfo}

#### _userInfo

```cpp
std::string _userInfo
```

Defined in src/http/include/icy/http/url.h:172

---

{#_host}

#### _host

```cpp
std::string _host
```

Defined in src/http/include/icy/http/url.h:173

---

{#_port}

#### _port

```cpp
uint16_t _port
```

Defined in src/http/include/icy/http/url.h:174

---

{#_path-2}

#### _path

```cpp
std::string _path
```

Defined in src/http/include/icy/http/url.h:175

---

{#_query}

#### _query

```cpp
std::string _query
```

Defined in src/http/include/icy/http/url.h:176

---

{#_fragment}

#### _fragment

```cpp
std::string _fragment
```

Defined in src/http/include/icy/http/url.h:177

---

{#_hasport}

#### _hasPort

```cpp
bool _hasPort
```

Defined in src/http/include/icy/http/url.h:178

{#datecache}

## DateCache

```cpp
#include <icy/http/server.h>
```

```cpp
struct DateCache
```

Defined in src/http/include/icy/http/server.h:228

Caches the formatted Date header, updated once per second. Avoids per-request time formatting and string allocation.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `char` | [`buf`](#buf)  |  |
| `size_t` | [`len`](#len)  |  |
| `std::time_t` | [`lastSecond`](#lastsecond)  |  |

---

{#buf}

#### buf

```cpp
char buf {}
```

Defined in src/http/include/icy/http/server.h:230

---

{#len}

#### len

```cpp
size_t len = 0
```

Defined in src/http/include/icy/http/server.h:231

---

{#lastsecond}

#### lastSecond

```cpp
std::time_t lastSecond = 0
```

Defined in src/http/include/icy/http/server.h:232

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`update`](#update) `inline` | Refreshes the cached Date header string if the current second has changed. No-op if called multiple times within the same second. |
| `const char *` | [`data`](#data-2) `const` `inline` `nodiscard` | Returns a pointer to the formatted "Date: ...\r\n" header string. |
| `size_t` | [`size`](#size-1) `const` `inline` `nodiscard` | Returns the byte length of the formatted Date header string. |

---

{#update}

#### update

`inline`

```cpp
inline void update()
```

Defined in src/http/include/icy/http/server.h:236

Refreshes the cached Date header string if the current second has changed. No-op if called multiple times within the same second.

---

{#data-2}

#### data

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * data() const
```

Defined in src/http/include/icy/http/server.h:253

Returns a pointer to the formatted "Date: ...\r\n" header string.

---

{#size-1}

#### size

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t size() const
```

Defined in src/http/include/icy/http/server.h:256

Returns the byte length of the formatted Date header string.

{#method}

## Method

```cpp
#include <icy/http/request.h>
```

```cpp
struct Method
```

Defined in src/http/include/icy/http/request.h:26

HTTP request methods.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const std::string` | [`Get`](#get-4) `static` |  |
| `const std::string` | [`Head`](#head) `static` |  |
| `const std::string` | [`Put`](#put) `static` |  |
| `const std::string` | [`Post`](#post-2) `static` |  |
| `const std::string` | [`Options`](#options-3) `static` |  |
| `const std::string` | [`Delete`](#delete) `static` |  |
| `const std::string` | [`Trace`](#trace) `static` |  |
| `const std::string` | [`Connect`](#connect-13) `static` |  |

---

{#get-4}

#### Get

`static`

```cpp
const std::string Get
```

Defined in src/http/include/icy/http/request.h:28

---

{#head}

#### Head

`static`

```cpp
const std::string Head
```

Defined in src/http/include/icy/http/request.h:29

---

{#put}

#### Put

`static`

```cpp
const std::string Put
```

Defined in src/http/include/icy/http/request.h:30

---

{#post-2}

#### Post

`static`

```cpp
const std::string Post
```

Defined in src/http/include/icy/http/request.h:31

---

{#options-3}

#### Options

`static`

```cpp
const std::string Options
```

Defined in src/http/include/icy/http/request.h:32

---

{#delete}

#### Delete

`static`

```cpp
const std::string Delete
```

Defined in src/http/include/icy/http/request.h:33

---

{#trace}

#### Trace

`static`

```cpp
const std::string Trace
```

Defined in src/http/include/icy/http/request.h:34

---

{#connect-13}

#### Connect

`static`

```cpp
const std::string Connect
```

Defined in src/http/include/icy/http/request.h:35

{#staticfileinfo}

## StaticFileInfo

```cpp
#include <icy/http/server.h>
```

```cpp
struct StaticFileInfo
```

Defined in src/http/include/icy/http/server.h:261

Metadata needed to serve a static file with HTTP validators.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`size`](#size-2)  | File size in bytes. |
| `Timestamp` | [`lastModified`](#lastmodified)  | Last modification time, normalized to HTTP-second precision. |
| `std::string` | [`etag`](#etag)  | Weak validator suitable for ETag/If-None-Match. |

---

{#size-2}

#### size

```cpp
uint64_t size = 0
```

Defined in src/http/include/icy/http/server.h:263

File size in bytes.

---

{#lastmodified}

#### lastModified

```cpp
Timestamp lastModified
```

Defined in src/http/include/icy/http/server.h:264

Last modification time, normalized to HTTP-second precision.

---

{#etag}

#### etag

```cpp
std::string etag
```

Defined in src/http/include/icy/http/server.h:265

Weak validator suitable for ETag/If-None-Match.

{#ws}

# ws

[WebSocket](#websocket) framing, handshakes, and connection helpers.

### Classes

| Name | Description |
|------|-------------|
| [`WebSocketException`](#websocketexception) |  |
| [`WebSocketFramer`](#websocketframer) | [WebSocket](#websocket) frame encoder/decoder and handshake validator for RFC 6455. |
| [`WebSocketAdapter`](#websocketadapter-1) | [WebSocket](#websocket) protocol adapter for both client and server endpoints. |
| [`WebSocket`](#websocket) | Standalone [WebSocket](#websocket) class. |
| [`ConnectionAdapter`](#connectionadapter-3) | [WebSocket](#websocket) class which belongs to an HTTP connection. |

### Enumerations

| Name | Description |
|------|-------------|
| [`Mode`](#mode-2)  | [WebSocket](#websocket) endpoint mode. |
| [`FrameFlags`](#frameflags)  | Frame header flags. |
| [`Opcode`](#opcode)  | Frame header opcodes. |
| [`SendFlags`](#sendflags)  | Combined header flags and opcodes for identifying the payload type of sent frames. |
| [`CloseStatusCode`](#closestatuscode)  | StatusCodes for CLOSE frames sent with shutdown(). |
| [`ErrorCode`](#errorcode)  | These error codes can be obtained from [WebSocket](#websocket) exceptions to determine the exact cause of the error. |
| [`CloseState`](#closestate)  |  |

---

{#mode-2}

#### Mode

```cpp
enum Mode
```

[WebSocket](#websocket) endpoint mode.

| Value | Description |
|-------|-------------|
| `ServerSide` | Server-side [WebSocket](#websocket). |
| `ClientSide` | Client-side [WebSocket](#websocket). |

---

{#frameflags}

#### FrameFlags

```cpp
enum FrameFlags
```

Frame header flags.

| Value | Description |
|-------|-------------|
| `Fin` | FIN bit: final fragment of a multi-fragment message. |
| `Rsv1` | Reserved for future use. Must be zero. |
| `Rsv2` | Reserved for future use. Must be zero. |
| `Rsv3` | Reserved for future use. Must be zero. |

---

{#opcode}

#### Opcode

```cpp
enum Opcode
```

Frame header opcodes.

| Value | Description |
|-------|-------------|
| `Continuation` | Continuation frame. |
| `Text` | Text frame. |
| `Binary` | Binary frame. |
| `Close` | Close connection. |
| `Ping` | Ping frame. |
| `Pong` | Pong frame. |
| `Bitmask` | Bit mask for opcodes. |

---

{#sendflags}

#### SendFlags

```cpp
enum SendFlags
```

Combined header flags and opcodes for identifying the payload type of sent frames.

| Value | Description |
|-------|-------------|
| `Text` |  |
| `Binary` |  |

---

{#closestatuscode}

#### CloseStatusCode

```cpp
enum CloseStatusCode
```

StatusCodes for CLOSE frames sent with shutdown().

| Value | Description |
|-------|-------------|
| `NormalClose` |  |
| `EndpointGoingAway` |  |
| `ProtocolError` |  |
| `PayloadNotAcceptable` |  |
| `Reserved` |  |
| `ReservedNoStatusCode` |  |
| `ReservedAbnormalClose` |  |
| `MalformedPayload` |  |
| `PolicyViolation` |  |
| `PayloadTooBig` |  |
| `ExtensionRequired` |  |
| `UnexpectedCondition` |  |
| `ReservedTLSFailure` |  |

---

{#errorcode}

#### ErrorCode

```cpp
enum ErrorCode
```

These error codes can be obtained from [WebSocket](#websocket) exceptions to determine the exact cause of the error.

| Value | Description |
|-------|-------------|
| `NoHandshake` | No [Connection](#connection-1): Upgrade or Upgrade: websocket header in handshake request. |
| `HandshakeNoVersion` | No Sec-WebSocket-Version header in handshake request. |
| `HandshakeUnsupportedVersion` | Unsupported [WebSocket](#websocket) version requested by client. |
| `HandshakeNoKey` | No Sec-WebSocket-Key header in handshake request. |
| `HandshakeAccept` | No Sec-WebSocket-Accept header or wrong value. |
| `Unauthorized` | The server rejected the username or password for authentication. |
| `PayloadTooBig` | Payload too big for supplied buffer. |
| `IncompleteFrame` | Incomplete frame received. |
| `InvalidRsvBits` | RSV bits set without extension negotiation. |
| `InvalidOpcode` | Unknown or reserved opcode received. |
| `UnmaskedClientFrame` | Client-to-server frame not masked (RFC 6455 violation). |
| `ProtocolViolation` | General RFC 6455 protocol violation. |

---

{#closestate}

#### CloseState

```cpp
enum CloseState
```

| Value | Description |
|-------|-------------|
| `Open` |  |
| `CloseSent` |  |
| `CloseReceived` |  |
| `Closed` |  |

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `auto` | [`ServerSide`](#serverside) `static` `constexpr` |  |
| `auto` | [`ClientSide`](#clientside) `static` `constexpr` |  |
| `char` | [`ProtocolGuid`](#protocolguid) `constexpr` |  |
| `char` | [`ProtocolVersion`](#protocolversion) `constexpr` | The [WebSocket](#websocket) protocol version supported (13). |

---

{#serverside}

#### ServerSide

`static` `constexpr`

```cpp
auto ServerSide = 
```

---

{#clientside}

#### ClientSide

`static` `constexpr`

```cpp
auto ClientSide = 
```

---

{#protocolguid}

#### ProtocolGuid

`constexpr`

```cpp
char ProtocolGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
```

---

{#protocolversion}

#### ProtocolVersion

`constexpr`

```cpp
char ProtocolVersion = "13"
```

The [WebSocket](#websocket) protocol version supported (13).

{#websocketexception}

## WebSocketException

```cpp
#include <icy/http/websocket.h>
```

```cpp
class WebSocketException
```

Defined in src/http/include/icy/http/websocket.h:121

> **Inherits:** `runtime_error`

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebSocketException`](#websocketexception-1) `inline` |  |
| `ErrorCode` | [`code`](#code) `const` `inline` `nodiscard` |  |
| `bool` | [`hasCloseStatus`](#hasclosestatus) `const` `inline` `nodiscard` |  |
| `uint16_t` | [`closeStatus`](#closestatus) `const` `inline` `nodiscard` |  |

---

{#websocketexception-1}

#### WebSocketException

`inline`

```cpp
inline WebSocketException(ErrorCode code, std::string message, uint16_t closeStatus = 0)
```

Defined in src/http/include/icy/http/websocket.h:124

---

{#code}

#### code

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline ErrorCode code() const
```

Defined in src/http/include/icy/http/websocket.h:131

---

{#hasclosestatus}

#### hasCloseStatus

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool hasCloseStatus() const
```

Defined in src/http/include/icy/http/websocket.h:132

---

{#closestatus}

#### closeStatus

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uint16_t closeStatus() const
```

Defined in src/http/include/icy/http/websocket.h:133

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ErrorCode` | [`_code`](#_code)  |  |
| `uint16_t` | [`_closeStatus`](#_closestatus)  |  |

---

{#_code}

#### _code

```cpp
ErrorCode _code
```

Defined in src/http/include/icy/http/websocket.h:136

---

{#_closestatus}

#### _closeStatus

```cpp
uint16_t _closeStatus
```

Defined in src/http/include/icy/http/websocket.h:137

{#websocketframer}

## WebSocketFramer

```cpp
#include <icy/http/websocket.h>
```

```cpp
class WebSocketFramer
```

Defined in src/http/include/icy/http/websocket.h:156

[WebSocket](#websocket) frame encoder/decoder and handshake validator for RFC 6455.

### Friends

| Name | Description |
|------|-------------|
| [`WebSocketAdapter`](#websocketadapter)  |  |
| [`wsFramerTestAccess`](#wsframertestaccess)  |  |
| [`wsFramerGetFlags`](#wsframergetflags)  |  |

---

{#websocketadapter}

#### WebSocketAdapter

```cpp
friend class WebSocketAdapter
```

Defined in src/http/include/icy/http/websocket.h:253

---

{#wsframertestaccess}

#### wsFramerTestAccess

```cpp
friend void wsFramerTestAccess(WebSocketFramer & f, int state)
```

Defined in src/http/include/icy/http/websocket.h:256

---

{#wsframergetflags}

#### wsFramerGetFlags

```cpp
friend int wsFramerGetFlags(const WebSocketFramer & f)
```

Defined in src/http/include/icy/http/websocket.h:257

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebSocketFramer`](#websocketframer-1)  | Creates a [WebSocketFramer](#websocketframer) operating in the given endpoint mode. Client-side framers mask outgoing payloads; server-side framers do not. |
| `size_t` | [`writeFrame`](#writeframe) `virtual` | Encodes `data` into a [WebSocket](#websocket) frame and writes it to `frame`. |
| `uint64_t` | [`readFrame`](#readframe) `virtual` | Decodes a single [WebSocket](#websocket) frame from `frame`. |
| `bool` | [`handshakeComplete`](#handshakecomplete) `const` `nodiscard` | Returns true if the [WebSocket](#websocket) handshake has completed successfully. |
| `void` | [`acceptServerRequest`](#acceptserverrequest)  | [Server](#server) side. |
| `void` | [`createClientHandshakeRequest`](#createclienthandshakerequest)  | [Client](#client) side. |
| `bool` | [`checkClientHandshakeResponse`](#checkclienthandshakeresponse)  | Validates the server's 101 Switching Protocols response. |
| `void` | [`completeClientHandshake`](#completeclienthandshake)  | Completes the client-side handshake by verifying [Connection](#connection-1), Upgrade and Sec-WebSocket-Accept headers. Advances internal state to "complete". |

---

{#websocketframer-1}

#### WebSocketFramer

```cpp
WebSocketFramer(ws::Mode mode)
```

Defined in src/http/include/icy/http/websocket.h:162

Creates a [WebSocketFramer](#websocketframer) operating in the given endpoint mode. Client-side framers mask outgoing payloads; server-side framers do not. 
#### Parameters
* `mode` ServerSide or ClientSide.

---

{#writeframe}

#### writeFrame

`virtual`

```cpp
virtual size_t writeFrame(const char * data, size_t len, int flags, BitWriter & frame)
```

Defined in src/http/include/icy/http/websocket.h:173

Encodes `data` into a [WebSocket](#websocket) frame and writes it to `frame`. 
#### Parameters
* `data` Pointer to the payload data. 

* `len` Payload length in bytes. 

* `flags` Frame flags: [ws::SendFlags::Text](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2afdf2a2c6b74320a613fe71490a96e2ae), [ws::SendFlags::Binary](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2a0ca55f2c2a2e576e0aa1c08a4e845ed7), or a control frame opcode combined with [FrameFlags::Fin](#namespaceicy_1_1http_1_1ws_1a4317f7f44088d3ab553950bab6b16ab8a0475def81fbd4e70bf7cd07ee307793a). 

* `frame` [BitWriter](base.md#bitwriter) to write the encoded frame into. 

#### Returns
Total number of bytes written to the frame buffer (header + payload).

---

{#readframe}

#### readFrame

`virtual`

```cpp
virtual uint64_t readFrame(BitReader & frame, char *& payload)
```

Defined in src/http/include/icy/http/websocket.h:184

Decodes a single [WebSocket](#websocket) frame from `frame`.

The payload is unmasked in-place in the source buffer; no copy is made. `payload` is set to point at the start of the payload within `frame`'s buffer.

#### Parameters
* `frame` [BitReader](base.md#bitreader) positioned at the start of a frame. 

* `payload` Set to point at the start of the decoded payload. Not null-terminated. 

#### Returns
Payload length in bytes. 

#### Exceptions
* `std::runtime_error` on protocol violations or if the buffer is too small.

---

{#handshakecomplete}

#### handshakeComplete

`const` `nodiscard`

```cpp
[[nodiscard]] bool handshakeComplete() const
```

Defined in src/http/include/icy/http/websocket.h:187

Returns true if the [WebSocket](#websocket) handshake has completed successfully.

---

{#acceptserverrequest}

#### acceptServerRequest

```cpp
void acceptServerRequest(http::Request & request, http::Response & response)
```

Defined in src/http/include/icy/http/websocket.h:197

[Server](#server) side.

Validates the client upgrade request and writes a 101 Switching Protocols response. Sets the internal state to mark the handshake as complete. 
#### Parameters
* `request` Incoming HTTP upgrade request. 

* `response` HTTP response to populate with the handshake reply. 

#### Exceptions
* `std::runtime_error` if the request is not a valid [WebSocket](#websocket) upgrade.

---

{#createclienthandshakerequest}

#### createClientHandshakeRequest

```cpp
void createClientHandshakeRequest(http::Request & request)
```

Defined in src/http/include/icy/http/websocket.h:205

[Client](#client) side.

Populates `request` with the [WebSocket](#websocket) upgrade headers ([Connection](#connection-1), Upgrade, Sec-WebSocket-Key, Sec-WebSocket-Version) to initiate the handshake. 
#### Parameters
* `request` HTTP request to add upgrade headers to.

---

{#checkclienthandshakeresponse}

#### checkClientHandshakeResponse

```cpp
bool checkClientHandshakeResponse(http::Response & response)
```

Defined in src/http/include/icy/http/websocket.h:211

Validates the server's 101 Switching Protocols response. 
#### Parameters
* `response` The HTTP response received from the server. 

#### Returns
true if the handshake succeeded and data can flow. 

#### Exceptions
* `std::runtime_error` if the server rejected or mishandled the upgrade.

---

{#completeclienthandshake}

#### completeClientHandshake

```cpp
void completeClientHandshake(http::Response & response)
```

Defined in src/http/include/icy/http/websocket.h:217

Completes the client-side handshake by verifying [Connection](#connection-1), Upgrade and Sec-WebSocket-Accept headers. Advances internal state to "complete". 
#### Parameters
* `response` The 101 Switching Protocols response from the server. 

#### Exceptions
* `std::runtime_error` if any required header is missing or incorrect.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`frameFlags`](#frameflags-1) `const` `nodiscard` | Returns the frame flags of the most recently received frame. Set by [readFrame()](#readframe) |
| `bool` | [`mustMaskPayload`](#mustmaskpayload) `const` `nodiscard` | Returns true if the payload must be masked. Used by [writeFrame()](#writeframe) |
| `ws::Mode` | [`mode`](#mode-3) `const` `nodiscard` |  |

---

{#frameflags-1}

#### frameFlags

`const` `nodiscard`

```cpp
[[nodiscard]] int frameFlags() const
```

Defined in src/http/include/icy/http/websocket.h:222

Returns the frame flags of the most recently received frame. Set by [readFrame()](#readframe)

---

{#mustmaskpayload}

#### mustMaskPayload

`const` `nodiscard`

```cpp
[[nodiscard]] bool mustMaskPayload() const
```

Defined in src/http/include/icy/http/websocket.h:226

Returns true if the payload must be masked. Used by [writeFrame()](#writeframe)

---

{#mode-3}

#### mode

`const` `nodiscard`

```cpp
[[nodiscard]] ws::Mode mode() const
```

Defined in src/http/include/icy/http/websocket.h:228

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ws::Mode` | [`_mode`](#_mode-3)  |  |
| `int` | [`_frameFlags`](#_frameflags)  |  |
| `int` | [`_headerState`](#_headerstate)  |  |
| `bool` | [`_maskPayload`](#_maskpayload)  |  |
| `Random` | [`_rnd`](#_rnd)  |  |
| `std::string` | [`_key`](#_key)  |  |
| `bool` | [`_fragmented`](#_fragmented)  | Currently receiving a fragmented message. |
| `int` | [`_fragmentOpcode`](#_fragmentopcode)  | [Opcode](#opcode) of the first frame in the fragment sequence. |
| `Buffer` | [`_fragmentBuffer`](#_fragmentbuffer)  | Accumulated payload from continuation frames. |
| `Buffer` | [`_incompleteFrame`](#_incompleteframe)  | [Buffer](base.md#buffer-2) for incomplete frame data across TCP segments. |

---

{#_mode-3}

#### _mode

```cpp
ws::Mode _mode
```

Defined in src/http/include/icy/http/websocket.h:238

---

{#_frameflags}

#### _frameFlags

```cpp
int _frameFlags
```

Defined in src/http/include/icy/http/websocket.h:239

---

{#_headerstate}

#### _headerState

```cpp
int _headerState
```

Defined in src/http/include/icy/http/websocket.h:240

---

{#_maskpayload}

#### _maskPayload

```cpp
bool _maskPayload
```

Defined in src/http/include/icy/http/websocket.h:241

---

{#_rnd}

#### _rnd

```cpp
Random _rnd
```

Defined in src/http/include/icy/http/websocket.h:242

---

{#_key}

#### _key

```cpp
std::string _key
```

Defined in src/http/include/icy/http/websocket.h:243

---

{#_fragmented}

#### _fragmented

```cpp
bool _fragmented {false}
```

Defined in src/http/include/icy/http/websocket.h:246

Currently receiving a fragmented message.

---

{#_fragmentopcode}

#### _fragmentOpcode

```cpp
int _fragmentOpcode {0}
```

Defined in src/http/include/icy/http/websocket.h:247

[Opcode](#opcode) of the first frame in the fragment sequence.

---

{#_fragmentbuffer}

#### _fragmentBuffer

```cpp
Buffer _fragmentBuffer
```

Defined in src/http/include/icy/http/websocket.h:248

Accumulated payload from continuation frames.

---

{#_incompleteframe}

#### _incompleteFrame

```cpp
Buffer _incompleteFrame
```

Defined in src/http/include/icy/http/websocket.h:251

[Buffer](base.md#buffer-2) for incomplete frame data across TCP segments.

{#websocketadapter-1}

## WebSocketAdapter

```cpp
#include <icy/http/websocket.h>
```

```cpp
class WebSocketAdapter
```

Defined in src/http/include/icy/http/websocket.h:267

> **Inherits:** [`SocketEmitter`](net.md#socketemitter)
> **Subclassed by:** [`ConnectionAdapter`](#connectionadapter-3), [`WebSocket`](#websocket)

[WebSocket](#websocket) protocol adapter for both client and server endpoints.

### Friends

| Name | Description |
|------|-------------|
| [`WebSocketFramer`](#websocketframer-2)  |  |

---

{#websocketframer-2}

#### WebSocketFramer

```cpp
friend class WebSocketFramer
```

Defined in src/http/include/icy/http/websocket.h:350

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::Socket::Ptr` | [`socket`](#socket-6)  | Pointer to the underlying socket. Sent data will be proxied to this socket. |

---

{#socket-6}

#### socket

```cpp
net::Socket::Ptr socket
```

Defined in src/http/include/icy/http/websocket.h:304

Pointer to the underlying socket. Sent data will be proxied to this socket.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebSocketAdapter`](#websocketadapter-2)  | Creates a [WebSocketAdapter](#websocketadapter-1) using the given socket, mode and HTTP message objects. |
| `ssize_t` | [`send`](#send-14) `virtual` `override` | Frames and sends data to the peer's address. |
| `ssize_t` | [`send`](#send-15) `virtual` `override` | Frames and sends data to a specific peer address (for UDP-backed sockets). |
| `ssize_t` | [`sendOwned`](#sendowned-12) `virtual` `override` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`sendOwned`](#sendowned-13) `virtual` `override` |  |
| `bool` | [`shutdown`](#shutdown-8) `virtual` | Sends a [WebSocket](#websocket) CLOSE frame with the given status code and message, then closes the underlying socket. |
| `void` | [`sendClientRequest`](#sendclientrequest) `virtual` | [Client](#client) side. |
| `void` | [`handleClientResponse`](#handleclientresponse) `virtual` | Parses the server's HTTP upgrade response and completes the handshake. Any data remaining in the buffer after the HTTP response is re-fed as [WebSocket](#websocket) frames. |
| `void` | [`handleServerRequest`](#handleserverrequest) `virtual` | [Server](#server) side. |
| `void` | [`onHandshakeComplete`](#onhandshakecomplete) `virtual` | Called when the [WebSocket](#websocket) handshake completes. Emits the connect event to downstream handlers. |

---

{#websocketadapter-2}

#### WebSocketAdapter

```cpp
WebSocketAdapter(const net::Socket::Ptr & socket, ws::Mode mode, http::Request & request, http::Response & response)
```

Defined in src/http/include/icy/http/websocket.h:275

Creates a [WebSocketAdapter](#websocketadapter-1) using the given socket, mode and HTTP message objects. 
#### Parameters
* `socket` The underlying TCP or SSL socket. 

* `mode` ServerSide or ClientSide. 

* `request` HTTP request used for the handshake. 

* `response` HTTP response used for the handshake.

---

{#send-14}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/http/include/icy/http/websocket.h:283

Frames and sends data to the peer's address. 
#### Parameters
* `data` Pointer to the payload. 

* `len` Payload length in bytes. 

* `flags` [ws::SendFlags::Text](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2afdf2a2c6b74320a613fe71490a96e2ae) or [ws::SendFlags::Binary](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2a0ca55f2c2a2e576e0aa1c08a4e845ed7). 

#### Returns
Number of bytes sent, or -1 on error.

---

{#send-15}

#### send

`virtual` `override`

```cpp
virtual ssize_t send(const char * data, size_t len, const net::Address & peerAddr, int flags = 0) override
```

Defined in src/http/include/icy/http/websocket.h:291

Frames and sends data to a specific peer address (for UDP-backed sockets). 
#### Parameters
* `data` Pointer to the payload. 

* `len` Payload length in bytes. 

* `peerAddr` Destination address. 

* `flags` [ws::SendFlags::Text](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2afdf2a2c6b74320a613fe71490a96e2ae) or [ws::SendFlags::Binary](#namespaceicy_1_1http_1_1ws_1a366b9e6ee51be48df27ea06f8d20e7a2a0ca55f2c2a2e576e0aa1c08a4e845ed7). 

#### Returns
Number of bytes sent, or -1 on error.

---

{#sendowned-12}

#### sendOwned

`virtual` `override`

```cpp
virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/http/include/icy/http/websocket.h:292

Sends an owned payload buffer to the connected peer.

The buffer is moved through the adapter chain and retained by the transport layer until async write completion.

---

{#sendowned-13}

#### sendOwned

`virtual` `override`

```cpp
virtual ssize_t sendOwned(Buffer && buffer, const net::Address & peerAddr, int flags = 0) override
```

Defined in src/http/include/icy/http/websocket.h:293

---

{#shutdown-8}

#### shutdown

`virtual`

```cpp
virtual bool shutdown(uint16_t statusCode, const std::string & statusMessage)
```

Defined in src/http/include/icy/http/websocket.h:300

Sends a [WebSocket](#websocket) CLOSE frame with the given status code and message, then closes the underlying socket. 
#### Parameters
* `statusCode` [WebSocket](#websocket) close status code (e.g. 1000 for normal close). 

* `statusMessage` Human-readable reason for closing. 

#### Returns
true if the close frame was sent successfully.

---

{#sendclientrequest}

#### sendClientRequest

`virtual`

```cpp
virtual void sendClientRequest()
```

Defined in src/http/include/icy/http/websocket.h:311

[Client](#client) side.

Sends the [WebSocket](#websocket) HTTP upgrade request to initiate the handshake. Called automatically on socket connect.

---

{#handleclientresponse}

#### handleClientResponse

`virtual`

```cpp
virtual void handleClientResponse(const MutableBuffer & buffer, const net::Address & peerAddr)
```

Defined in src/http/include/icy/http/websocket.h:317

Parses the server's HTTP upgrade response and completes the handshake. Any data remaining in the buffer after the HTTP response is re-fed as [WebSocket](#websocket) frames. 
#### Parameters
* `buffer` [Buffer](base.md#buffer-2) containing the server's HTTP response. 

* `peerAddr` Address of the peer.

---

{#handleserverrequest}

#### handleServerRequest

`virtual`

```cpp
virtual void handleServerRequest(const MutableBuffer & buffer, const net::Address & peerAddr)
```

Defined in src/http/include/icy/http/websocket.h:325

[Server](#server) side.

Parses the client's HTTP upgrade request and sends the 101 response. 
#### Parameters
* `buffer` [Buffer](base.md#buffer-2) containing the client's HTTP upgrade request. 

* `peerAddr` Address of the peer.

---

{#onhandshakecomplete}

#### onHandshakeComplete

`virtual`

```cpp
virtual void onHandshakeComplete()
```

Defined in src/http/include/icy/http/websocket.h:339

Called when the [WebSocket](#websocket) handshake completes. Emits the connect event to downstream handlers.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `WebSocketFramer` | [`framer`](#framer)  |  |
| `http::Request &` | [`_request`](#_request-2)  |  |
| `http::Response &` | [`_response`](#_response-2)  |  |
| `ws::CloseState` | [`_closeState`](#_closestate)  |  |

---

{#framer}

#### framer

```cpp
WebSocketFramer framer
```

Defined in src/http/include/icy/http/websocket.h:352

---

{#_request-2}

#### _request

```cpp
http::Request & _request
```

Defined in src/http/include/icy/http/websocket.h:353

---

{#_response-2}

#### _response

```cpp
http::Response & _response
```

Defined in src/http/include/icy/http/websocket.h:354

---

{#_closestate}

#### _closeState

```cpp
ws::CloseState _closeState {}
```

Defined in src/http/include/icy/http/websocket.h:355

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`sendControlFrame`](#sendcontrolframe)  |  |
| `void` | [`resetFrameState`](#resetframestate)  |  |

---

{#sendcontrolframe}

#### sendControlFrame

```cpp
bool sendControlFrame(ws::Opcode opcode, const char * payload, size_t payloadLen, const net::Address & peerAddr)
```

Defined in src/http/include/icy/http/websocket.h:344

---

{#resetframestate}

#### resetFrameState

```cpp
void resetFrameState()
```

Defined in src/http/include/icy/http/websocket.h:348

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`onSocketConnect`](#onsocketconnect-4) `virtual` `override` | Called by the socket on connect; initiates the client handshake. |
| `bool` | [`onSocketRecv`](#onsocketrecv-6) `virtual` `override` | Called by the socket on each received buffer; handles handshake or frame parsing depending on state. |
| `bool` | [`onSocketClose`](#onsocketclose-4) `virtual` `override` | Called by the socket on close; resets framer state. |

---

{#onsocketconnect-4}

#### onSocketConnect

`virtual` `override`

```cpp
virtual bool onSocketConnect(net::Socket & socket) override
```

Defined in src/http/include/icy/http/websocket.h:328

Called by the socket on connect; initiates the client handshake.

---

{#onsocketrecv-6}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress) override
```

Defined in src/http/include/icy/http/websocket.h:332

Called by the socket on each received buffer; handles handshake or frame parsing depending on state.

---

{#onsocketclose-4}

#### onSocketClose

`virtual` `override`

```cpp
virtual bool onSocketClose(net::Socket & socket) override
```

Defined in src/http/include/icy/http/websocket.h:335

Called by the socket on close; resets framer state.

{#websocket}

## WebSocket

```cpp
#include <icy/http/websocket.h>
```

```cpp
class WebSocket
```

Defined in src/http/include/icy/http/websocket.h:365

> **Inherits:** [`WebSocketAdapter`](#websocketadapter-1)

Standalone [WebSocket](#websocket) class.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebSocket`](#websocket-1)  | Creates the [WebSocket](#websocket) with the given Socket. The Socket should be a TCPSocket or a SSLSocket, depending on the protocol used (ws or wss). |
| `http::Request &` | [`request`](#request-11) `nodiscard` | Returns the HTTP request used during the [WebSocket](#websocket) handshake. |
| `http::Response &` | [`response`](#response-9) `nodiscard` | Returns the HTTP response received during the [WebSocket](#websocket) handshake. |

---

{#websocket-1}

#### WebSocket

```cpp
WebSocket(const net::Socket::Ptr & socket)
```

Defined in src/http/include/icy/http/websocket.h:373

Creates the [WebSocket](#websocket) with the given Socket. The Socket should be a TCPSocket or a SSLSocket, depending on the protocol used (ws or wss).

---

{#request-11}

#### request

`nodiscard`

```cpp
[[nodiscard]] http::Request & request()
```

Defined in src/http/include/icy/http/websocket.h:378

Returns the HTTP request used during the [WebSocket](#websocket) handshake.

---

{#response-9}

#### response

`nodiscard`

```cpp
[[nodiscard]] http::Response & response()
```

Defined in src/http/include/icy/http/websocket.h:381

Returns the HTTP response received during the [WebSocket](#websocket) handshake.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `http::Request` | [`_request`](#_request-3)  |  |
| `http::Response` | [`_response`](#_response-3)  |  |

---

{#_request-3}

#### _request

```cpp
http::Request _request
```

Defined in src/http/include/icy/http/websocket.h:384

---

{#_response-3}

#### _response

```cpp
http::Response _response
```

Defined in src/http/include/icy/http/websocket.h:385

### Public Types

| Name | Description |
|------|-------------|
| [`Vec`](#vec-4)  |  |

---

{#vec-4}

#### Vec

```cpp
using Vec = std::vector< WebSocket >
```

Defined in src/http/include/icy/http/websocket.h:368

{#connectionadapter-3}

## ConnectionAdapter

```cpp
#include <icy/http/websocket.h>
```

```cpp
class ConnectionAdapter
```

Defined in src/http/include/icy/http/websocket.h:395

> **Inherits:** [`WebSocketAdapter`](#websocketadapter-1)

[WebSocket](#websocket) class which belongs to an HTTP connection.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ConnectionAdapter`](#connectionadapter-4)  | Creates a [ConnectionAdapter](#connectionadapter-3) for upgrading an existing HTTP connection to [WebSocket](#websocket). Disables automatic header sending on the underlying connection. |
| `void` | [`onHandshakeComplete`](#onhandshakecomplete-1) `virtual` | Called when the [WebSocket](#websocket) handshake completes. Emits the connect event via the socket emitter chain. |

---

{#connectionadapter-4}

#### ConnectionAdapter

```cpp
ConnectionAdapter(Connection * connection, ws::Mode mode)
```

Defined in src/http/include/icy/http/websocket.h:402

Creates a [ConnectionAdapter](#connectionadapter-3) for upgrading an existing HTTP connection to [WebSocket](#websocket). Disables automatic header sending on the underlying connection. 
#### Parameters
* `connection` The HTTP connection to upgrade. 

* `mode` ServerSide or ClientSide.

---

{#onhandshakecomplete-1}

#### onHandshakeComplete

`virtual`

```cpp
virtual void onHandshakeComplete()
```

Defined in src/http/include/icy/http/websocket.h:407

Called when the [WebSocket](#websocket) handshake completes. Emits the connect event via the socket emitter chain.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Connection *` | [`_connection`](#_connection-3)  |  |

---

{#_connection-3}

#### _connection

```cpp
Connection * _connection
```

Defined in src/http/include/icy/http/websocket.h:410

