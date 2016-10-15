#ifndef SCY_MediaServer_Config_H
#define SCY_MediaServer_Config_H

#include "scy/logger.h"
#include "scy/application.h"
#include "scy/util.h"
#include "scy/packetstream.h"
#include "scy/av/multiplexencoder.h"
#include "scy/av/mediafactory.h"
#include "scy/http/server.h"



namespace scy { 
    
    
//const std::string kPublicIP = "124.171.220.107";    // Current external IP for TURN permissions
const std::string kRelayServerIP = "127.0.0.1";        // TURN server IP


} // namespace scy


#endif // SCY_MediaServer_Config_H