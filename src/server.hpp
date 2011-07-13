#include <iostream>
#include <map>
#include "socket.h"

#ifndef _Net_Server_
#define _Net_Server_

namespace Net {
  const int SERVER_BACKLOG = 1024;
  const int BUFFER_SIZE = 1024 * 8 + 1; // 8kb buffer ( +1 for '\0' )
  
  template <class ConnectionClass>
  class Server {
    typedef typename std::map<int, ConnectionClass*> ConnectionTable;
    typedef typename std::map<int, ConnectionClass*>::iterator ConnectionTableIter;
    
  private:
    bool running;  
    struct event serverEvent;
    char buffer[BUFFER_SIZE];
    ConnectionTable clients; // list of descriptors and coresponding connections

  public:
    evutil_socket_t socket;
    sockaddr_in addr;
    
    Server() {
    }
    
    ~Server() {  
      // close the server socket
      this->removeSocketCallback(&this->serverEvent, this->socket);
      
      // remove all socket objects
      ConnectionTableIter iter;
      for (iter = this->clients.begin(); iter != this->clients.end(); iter++) {
        this->removeConnection(iter->second);
      }
    }
    
    // called when a client connects
    void onConnect() {
      Socket *socket = new Socket();
      socket->fd = accept(this->socket, (sockaddr *)&socket->addr, &socket->addr_size);
    
      // avoid blocking sockets
      if (evutil_make_socket_nonblocking(socket->fd) == -1) {
        throw new Exception("evutil_make_socket_nonblocking() failed");
      }
      
      // add a new client to the list of clients
      if (socket->fd != -1) {
        // the created connection will be cleaned in the removeConnection method
        ConnectionClass *connection = new ConnectionClass();
        connection->setSocket(socket);
        this->addConnection(connection);
      } else {
        // error free the created socket again
        free(socket);
      }
    }
    
    // called after a client disconnected
    inline void onDisconnect(ConnectionClass* connection, Socket* socket, int fd) {
      this->removeConnection(connection);
    }
    
    // called when data was received
    inline void onData(ConnectionClass* connection, Socket* socket, int fd) {
      int received = recv(fd, &buffer, BUFFER_SIZE, 0);
      
      // send recieved data to the socket connection handler
      if (received > 0) {
        this->buffer[received] = '\0'; // make the data a normal c string
        // call client callback for data
        connection->receivedData((char *)&this->buffer, received);
        
        // check if the connection should be closed
        if (connection->shouldClose == true) {
          this->removeConnection(connection);
        }
      } else if (received == 0) {
        this->onDisconnect(connection, socket, fd);
      }
    }
    
    // starts the server loop
    void start() {
      // make the socket non blocking
      if (evutil_make_socket_nonblocking(this->socket)) {
        throw new Exception("evutil_make_socket_nonblocking() failed");
      }
      
      // start up the server by binding it
      if (bind(this->socket, (sockaddr *)&this->addr, sizeof(this->addr)) == -1) {
        throw new Exception("bind() failed");
      }
      
      // start listening on the server socket
      if (listen(this->socket, SERVER_BACKLOG) == -1) {
        throw new Exception("listen() failed");
      }
      
      // start a loop where new connections are accepted
      this->running = true;
      
      // handle client connects
      this->addSocketCallback(&this->serverEvent, this->socket,
                              Server<ConnectionClass>::onConnect);
    }
    
    // stops the server
    void stop() {
      this->running = false;
    }
    
    // adds the connection to the server (will add a listener for this server)
    inline void addConnection(ConnectionClass * connection) {
      this->clients.insert(std::make_pair(connection->socket->fd, connection));
      this->addSocketCallback(&connection->socket->clientEvent, 
                              connection->socket->fd,
                              Server<ConnectionClass>::onData);
      
      // callback for client code
      connection->postInit();
    }
    
    // removes a connection from the server
    inline void removeConnection(ConnectionClass * connection) {
      this->removeSocketCallback(&connection->socket->clientEvent,
                                 connection->socket->fd);
      // callback for client code
      connection->unbind();
      free(connection);
      this->clients.erase(connection->socket->fd);
    }
    
    // adds a socket based callback to the event look
    inline void addSocketCallback(struct event * eventSocket, evutil_socket_t fd,
                           void (callback)(int, short, void *)) {
      event_set(eventSocket, fd, EV_READ | EV_PERSIST, callback, this);
      if (event_add(eventSocket, NULL) == -1) {
        throw new Exception("event_add() failed");
      }
    }
    
    // removes a socket based callback from the event loop
    inline void removeSocketCallback(struct event * eventSocket, evutil_socket_t fd) {
      evutil_closesocket(fd);
      event_del(eventSocket);
    }

    inline static void onConnect(int fd, short event, void *arg) {
      Server<ConnectionClass>* server = (Server<ConnectionClass>*)arg;
      server->onConnect();
    }
    
    inline static void onData(int fd, short event, void *arg) {
      Server<ConnectionClass>* server = (Server<ConnectionClass>*)arg;
      ConnectionClass* connection = server->clients[fd];
      Socket* socket = connection->socket;
      server->onData(connection, socket, fd);
    }
  };
  
  // start a tcp server on the passed interface and port
  template <class ConnectionClass>
  void startServer(const char * interface, short port, Server<ConnectionClass>* server) {   
#ifdef NET_DEBUG
    std::cout << "Start server on " << interface << ":" << port << std::endl;
#endif
    
    // create a tcp/ip socket
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket == -1) {
      throw new Exception("socket() failed");
    }
        
    // set interface, port and family for tcp/ip servers
    server->addr.sin_addr.s_addr = inet_addr(interface);
    server->addr.sin_port = htons(port);
    server->addr.sin_family = AF_INET;
    
    // accept incomming connections from clients
    server->start();
  }
}

#endif
