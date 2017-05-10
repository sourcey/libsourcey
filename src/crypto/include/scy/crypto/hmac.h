///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup crypto
/// @{


#ifndef SCY_Crypto_HMAC_H
#define SCY_Crypto_HMAC_H


#include "scy/crypto/crypto.h"
#include <string>


namespace scy {
namespace crypto {


/// HMAC is a MAC (message authentication code), i.e. a keyed hash function
/// used for message authentication, which is based on a hash function (SHA1).
///
/// Input is the data to be signed, and key is the private password.
std::string computeHMAC(const std::string& input, const std::string& key);


} // namespace crypto
} // namespace scy


#endif // SCY_Crypto_HMAC_H


/// @\}
