
#include "ParaverTraceThread.h"

namespace libparaver {

unsigned int ParaverTraceThread::globalKey=0;

ParaverTraceThread::ParaverTraceThread() {
  particularKey = globalKey++;
}

ParaverTraceThread::ParaverTraceThread(unsigned int & key):key(key) {
  particularKey = globalKey++;
}

ParaverTraceThread::~ParaverTraceThread() {
}

void ParaverTraceThread::set_key(unsigned int value) {
  key = value;
}


} // namespace libparaver
