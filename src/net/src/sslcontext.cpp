///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup net
/// @{


#include "scy/net/sslcontext.h"
#include "scy/crypto/crypto.h"
#include "scy/datetime.h"
#include "scy/filesystem.h"
#include "scy/net/sslmanager.h"


using namespace std;


namespace scy {
namespace net {


SSLContext::SSLContext(Usage usage, const std::string& privateKeyFile,
                       const std::string& certificateFile,
                       const std::string& caLocation,
                       VerificationMode verificationMode, int verificationDepth,
                       bool loadDefaultCAs, const std::string& cipherList)
    : _usage(usage)
    , _mode(verificationMode)
    , _sslContext(0)
    , _extendedVerificationErrorDetails(true)
{
    crypto::initializeEngine();

    createSSLContext();

    int errCode = 0;
    if (!caLocation.empty()) {
        if (fs::isdir(caLocation))
            errCode = SSL_CTX_load_verify_locations(
                _sslContext, 0, fs::transcode(caLocation).c_str());
        else
            errCode = SSL_CTX_load_verify_locations(
                _sslContext, fs::transcode(caLocation).c_str(), 0);
        if (errCode != 1) {
            std::string msg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                std::string("SSL Error: Cannot load CA file/directory at ") +
                caLocation + ": " + msg);
        }
    }

    if (loadDefaultCAs) {
        errCode = SSL_CTX_set_default_verify_paths(_sslContext);
        if (errCode != 1) {
            std::string msg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                "SSL Error: Cannot load default CA certificates: " + msg);
        }
    }

    if (!privateKeyFile.empty()) {
        errCode = SSL_CTX_use_PrivateKey_file(
            _sslContext, fs::transcode(privateKeyFile).c_str(),
            SSL_FILETYPE_PEM);
        if (errCode != 1) {
            std::string msg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                std::string("SSL Error: Error loading private key from file ") +
                privateKeyFile + ": " + msg);
        }
    }

    if (!certificateFile.empty()) {
        errCode = SSL_CTX_use_certificate_chain_file(
            _sslContext, fs::transcode(certificateFile).c_str());
        if (errCode != 1) {
            std::string errMsg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                std::string("SSL Error: Error loading certificate from file ") +
                certificateFile + ": " + errMsg); //, errMsg);
        }
    }

    if (isForServerUse())
        SSL_CTX_set_verify(_sslContext, verificationMode,
                           &SSLManager::verifyServerCallback);
    else
        SSL_CTX_set_verify(_sslContext, verificationMode,
                           &SSLManager::verifyClientCallback);

    SSL_CTX_set_cipher_list(_sslContext, cipherList.c_str());
    SSL_CTX_set_verify_depth(_sslContext, verificationDepth);
    SSL_CTX_set_mode(_sslContext, SSL_MODE_AUTO_RETRY);
    SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
}


SSLContext::SSLContext(Usage usage, const std::string& caLocation,
                       VerificationMode verificationMode, int verificationDepth,
                       bool loadDefaultCAs, const std::string& cipherList)
    : _usage(usage)
    , _mode(verificationMode)
    , _sslContext(0)
    , _extendedVerificationErrorDetails(true)
{
    crypto::initializeEngine();

    createSSLContext();

    int errCode = 0;
    if (!caLocation.empty()) {
        if (fs::isdir(caLocation))
            errCode = SSL_CTX_load_verify_locations(
                _sslContext, 0, fs::transcode(caLocation).c_str());
        else
            errCode = SSL_CTX_load_verify_locations(
                _sslContext, fs::transcode(caLocation).c_str(), 0);
        if (errCode != 1) {
            std::string msg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                std::string("SSL Error: Cannot load CA file/directory at ") +
                caLocation + ": " + msg);
        }
    }

    if (loadDefaultCAs) {
        errCode = SSL_CTX_set_default_verify_paths(_sslContext);
        if (errCode != 1) {
            std::string msg = getLastError();
            SSL_CTX_free(_sslContext);
            throw std::runtime_error(
                "SSL Error: Cannot load default CA certificates: " + msg);
        }
    }

    if (isForServerUse())
        SSL_CTX_set_verify(_sslContext, verificationMode,
                           &SSLManager::verifyServerCallback);
    else
        SSL_CTX_set_verify(_sslContext, verificationMode,
                           &SSLManager::verifyClientCallback);

    SSL_CTX_set_cipher_list(_sslContext, cipherList.c_str());
    SSL_CTX_set_verify_depth(_sslContext, verificationDepth);
    SSL_CTX_set_mode(_sslContext, SSL_MODE_AUTO_RETRY);
    SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
}


SSLContext::~SSLContext()
{
    SSL_CTX_free(_sslContext);

    crypto::uninitializeEngine();
}


