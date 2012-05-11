ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS =

include_HEADERS = ntcanopen.h

lib_LTLIBRARIES = libntcanopen.la
bin_PROGRAMS = esdcantool

libntcanopen_la_SOURCES = ntcanopen.c

esdcantool_SOURCES = esdcantool.c
esdcantool_LDADD = libntcanopen.la
