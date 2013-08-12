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


#ifndef SOURCEY_Crypto_Hash_H
#define SOURCEY_Crypto_Hash_H


#include "Sourcey/Crypto/Crypto.h"
#include "Sourcey/Types.h"

#include <openssl/evp.h>


namespace scy {
namespace crypto {
	

//typedef std::vector<unsigned char> Digest;
	/// The storage container for reading the digest result.


class Hash
{
public:
	Hash(const std::string& algorithm);
	~Hash();
		/// Re-initializes the digest structure.
	
	void update(char data);
	void update(const std::string& data);
	void update(const void* data, unsigned length);
		/// Hash the given data.
		/// This function may (and normally will) be called
		/// many times for large blocks of data.

	const ByteVec& digest();
		/// Finish up the digest operation and return the result.

	void reset();
		/// Resets the engine and digest state ready for the next computation.

	const std::string& algorithm(void) const;
		/// Returns the hash algorithm being used.

protected:
	Hash& operator=(Hash const&) {}

	EVP_MD_CTX		_ctx;
	const EVP_MD*	_md;
	crypto::ByteVec	_digest;
	std::string		_algorithm;
};


inline std::string computeHash(const std::string& algorithm, const std::string& data)
{
	Hash engine(algorithm);
	engine.update(data);
	return toHex(engine.digest());
}
	

} } // namespace scy::crypto


#endif // SOURCEY_Crypto_Hash_H


	//void finalize();
		 // Finish up the Digest operation.
	

	//unsigned char	_dValue[EVP_MAX_MD_SIZE];	// Final output
	
	
	//virtual void digest(std::string& result);
	//virtual unsigned int result(unsigned char* hash,std::string& result
	//							unsigned int maxLength);
		/// Reads the digest result into the given buffer and
		/// returns the number of bytes copied.
	
	//virtual void finalize();
		 // Finish up a Digest operation.
	
	//virtual unsigned int finish(unsigned char* hash,
	//							unsigned int maxLength);

	//Flags daysExcluded;
	
		//Timespan ts = now - scheduleAt;
	/*
	virtual void setTimeout(DateTime& time) {
		DateTime now;
		Timespan ts = now - time;
		timeout.setDelay(ts.totalMilliseconds());
	}
	
	DateTime createdAt;
	DateTime scheduleAt;
	*/
		
		//int nextDay;
		//nextDay = now.dayOfWeek();
		//setTimeout(timeOfDay);
	//Timeout timeout;

	//virtual void setTimeout(DateTime& time) {
	//{
	//	Timestamp now;
	//	return scheduleAt > now;
	//}
		
		//Timestamp now;
		//Timespan ts = now - scheduleAt;
		//timeout.setDelay(ts / 1000);
		
		//DateTime now;
			
		//while (true) {	
		//}



		//if ()
		//DateTime scheduleAt;



		//DateTime now;
		//Timespan ts = now - scheduleAt;
		//timeout.setDelay(ts / 1000);
	
	//now;
	
		//scheduleAt.timestamp();
		//Timestamp now;
		//return scheduleAt > now;
		//Timestamp now
	//Timeout timeout;


//#include "Anionu/Spot/API/Module.h"
//#include "Sourcey/JSON/ISerializable.h"

	//Hash(Scheduler& runner, long timeout = 0, long interval = 0);	
//long timeout = 0, long interval = 0
//protected:
	//Timestamp time();
		/// Returns the time the task will be run.

	//virtual const char* className() const { return "Hash"; }

	/*

	/// Override onTimeout
protected:	
	virtual void onTimeout() = 0;
		/// Performs task processing when the timer fires.
*/
	//mutable Mutex	_mutex;	
	//Timestamp _time;
	//Timestamp _scheduleAt;
	//bool _repeat;

	
	//virtual void run() = 0;
	//virtual bool valid() = 0;

	
	
	//virtual void serialize(json::Value& root);
	//virtual void deserialize(json::Value& root);
	//void schedule(std::time_t time);
	//void schedule(const std::string& time, const std::string& fmt = TimestampFormat::ISO8601_FORMAT);
		/// Sets the time the task will be run.