#ifndef SOURCEY_Crypto_RSA_H
#define SOURCEY_Crypto_RSA_H


#include "Sourcey/Crypto/Crypto.h"
#include <openssl/rsa.h>


namespace scy {
namespace crypto {


/// Forward the OpenSSL type to our namespace.
/// This may become a class/wrapper in the future.
typedef ::RSA RSAKey;
	
	
} } // namespace scy::crypto


#endif // SOURCEY_Crypto_RSA_H
