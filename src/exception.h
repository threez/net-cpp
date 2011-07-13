#include <exception>

#ifndef _Net_Exception_
#define _Net_Exception_

namespace Net {
  class Exception : public std::exception {
  private:
    const char * msg;
  public:
    Exception(const char * msg);
    const char * what();
  };
}

#endif
