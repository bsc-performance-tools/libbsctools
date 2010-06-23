#define BOOST_TEST_MODULE UIParaverTraceConfig Test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <fstream>

#include "UIParaverTraceConfig.h"


struct SampleTrace {
    SampleTrace(){
		BOOST_TEST_MESSAGE( "Setup SampleTrace fixture" );
		t1 = new libparaver::UIParaverTraceConfig();
		std::string filename = "sampletrace.pcf";
		fb.open(filename.c_str(), std::ios::in);
		std::istream is(&fb);
		t1->parse(is, filename);
	}
    ~SampleTrace(){
		BOOST_TEST_MESSAGE( "Teardown SampleTrace fixture" );
		fb.close();
		delete t1;
	}

    libparaver::UIParaverTraceConfig * t1;
    std::filebuf fb;
};

BOOST_AUTO_TEST_CASE( Parsing_sampletrace_pcf ){
	libparaver::UIParaverTraceConfig * t1 = new libparaver::UIParaverTraceConfig();
	std::string filename = "sampletrace.pcf";
	std::filebuf fb;
	fb.open(filename.c_str(), std::ios::in);
	std::istream is(&fb);
	BOOST_CHECK(t1->parse(is, filename));
	fb.close();
	delete t1;
}


BOOST_FIXTURE_TEST_SUITE( Test_Using_SampleTrace_fixture, SampleTrace )

BOOST_AUTO_TEST_CASE( General_Options_test ){
	bool oldString = t1->getDebug();
	t1->setDebug(true);
	BOOST_CHECK(t1->getDebug());
	t1->setDebug(false);
	BOOST_CHECK(t1->getDebug() == false);
	t1->setDebug(oldString);
}

BOOST_AUTO_TEST_CASE( Default_Options_test ){
	std::string text = "THREAD";
	std::string oldString;
	int number = 5;
	int oldInt;
	
	oldString = t1->getLevel();
	t1->setLevel(text);
	BOOST_CHECK(t1->getLevel().compare(text)==0);
	t1->setLevel(oldString);
	
	oldString = t1->getUnits();
	t1->setUnits(text);
	BOOST_CHECK(t1->getUnits().compare(text)==0);
	t1->setUnits(oldString);
	
	oldInt = t1->getLook_back();
	t1->setLook_back(number);
	BOOST_CHECK(t1->getLook_back() == number);
	t1->setLook_back(oldInt);

	oldInt = t1->getSpeed();
	t1->setSpeed(number);
	BOOST_CHECK(t1->getSpeed() == number);
	t1->setSpeed(oldInt);
	
	oldString = t1->getFlag_icons();
	t1->setFlag_icons(text);
	BOOST_CHECK(t1->getFlag_icons().compare(text) == 0);
	t1->setFlag_icons(oldString);
	
	oldInt = t1->getNum_of_state_colors();
	t1->setNum_of_state_colors(number);
	BOOST_CHECK(t1->getNum_of_state_colors() == number);
	t1->setNum_of_state_colors(number);
	
	oldInt = t1->getYmax_scale();
	t1->setYmax_scale(number);
	BOOST_CHECK(t1->getYmax_scale() == number);
	t1->setYmax_scale(oldInt);
	
	oldString = t1->getThread_func();
	t1->setThread_func(text);
	BOOST_CHECK(t1->getThread_func().compare(text) == 0);
	t1->setThread_func(oldString);
}

BOOST_AUTO_TEST_CASE( getStates_test ){
	/* STATES
	 * 0    Idle
	 * 1    Running
	 * 2    Not created
	 * 3    Waiting a message
	 * 4    Bloking Send
	 * 5    Thd. Synchr.
	 * 6    Test/Probe
	 * 7    Sched. and Fork/Join
	 * 8    Wait/WaitAll
	 * 9    Blocked
	 * 10    Inmediate Send
	 * 11    Inmediate Receive
	 * 12    I/O
	 * 13    Group Communication
	 * 14    Tracing Disabled
	 * 15    Others
	 * 16    Send Receive
	 */
	std::vector<int> keys;
	keys = t1->getStates();
	
	BOOST_CHECK(t1->getState(keys[0]).compare("Idle") == 0);
	BOOST_CHECK(t1->getState(keys[6]).compare("Test/Probe") == 0);
	BOOST_CHECK(t1->getState(keys[16]).compare("Send Receive") == 0);
	BOOST_CHECK_THROW(t1->getState(18),
		libparaver::UIParaverTraceConfig::value_not_found);
}

BOOST_AUTO_TEST_CASE( getStateColor_test ){
	/*STATES_COLOR
	 * 0    {117,195,255}
	 * 1    {0,0,255}
	 * 2    {255,255,255}
	 * 3    {255,0,0}
	 * 4    {255,0,174}
	 * 5    {179,0,0}
	 * 6    {0,255,0}
	 * 7    {255,255,0}
	 * 8    {235,0,0}
	 * 9    {0,162,0}
	 * 10    {255,0,255}
	 * 11    {100,100,177}
	 * 12    {172,174,41}
	 * 13    {255,144,26}
	 * 14    {2,255,177}
	 * 15    {192,224,0}
	 * 16    {255,0,255}
	 */
	std::vector<int> keys;
	keys = t1->getStateColors();

	libparaver::UIParaverTraceConfig::StateColor sc1(117, 195, 255);
	BOOST_CHECK(sc1 == t1->getStateColor(keys[0]));
	BOOST_CHECK(sc1 != t1->getStateColor(keys[1]));
	sc1.setRed(255);
	sc1.setGreen(0);
	BOOST_CHECK(sc1 == t1->getStateColor(keys[16]));
	BOOST_CHECK_THROW(sc1 == t1->getGradientColor(keys.at(17)),
		std::out_of_range);
}

