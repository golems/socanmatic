ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . doc


EXTRA_DIST = eds/dsp301.eds eds/dsp402.eds eds/pdo-template.eds

AM_CPPFLAGS = -I$(top_srcdir)/include

TESTS = test_sdo

include_HEADERS = include/socanmatic.h
pkginclude_HEADERS = 	                     \
	include/socanmatic/byteorder.h       \
	include/socanmatic/status.h          \
	include/socanmatic/descriptor.h      \
	include/socanmatic/nmt.h             \
	include/socanmatic/eds.h             \
	include/socanmatic/sdo.h             \
	include/socanmatic/dict.h            \
	include/socanmatic/dict_fun.h        \
	include/socanmatic/pdo.h             \
	include/socanmatic/probe.h           \
	include/socanmatic/iface.h           \
	socanmatic/enum301.h                 \
	socanmatic/enum402.h                 \
	include/socanmatic/ds301.h           \
	include/socanmatic/emcy.h            \
	include/socanmatic/ds402.h

noinst_HEADERS = include/socanmatic_private.h

bin_PROGRAMS = canmat
dist_bin_SCRIPTS = canmatc
noinst_PROGRAMS = test_sdo

lib_LTLIBRARIES = libsocanmatic.la
libsocanmatic_la_SOURCES =                   \
	src/sdo.c                            \
	src/ds301.c                          \
	src/error.c                          \
	src/dict.c                           \
	src/util.c                           \
	src/iface/iface.c                    \
	src/probe.c                          \
	src/pdo.c                            \
	src/nmt.c
libsocanmatic_la_LIBADD = -ldl


# Profiles
lib_LTLIBRARIES += libsocanmatic402.la
libsocanmatic402_la_SOURCES = dict402.c src/ds402.c



# Interfaces
lib_LTLIBRARIES += libsocanmatic_iface_socketcan.la
libsocanmatic_iface_socketcan_la_SOURCES = src/iface/iface_socketcan.c
libsocanmatic_iface_socketcan_la_LDFLAGS = -shared

if HAVE_NTCAN
lib_LTLIBRARIES += libsocanmatic_iface_ntcan.la
libsocanmatic_iface_ntcan_la_SOURCES = src/iface/iface_ntcan.c
libsocanmatic_iface_ntcan_la_LIBADD = -lntcan
libsocanmatic_iface_ntcan_la_LDFLAGS = -shared
endif

test_sdo_SOURCES = src/test_sdo.c
test_sdo_LDADD = libsocanmatic.la  libsocanmatic402.la

canmat_SOURCES = src/canmat.c src/display.c
canmat_LDADD = libsocanmatic.la libsocanmatic402.la


if HAVE_SNS
bin_PROGRAMS += can402
can402_SOURCES = src/can402.c
can402_LDADD = libsocanmatic.la libsocanmatic402.la -lsns -lamino -lach -llapack -lblas
endif



BUILT_SOURCES =                              \
	dict402.c                            \
	socanmatic/dict402.h                 \
	socanmatic/enum301.h                 \
	socanmatic/enum402.h


socanmatic/dict402.h dict402.c: $(top_srcdir)/canmatc $(top_srcdir)/eds/dsp301.eds $(top_srcdir)/eds/dsp402.eds pdo.eds
	@MKDIR_P@ socanmatic
	$(top_srcdir)/canmatc --exclude-enum -n canmat_dict402 -N canmat_402 \
		-o dict402.c --header socanmatic/dict402.h                   \
		$(top_srcdir)/eds/dsp301.eds pdo.eds $(top_srcdir)/eds/dsp402.eds

socanmatic/enum301.h: $(top_srcdir)/canmatc $(top_srcdir)/eds/dsp301.eds
	@MKDIR_P@ socanmatic
	$(top_srcdir)/canmatc --exclude-dictionary -o enum301.c --header socanmatic/enum301.h \
		 $(top_srcdir)/eds/dsp301.eds

socanmatic/enum402.h: $(top_srcdir)/canmatc $(top_srcdir)/eds/dsp402.eds
	@MKDIR_P@ socanmatic
	$(top_srcdir)/canmatc --exclude-dictionary -N canmat_402_ -o enum402.c --header socanmatic/enum402.h \
		 $(top_srcdir)/eds/dsp402.eds

pdo.eds: $(top_srcdir)/eds/pdo-template.eds
	rm -f pdo.eds
	i=0; while [ $$i -lt 10 ]; do \
		sed  $(top_srcdir)/eds/pdo-template.eds -e s/XX/`printf %02x $$i`/ -e '/^;.*/d' >> pdo.eds; \
		i=$$((i+1)); \
	done


lib_LTLIBRARIES += libsocanmatic_prof_mfr_schunk.la
libsocanmatic_prof_mfr_schunk_la_SOURCES = mfr_schunk.c

socanmatic/mfr_schunk.h mfr_schunk.c: $(top_srcdir)/canmatc $(top_srcdir)/eds/dsp301.eds $(top_srcdir)/eds/dsp402.eds pdo.eds $(top_srcdir)/eds/mfr/schunk.eds
	@MKDIR_P@ socanmatic
	$(top_srcdir)/canmatc --exclude-enum -n canmat_dict402 -N canmat_402 \
		-o mfr_schunk.c --header socanmatic/mfr_schunk.h             \
		$(top_srcdir)/eds/dsp301.eds pdo.eds $(top_srcdir)/eds/dsp402.eds $(top_srcdir)/eds/mfr/schunk.eds


distclean-local:
	-rm -rf pdo.eds socanmatic/dict402.h dict402.c

#clean-local:
#rm -f dict402.c
