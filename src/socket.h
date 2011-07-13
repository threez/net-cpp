#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <event.h>
#include "exception.h"

#ifndef _Net_Socket_
#define _Net_Socket_

namespace Net {
  class Socket {
  public:
    evutil_socket_t fd;
    sockaddr_in addr;
    socklen_t addr_size;
    struct event clientEvent;
    
    Socket();
    ~Socket();
    void close();
    const char * address();
    int port();
    void sendData(const char * data, int length);
  };
}

#endif
