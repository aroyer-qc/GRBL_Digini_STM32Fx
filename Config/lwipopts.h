#pragma once

#define INT_MAX                         UINT32_MAX

#define DEFAULT_THREAD_PRIO             3
#define DEFAULT_THREAD_STACKSIZE        2048

#define TCPIP_THREAD_PRIO               3
#define TCPIP_THREAD_STACKSIZE          2048

#define NO_SYS                          0                           // NO_SYS==0: Use lwIP with OS-awareness
#define LWIP_NETCONN                    0                           // LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
#define LWIP_SOCKET                     0                           // LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
#define SYS_LIGHTWEIGHT_PROT            1                           // SYS_LIGHTWEIGHT_PROT==1: enable inter-task protection (and task-vs-interrupt protection) for certain critical regions during buffer allocation,
                                                                    // deallocation and memory allocation and deallocation.
                                                                    // ATTENTION: This is required when using lwIP from more than one context! If you disable this, you must be sure what you are doing!

#define LWIP_ETHERNET                   1                           // LWIP_ETHERNET==1: enable ethernet support for PPPoE even though ARP might be disabled
#define LWIP_IPV4                       1                           // LWIP_IPV4==1: Enable IPv4
#define LWIP_IPV6                       0                           // LWIP_IPV6==1: Enable IPv6


#define TCPIP_MBOX_SIZE                 10                          // The mailbox size for the tcpip thread messages

// Protocol
#define LWIP_DHCP                       1                           // LWIP_DHCP==1:   Turn on DHCP module       (Dynamic Host Configuration Protocol)
#define LWIP_UDP                        1                           // LWIP_UDP==1:    Turn on UDP module.       (User Datagram Protocol)
#define LWIP_IGMP                       1                           // LWIP_IGMP==1:   Turn on IGMP module.      (Internet Group Management Protocol)
#define LWIP_SNMP                       1                           // LWIP_SNMP==1:   Turn on SNMP module.      (Simple Network Management Protocol)
#define LWIP_ARP                        1                           // LWIP_ARP==1:    Turn on ARP module.       (Address Resolution Protocol)

// Service
#define LWIP_DNS                        1                           // LWIP_DNS==1:    Turn on DNS module.       (Domain Name System) UDP must be available for DNS transport.
#define LWIP_AUTOIP                     1                           // LWIP_AUTOIP==1: Turn on AUTOIP module.
#define LWIP_ALTCP                      1                           // LWIP_ALTCP==1:  Turn on the altcp API.    altcp is an abstraction layer that prevents applications linking against the tcp.h functions
                                                                    //                                           but provides the same functionality. It is used to e.g. add SSL/TLS or proxy-connect support to an application
                                                                    //                                           written for the tcp callback API without that application knowing the protocol details.

// Turn off checksum verification of fuzzed data
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_CHECK_TCP              0
#define CHECKSUM_CHECK_ICMP             0
#define CHECKSUM_CHECK_ICMP6            0

#define MEM_SIZE                        (16*1024)                   // The size of the heap memory. If the application will send a lot of data that needs to be copied, this should be set high.
#define TCP_SND_QUEUELEN                20                          // TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_OVERSIZE                    TCP_MSS                     // TCP_OVERSIZE: The maximum number of bytes that tcp_write may allocate ahead of time in an attempt to create shorter pbuf chains for transmission
                                                                    // The meaningful range is 0 to TCP_MSS. Some suggested values are:
                                                                    //      * 0:         Disable oversized allocation. Each tcp_write() allocates a new pbuf (old behaviour).
                                                                    //      * 1:         Allocate size-aligned pbufs with minimal excess. Use this if your scatter-gather DMA requires aligned fragments.
                                                                    //      * 128:       Limit the pbuf/memory overhead to 20%.
                                                                    //      * TCP_MSS:   Try to create unfragmented TCP packets.
                                                                    //      * TCP_MSS/4: Try to create 4 fragments or less per TCP packet.
#define TCP_SND_BUF                     (6 * TCP_MSS)               // TCP_SND_BUF: TCP sender buffer space (bytes). To achieve good performance, this should be at least 2 * TCP_MSS.
#define TCP_WND                         (8 * TCP_MSS)

// When LWIP_WND_SCALE is enabled but TCP_RCV_SCALE is 0, we can use a large send window while having a small receive window only.
#define LWIP_WND_SCALE                  1                           // Set LWIP_WND_SCALE to 1 to enable window scaling.
#define TCP_RCV_SCALE                   2                           // Set TCP_RCV_SCALE to the desired scaling factor (shift count in the range of [0..14]).

// PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is designed to accommodate single full size TCP frame in one pbuf, including TCP_MSS, IP header, and link header.
#define PBUF_POOL_SIZE                  8 //LWIP_MEM_ALIGN_SIZE(TCP_MSS + 40 + PBUF_LINK_ENCAPSULATION_HLEN + PBUF_LINK_HLEN)

#define ETHARP_SUPPORT_STATIC_ENTRIES   1

#define LWIP_NUM_NETIF_CLIENT_DATA      1
#define MIB2_STATS                      0
#define LWIP_MDNS_RESPONDER             1



#define LWIP_PROVIDE_ERRNO              1

#define LWIP_DEBUG

#ifdef LWIP_DEBUG
#define TCP_DEBUG                       LWIP_DBG_ON
#define ETHARP_DEBUG                    LWIP_DBG_ON
#define PBUF_DEBUG                      LWIP_DBG_ON
#define IP_DEBUG                        LWIP_DBG_ON
#define TCPIP_DEBUG                     LWIP_DBG_ON
#define DHCP_DEBUG                      LWIP_DBG_ON
#define UDP_DEBUG                       LWIP_DBG_ON
#endif
