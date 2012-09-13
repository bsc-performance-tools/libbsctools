
#include "ParaverTrace.h"
#include "ParaverTraceThread.h"
#include "ParaverTraceTask.h"
#include "ParaverTraceApplication.h"

#define UNREFERENCED(x) ((x)=(x))

namespace libparaver {

class ThreadInformation
{
};

class TaskInformation
{
	private:
	int numThreads;
	ThreadInformation *ThreadsInfo;

	public:
	const int getNumThreads (void)
	{ return numThreads; };

	ThreadInformation* getThreadsInformation (void)
	{ return ThreadsInfo; };

	void AllocateThreads (int numThreads);
	~TaskInformation();
};

TaskInformation::~TaskInformation()
{
	for (int i = 0; i < numThreads; i++)
		delete [] ThreadsInfo;
}

void TaskInformation::AllocateThreads (int numThreads)
{
	this->numThreads = numThreads;
	ThreadsInfo = new ThreadInformation[this->numThreads];
}

class InformationHolder
{
	private:
	int numTasks;
	TaskInformation *TasksInfo;
	
	public:
	const int getNumTasks (void)
	{ return numTasks; };

	TaskInformation* getTasksInformation (void)
	{ return TasksInfo; };

	void AllocateTasks (int numTasks);
	~InformationHolder();
};

InformationHolder::~InformationHolder()
{
	for (int i = 0; i < numTasks; i++)
		delete [] TasksInfo;
}

void InformationHolder::AllocateTasks (int numTasks)
{
	this->numTasks = numTasks;
	TasksInfo = new TaskInformation[this->numTasks];
}

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
	InformationHolder ih;
};

Process::Process (string prvFile, bool multievents) : ParaverTrace (prvFile, multievents)
{
}

void Process::processComment (string &c)
{
  cout << "COMMENT " << c << endl;
}

void Process::processCommunicator (string &c)
{
  cout << "COMMUNICATOR " << c << endl;
}

void Process::processState (struct state_t &s)
{
	cout << "STATE " << s.State << " for " << s.ObjectID.task << "." << s.ObjectID.thread << " from " << s.Begin_Timestamp << " to " << s.End_Timestamp << endl;
} 

void Process::processMultiEvent (struct multievent_t &e)
{
	cout << "EVENT for " << e.ObjectID.task << "." << e.ObjectID.thread  << " at " << e.Timestamp << " #" << e.events.size() << " [";
	for (vector<struct event_t>::iterator it = e.events.begin(); it != e.events.end(); it++)
		cout << " " << (*it).Type << "," << (*it).Value << " ";
	cout << "]" << endl;
}

void Process::processEvent (struct singleevent_t &e)
{
	/* Paraver traces count tasks & threads from 1..N */
	int task = e.ObjectID.task;
	int thread = e.ObjectID.thread;
	unsigned long long type = e.event.Type;
	unsigned long long value = e.event.Value;

	UNREFERENCED(task);
	UNREFERENCED(thread);
	UNREFERENCED(type);
	UNREFERENCED(value);
}

void Process::processCommunication (struct comm_t &c)
{
	cout << "COMM from " << c.Send_ObjectID.task << "." << c.Send_ObjectID.thread << " to " << c.Recv_ObjectID.task << "." << c.Recv_ObjectID.thread << endl;
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

	Process *p = new Process (argv[1], true);
	vector<ParaverTraceApplication *> va = p->get_applications();
	if (va.size() != 1)
	{
		cerr << "ERROR Cannot parse traces with more than one application" << endl;
		return -1;
	}

	p->parseBody();

	return 0;
}

