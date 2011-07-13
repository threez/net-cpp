#include <iostream>
#include "connection.h"

Net::Connection::Connection() : shouldClose(false) {}

Net::Connection::~Connection() {
  free(this->socket);
}

// method that will be called when the client connected
void Net::Connection::postInit() {
#ifdef NET_DEBUG
  std::cout << "Client connected (" <<  this->socket->address() << ":"
            << this->socket->port() << ")" << std::endl;
#endif
}

// method that will be called when the client disconnected
void Net::Connection::unbind() {
#ifdef NET_DEBUG
  std::cout << "Client disconnected (" <<  this->socket->address() << ":"
            << this->socket->port() << ")" << std::endl;
#endif
}

// method that will be called when the client sends some data
void Net::Connection::receivedData(const char * data, int length) {
#ifdef NET_DEBUG
  std::cout << "Client received (" <<  length << "):" << std::endl
            << data << std::endl;
#endif
}

// sends the data for the client over the wire
void Net::Connection::sendData(const char * data, int length) {  
#ifdef NET_DEBUG
  std::cout << "Client send (" << length << "):" << std::endl
            << data << std::endl;
#endif
  this->socket->sendData(data, length);
}

// sets the client socket for this connection
void Net::Connection::setSocket(Socket *socket) {
  this->socket = socket;
}

void Net::Connection::closeConnection() {
  this->shouldClose = true;
}
