#pragma once


#define INT_MAX                         UINT32_MAX

#define DEFAULT_THREAD_STACKSIZE        2048
#define TCPIP_THREAD_STACKSIZE          2048

#define NO_SYS                          0
#define LWIP_NETCONN                    1
#define LWIP_SOCKET                     0
#define SYS_LIGHTWEIGHT_PROT            0

#define LWIP_ETHERNET                   1
#define LWIP_IPV4                       1
#define LWIP_IPV6                       0
#define IPV6_FRAG_COPYHEADER            1
#define LWIP_IPV6_DUP_DETECT_ATTEMPTS   0


#define TCPIP_MBOX_SIZE                 10

// Enable some protocols to test them
#define LWIP_DHCP                       1
#define LWIP_AUTOIP                     1

#define LWIP_IGMP                       1
#define LWIP_DNS                        1

#define LWIP_ALTCP                      1

// Turn off checksum verification of fuzzed data
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_CHECK_TCP              0
#define CHECKSUM_CHECK_ICMP             0
#define CHECKSUM_CHECK_ICMP6            0

// Minimal changes to opt.h required for tcp unit tests:
#define MEM_SIZE                        8000
#define TCP_SND_QUEUELEN                20
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_OVERSIZE                    1
#define TCP_SND_BUF                     (6 * TCP_MSS)
#define TCP_WND                         (50 * TCP_MSS)
#define LWIP_WND_SCALE                  1
#define TCP_RCV_SCALE                   2
#define PBUF_POOL_SIZE                  128

// Minimal changes to opt.h required for etharp unit tests:
#define ETHARP_SUPPORT_STATIC_ENTRIES   1

#define LWIP_NUM_NETIF_CLIENT_DATA      1
#define LWIP_SNMP                       1
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
