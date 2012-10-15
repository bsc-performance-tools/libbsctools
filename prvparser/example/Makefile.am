
EXTRA_DIST= example1.cpp \
 example2.cpp \
 example3.cpp \
 example4.cpp \
 Makefile.todistribute

install-data-hook:
	$(mkdir_p) $(DESTDIR)$(datadir)/example
	cp -f example[1234].cpp $(DESTDIR)$(datadir)/example
	cp -f Makefile.todistribute $(DESTDIR)$(datadir)/example/Makefile

