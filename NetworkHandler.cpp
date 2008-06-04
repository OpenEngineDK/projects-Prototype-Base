
#include "NetworkHandler.h"

#include <Logging/Logger.h>
#include <Utils/Convert.h>

#include <netinet/in.h>

using OpenEngine::Utils::Convert;
//using OpenEngine::Logging::Logger;

NetworkHandler::NetworkHandler(GameFactory* gf, Event<NetworkEventArg>* net) {
    this->id  = 0; // received on server connect
    this->gf  = gf;
    this->net = net;
}

NetworkHandler::~NetworkHandler() {

}

void NetworkHandler::Notify(){
    //logger.info << "notify: " << Convert::int2string(this->id) << logger.end;
    // send position
    NetworkEventArg arg;
    arg.id = CPOS;
    arg.length = 2 + 3*sizeof(float);

    float pos[3];
    this->gf->tankMgr->GetTank(0)->GetTankTransformationNode()->GetPosition().ToArray(pos);

    char buf[arg.length];

    uint16_t tid = htons(this->id);
    memcpy(buf, &tid, 2);                  // prefix with our id
    memcpy(&buf[2], pos, 3*sizeof(float)); // out position
    arg.payload = buf;
    net->Notify(arg);
}

void NetworkHandler::Handle(NetworkEventArg arg) {
    uint16_t cid;
    char* buf = arg.payload;
    float pos[3];

    //logger.info << "received net pkg: " + Convert::int2string(arg.id) << logger.end;

    switch (arg.id) {

    case CLST: // client list we receive on sign on
        while (buf < arg.payload + arg.length - 2) {
            memcpy(&cid, buf, 2);
            CreatePlayerNode(ntohs(cid));
            buf += 2;
        }
        // our own id is last (since we just connected)
        memcpy(&this->id, buf, 2);
        this->id = ntohs(this->id);
        logger.info << "received id: " + Convert::int2string(this->id) << logger.end;
        break;

    case CCON: // a new client signed on
        memcpy(&cid, buf, 2);
        CreatePlayerNode(ntohs(cid));
        break;

    case CDIS: // a client disconnected
        memcpy(&cid, buf, 2);
        DeletePlayerNode(ntohs(cid));
        break;

    case CPOS: // update a client position
        memcpy(&cid, buf, 2);
        buf += 2;
        memcpy(pos, buf, 3*sizeof(float));
        UpdatePlayerNode(ntohs(cid), Vector<3,float>(pos));
        break;

    default:
        break;

    }
}

void NetworkHandler::CreatePlayerNode(int id) {
    logger.info << "creating tank for player id: " + Convert::int2string(id)
                << logger.end;
    ITank* t = this->gf->AddTank(1);
    this->players[id] = t;
}

void NetworkHandler::DeletePlayerNode(int id) {

}

void NetworkHandler::UpdatePlayerNode(int id, Vector<3,float> pos) {
    this->players[id]->GetTankTransformationNode()->SetPosition(pos);
}
