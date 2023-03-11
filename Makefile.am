ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libpngloader.la

libpngloader_la_SOURCES = src/pngloader.c \
	doc/pngloader.tex

if HAVE_WINDRES
libpngloader_la_SOURCES +=src/resources.rc
endif
libpngloader_la_CFLAGS=-I$(srcdir)/include
libpngloader_la_LIBADD=-lz
libpngloader_la_LDFLAGS=-no-undefined

info_TEXINFOS = doc/pngloader.texi
TEXI2DVI = TEXI2DVI_USE_RECORDER=no texi2dvi

if HAVE_WINDRES
.rc.lo :
	$(LIBTOOL) --tag=RC --mode=compile $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	$(RC) $(AM_RCFLAGS) $(RCFLAGS) $< -o $@
endif

include_HEADERS=include/pngloader.h

# Docs

doc_DATA = doc/pngloader.pdf

.tex.pdf :
	$(AM_V_GEN)$(PDFLATEX) -interaction=batchmode $< -o $@
	$(AM_V_GEN)$(PDFLATEX) -interaction=batchmode $< -o $@

CLEANFILES = *.log \
	*.toc \
	*.pdf \
	*.aux \
	*.idx \
	*.out

# Tests

TESTS = tests/t_version \
	tests/t_load8 \
	tests/t_load16 \
	tests/t_load24 \
	tests/t_load32 \
	tests/t_load16bpc \
	tests/t_chunk_error \
	tests/t_compress_error \
	tests/t_png_error \
	tests/t_empty_error \
	tests/t_file_error

XFAIL_TESTS = tests/t_chunk_error \
	tests/t_compress_error \
	tests/t_png_error \
	tests/t_empty_error \
	tests/t_file_error

check_PROGRAMS = tests/tpngloader8 \
  tests/tpngloader16 \
  tests/tpngloader24 \
  tests/tpngloader32 \
  tests/tpngloader_error

tests_tpngloader8_SOURCES = tests/tpngloader8.c
tests_tpngloader8_LDADD = libpngloader.la
tests_tpngloader8_CFLAGS = -I$(srcdir)/include

tests_tpngloader16_SOURCES = tests/tpngloader16.c
tests_tpngloader16_LDADD = libpngloader.la
tests_tpngloader16_CFLAGS = -I$(srcdir)/include

tests_tpngloader24_SOURCES = tests/tpngloader24.c
tests_tpngloader24_LDADD = libpngloader.la
tests_tpngloader24_CFLAGS = -I$(srcdir)/include

tests_tpngloader32_SOURCES = tests/tpngloader32.c
tests_tpngloader32_LDADD = libpngloader.la
tests_tpngloader32_CFLAGS = -I$(srcdir)/include

tests_tpngloader_error_SOURCES = tests/tpngloader_error.c
tests_tpngloader_error_LDADD = libpngloader.la
tests_tpngloader_error_CFLAGS = -I$(srcdir)/include
