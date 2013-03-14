ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS = test_sdo

include_HEADERS = include/socia.h
pkginclude_HEADERS = 	                \
	include/socia/byteorder.h       \
	include/socia/canopen.h         \
	include/socia/ds402.h           \
	include/socia/sdo.h

noinst_HEADERS = include/socia_private.h

lib_LTLIBRARIES = libsocia.la libsocia_socketcan.la
m4_dnl bin_PROGRAMS = esdcantool

noinst_PROGRAMS = test_sdo

libsocia_la_SOURCES = src/canopen.c

libsocia_socketcan_la_SOURCES = src/socketcan.c

test_sdo_SOURCES = src/test_sdo.c
test_sdo_LDADD = libsocia.la libsocia_socketcan.la
