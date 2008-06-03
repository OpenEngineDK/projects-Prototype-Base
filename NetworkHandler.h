
#ifndef _PROTO_NETWORK_HANDLER_H_
#define _PROTO_NETWORK_HANDLER_H_

#include <Network/ErlNetwork.h>
#include <Core/IListener.h>
#include "GameFactory.h"

#include <map>
using std::map;

using OpenEngine::Core::IListener;
using OpenEngine::Network::NetworkEventArg;

class NetworkHandler : public IListener<NetworkEventArg> {
private:
    enum Messages {
        CCON=101,              // client connected
        CDIS=102,              // client disconnected
        CLST=103,              // client list
        CPOS=1001              // client position
    };
    GameFactory* gf;
    map<uint16_t,ITank*> players;
    Event<NetworkEventArg>* net;
    uint16_t id; // our player id (received on server connect)

public:
    NetworkHandler(GameFactory* gf, Event<NetworkEventArg>* net);
    virtual ~NetworkHandler();

    void Notify();
    void Handle(NetworkEventArg arg);

    void CreatePlayerNode(int id);
    void DeletePlayerNode(int id);
    void UpdatePlayerNode(int id, Vector<3,float> pos);
};

#endif // _PROTO_NETWORK_HANDLER_H_
