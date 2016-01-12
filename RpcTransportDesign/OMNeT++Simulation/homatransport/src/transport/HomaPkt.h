/*
 * HomaPkt.h
 *
 *  Created on: Sep 16, 2015
 *      Author: behnamm
 */

#ifndef HOMAPKT_H_
#define HOMAPKT_H_

#include "transport/HomaPkt_m.h"

/**
 * All of the constants packet and header byte sizes for different types of
 * datagrams.
 */
static const uint32_t ETHERNET_PREAMBLE_SIZE = 8;
static const uint32_t ETHERNET_HDR_SIZE = 14;
static const uint32_t MAX_ETHERNET_PAYLOAD_BYTES = 1500;
static const uint32_t MIN_ETHERNET_PAYLOAD_BYTES = 46;
static const uint32_t MIN_ETHERNET_FRAME_SIZE = 64;
static const uint32_t IP_HEADER_SIZE = 20;
static const uint32_t UDP_HEADER_SIZE = 8;
static const uint32_t ETHERNET_CRC_SIZE = 4;
static const uint32_t INTER_PKT_GAP = 12;

class HomaPkt : public HomaPkt_Base
{

  private:
    void copy(const HomaPkt& other);

  public:
    HomaPkt(const char *name=NULL, int kind=0);
    HomaPkt(const HomaPkt& other);
    HomaPkt& operator=(const HomaPkt& other);


    virtual HomaPkt *dup() const;

    // ADD CODE HERE to redefine and implement pure virtual functions from HomaPkt_Base

    /**
     * returns the header size of this packet.
     */
    uint32_t headerSize();

    /**
     * This function checks if there is a HomaPkt packet encapsulated in the messages
     * and returns it. Returns null if no HomaPkt is encapsulated.
     */
    static cPacket* searchEncapHomaPkt(cPacket* msg);

    /**
     * This function compares priority of two HomaPkt.
     */
    static int comparePrios(cObject* obj1, cObject* obj2);

    /**
     * This function compares priority and msg sizes of two Unsched. HomaPkt.
     */
    static int compareSizeAndPrios(cObject* obj1, cObject* obj2);

    static uint32_t getBytesOnWire(uint32_t numDataBytes, PktType homaPktType);
};

#endif /* HOMAPKT_H_ */
