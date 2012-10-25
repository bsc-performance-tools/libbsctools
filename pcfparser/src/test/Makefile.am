noinst_PROGRAMS = TestUIParaverTraceConfig

TestUIParaverTraceConfig_SOURCES = TestUIParaverTraceConfig.cpp
TestUIParaverTraceConfig_LDFLAGS = -L../.libs -lparavertraceconfig
TestUIParaverTraceConfig_CXXFLAGS = -I ..

if NEED_BOOST_HOME
TestUIParaverTraceConfig_CXXFLAGS += -I $(BOOST_HOME)/include
endif

