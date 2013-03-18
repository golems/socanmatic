EXTRA_DIST = socia-manual.xml

# Rules to build
if HAVE_HELP2MAN
man/socia.1: $(top_srcdir)/src/socia.c $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "socia shell tool" $(top_builddir)/socia$(EXEEXT) -o $@

man/sociac.1: $(top_srcdir)/sociac $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "socia profile compiler" $(top_builddir)/socia$(EXEEXT) -o $@

if HAVE_MAN2HTML
.1.html:
	$(MAN2HTML) -r $< | sed -e '1d;2d;8d;s/@\|&amp;/{at}/g' > $@
endif
endif

if HAVE_DB2HTML
manual/socia-manual.html: socia-manual.xml
	db2html -u -o manual $<
endif

# test what to install
if HAVE_MANPAGES
dist_man_MANS = man/socia.1 man/sociac.1
endif

if HAVE_MANHTML
MAN_HTML = man/socia.html man/sociac.html
endif

if HAVE_MANUAL
DOCBOOK_HTML =  manual/socia-manual.html
endif

dist_doc_DATA = $(DOCBOOK_HTML) $(MAN_HTML)


maintainer-clean-local:
	-rm -rf *.1 *.html man/*.html man/*.1
