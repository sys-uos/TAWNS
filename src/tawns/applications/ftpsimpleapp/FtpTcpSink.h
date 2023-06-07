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


#ifndef TAWNS_FTPTCPSINK_H
#define TAWNS_FTPTCPSINK_H

#include <fstream>

#include "tawns/applications/contract/IAcousticApp.h"

#include "inet/applications/tcpapp/TcpServerHostApp.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

namespace tawns {

class INET_API FtpTcpSink : public inet::TcpServerHostApp, IAcousticApp
{
  protected:
    long bytesRcvd = 0;
    int ftpPacketSize = 536;
  protected:
    virtual Recording popRecording(Disc *disc) override;
    virtual void handleMessageWhenUp(inet::cMessage *msg) override;

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void finish() override;
    virtual void refreshDisplay() const override;

  public:
    FtpTcpSink();
    ~FtpTcpSink();

    friend class FtpTcpSinkThread;
};

class INET_API FtpTcpSinkThread : public inet::TcpServerThreadBase
{
  protected:
    long bytesRcvd;
    bool receiving = false;
    std::vector<uint8_t> buffer;
    //TODO: these should become parameters probably
    std::string sink_path = "./../../out/test-server";
    std::string filename = "./../../out/test-server";

    long size = 0;
    FtpTcpSink *sinkAppModule = nullptr;

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void refreshDisplay() const override;

    // TcpServerThreadBase:
    /**
     * Called when connection is established.
     */
    virtual void established() override;

    /*
     * Called when a data packet arrives.
     */
    virtual void dataArrived(inet::Packet *msg, bool urgent) override;

    /*
     * Called when a timer (scheduled via scheduleAt()) expires.
     */
    virtual void timerExpired(inet::cMessage *timer) override { throw inet::cRuntimeError("Model error: unknown timer message arrived"); }

    virtual void init(inet::TcpServerHostApp *hostmodule, inet::TcpSocket *socket) override { inet::TcpServerThreadBase::init(hostmodule, socket); sinkAppModule = inet::check_and_cast<FtpTcpSink *>(hostmod); }
};

} // namespace tawns

#endif

