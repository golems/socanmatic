#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([socanmatic], [0.0.1], [ntd@gatech.edu])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([include/socanmatic.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_LIBTOOL

AC_C_RESTRICT
AC_C_INLINE
AC_C_CONST

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h stdint.h stdlib.h string.h sys/socket.h syslog.h unistd.h time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(uint64_t)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(unsigned long long)

# Check 32/64 safe format specifiers
AC_CHECK_DECLS([PRIuPTR,PRIu64,PRIx64])
AC_CHECK_DECL([PRIuPTR],
        [],
        [AC_DEFINE([PRIuPTR], ["ul"], [Assume this is unsigned long])])

# Checks for library functions.
AC_SEARCH_LIBS([clock_gettime],[rt])

# Look for ntcan library
AC_CHECK_HEADER([ntcan.h], [FOUND_NTCAN=yes])
AM_CONDITIONAL([HAVE_NTCAN], [test x$FOUND_NTCAN = xyes])


# Look for sns library
AC_CHECK_HEADER([sns.h], [FOUND_SNS=yes])
AM_CONDITIONAL([HAVE_SNS], [test x$FOUND_SNS = xyes])

# Enable maximum warnings
AX_CFLAGS_WARN_ALL

dnl The AX_CHECK_COMPILER_FLAGS maco was renamed to AX_CHECK_COMPILE_FLAG
dnl Checking for both versions
m4_ifdef([AX_CHECK_COMPILE_FLAG],
         [AC_DEFUN([APPEND_FLAG],
                   [AX_CHECK_COMPILE_FLAG([$1], [CFLAGS="$1 $CFLAGS"])])])

m4_ifdef([AX_CHECK_COMPILER_FLAGS],
         [AC_DEFUN([APPEND_FLAG],
                   [AX_CHECK_COMPILER_FLAGS([$1], [CFLAGS="$1 $CFLAGS"])])])

dnl If we found the flag checking macro, check some flags
m4_ifdef([APPEND_FLAG],
         [APPEND_FLAG([-Wextra])
          APPEND_FLAG([-Wconversion])
          APPEND_FLAG([-Wpointer-arith])
          APPEND_FLAG([-Wfloat-equal])
          APPEND_FLAG([-Wshadow])
          APPEND_FLAG([-Wwrite-strings])
          APPEND_FLAG([-Wc++-compat])])

# Doxygen
DX_HTML_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(ach)


# Check for programs to make documentation
AC_CHECK_PROGS([HELP2MAN], [help2man])
AC_CHECK_PROGS([MAN2HTML], [man2html])
AC_CHECK_PROGS([DB2HTML], [db2html])
AM_CONDITIONAL([HAVE_HELP2MAN], [test x$HELP2MAN != x])
AM_CONDITIONAL([HAVE_MAN2HTML], [test x$MAN2HTML != x])
AM_CONDITIONAL([HAVE_DB2HTML],  [test x$DB2HTML != x])

# Check if we already have docs (from tarball)
AC_CHECK_FILE( [$srcdir/doc/man/socanmatic.1],              [FOUND_MANPAGES=yes])
AC_CHECK_FILE( [$srcdir/doc/man/socanmatic.html],           [FOUND_MANHTML=yes])
AC_CHECK_FILE( [$srcdir/doc/manual/socanmatic-manual.html], [FOUND_MANUAL=yes])

# If we can make the docs or already have them, then we can install them
AM_CONDITIONAL([HAVE_MANPAGES], [test x$HELP2MAN != x -o x$FOUND_MANPAGES = xyes])
AM_CONDITIONAL([HAVE_MANHTML],  [test '(' x$HELP2MAN != x -a x$MAN2HTML != x ')' -o x$FOUND_MANHTML = xyes])
AM_CONDITIONAL([HAVE_MANUAL],   [test x$DB2HTML != x -o x$FOUND_MANUAL = xyes])


AC_CONFIG_FILES([Makefile doc/Makefile])
AC_OUTPUT
