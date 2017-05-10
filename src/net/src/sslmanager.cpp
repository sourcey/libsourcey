///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{
// This file uses functions from POCO C++ Libraries (license below)
//


#include "scy/net/sslmanager.h"
#include "scy/net/sslcontext.h"
#include "scy/singleton.h"


using namespace std;


namespace scy {
namespace net {


int SSLManager::verifyServerCallback(int ok, X509_STORE_CTX* pStore)
{
    return SSLManager::verifyCallback(true, ok, pStore);
}


int SSLManager::verifyClientCallback(int ok, X509_STORE_CTX* pStore)
{
    return SSLManager::verifyCallback(false, ok, pStore);
}


void SSLManager::initNoVerifyClient()
{
    net::SSLManager::instance().initializeClient(
        std::shared_ptr<net::SSLContext>(
            new net::SSLContext(net::SSLContext::CLIENT_USE, "", "", "",
                                net::SSLContext::VERIFY_NONE, 9, false,
                                "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")));
}


void SSLManager::initNoVerifyServer(const std::string& privateKeyFile,
                                    const std::string& certificateFile)
{
    // Create server/client self-signed certificate/key (self signed, DONT ADD
    // PASSWORD)
    //
    // openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout
    // client-key.pem -out client-cert.pem
    // openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout
    // server-key.pem -out server-cert.pem

    // const char defaultPrivateKey[] =
    //     "-----BEGIN RSA PRIVATE KEY-----\n"
    //     "MIICXAIBAAKBgQC2tDv6v//aJX8HoX7hugfReoWftqVxX2WmO8CbBFc0qfEChrR/\n"
    //     "3sCNg8Y0squOmQ1deEElE4h1tFtmcI14Ll/NfVr4kKjspK3MFe4ZJmvbtO0WZxXg\n"
    //     "f72AhEhw0e1mYkufFsmwiGQZHzJVh2Yll7h5PmV2TXOgHVp2A8XWFmEIEwIDAQAB\n"
    //     "AoGAAlVY8sHi/aE+9xT77twWX3mGHV0SzdjfDnly40fx6S1Gc7bOtVdd9DC7pk6l\n"
    //     "3ENeJVR02IlgU8iC5lMHq4JEHPE272jtPrLlrpWLTGmHEqoVFv9AITPqUDLhB9Kk\n"
    //     "Hjl7h8NYBKbr2JHKICr3DIPKOT+RnXVb1PD4EORbJ3ooYmkCQQDfknUnVxPgxUGs\n"
    //     "ouABw1WJIOVgcCY/IFt4Ihf6VWTsxBgzTJKxn3HtgvE0oqTH7V480XoH0QxHhjLq\n"
    //     "DrgobWU9AkEA0TRJ8/ouXGnFEPAXjWr9GdPQRZ1Use2MrFjneH2+Sxc0CmYtwwqL\n"
    //     "Kr5kS6mqJrxprJeluSjBd+3/ElxURrEXjwJAUvmlN1OPEhXDmRHd92mKnlkyKEeX\n"
    //     "OkiFCiIFKih1S5Y/sRJTQ0781nyJjtJqO7UyC3pnQu1oFEePL+UEniRztQJAMfav\n"
    //     "AtnpYKDSM+1jcp7uu9BemYGtzKDTTAYfoiNF42EzSJiGrWJDQn4eLgPjY0T0aAf/\n"
    //     "yGz3Z9ErbhMm/Ysl+QJBAL4kBxRT8gM4ByJw4sdOvSeCCANFq8fhbgm8pGWlCPb5\n"
    //     "JGmX3/GHFM8x2tbWMGpyZP1DLtiNEFz7eCGktWK5rqE=\n"
    //     "-----END RSA PRIVATE KEY-----";

    // const char defaultCertificate[] =
    //     "-----BEGIN CERTIFICATE-----\n"
    //     "MIICATCCAWoCCQDPufXH86n2QzANBgkqhkiG9w0BAQUFADBFMQswCQYDVQQGEwJu\n"
    //     "bzETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50ZXJuZXQgV2lkZ2l0\n"
    //     "cyBQdHkgTHRkMB4XDTEyMDEwMTE0NDQwMFoXDTIwMDMxOTE0NDQwMFowRTELMAkG\n"
    //     "A1UEBhMCbm8xEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoMGEludGVybmV0\n"
    //     "IFdpZGdpdHMgUHR5IEx0ZDCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAtrQ7\n"
    //     "+r//2iV/B6F+4boH0XqFn7alcV9lpjvAmwRXNKnxAoa0f97AjYPGNLKrjpkNXXhB\n"
    //     "JROIdbRbZnCNeC5fzX1a+JCo7KStzBXuGSZr27TtFmcV4H+9gIRIcNHtZmJLnxbJ\n"
    //     "sIhkGR8yVYdmJZe4eT5ldk1zoB1adgPF1hZhCBMCAwEAATANBgkqhkiG9w0BAQUF\n"
    //     "AAOBgQCeWBEHYJ4mCB5McwSSUox0T+/mJ4W48L/ZUE4LtRhHasU9hiW92xZkTa7E\n"
    //     "QLcoJKQiWfiLX2ysAro0NX4+V8iqLziMqvswnPzz5nezaOLE/9U/QvH3l8qqNkXu\n"
    //     "rNbsW1h/IO6FV8avWFYVFoutUwOaZ809k7iMh2F2JMgXQ5EymQ==\n"
    //     "-----END CERTIFICATE-----";

    net::SSLManager::instance().initializeServer(
        std::shared_ptr<net::SSLContext>(new net::SSLContext(
            net::SSLContext::SERVER_USE,
            privateKeyFile.empty()
                ? (std::string(SCY_SOURCE_DIR) + "/net/tests/key.pem")
                : privateKeyFile,
            certificateFile.empty()
                ? (std::string(SCY_SOURCE_DIR) + "/net/tests/cert.pem")
                : certificateFile,
            "", net::SSLContext::VERIFY_NONE, 9, false,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")));
}


SSLManager::SSLManager()
{
}


SSLManager::~SSLManager()
{
    shutdown();
}


void SSLManager::shutdown()
{
    PrivateKeyPassphraseRequired.detachAll();
    ClientVerificationError.detachAll();
    ServerVerificationError.detachAll();
    _defaultServerContext = nullptr;
    _defaultClientContext = nullptr;
}


Singleton<SSLManager>& singleton()
{
    static Singleton<SSLManager> singleton;
    return singleton;
}


SSLManager& SSLManager::instance()
{
    return *singleton().get();
}


void SSLManager::destroy()
{
    singleton().destroy();
}


void SSLManager::initializeServer(SSLContext::Ptr ptrContext)
{
    _defaultServerContext = ptrContext;
}


void SSLManager::initializeClient(SSLContext::Ptr ptrContext)
{
    _defaultClientContext = ptrContext;
}


SSLContext::Ptr SSLManager::defaultServerContext()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _defaultServerContext;
}


SSLContext::Ptr SSLManager::defaultClientContext()
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _defaultClientContext;
}


int SSLManager::verifyCallback(bool server, int ok, X509_STORE_CTX* pStore)
{
    if (!ok) {
        X509* pCert = X509_STORE_CTX_get_current_cert(pStore);
         crypto::X509Certificate x509(pCert, true);
        int depth = X509_STORE_CTX_get_error_depth(pStore);
        int err = X509_STORE_CTX_get_error(pStore);
        std::string error(X509_verify_cert_error_string(err));
        VerificationErrorDetails args(x509, depth, err, error);
        if (server)
            SSLManager::instance().ServerVerificationError.emit(args);
        else
            SSLManager::instance().ClientVerificationError.emit(args);
        ok = args.getIgnoreError() ? 1 : 0;
    }

    return ok;
}


int SSLManager::privateKeyPassphraseCallback(char* pBuf, int size, int flag,
                                             void* userData)
{
    std::string pwd;
    SSLManager::instance().PrivateKeyPassphraseRequired.emit(
        /*&SSLManager::instance(), */ pwd);

    strncpy(pBuf, (char*)(pwd.c_str()), size);
    pBuf[size - 1] = '\0';
    if (size > (int)pwd.length())
        size = (int)pwd.length();

    return size;
}


void initializeSSL()
{
    crypto::initializeEngine();
}


void uninitializeSSL()
{
    SSLManager::instance().shutdown();
    crypto::uninitializeEngine();
}


//
// Verification Error Details
//


VerificationErrorDetails::VerificationErrorDetails(
    const crypto::X509Certificate& cert, int errDepth, int errNum,
    const std::string& errMsg)
    : _cert(cert)
    , _errorDepth(errDepth)
    , _errorNumber(errNum)
    , _errorMessage(errMsg)
    , _ignoreError(false)
{
}


VerificationErrorDetails::~VerificationErrorDetails()
{
}


} // namespace net
} // namespace scy


/// @\}


// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
