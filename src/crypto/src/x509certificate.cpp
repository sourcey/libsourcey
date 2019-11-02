///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup crypto
/// @{


#include "scy/crypto/x509certificate.h"
#include <sstream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509v3.h>


namespace scy {
namespace crypto {


X509Certificate::X509Certificate(const char* data, size_t length)
    : _certificate(0)
{
    std::string res;
    Timestamp ts;
    DateTimeFormatter::append(res, ts, DateTimeFormat::HTTP_FORMAT);

    std::string dateTime;
    int tzd;
    DateTime t = DateTimeParser::parse("%y%m%d%H%M%S", dateTime, tzd);
    Timespan t1;

    load(data, length);
}


X509Certificate::X509Certificate(const std::string& path)
    : _certificate(0)
{
    load(path);
}


X509Certificate::X509Certificate(X509* pCert)
    : _certificate(pCert)
{
    assert(_certificate);

    init();
}


X509Certificate::X509Certificate(X509* pCert, bool shared)
    : _certificate(pCert)
{
    assert(_certificate);

    if (shared) {
    #if OPENSSL_VERSION_NUMBER < 0x10100000L
        _certificate->references++;
    #else
        X509_up_ref(_certificate); // OpenSSL >= 1.1.0
    #endif
    }

    init();
}


X509Certificate::X509Certificate(const X509Certificate& cert)
    : _issuerName(cert._issuerName)
    , _subjectName(cert._subjectName)
    , _certificate(cert._certificate)
{
    _certificate = X509_dup(_certificate);
}


X509Certificate& X509Certificate::operator=(const X509Certificate& cert)
{
    X509Certificate tmp(cert);
    swap(tmp);
    return *this;
}


void X509Certificate::swap(X509Certificate& cert)
{
    std::swap(cert._issuerName, _issuerName);
    std::swap(cert._subjectName, _subjectName);
    std::swap(cert._certificate, _certificate);
}


X509Certificate::~X509Certificate()
{
    X509_free(_certificate);
}


void X509Certificate::load(const char* data, size_t length)
{
    assert(!_certificate);

    std::string cert(data, length);
    BIO* pBIO = BIO_new_mem_buf(const_cast<char*>(cert.data()),
                                static_cast<int>(cert.length()));
    if (!pBIO)
        throw std::runtime_error(
            "SSL IO error: Cannot create BIO for reading certificate");
    _certificate = PEM_read_bio_X509(pBIO, 0, 0, 0);
    BIO_free(pBIO);

    if (!_certificate)
        throw std::runtime_error(
            "SSL IO error: Faild to load X509 certificate from stream");

    init();
}


void X509Certificate::load(const std::string& path)
{
    assert(!_certificate);

    BIO* pBIO = BIO_new(BIO_s_file());
    if (!pBIO)
        throw std::runtime_error(
            "SSL error: Cannot create BIO for reading certificate file: " +
            path);
    if (!BIO_read_filename(pBIO, path.c_str())) {
        BIO_free(pBIO);
        throw std::runtime_error(
            "SSL file error: Cannot open certificate file for reading: " +
            path);
    }

    _certificate = PEM_read_bio_X509(pBIO, 0, 0, 0);
    BIO_free(pBIO);

    if (!_certificate)
        throw std::runtime_error(
            "SSL file error: Faild to load certificate from: " + path);

    init();
}


void X509Certificate::save(std::ostream& stream) const
{
    BIO* pBIO = BIO_new(BIO_s_mem());
    if (!pBIO)
        throw std::runtime_error(
            "SSL IO error: Cannot create BIO for writing certificate");
    try {
        if (!PEM_write_bio_X509(pBIO, _certificate))
            throw std::runtime_error(
                "SSL IO error: Failed to write certificate to stream");

        char* pData;
        long size;
        size = BIO_get_mem_data(pBIO, &pData);
        stream.write(pData, size);
    } catch (...) {
        BIO_free(pBIO);
        throw;
    }
    BIO_free(pBIO);
}


void X509Certificate::save(const std::string& path) const
{
    BIO* pBIO = BIO_new(BIO_s_file());
    if (!pBIO)
        std::runtime_error(
            "SSL IO error: Cannot create BIO for reading certificate file: " +
            path);
    if (!BIO_write_filename(pBIO, const_cast<char*>(path.c_str()))) {
        BIO_free(pBIO);
        throw std::runtime_error(
            "SSL file error: Cannot create certificate file: " + path);
    }

    try {
        if (!PEM_write_bio_X509(pBIO, _certificate))
            throw std::runtime_error(
                "SSL file error: Failed to write certificate to file: " + path);
    } catch (...) {
        BIO_free(pBIO);
        throw;
    }

    BIO_free(pBIO);
}


void X509Certificate::init()
{
    char buffer[NAME_BUFFER_SIZE];
    X509_NAME_oneline(X509_get_issuer_name(_certificate), buffer, sizeof(buffer));
    _issuerName = buffer;
    X509_NAME_oneline(X509_get_subject_name(_certificate), buffer, sizeof(buffer));
    _subjectName = buffer;
}


std::string X509Certificate::commonName() const
{
    return subjectName(NID_COMMON_NAME);
}


std::string X509Certificate::issuerName(NID nid) const
{
    if (X509_NAME* issuer = X509_get_issuer_name(_certificate)) {
        char buffer[NAME_BUFFER_SIZE];
        X509_NAME_get_text_by_NID(issuer, nid, buffer, sizeof(buffer));
        return std::string(buffer);
    } else
        return std::string();
}


std::string X509Certificate::subjectName(NID nid) const
{
    if (X509_NAME* subj = X509_get_subject_name(_certificate)) {
        char buffer[NAME_BUFFER_SIZE];
        X509_NAME_get_text_by_NID(subj, nid, buffer, sizeof(buffer));
        return std::string(buffer);
    } else
        return std::string();
}


void X509Certificate::extractNames(std::string& cmnName,
                                   std::set<std::string>& domainNames) const
{
    domainNames.clear();
    if (STACK_OF(GENERAL_NAME)* names = static_cast<STACK_OF(GENERAL_NAME)*>(
            X509_get_ext_d2i(_certificate, NID_subject_alt_name, 0, 0))) {
        for (int i = 0; i < sk_GENERAL_NAME_num(names); ++i) {
            const GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
            if (name->type == GEN_DNS) {
                #if OPENSSL_VERSION_NUMBER < 0x10100000L
                    const char* data =
                    reinterpret_cast<char*>(ASN1_STRING_data(name->d.ia5));
                #else
                    const char* data = 
                    reinterpret_cast<const char*>(ASN1_STRING_get0_data(name->d.ia5));
                #endif

                size_t len = ASN1_STRING_length(name->d.ia5);
                domainNames.insert(std::string(data, len));
            }
        }
        GENERAL_NAMES_free(names);
    }

    cmnName = commonName();
    if (!cmnName.empty() && domainNames.empty()) {
        domainNames.insert(cmnName);
    }
}


DateTime X509Certificate::validFrom() const
{
    ASN1_TIME* certTime = X509_get_notBefore(_certificate);
    std::string dateTime(reinterpret_cast<char*>(certTime->data));
    int tzd;
    return DateTimeParser::parse("%y%m%d%H%M%S", dateTime, tzd);
}


DateTime X509Certificate::expiresOn() const
{
    ASN1_TIME* certTime = X509_get_notAfter(_certificate);
    std::string dateTime(reinterpret_cast<char*>(certTime->data));
    int tzd;
    return DateTimeParser::parse("%y%m%d%H%M%S", dateTime, tzd);
}


bool X509Certificate::issuedBy(const X509Certificate& issuerCertificate) const
{
    X509* pCert = const_cast<X509*>(_certificate);
    X509* pIssuerCert = const_cast<X509*>(issuerCertificate.certificate());
    EVP_PKEY* pIssuerPublicKey = X509_get_pubkey(pIssuerCert);
    if (!pIssuerPublicKey)
        throw std::invalid_argument("Issuer certificate has no public key");
    int rc = X509_verify(pCert, pIssuerPublicKey);
    EVP_PKEY_free(pIssuerPublicKey);
    return rc != 0;
}


const std::string& X509Certificate::issuerName() const
{
    return _issuerName;
}


const std::string& X509Certificate::subjectName() const
{
    return _subjectName;
}


const X509* X509Certificate::certificate() const
{
    return _certificate;
}


} // namespace crypto
} // namespace scy


/// @\}
