//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU  General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU  General Public License for more details.
//
// You should have received a copy of the GNU  General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef TAWNS_FTPTCPSIMPLE_H
#define TAWNS_FTPTCPSIMPLE_H

#include <vector>

#include <fstream>
#include "tawns/applications/contract/IAcousticApp.h"
#include "tawns/node/base/Disc.h"

#include "inet/applications/tcpapp/TcpAppBase.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/lifecycle/NodeStatus.h"

namespace tawns {

class INET_API FtpTcpSimple : public inet::TcpAppBase, IAcousticApp
{
  protected:
    // parameters
    struct Command {
        inet::simtime_t tSend;
        std::string name;
        std::string path;
        float srt;
        float ent;
        Command (inet::simtime_t t, std::string n, std::string p, float s, float e)
                { tSend = t; name = n; path = p; srt = s; ent = e;}
        Command()
        {tSend = 0; name = ""; path = ""; srt = 0; ent = 1;}
    };
    typedef std::vector<Command> CommandVector;
    CommandVector commands;

    bool announced = false;

    bool activeOpen = false;

    double generalTimeOut = 0;
    int counter = 0;
    int ftpPacketSize = 536;

    inet::simtime_t tOpen;
    inet::simtime_t tSend;
    inet::simtime_t tClose;
    int sendBytes = 0;

    Disc* disc = nullptr;

    // state
    int commandIndex = -1;
    inet::cMessage *ftpTimeout = nullptr;

  protected:
    virtual Recording popRecording(Disc *disc) override;
    virtual std::vector<uint8_t> announceDataPacket();

    virtual void handleMessageWhenUp(inet::cMessage *msg) override;

    virtual void handleStartOperation(inet::LifecycleOperation *operation) override;
    virtual void handleStopOperation(inet::LifecycleOperation *operation) override;
    virtual void handleCrashOperation(inet::LifecycleOperation *operation) override;

    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void createDataPacket(std::vector<uint8_t> announce);
    virtual void sendData();

    virtual void handleTimer(inet::cMessage *msg) override;
    virtual void socketEstablished(inet::TcpSocket *socket) override;
    virtual void socketDataArrived(inet::TcpSocket *socket, inet::Packet *msg, bool urgent) override;
    virtual void socketClosed(inet::TcpSocket *socket) override;
    virtual void socketFailure(inet::TcpSocket *socket, int code) override;

  public:
    FtpTcpSimple() {}
    virtual ~FtpTcpSimple();
};

} // namespace tawns

#endif

