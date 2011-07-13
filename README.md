# Net-cpp

This is a simple library and test on building a EventMachine like interface
for implementing servers in C++. The Naming conventions should be familiar.
The implementation uses the very known libevent for implementing the reactor.

## Echo server sample

Here is a simple Application starting an Echo Server on port 9002.

    #include "net.h"

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

    int main (int argc, char * const argv[]) {
      Net::Server<EchoConnection> echoServer;
      try {
        Net::init();
        Net::startServer("0.0.0.0", 9002, &echoServer);
        Net::run();
        return 0;
      } catch (Net::Exception* ex) {
        perror(ex->what()); // print network errors
        return 1;
      }
    }
    
## HTTP server sample

This is another sample application implementing an http server:

    #include "net.h"
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
      Net::Server<HTTPConnection> httpServer;
      try {
        Net::init();
        Net::startServer("0.0.0.0", 9292, &httpServer);
        Net::run();
        return 0;
      } catch (Net::Exception* ex) {
        perror(ex->what()); // print network errors
        return 1;
      }
    }

## Running more servers

Both Servers can easily run at the same time:

    #include "net.h"

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

# Trying out

Simply install libevent, example for Mac OS X:

    brew install libevent
    
And then compile using the Makefile

    make

Start the server using:

    ./net-cpp-test
    
And connect using a telnet client or curl:

    telnet localhost 9002
    curl http://localhost:9292/




