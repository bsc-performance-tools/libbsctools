
PRVPARSER_CONFIG = prvparser-config

EXTRA_DIST = $(PRVPARSER_CONFIG).in 

bin_SCRIPTS = $(PRVPARSER_CONFIG) 

$(PRVPARSER_CONFIG): $(PRVPARSER_CONFIG).in
	cp $^ $@
	$(top_srcdir)/substitute $(SED) "@sub_PREFIX@" "${prefix}" $@
	$(top_srcdir)/substitute $(SED) "@sub_LIBDIR@" "${libdir}" $@
	$(top_srcdir)/substitute $(SED) "@sub_VERSION@" "${VERSION}" $@
	$(top_srcdir)/substitute $(SED) "@sub_BOOST_CFLAGS@" "-I$(BOOST_HOME)/include" $@
	chmod u+x $@

install-data-hook:
	chmod u+x $(DESTDIR)${bindir}/$(PRVPARSER_CONFIG)

clean-local:
	rm -f $(PRVPARSER_CONFIG) 

