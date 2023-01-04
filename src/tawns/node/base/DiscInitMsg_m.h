//
// Generated file, do not edit! Created by opp_msgtool 6.0 from tawns/node/base/DiscInitMsg.msg.
//

#ifndef __TAWNS_DISCINITMSG_M_H
#define __TAWNS_DISCINITMSG_M_H

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

class DiscInitMsg;

}  // namespace tawns


namespace tawns {

/**
 * Class generated from <tt>tawns/node/base/DiscInitMsg.msg:19</tt> by opp_msgtool.
 * <pre>
 * message DiscInitMsg
 * {
 * }
 * </pre>
 */
class DiscInitMsg : public ::omnetpp::cMessage
{
  protected:

  private:
    void copy(const DiscInitMsg& other);

  protected:
    bool operator==(const DiscInitMsg&) = delete;

  public:
    DiscInitMsg(const char *name=nullptr, short kind=0);
    DiscInitMsg(const DiscInitMsg& other);
    virtual ~DiscInitMsg();
    DiscInitMsg& operator=(const DiscInitMsg& other);
    virtual DiscInitMsg *dup() const override {return new DiscInitMsg(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DiscInitMsg& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DiscInitMsg& obj) {obj.parsimUnpack(b);}


}  // namespace tawns


namespace omnetpp {

template<> inline tawns::DiscInitMsg *fromAnyPtr(any_ptr ptr) { return check_and_cast<tawns::DiscInitMsg*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __TAWNS_DISCINITMSG_M_H
