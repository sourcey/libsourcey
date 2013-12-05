//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "scy/crypto/hmac.h"
#include "scy/util.h"
#include <assert.h>
	
#ifdef WIN32
// hack for name collision of OCSP_RESPONSE and wincrypto.h in openssl release 0.9.8h
// http://www.google.com/search?q=OCSP%5fRESPONSE+wincrypt%2eh
// continue to watch this issue for a real fix.
#undef OCSP_RESPONSE
#endif
#include <openssl/hmac.h>


namespace scy {
namespace crypto {


std::string computeHMAC(const std::string& input, const std::string& key) 
{	
    //DebugL << "Compute HMAC: input='" << util::dumpbin(input.c_str(), input.length()) << "', length=" << input.length() << ", key='" << key << "', keySize=" << key.length();
	unsigned int len = 0;
	char buf[20];	
	HMAC(EVP_sha1(), 
		key.c_str(), key.length(), 
        reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), 
        reinterpret_cast<unsigned char*>(&buf), &len);
	assert(len == 20);
	return std::string(buf, len);
}


} } // namespace scy::crypto