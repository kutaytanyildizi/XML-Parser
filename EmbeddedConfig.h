#ifndef EmbeddedConfig_H
#define EmbeddedConfig_H

#include "XMLParser.h"

class EmbeddedConfig
{
public:
    // Singleton Class //
    EmbeddedConfig(const EmbeddedConfig&) = delete;
    EmbeddedConfig& operator=(const EmbeddedConfig&) = delete;

    const std::string TCPServerPort{};
    const std::string UDPListenerPort{};
    const std::string TCPClientIP{};
    const std::string TCPClientPort{};
    const std::string logPath{};

    static EmbeddedConfig* getInstance();
private:
    static EmbeddedConfig *embeddedConfig;
    EmbeddedConfig();
    EmbeddedConfig(const XMLParser& parser);
    ~EmbeddedConfig();
};

EmbeddedConfig* EmbeddedConfig::embeddedConfig = nullptr;

EmbeddedConfig::EmbeddedConfig() : TCPServerPort{"8080"}, UDPListenerPort{"8081"}, TCPClientIP{"127.0.0.1"}, TCPClientPort{"8080"}, logPath{"/temp"} {}

EmbeddedConfig::EmbeddedConfig(const XMLParser& parser) : TCPServerPort{ parser.get("GTSConfig","NetworkConfig", "EmbeddedNetworkComm", "TCPServerPort") },
                                                          UDPListenerPort{ parser.get("GTSConfig","NetworkConfig", "EmbeddedNetworkComm", "UDPListenerPort") },
                                                          TCPClientIP{ parser.get("GTSConfig","NetworkConfig", "RemoteDeviceComm", "TCPClientIP") },
                                                          TCPClientPort{ parser.get("GTSConfig","NetworkConfig", "RemoteDeviceComm", "TCPClientPort") },
                                                          logPath{ parser.get("GTSConfig","LogPath", "Path") } {}

EmbeddedConfig::~EmbeddedConfig()
{
    if(embeddedConfig!=nullptr){
        delete embeddedConfig;
    }
}                                                            

EmbeddedConfig* EmbeddedConfig::getInstance()
{
    if( embeddedConfig == nullptr ) 
    {
        try{
            embeddedConfig = new EmbeddedConfig(XMLParser("Config/GTSConfig.xml"));
        }
        catch(std::exception &ex) 
        {
            embeddedConfig = new EmbeddedConfig();
            return embeddedConfig;
        }
    }
    else
    {
        EmbeddedConfig* temp = embeddedConfig;
        embeddedConfig = nullptr;
        delete temp;
        return getInstance();
    }
    return embeddedConfig;

}

#endif // EmbeddedConfig_H