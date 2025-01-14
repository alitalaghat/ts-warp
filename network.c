/* ------------------------------------------------------------------------------------------------------------------ */
/* TS-Warp - Transparent SOCKS protocol Wrapper                                                                       */
/* ------------------------------------------------------------------------------------------------------------------ */

/* Copyright (c) 2021, 2022, Mikhail Zakharov <zmey20000@yahoo.com>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
   disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
   disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */


/* -- Network functions --------------------------------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "logfile.h"
#include "pidfile.h"
#include "utility.h"

 
/* ------------------------------------------------------------------------------------------------------------------ */
int connect_desnation(struct sockaddr dest) {
    /* Establish TCP connetion with a det address */
    
    int sock;

    if ((sock = socket(dest.sa_family, SOCK_STREAM, 0)) < 0) {
        printl(LOG_CRIT, "Error creating a socket for the destination address");
        return sock;
    }

    #if (WITH_TCP_NODELAY)
        unsigned char tpc_ndelay = 1;
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (unsigned char *)&tpc_ndelay, sizeof(tpc_ndelay)) == -1)
            printl(LOG_WARN, "Error setting TCP_NODELAY socket option for outgoing connections");
    #endif
    
    printl(LOG_VERB, "Socket to connect with destination address created");

    if ((connect(sock, &dest, sizeof dest)) < 0) {
        printl(LOG_CRIT, "Unable to connect with destination address");
        return -1;
    }

    return sock;
}

/* ------------------------------------------------------------------------------------------------------------------ */
char *inet2str(struct sockaddr *ai_addr, char *str_addr) {
    /* inet_ntop() wrapper. If str_add is NULL, memory is auto-allocated,
     don't forget to free it after usage! */

    if (!str_addr) str_addr = (char *)malloc(INET6_ADDRSTRLEN);

    memset(str_addr, 0, INET6_ADDRSTRLEN);

    switch (ai_addr->sa_family) {
        case AF_INET:
            return (char *)inet_ntop(AF_INET, &SIN4_ADDR(*ai_addr), str_addr, INET_ADDRSTRLEN);

        case AF_INET6:
            return (char *)inet_ntop(AF_INET6, &SIN6_ADDR(*ai_addr), str_addr, INET6_ADDRSTRLEN);

        default:
            printl(LOG_WARN, "Unrecognized address family: %d", ai_addr->sa_family);
            return NULL;
    }
    return str_addr;
}

/* ------------------------------------------------------------------------------------------------------------------ */
struct sockaddr *str2inet(char *str_addr, char *str_port, struct addrinfo *res, struct addrinfo *hints) {
    
    int free_mem = 0;
    int ret;

    if (!hints) {
        hints = (struct addrinfo *)malloc(sizeof(struct addrinfo));
        memset(hints, 0, sizeof(struct addrinfo));
        hints->ai_family = PF_UNSPEC;
        hints->ai_socktype = SOCK_STREAM;
        free_mem = 1;
    }

    if ((ret = getaddrinfo(str_addr, str_port, hints, &res)) > 0) {
        printl(LOG_CRIT, "Error resolving address [%s]:[%s]: [%s]", str_addr, str_port, gai_strerror(ret));
        mexit(1, pfile_name);
    }
        
    if (free_mem) free(hints);
    return res->ai_addr;
}
