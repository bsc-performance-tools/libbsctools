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

}

BOOST_AUTO_TEST_CASE( getEventType_test ){
	BOOST_CHECK(t1->getEventType("Send Size in MPI Global OP") == 51000001);
	BOOST_CHECK(t1->getEventType(51000001).compare("Send Size in MPI Global OP")==0);
}


BOOST_AUTO_TEST_SUITE_END()
