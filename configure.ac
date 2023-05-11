#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pngloader], [0.2.0.5], [dimao8@yandex.ru])
AC_CONFIG_SRCDIR([src/pngloader.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.10 foreign subdir-objects silent-rules])
AC_LANG([C])

LT_INIT([win32-dll])

LT_LANG([Windows Resource])

# Checks for programs.
CFLAGS=-std=gnu11
AC_PROG_CC([clang llwm-gcc gcc])

# Check endiannes
AC_C_BIGENDIAN

# Check zlib
ac_libs_save="$LIBS"
AC_CHECK_LIB([z], [uncompress], [], [AC_MSG_ERROR(*** There is no zlib development package installed!)])
AC_CHECK_HEADERS([zlib.h], [], [AC_MSG_ERROR(*** There is no zlib development package installed!)])
LIBS="$ac_libs_save"

# Check windres
AC_CHECK_PROG([ac_have_windres], [windres], [yes], [no])
AM_CONDITIONAL([HAVE_WINDRES], [test "x$ac_have_windres" = "xyes"])

# Enable docs
AC_ARG_ENABLE([docs],
              [AS_HELP_STRING([--disable-docs],[Disable if you do not want compile docs])])
AM_CONDITIONAL([ENABLE_DOCS], [test "x$enable_docs" = "xyes"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
