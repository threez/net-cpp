#include "socket.h"

Net::Socket::Socket() {
  this->addr_size = sizeof(sockaddr_in);
}

Net::Socket::~Socket() {
  this->close(); // close the still open connections
}

void Net::Socket::close() {
  evutil_closesocket(this->fd);
}

// returns the address used in dot format e.g. "127.0.0.1"
const char * Net::Socket::address() {
  return inet_ntoa(this->addr.sin_addr);
}

// returns the port used e.g. 56463
int Net::Socket::port() {
  return ntohs(this->addr.sin_port);
}

// send the data to the client socket
void Net::Socket::sendData(const char * data, int length) {
  send(this->fd, data, length, NULL);
}
