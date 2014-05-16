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
// This file uses functions from POCO C++ Libraries (license below)
//

#ifndef SCY_Net_SSLManager_H
#define SCY_Net_SSLManager_H


#include "scy/net/types.h"
#include "scy/net/sslsession.h"
#include "scy/net/sslcontext.h"
#include "scy/singleton.h"

#include <openssl/ssl.h>


namespace scy {
namespace net {
	

class VerificationErrorDetails;


class SSLManager
	/// SSLManager is a singleton for holding the default server/client 
	/// Context and handling callbacks for certificate verification errors
	/// and private key passphrases.
{
public:
	void initializeServer(SSLContext::Ptr ptrContext);
		// Initializes the server side of the SSLManager server-side SSLContext.

	void initializeClient(SSLContext::Ptr ptrContext);
		// Initializes the client side of the SSLManager with a default client-side SSLContext.

	SSLContext::Ptr defaultServerContext();
		// Returns the default Context used by the server if initialized. 

	SSLContext::Ptr defaultClientContext();
		// Returns the default Context used by the client if initialized. 

	Signal<VerificationErrorDetails&> ServerVerificationError;
		// Fired whenever a certificate verification error is detected by the server during a handshake.

	Signal<VerificationErrorDetails&> ClientVerificationError;
		// Fired whenever a certificate verification error is detected by the client during a handshake.

	Signal<std::string&> PrivateKeyPassphraseRequired;
		// Fired when a encrypted certificate is loaded. Not setting the password
		// in the event parameter will result in a failure to load the certificate.
		
	void shutdown();
		// Shuts down the SSLManager and releases the default Context
		// objects. After a call to shutdown(), the SSLManager can no
		// longer be used.
		//
		// Normally, it's not necessary to call this method directly, as this
		// will be called either by uninitializeSSL(), or when
		// the SSLManager instance is destroyed.

	static SSLManager& instance();
		// Returns the instance of the SSLManager singleton.

	static void destroy();
		// Shuts down and destroys the SSLManager singleton instance.
	
	static void initNoVerifyClient();
		// Initializes a default no verify client context that's useful for testing.

protected:
	static int verifyClientCallback(int ok, X509_STORE_CTX* pStore);
		// The return value of this method defines how errors in
		// verification are handled. Return 0 to terminate the handshake,
		// or 1 to continue despite the error.

	static int verifyServerCallback(int ok, X509_STORE_CTX* pStore);
		// The return value of this method defines how errors in
		// verification are handled. Return 0 to terminate the handshake,
		// or 1 to continue despite the error.

	static int privateKeyPassphraseCallback(char* pBuf, int size, int flag, void* userData);
		// Method is invoked by OpenSSL to retrieve a passwd for an encrypted certificate.
		// The request is delegated to the PrivatekeyPassword event. This method returns the
		// length of the password.

private:
	SSLManager();
		// Creates the SSLManager.

	~SSLManager();
		// Destroys the SSLManager.

	static int verifyCallback(bool server, int ok, X509_STORE_CTX* pStore);
		// The return value of this method defines how errors in
		// verification are handled. Return 0 to terminate the handshake,
		// or 1 to continue despite the error.

	SSLContext::Ptr _defaultServerContext;
	SSLContext::Ptr _defaultClientContext;
	Mutex _mutex;

	friend class Singleton<SSLManager>;
	friend class SSLContext;
};


inline int SSLManager::verifyServerCallback(int ok, X509_STORE_CTX* pStore)
{
	return SSLManager::verifyCallback(true, ok, pStore);
}


inline int SSLManager::verifyClientCallback(int ok, X509_STORE_CTX* pStore)
{
	return SSLManager::verifyCallback(false, ok, pStore);
}


inline void SSLManager::initNoVerifyClient()
{
	net::SSLManager::instance().initializeClient(
		std::shared_ptr<net::SSLContext>(
			new net::SSLContext(
				net::SSLContext::CLIENT_USE, "", "", "", 
				net::SSLContext::VERIFY_NONE, 9, false, 
				"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")));
}


//
// Verification Error Details
//


class VerificationErrorDetails
	/// A utility class for certificate error handling.
{
public:
	VerificationErrorDetails(const crypto::X509Certificate& cert, int errDepth, int errNum, const std::string& errMsg);
		// Creates the VerificationErrorDetails. _ignoreError is per default set to false.

	~VerificationErrorDetails();
		// Destroys the VerificationErrorDetails.

	const crypto::X509Certificate& certificate() const;
		// Returns the certificate that caused the error.

	int errorDepth() const;
		// Returns the position of the certificate in the certificate chain.

	int errorNumber() const;
		// Returns the id of the error

	const std::string& errorMessage() const;
		// Returns the textual presentation of the errorNumber.

	void setIgnoreError(bool ignoreError);
		// setIgnoreError to true, if a verification error is judged non-fatal by the user.

	bool getIgnoreError() const;
		// returns the value of _ignoreError

private:
	crypto::X509Certificate	_cert;
	std::string _errorMessage; // Textual representation of the _errorNumber
	bool _ignoreError;
	int _errorDepth;
	int _errorNumber;
};


inline const crypto::X509Certificate& VerificationErrorDetails::certificate() const
{
	return _cert;
}


inline int VerificationErrorDetails::errorDepth() const
{
	return _errorDepth;
}


inline int VerificationErrorDetails::errorNumber() const
{
	return _errorNumber;
}


inline const std::string& VerificationErrorDetails::errorMessage() const
{
	return _errorMessage;
}


inline void VerificationErrorDetails::setIgnoreError(bool ignoreError)
{
	_ignoreError = ignoreError;
}


inline bool VerificationErrorDetails::getIgnoreError() const
{
	return _ignoreError;
}


} } // namespace scy::net


#endif // SCY_Net_SSLManager_H


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
