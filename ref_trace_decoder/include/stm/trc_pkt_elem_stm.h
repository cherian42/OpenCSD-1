/*
 * \file       trc_pkt_elem_stm.h
 * \brief      Reference CoreSight Trace Decoder : 
 * 
 * \copyright  Copyright (c) 2015, ARM Limited. All Rights Reserved.
 */

/* 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */ 

#ifndef ARM_TRC_PKT_ELEM_STM_H_INCLUDED
#define ARM_TRC_PKT_ELEM_STM_H_INCLUDED

#include "trc_pkt_types_stm.h"
#include "trc_printable_elem.h"

class StmTrcPacket : public rctdl_stm_pkt, public trcPrintableElem
{
public:
    StmTrcPacket();
    ~StmTrcPacket() {};
    
    StmTrcPacket &operator =(const rctdl_stm_pkt *p_pkt);

    void initStartState();
    void initNextPacket();

    void setPacketType(const rctdl_stm_pkt_type type, const bool bMarker);
    void updateErrType(const rctdl_stm_pkt_type err_type);
    void setMaster(const uint8_t master);
    void setChannel(const uint16_t channel, const bool b8Bit);
    void setTS(const uint64_t ts_val, const uint8_t updatedBits);
    void onVersionPkt(const rctdl_stm_ts_type type);
    void setD8Payload(const uint8_t value);
    void setD16Payload(const uint16_t value);
    void setD32Payload(const uint32_t value);
    void setD64Payload(const uint64_t value);

    const bool isMarkerPkt() const;
    const bool isTSPkt() const;
    
    const rctdl_stm_pkt_type getPktType() const;
    const rctdl_stm_pkt_type getPktErrType() const;
    const uint8_t getMaster() const;
    const uint16_t getChannel() const;
    const rctdl_stm_ts_type getTSType() const;
    const uint64_t getCurrentTSVal() const;

    const uint8_t getD8Val() const;
    const uint16_t getD16Val() const;
    const uint32_t getD32Val() const;
    const uint64_t getD64Val() const;

    const bool isBadPacket() const;

    // printing
    virtual void toString(std::string &str) const;
    virtual void toStringFmt(const uint32_t fmtFlags, std::string &str) const;


private:
    std::string &pktTypeName(const rctdl_stm_pkt_type pkt_type, std::string &desc) const;
};

inline void StmTrcPacket::setPacketType(const rctdl_stm_pkt_type type, const bool bMarker)
{
    this->type = type;
    if(bMarker)
        pkt_has_marker = 1;
}

inline void StmTrcPacket::updateErrType(const rctdl_stm_pkt_type err_type)
{
    this->err_type = this->type;    // original type is the err type;
    this->type = err_type;          // mark main type as an error.
}

inline void StmTrcPacket::setMaster(const uint8_t master)
{
    this->master = master;
    channel = 0;    // M8 forces current channel to 0.
}

inline void StmTrcPacket::setChannel(const uint16_t channel, const bool b8Bit)
{
    if(b8Bit)
        this->channel = (this->channel & 0xFF00) | (channel & 0xFF);
    else
        this->channel = channel;
}

inline void StmTrcPacket::onVersionPkt(const rctdl_stm_ts_type type)
{
    this->ts_type = type;
    master = 0;
    channel = 0;
}

inline void StmTrcPacket::setD8Payload(const uint8_t value)
{
    payload.D8 = value;
}

inline void StmTrcPacket::setD16Payload(const uint16_t value)
{
    payload.D16 = value;
}

inline void StmTrcPacket::setD32Payload(const uint32_t value)
{
    payload.D32 = value;
}

inline void StmTrcPacket::setD64Payload(const uint64_t value)
{
    payload.D64 = value;
}

inline const bool StmTrcPacket::isMarkerPkt() const
{
    return (pkt_has_marker != 0);
}

inline const bool StmTrcPacket::isTSPkt() const
{
    return (pkt_ts_bits != 0);
}

inline const rctdl_stm_pkt_type StmTrcPacket::getPktType() const
{
    return type;
}

inline const rctdl_stm_pkt_type StmTrcPacket::getPktErrType() const
{
    return err_type;
}

inline const uint8_t StmTrcPacket::getMaster() const
{
    return master;
}

inline const uint16_t StmTrcPacket::getChannel() const
{
    return channel;
}

inline const rctdl_stm_ts_type StmTrcPacket::getTSType() const
{
    return ts_type;
}

inline const uint64_t StmTrcPacket::getCurrentTSVal() const
{
    return timestamp;
}

inline const uint8_t StmTrcPacket::getD8Val() const
{
    return payload.D8;
}

inline const uint16_t StmTrcPacket::getD16Val() const
{
    return payload.D16;
}

inline const uint32_t StmTrcPacket::getD32Val() const
{
    return payload.D32;
}

inline const uint64_t StmTrcPacket::getD64Val() const
{
    return payload.D64;
}

inline const bool StmTrcPacket::isBadPacket() const
{
    return (bool)(type >= STM_PKT_BAD_SEQUENCE);
}


#endif // ARM_TRC_PKT_ELEM_STM_H_INCLUDED

/* End of File trc_pkt_elem_stm.h */