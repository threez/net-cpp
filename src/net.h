#include "server.hpp"
#include "connection.h"

#ifndef _Net_
#define _Net_

namespace Net {
  // Initalize the event library
  void init();  
  void run();
}

#endif
