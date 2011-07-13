#include "net.h"

// simple echo server
class EchoConnection : public Net::Connection {
public:
  void receivedData(const char * data, int length) {
    if (strcmp(data, "exit\r\n") == 0) {
      closeConnection();
    } else {
      sendData(data, length);
    }
  }
};

const char * response = "HTTP/1.0 200 OK\r\nServer: SimpleHTTP\r\nHost: localhost\r\nContent-Length: 4\r\nContent-Type: text/html\r\n\r\nTest";
const int responseSize = strlen(response);

class HTTPConnection : public Net::Connection {
public:
  void receivedData(const char * data, int length) {
    sendData(response, responseSize);
    closeConnection();
  }
};

int main (int argc, char * const argv[]) {
  Net::Server<EchoConnection> echoServer;
  Net::Server<HTTPConnection> httpServer;
  try {
    Net::init();
    Net::startServer("0.0.0.0", 9002, &echoServer);
    Net::startServer("0.0.0.0", 9292, &httpServer);
    Net::run();
    return 0;
  } catch (Net::Exception* ex) {
    perror(ex->what()); // print network errors
    return 1;
  }
}
