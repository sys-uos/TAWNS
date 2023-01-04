//
// Generated file, do not edit! Created by opp_msgtool 6.0 from tawns/node/base/MicrophoneControlMsg.msg.
//

#ifndef __TAWNS_MICROPHONECONTROLMSG_M_H
#define __TAWNS_MICROPHONECONTROLMSG_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif


namespace tawns {

class MicrophoneControlMsg;

}  // namespace tawns


namespace tawns {

/**
 * Class generated from <tt>tawns/node/base/MicrophoneControlMsg.msg:21</tt> by opp_msgtool.
 * <pre>
 * message MicrophoneControlMsg
 * {
 *     string path; // refers to path to audio
 *     double start; // refers start of the recorded audio interval
 *     double end; // refers start of the recorded audio interval
 * }
 * </pre>
 */
class MicrophoneControlMsg : public ::omnetpp::cMessage
{
  protected:
    ::omnetpp::opp_string path;
    double start = 0;
    double end = 0;

  private:
    void copy(const MicrophoneControlMsg& other);

  protected:
    bool operator==(const MicrophoneControlMsg&) = delete;

  public:
    MicrophoneControlMsg(const char *name=nullptr, short kind=0);
    MicrophoneControlMsg(const MicrophoneControlMsg& other);
    virtual ~MicrophoneControlMsg();
    MicrophoneControlMsg& operator=(const MicrophoneControlMsg& other);
    virtual MicrophoneControlMsg *dup() const override {return new MicrophoneControlMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual const char * getPath() const;
    virtual void setPath(const char * path);

    virtual double getStart() const;
    virtual void setStart(double start);

    virtual double getEnd() const;
    virtual void setEnd(double end);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MicrophoneControlMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MicrophoneControlMsg& obj) {obj.parsimUnpack(b);}


}  // namespace tawns


namespace omnetpp {

template<> inline tawns::MicrophoneControlMsg *fromAnyPtr(any_ptr ptr) { return check_and_cast<tawns::MicrophoneControlMsg*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __TAWNS_MICROPHONECONTROLMSG_M_H