BOOST_AUTO_TEST_CASE( getGradientName_test ){
	/* GRADIENT_NAMES
	 * 0    Gradient 0
	 * 1    Grad. 1/MPI Events
	 * 2    Grad. 2/OMP Events
	 * 3    Grad. 3/OMP locks
	 * 4    Grad. 4/User func
	 * 5    Grad. 5/User Events
	 * 6    Grad. 6/General Events
	 * 7    Grad. 7/Hardware Counters
	 * 8    Gradient 8
	 * 9    Gradient 9
	 * 10    Gradient 10
	 * 11    Gradient 11
	 * 12    Gradient 12
	 * 13    Gradient 13
	 * 14    Gradient 14
	 */
	std::vector<int> keys;
	keys = t1->getGradientNames();

	BOOST_CHECK(t1->getGradientName(keys[0]).compare("Gradient 0")==0);
	BOOST_CHECK(t1->getGradientName(keys[7]).compare("Grad. 7/Hardware Counters")==0);
	BOOST_CHECK(t1->getGradientName(keys[14]).compare("Gradient 14")==0);
	BOOST_CHECK_THROW(t1->getGradientName(keys.at(15)),
		std::out_of_range);
}

BOOST_AUTO_TEST_CASE( getGradientColor_test ){
	/* GRADIENT_COLOR
	 * 0    {0,255,2}
	 * 1    {0,244,13}
	 * 2    {0,232,25}
	 * 3    {0,220,37}
	 * 4    {0,209,48}
	 * 5    {0,197,60}
	 * 6    {0,185,72}
	 * 7    {0,173,84}
	 * 8    {0,162,95}
	 * 9    {0,150,107}
	 * 10    {0,138,119}
	 * 11    {0,127,130}
	 * 12    {0,115,142}
	 * 13    {0,103,154}
	 * 14    {0,91,166}
	 */
	std::vector<int> keys;
	keys = t1->getGradientColors();

	libparaver::UIParaverTraceConfig::GradientColor gc1(0, 255, 2);
	BOOST_CHECK(gc1 == t1->getGradientColor(keys[0]));
	BOOST_CHECK(gc1 != t1->getGradientColor(keys[1]));
	gc1.setRed(0);
	gc1.setGreen(91);
	gc1.setBlue(166);
	BOOST_CHECK(gc1 == t1->getGradientColor(keys[14]));
	BOOST_CHECK_THROW(gc1 == t1->getGradientColor(keys.at(15)),
		std::out_of_range);
}


BOOST_AUTO_TEST_CASE( getEventType_test ){
	BOOST_CHECK(t1->getEventType("Send Size in MPI Global OP") == 51000001);
	BOOST_CHECK_THROW(t1->getEventType("AAAAAAA"), 
		libparaver::UIParaverTraceConfig::value_not_found);
	BOOST_CHECK(t1->getEventType(51000001).compare("Send Size in MPI Global OP")==0);
	BOOST_CHECK_THROW(t1->getEventType(0), 
		libparaver::UIParaverTraceConfig::value_not_found);
	
	std::vector<unsigned int> keys;
	keys = t1->getEventTypes();
	BOOST_CHECK(keys.size() == 15);
	BOOST_CHECK(t1->getEventType(keys[0]).compare("Application")==0);
	BOOST_CHECK(t1->getEventType(keys[5]).compare("Tracing")==0);
	BOOST_CHECK(t1->getEventType(keys[14]).compare("MPI caller line")==0);
	BOOST_CHECK_THROW(t1->getEventType(100), 
		libparaver::UIParaverTraceConfig::value_not_found);
	BOOST_CHECK_THROW(t1->getEventType(keys.at(100)), 
		std::out_of_range);
}

BOOST_AUTO_TEST_CASE( getEventValue_test ){
	/* EVENT_TYPE
	 * 9   50000001    MPI Point-to-point
	 * VALUES
	 * 4   MPI_Irecv
	 * 3   MPI_Isend
	 * 5   MPI_Wait
	 * 6   MPI_Waitall
	 * 0   End
	 */
	std::vector<unsigned int> values;

	unsigned int MPIGlobals = t1->getEventType("MPI Point-to-point");
	
	values = t1->getEventValues(MPIGlobals);
	BOOST_CHECK(values.size() == 5);
	BOOST_CHECK(t1->getEventValue(MPIGlobals, "MPI_Irecv") == 4);
	BOOST_CHECK(t1->getEventValue(MPIGlobals, 4).compare("MPI_Irecv") == 0);
	BOOST_CHECK(t1->getEventValue(MPIGlobals, 0).compare("End") == 0);
	BOOST_CHECK_THROW(t1->getEventValue(MPIGlobals, 1000), 
		libparaver::UIParaverTraceConfig::value_not_found);
	BOOST_CHECK_THROW(t1->getEventValue(90000, 10), 
		libparaver::UIParaverTraceConfig::value_not_found);
}

BOOST_AUTO_TEST_SUITE_END()
