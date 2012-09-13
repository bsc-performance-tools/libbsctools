
#include "ParaverTrace.h"
#include "ParaverTraceThread.h"
#include "ParaverTraceTask.h"
#include "ParaverTraceApplication.h"

#include "map"

using namespace std;

map<unsigned long long, unsigned long long> freq;

#define UNREFERENCED(x) ((x)=(x))

namespace libparaver {

class Process : public ParaverTrace
{
	public:
  Process (string prvFile, bool multievents);

	void processState (struct state_t &s);
	void processMultiEvent (struct multievent_t &e);
	void processEvent (struct singleevent_t &e);
	void processCommunication (struct comm_t &c);
	void processCommunicator (string &c);
	void processComment (string &c);
};

Process::Process (string prvFile, bool multievents) : ParaverTrace (prvFile, multievents)
{
}

void Process::processComment (string &c)
{
}

void Process::processCommunicator (string &c)
{
}

void Process::processState (struct state_t &s)
{
} 

void Process::processMultiEvent (struct multievent_t &e)
{
}

void Process::processEvent (struct singleevent_t &e)
{
	unsigned long long type = e.event.Type;

	if (type >= 42000000 && type <= 42999999)
		type = 42000000;

	if (type >= 30000000 && type <= 31999999)
		type = 30000000;

	if (type >= 80000000 && type <= 80000099)
		type = 80000000;

	if (type >= 70000000 && type <= 70000099)
		type = 70000000;

	freq[type]++;
}

void Process::processCommunication (struct comm_t &c)
{
}

} /* namespace libparaver */

using namespace::libparaver;
using namespace::std;

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "You must provide a tracefile!" << endl;
		return -1;
	}

	Process *p = new Process (argv[1], false);
	vector<ParaverTraceApplication *> va = p->get_applications();
	if (va.size() != 1)
	{
		cerr << "ERROR Cannot parse traces with more than one application" << endl;
		return -1;
	}

	p->parseBody();

	unsigned long long total = 0;
	{
		map<unsigned long long, unsigned long long>::iterator iter = freq.begin();
		for (; iter != freq.end(); ++iter)
			total += iter->second;
	}

	cout.setf(ios::fixed,ios::floatfield);
	cout.precision(2);

	{
		map<unsigned long long, unsigned long long>::iterator iter = freq.begin();
		for (; iter != freq.end(); ++iter)
		{
			double pct = (100.0f * (double) iter->second) / ((double) total);

			cout << iter->first << " " << iter->second << " (" << pct << "%)" << endl;
		}
	}

	cout << "Total records = " << total << endl;

	return 0;
}