void SSLContext::useCertificate(const crypto::X509Certificate& certificate)
{
    int errCode = SSL_CTX_use_certificate(_sslContext, const_cast<X509*>(certificate.certificate()));
    if (errCode != 1) {
        std::string msg = getLastError();
        throw std::runtime_error(
            "SSL Error: Cannot set certificate for Context: " + msg);
    }
}


void SSLContext::addChainCertificate(const crypto::X509Certificate& certificate)
{
    int errCode = SSL_CTX_add_extra_chain_cert(_sslContext, const_cast<X509*>(certificate.certificate()));
    if (errCode != 1) {
        std::string msg = getLastError();
        throw std::runtime_error(
            "SSL Error: Cannot add chain certificate to Context: " + msg);
    }
}


// void SSLContext::usePrivateKey(const crypto::RSAKey& key)
// {
//     int errCode = SSL_CTX_use_RSAPrivateKey(_sslContext,
//     const_cast<RSA*>(&key));
//     if (errCode != 1)
//     {
//         std::string msg = getLastError();
//         throw std::runtime_error("SSL Error: Cannot set private key for
//         Context: " + msg);
//     }
// }


void SSLContext::addVerificationCertificate(
    const crypto::X509Certificate& certificate)
{
    int errCode =
        X509_STORE_add_cert(SSL_CTX_get_cert_store(_sslContext),
                            const_cast<X509*>(certificate.certificate()));
    if (errCode != 1) {
        std::string msg = getLastError();
        throw std::runtime_error(
            "SSL Error: Cannot add verification certificate: " + msg);
    }
}


void SSLContext::enableSessionCache(bool flag)
{
    if (flag) {
        SSL_CTX_set_session_cache_mode(
            _sslContext,
            isForServerUse() ? SSL_SESS_CACHE_SERVER : SSL_SESS_CACHE_CLIENT);
    } else {
        SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
    }
}


void SSLContext::enableSessionCache(bool flag,
                                    const std::string& sessionIdContext)
{
    //assert(isForServerUse());

    if (flag) {
        SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_SERVER);
    } else {
        SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
    }

    unsigned length = static_cast<unsigned>(sessionIdContext.length());
    if (length > SSL_MAX_SSL_SESSION_ID_LENGTH)
        length = SSL_MAX_SSL_SESSION_ID_LENGTH;
    int rc = SSL_CTX_set_session_id_context(
        _sslContext,
        reinterpret_cast<const unsigned char*>(sessionIdContext.data()),
        length);
    if (rc != 1)
        throw std::runtime_error("SSL Error: cannot set session ID context");
}


bool SSLContext::sessionCacheEnabled() const
{
    return SSL_CTX_get_session_cache_mode(_sslContext) != SSL_SESS_CACHE_OFF;
}


void SSLContext::setSessionCacheSize(size_t size)
{
    assert(isForServerUse());

    SSL_CTX_sess_set_cache_size(_sslContext, static_cast<long>(size));
}


size_t SSLContext::getSessionCacheSize() const
{
    assert(isForServerUse());

    return static_cast<size_t>(SSL_CTX_sess_get_cache_size(_sslContext));
}


void SSLContext::setSessionTimeout(long seconds)
{
    assert(isForServerUse());

    SSL_CTX_set_timeout(_sslContext, seconds);
}


long SSLContext::getSessionTimeout() const
{
    assert(_usage == SERVER_USE);

    return SSL_CTX_get_timeout(_sslContext);
}


void SSLContext::flushSessionCache()
{
    assert(_usage == SERVER_USE);

    Timestamp now;
    SSL_CTX_flush_sessions(_sslContext, static_cast<long>(now.epochTime()));
}


void SSLContext::disableStatelessSessionResumption()
{
#if defined(SSL_OP_NO_TICKET)
    SSL_CTX_set_options(_sslContext, SSL_OP_NO_TICKET);
#endif
}


void SSLContext::createSSLContext()
{
    switch (_usage) {
        case CLIENT_USE:
            _sslContext = SSL_CTX_new(SSLv23_client_method());
            break;
        case SERVER_USE:
            _sslContext = SSL_CTX_new(SSLv23_server_method());
            break;
        case TLSV1_CLIENT_USE:
            _sslContext = SSL_CTX_new(TLSv1_client_method());
            break;
        case TLSV1_SERVER_USE:
            _sslContext = SSL_CTX_new(TLSv1_server_method());
            break;
        default:
            throw std::runtime_error("SSL Exception: Invalid usage");
    }
    if (!_sslContext) {
        unsigned long err = ERR_get_error();
        throw std::runtime_error(
            "SSL Exception: Cannot create SSL_CTX object: " +
            std::string(ERR_error_string(err, 0)));
    }

    SSL_CTX_set_default_passwd_cb(_sslContext, &SSLManager::privateKeyPassphraseCallback);
    clearErrorStack();
    SSL_CTX_set_options(_sslContext, SSL_OP_ALL);
}


} // namespace net
} // namespace scy


/// @\}
