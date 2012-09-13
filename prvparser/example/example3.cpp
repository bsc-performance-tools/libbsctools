
#include "ParaverTrace.h"
#include "ParaverTraceThread.h"
#include "ParaverTraceTask.h"
#include "ParaverTraceApplication.h"

#include <iostream>
#include <fstream>

using namespace std;

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
	private:
	ofstream traceout;

	public:
	Process (string prvFile, bool multievents, string tracename);
	~Process ();

	void processState (struct state_t &s);
	void processMultiEvent (struct multievent_t &e);
	void processEvent (struct singleevent_t &e);
	void processCommunication (struct comm_t &c);
	void processCommunicator (string &c);
	void processComment (string &c);
	InformationHolder ih;
};

Process::Process (string prvFile, bool multievents, string tracename) : ParaverTrace (prvFile, multievents)
{
	traceout.open (tracename.c_str());
	if (!traceout.is_open())
	{
		cout << "Unable to create " << tracename << endl;
		exit (-1);
	}
}

Process::~Process ()
{
	traceout.close ();
}

void Process::processComment (string &c)
{
	traceout << "#" << c << endl;
}

void Process::processCommunicator (string &c)
{
	traceout << "c" << c << endl;
}

void Process::processState (struct state_t &s)
{
	traceout << "1:"
		<< s.ObjectID.cpu << ":" << s.ObjectID.ptask << ":" << s.ObjectID.task << ":" << s.ObjectID.thread << ":"
		<< s.Begin_Timestamp << ":" << s.End_Timestamp << ":" << s.State << endl; 
} 

void Process::processMultiEvent (struct multievent_t &e)
{
	traceout << "2:" 
		<< e.ObjectID.cpu << ":" << e.ObjectID.ptask << ":" << e.ObjectID.task << ":" << e.ObjectID.thread << ":"
		<< e.Timestamp;
	for (vector<struct event_t>::iterator it = e.events.begin(); it != e.events.end(); it++)
		traceout << ":" << (*it).Type << ":" << (*it).Value;

	traceout << endl;
}

void Process::processEvent (struct singleevent_t &e)
{
}

void Process::processCommunication (struct comm_t &c)
{
	traceout << "3:" 
		<< c.Send_ObjectID.cpu << ":" << c.Send_ObjectID.ptask << ":" << c.Send_ObjectID.task << ":" << c.Send_ObjectID.thread << ":"
		<< c.Logical_Send << ":" << c.Physical_Send << ":"
		<< c.Recv_ObjectID.cpu << ":" << c.Recv_ObjectID.ptask << ":" << c.Recv_ObjectID.task << ":" << c.Recv_ObjectID.thread << ":"
		<< c.Logical_Recv << ":" << c.Physical_Recv << ":"
		<< c.Size << ":" << c.Tag << endl;
}

} /* namespace libparaver */

using namespace::libparaver;
using namespace::std;

int main (int argc, char *argv[])
{
	string tracename;

	if (argc != 2)
	{
		cerr << "You must provide a tracefile!" << endl;
		return -1;
	}

	tracename = string(argv[1]);
	Process *p = new Process (argv[1], true, tracename.substr (0, tracename.rfind(".prv"))+string(".new.prv"));

	vector<ParaverTraceApplication *> va = p->get_applications();
	if (va.size() != 1)
	{
		cerr << "ERROR Cannot parse traces with more than one application" << endl;
		return -1;
	}

	p->parseBody();

	/* Copy .pcf and .row files */
	ifstream ifs_pcf ((tracename.substr (0, tracename.rfind(".prv"))+string(".pcf")).c_str());
	if (ifs_pcf.is_open())
	{
		ofstream ofs_pcf ((tracename.substr (0, tracename.rfind(".prv"))+string(".new.pcf")).c_str());
		ofs_pcf << ifs_pcf.rdbuf();
		ifs_pcf.close();
		ofs_pcf.close();
	}

	ifstream ifs_row ((tracename.substr (0, tracename.rfind(".prv"))+string(".row")).c_str());
	if (ifs_row.is_open())
	{
		ofstream ofs_row ((tracename.substr (0, tracename.rfind(".prv"))+string(".new.row")).c_str());
		ofs_row << ifs_row.rdbuf();
		ifs_row.close();
		ofs_row.close();
	}

	return 0;
}

