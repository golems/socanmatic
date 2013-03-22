ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . doc

AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS = test_sdo

include_HEADERS = include/socanmatic.h
pkginclude_HEADERS = 	                     \
	include/socanmatic/byteorder.h       \
	include/socanmatic/status.h          \
	include/socanmatic/nmt.h             \
	include/socanmatic/sdo.h             \
	include/socanmatic/dict.h            \
	include/socanmatic/ds402.h

noinst_HEADERS = include/socanmatic_private.h

lib_LTLIBRARIES = libsocanmatic.la libsocanmatic_socketcan.la libsocanmatic402.la
m4_dnl bin_PROGRAMS = esdcantool

bin_PROGRAMS = canmat
noinst_PROGRAMS = test_sdo

libsocanmatic_la_SOURCES = src/sdo.c src/ds301.c src/error.c src/dict.c

libsocanmatic_socketcan_la_SOURCES = src/socketcan.c

libsocanmatic402_la_SOURCES = dict402.c

test_sdo_SOURCES = src/test_sdo.c
test_sdo_LDADD = libsocanmatic.la libsocanmatic_socketcan.la libsocanmatic402.la

canmat_SOURCES = src/canmat.c
canmat_LDADD = libsocanmatic.la libsocanmatic_socketcan.la libsocanmatic402.la

dict402.c: $(top_srcdir)/canmatc $(top_srcdir)/eds/dsp301.eds $(top_srcdir)/eds/dsp402.eds
	$(top_srcdir)/canmatc -n canmat_dict402 -o dict402.c $(top_srcdir)/eds/dsp301.eds $(top_srcdir)/eds/dsp402.eds

clean-local:
	rm -f dict402.c
