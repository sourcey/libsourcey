///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup crypto
/// @{


#ifndef SCY_Crypto_X509Certificate_H
#define SCY_Crypto_X509Certificate_H


#include "scy/crypto/crypto.h"
#include "scy/datetime.h"
#include "scy/memory.h"
#include <istream>
#include <openssl/ssl.h>
#include <set>


namespace scy {
namespace crypto {


/// This class represents a X509 Certificate.
class SCY_EXTERN X509Certificate
{
public:
    /// Name identifier for extracting information from
    /// a certificate subject's or issuer's distinguished name.
    enum NID
    {
        NID_COMMON_NAME = 13,
        NID_COUNTRY = 14,
        NID_LOCALITY_NAME = 15,
        NID_STATE_OR_PROVINCE = 16,
        NID_ORGANIZATION_NAME = 17,
        NID_ORGANIZATION_UNIT_NAME = 18
    };

    /// Creates the X509Certificate object by reading
    /// a certificate in PEM format from the given buffer.
    explicit X509Certificate(const char* data, std::size_t length);

    /// Creates the X509Certificate object by reading
    /// a certificate in PEM format from a file.
    explicit X509Certificate(const std::string& path);

    /// Creates the X509Certificate from an existing
    /// OpenSSL certificate. Ownership is taken of
    /// the certificate.
    explicit X509Certificate(X509* pCert);

    /// Creates the X509Certificate from an existing
    /// OpenSSL certificate. Ownership is taken of
    /// the certificate. If shared is true, the
    /// certificate's reference count is incremented.
    X509Certificate(X509* pCert, bool shared);

    /// Creates the certificate by copying another one.
    X509Certificate(const X509Certificate& cert);

    /// Assigns a certificate.
    X509Certificate& operator=(const X509Certificate& cert);

    /// Exchanges the certificate with another one.
    void swap(X509Certificate& cert);

    /// Destroys the X509Certificate.
    ~X509Certificate();

    /// Returns the certificate issuer's distinguished name.
    const std::string& issuerName() const;

    /// Extracts the information specified by the given
    /// NID (name identifier) from the certificate issuer's
    /// distinguished name.
    std::string issuerName(NID nid) const;

    /// Returns the certificate subject's distinguished name.
    const std::string& subjectName() const;

    /// Extracts the information specified by the given
    /// NID (name identifier) from the certificate subject's
    /// distinguished name.
    std::string subjectName(NID nid) const;

    /// Returns the common name stored in the certificate
    /// subject's distinguished name.
    std::string commonName() const;

    /// Extracts the common name and the alias domain names from the
    /// certificate.
    void extractNames(std::string& commonName,
                      std::set<std::string>& domainNames) const;

    /// Returns the date and time the certificate is valid from.
    DateTime validFrom() const;

    /// Returns the date and time the certificate expires.
    DateTime expiresOn() const;

    /// Writes the certificate to the given stream.
    /// The certificate is written in PEM format.
    void save(std::ostream& stream) const;

    /// Writes the certificate to the file given by path.
    /// The certificate is written in PEM format.
    void save(const std::string& path) const;

    /// Checks whether the certificate has been issued by
    /// the issuer given by issuerCertificate. This can be
    /// used to validate a certificate chain.
    ///
    /// Verifies if the certificate has been signed with the
    /// issuer's private key, using the public key from the issuer
    /// certificate.
    ///
    /// Returns true if verification against the issuer certificate
    /// was successful, false otherwise.
    bool issuedBy(const X509Certificate& issuerCertificate) const;

    /// Returns the underlying OpenSSL certificate.
    const X509* certificate() const;

protected:
    /// Loads the certificate from the given buffer.
    /// The certificate must be in PEM format.
    void load(const char* data, std::size_t length);

    /// Loads the certificate from the given file.
    /// The certificate must be in PEM format.
    void load(const std::string& path);

    /// Extracts issuer and subject name from the certificate.
    void init();

private:
    enum
    {
        NAME_BUFFER_SIZE = 256
    };

    std::string _issuerName;
    std::string _subjectName;
    X509* _certificate;
};


} // namespace crypto
} // namespace scy


#endif // SCY_Crypto_X509Certificate_H


/// @\}


//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
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
//
