#include "net.h"

// Initalize the event library
void Net::init() {
  event_init();
}

void Net::run() {
  event_dispatch();
}
