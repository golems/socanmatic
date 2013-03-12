ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS =

include_HEADERS = include/socia.h

lib_LTLIBRARIES = libsocia.la libsocia_socketcan.la
m4_dnl bin_PROGRAMS = esdcantool

libsocia_la_SOURCES = src/canopen.c

libsocia_socketcan_la_SOURCES = src/socketcan.c

