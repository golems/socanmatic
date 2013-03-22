EXTRA_DIST = socanmatic-manual.xml

# Rules to build
if HAVE_HELP2MAN
man/canmat.1: $(top_srcdir)/src/canmat.c $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "canmat shell tool" $(top_builddir)/canmat$(EXEEXT) -o $@

man/canmatc.1: $(top_srcdir)/canmatc $(top_srcdir)/configure.ac
	$(HELP2MAN) -h -h -v -V --no-info -n "canmat profile compiler" $(top_builddir)/canmat$(EXEEXT) -o $@

if HAVE_MAN2HTML
.1.html:
	$(MAN2HTML) -r $< | sed -e '1d;2d;8d;s/@\|&amp;/{at}/g' > $@
endif
endif

if HAVE_DB2HTML
manual/socanmatic-manual.html: socanmatic-manual.xml
	db2html -u -o manual $<
endif

# test what to install
if HAVE_MANPAGES
dist_man_MANS = man/canmat.1 man/canmatc.1
endif

if HAVE_MANHTML
MAN_HTML = man/canmat.html man/canmatc.html
endif

if HAVE_MANUAL
DOCBOOK_HTML =  manual/socanmatic-manual.html
endif

dist_doc_DATA = $(DOCBOOK_HTML) $(MAN_HTML)


maintainer-clean-local:
	-rm -rf *.1 *.html man/*.html man/*.1
