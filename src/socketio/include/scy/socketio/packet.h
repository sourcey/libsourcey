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

#ifndef SCY_SocketIO_Packet_H
#define SCY_SocketIO_Packet_H


#include "scy/packet.h"
#include "scy/json/json.h"


namespace scy {
namespace sockio {
    

class Packet: public IPacket 
{
public:
    enum Type 
    {
        Disconnect        = 0, 
        Connect            = 1,
        Heartbeat        = 2,
        Message            = 3,
        JSON            = 4,
        Event            = 5,
        Ack                = 6,
        Error            = 7
    };
    
    Packet(Type type = Message, 
           int id = -1, 
           const std::string& endpoint = "", 
           const std::string& message = "", 
           bool ack = false);    
        // Default contructor
    
    Packet(Type type,
           const std::string& message = "", 
           bool ack = false);    
        // General contructor

    Packet(const std::string& message, 
           bool ack = false);
        // Message contructor

    Packet(const json::Value& data, 
           bool ack = false);
        // JSON contructor

    Packet(const std::string& event, 
           const json::Value& data, 
           bool ack = false);
        // Event contructor

    Packet(const Packet& r);    
    Packet& operator = (const Packet& r);
    virtual ~Packet();

    virtual IPacket* clone() const;

    Type type() const;
    int id() const;
    std::string endpoint() const;
    std::string message() const;    
    json::Value json() const;
    
    void setID(int id);
    void setEndpoint(const std::string& endpoint);
    void setMessage(const std::string& message);
    void setAck(bool flag);

    std::size_t read(const ConstBuffer& buf);
    void write(Buffer& buf) const;
    
    virtual size_t size() const;

    bool valid() const;

    std::string typeString() const;
    std::string toString() const;
    void print(std::ostream& os) const;

    virtual const char* className() const { return "SocketIOPacket"; }

protected:
    int _type;
    int _id;
    std::string _endpoint;
    std::string _message;
    bool _ack;
    size_t _size;
};


} } // namespace scy::sockio


#endif //  SCY_SocketIO_Packet_H

