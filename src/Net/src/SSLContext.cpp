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

#include "scy/filesystem.h"
#include "scy/datetime.h"
#include "scy/net/sslcontext.h"
#include "scy/net/sslmanager.h"
#include "scy/crypto/crypto.h"


using namespace std;


namespace scy {
namespace net {


SSLContext::SSLContext(
	Usage usage,
	const std::string& privateKeyFile, 
	const std::string& certificateFile,
	const std::string& caLocation, 
	VerificationMode verificationMode,
	int verificationDepth,
	bool loadDefaultCAs,
	const std::string& cipherList):
	_usage(usage),
	_mode(verificationMode),
	_sslContext(0),
	_extendedVerificationErrorDetails(true)
{
	crypto::initializeEngine();
	
	createSSLContext();

	int errCode = 0;
	if (!caLocation.empty())
	{
		if (fs::isdir(caLocation))
			errCode = SSL_CTX_load_verify_locations(_sslContext, 0, fs::transcode(caLocation).c_str());
		else
			errCode = SSL_CTX_load_verify_locations(_sslContext, fs::transcode(caLocation).c_str(), 0);
		if (errCode != 1)
		{
			std::string msg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error(std::string("SSL Error: Cannot load CA file/directory at ") + caLocation + ": " + msg);
		}
	}

	if (loadDefaultCAs)
	{
		errCode = SSL_CTX_set_default_verify_paths(_sslContext);
		if (errCode != 1)
		{
			std::string msg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error("SSL Error: Cannot load default CA certificates: " + msg);
		}
	}

	if (!privateKeyFile.empty())
	{
		errCode = SSL_CTX_use_PrivateKey_file(_sslContext, fs::transcode(privateKeyFile).c_str(), SSL_FILETYPE_PEM);
		if (errCode != 1)
		{
			std::string msg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error(std::string("SSL Error: Error loading private key from file ") + privateKeyFile + ": " + msg);
		}
	}

	if (!certificateFile.empty())
	{
		errCode = SSL_CTX_use_certificate_chain_file(_sslContext, fs::transcode(certificateFile).c_str());
		if (errCode != 1)
		{
			std::string errMsg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error(std::string("SSL Error: Error loading certificate from file ") + certificateFile + ": " + errMsg); //, errMsg);
		}
	}

	if (isForServerUse())
		SSL_CTX_set_verify(_sslContext, verificationMode, &SSLManager::verifyServerCallback);
	else
		SSL_CTX_set_verify(_sslContext, verificationMode, &SSLManager::verifyClientCallback);

	SSL_CTX_set_cipher_list(_sslContext, cipherList.c_str());
	SSL_CTX_set_verify_depth(_sslContext, verificationDepth);
	SSL_CTX_set_mode(_sslContext, SSL_MODE_AUTO_RETRY);
	SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
}


SSLContext::SSLContext(
	Usage usage,
	const std::string& caLocation, 
	VerificationMode verificationMode,
	int verificationDepth,
	bool loadDefaultCAs,
	const std::string& cipherList):
	_usage(usage),
	_mode(verificationMode),
	_sslContext(0),
	_extendedVerificationErrorDetails(true)
{
	crypto::initializeEngine();
	
	createSSLContext();

	int errCode = 0;
	if (!caLocation.empty())
	{
		if (fs::isdir(caLocation))
			errCode = SSL_CTX_load_verify_locations(_sslContext, 0, fs::transcode(caLocation).c_str());
		else
			errCode = SSL_CTX_load_verify_locations(_sslContext, fs::transcode(caLocation).c_str(), 0);
		if (errCode != 1)
		{
			std::string msg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error(std::string("SSL Error: Cannot load CA file/directory at ") + caLocation + ": " + msg);
		}
	}

	if (loadDefaultCAs)
	{
		errCode = SSL_CTX_set_default_verify_paths(_sslContext);
		if (errCode != 1)
		{
			std::string msg = getLastError();
			SSL_CTX_free(_sslContext);
			throw std::runtime_error("SSL Error: Cannot load default CA certificates: " + msg);
		}
	}

	if (isForServerUse())
		SSL_CTX_set_verify(_sslContext, verificationMode, &SSLManager::verifyServerCallback);
	else
		SSL_CTX_set_verify(_sslContext, verificationMode, &SSLManager::verifyClientCallback);

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
	if (errCode != 1)
	{
		std::string msg = getLastError();
		throw std::runtime_error("SSL Error: Cannot set certificate for Context: " + msg);
	}
}

	
void SSLContext::addChainCertificate(const crypto::X509Certificate& certificate)
{
	int errCode = SSL_CTX_add_extra_chain_cert(_sslContext, certificate.certificate());
	if (errCode != 1)
	{
		std::string msg = getLastError();
		throw std::runtime_error("SSL Error: Cannot add chain certificate to Context: " + msg);
	}
}

	
void SSLContext::usePrivateKey(const crypto::RSAKey& key)
{
	int errCode = SSL_CTX_use_RSAPrivateKey(_sslContext, const_cast<RSA*>(&key));
	if (errCode != 1)
	{
		std::string msg = getLastError();
		throw std::runtime_error("SSL Error: Cannot set private key for Context: " + msg);
	}
}


void SSLContext::enableSessionCache(bool flag)
{
	if (flag)
	{
		SSL_CTX_set_session_cache_mode(_sslContext, isForServerUse() ? SSL_SESS_CACHE_SERVER : SSL_SESS_CACHE_CLIENT);
	}
	else
	{
		SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
	}
}


void SSLContext::enableSessionCache(bool flag, const std::string& sessionIdContext)
{
	assert(isForServerUse());

	if (flag)
	{
		SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_SERVER);
	}
	else
	{
		SSL_CTX_set_session_cache_mode(_sslContext, SSL_SESS_CACHE_OFF);
	}
	
	unsigned length = static_cast<unsigned>(sessionIdContext.length());
	if (length > SSL_MAX_SSL_SESSION_ID_LENGTH) length = SSL_MAX_SSL_SESSION_ID_LENGTH;
	int rc = SSL_CTX_set_session_id_context(_sslContext, reinterpret_cast<const unsigned char*>(sessionIdContext.data()), length);
	if (rc != 1) throw std::runtime_error("SSL Error: cannot set session ID context");
}


bool SSLContext::sessionCacheEnabled() const
{
	return SSL_CTX_get_session_cache_mode(_sslContext) != SSL_SESS_CACHE_OFF;
}


void SSLContext::setSessionCacheSize(std::size_t size)
{
	assert(isForServerUse());
	
	SSL_CTX_sess_set_cache_size(_sslContext, static_cast<long>(size));
}

	
std::size_t SSLContext::getSessionCacheSize() const
{
	assert(isForServerUse());
	
	return static_cast<std::size_t>(SSL_CTX_sess_get_cache_size(_sslContext));
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
	switch (_usage)
	{
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
	if (!_sslContext)  {
		unsigned long err = ERR_get_error();
		throw std::runtime_error("SSL Exception: Cannot create SSL_CTX object: " + std::string(ERR_error_string(err, 0)));
	}

	SSL_CTX_set_default_passwd_cb(_sslContext, &SSLManager::privateKeyPassphraseCallback);
	clearErrorStack();
	SSL_CTX_set_options(_sslContext, SSL_OP_ALL);
}


} } // namespace scy::net



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
