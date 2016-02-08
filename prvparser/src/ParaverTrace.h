#ifndef DOMAIN_PARAVERTRACE_H
#define DOMAIN_PARAVERTRACE_H

#define MAX_NUM_LINES 20


#include <string>
using namespace std;
#include <vector>
using namespace std;
#include "ParaverTraceUnits.h"
#include "ParaverRecord.h"

#include <fstream>
#include <iostream>
using namespace std;
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
using namespace boost;
using namespace boost::spirit;

namespace libparaver { class ParaverTraceApplication; } 
namespace libparaver { class ParaverTraceNode; } 
class LibException;
namespace libparaver { template<typename Actions> class ParaverHeaderGrammar; } 

namespace libparaver {

class ParaverTrace {
  protected:
    string prvFile;

    unsigned long long lastTime;

    vector<ParaverTraceApplication *> applications;

    ParaverTraceUnits * units;

    vector<ParaverTraceNode *> nodes;

    bool multievents;

  public:
    ParaverTrace (string prvFile, bool multievents = true);

    virtual ~ParaverTrace();

		void parseBody (void);
    virtual void processState (struct state_t &s);
    virtual void processCommunication (struct comm_t &c);
    virtual void processEvent (struct singleevent_t &e);
    virtual void processMultiEvent (struct multievent_t &e);
    virtual void processComment (string &c);
    virtual void processCommunicator (string &c);

    void clearEvent (struct multievent_t &me);
    void newEvent (struct multievent_t &me, struct event_t &e);
    void processEvents (struct multievent_t &e);

  private:
    void parseHeader();

  public:
    inline unsigned long long get_lastTime() const;

    void set_lastTime(unsigned long long value);

    void setNumberOfApplications(unsigned int nappl);

    void addTasksToAppl(unsigned int appl, unsigned int task, unsigned int thread, unsigned int node);

    void setNumberOfNodes(unsigned int _nodes);

    void setNumberOfTasksForAppl(unsigned int appl, unsigned int tasks);

    inline const vector<ParaverTraceApplication *> & get_applications() const;

    void set_applications(vector<ParaverTraceApplication *> & value);

    inline const string get_prvFile() const;

    void set_prvFile(string value);

    inline const vector<ParaverTraceNode *> & get_nodes() const;

    void set_nodes(vector<ParaverTraceNode *> & value);

    void set_multievents (bool multievents);

    inline bool get_multievents (void) const;

};

inline unsigned long long ParaverTrace::get_lastTime() const {
  return lastTime;
}

inline const vector<ParaverTraceApplication *> & ParaverTrace::get_applications() const {
  return applications;
}

inline const string ParaverTrace::get_prvFile() const {
  return prvFile;
}

inline const vector<ParaverTraceNode *> & ParaverTrace::get_nodes() const {
  return nodes;
}

inline bool ParaverTrace::get_multievents (void) const
{
	return multievents;
}


} // namespace libparaver
#endif
