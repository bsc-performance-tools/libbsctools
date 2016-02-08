#ifndef DOMAIN_PARAVERTRACETHREAD_H
#define DOMAIN_PARAVERTRACETHREAD_H


namespace libparaver {

class ParaverTraceThread {
  protected:
    static unsigned int globalKey;

    unsigned int key;

    unsigned int particularKey;


  public:
    ParaverTraceThread();

    ParaverTraceThread(unsigned int & key);

    virtual ~ParaverTraceThread();

    inline unsigned int get_key() const;

    void set_key(unsigned int value);

};

inline unsigned int ParaverTraceThread::get_key() const {
  return key;
}


} // namespace libparaver
#endif
