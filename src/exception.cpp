#include "exception.h"

Net::Exception::Exception(const char * msg): msg(msg) {
  
}

const char * Net::Exception::what() {
  return this->msg;
}
