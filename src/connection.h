#include "socket.h"

#ifndef _Net_Connection_
#define _Net_Connection_

namespace Net {
  class Connection {    
  public:
    Connection();
    ~Connection();
    Socket* socket;
    bool shouldClose;
    virtual void postInit();
    virtual void unbind();
    virtual void receivedData(const char * data, int length);
    void sendData(const char * data, int length);
    void setSocket(Socket *socket);
    void closeConnection();
  };
}

#endif
