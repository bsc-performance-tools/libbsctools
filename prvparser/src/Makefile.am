
lib_LTLIBRARIES = libparavertraceparser.la

include_HEADERS = UIParaverTrace.h \
 ParaverTrace.h \
 ParaverTraceThread.h \
 ParaverTraceTask.h \
 ParaverTraceApplication.h \
 ParaverTraceUnits.h \
 ParaverRecord.h \
 ParaverTraceNode.h

LIBPARAVERTRACEPARSER_COREFILES= \
 ParaverHeaderGrammar.cpp ParaverHeaderGrammar.h \
 ParaverTraceApplication.cpp ParaverTraceApplication.h \
 ParaverTrace.cpp ParaverTrace.h \
 ParaverTraceCPU.cpp ParaverTraceCPU.h \
 ParaverTraceNode.cpp ParaverTraceNode.h \
 ParaverTraceTask.cpp ParaverTraceTask.h \
 ParaverTraceThread.cpp ParaverTraceThread.h \
 UIParaverTrace.cpp \
 ParaverBodyGrammar.h \
 ParaverDump.h \
 ParaverRecord.h \
 ParaverTraceUnits.h \
 UIParaverTrace.h

libparavertraceparser_la_SOURCES = $(LIBPARAVERTRACEPARSER_COREFILES)

libparavertraceparser_la_LIBADD  =
libparavertraceparser_la_LDFLAGS  =

libparavertraceparser_la_CXXFLAGS = -I $(BOOST_DIR)

