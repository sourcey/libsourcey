#ifndef SCY_Crypto_RSA_H
#define SCY_Crypto_RSA_H


#include "scy/crypto/crypto.h"
#include <openssl/rsa.h>


namespace scy {
namespace crypto {


/// Forward the OpenSSL type to our namespace.
/// This may become a class/wrapper in the future.
typedef ::RSA RSAKey;
	
	
} } // namespace scy::crypto


#endif // SCY_Crypto_RSA_H
