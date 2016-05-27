ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = $(EXTRA_CPPFLAGS)
AM_LDFLAGS = $(EXTRA_LIBS) $(EXTRA_LDFLAGS)

pkginclude_HEADERS = include/ccore.h include/cc_error.h include/cc_system.h include/cc_window.h

# Create a common library so the executables are real executables
noinst_LTLIBRARIES = libcommon.la
libcommon_la_LDFLAGS = $(EXTRA_LDFLAGS) -no-undefined
libcommon_la_LIBADD = $(EXTRA_LIBS)
libcommon_la_SOURCES = src/cc_error.c src/cc_event.c

if BUILD_LINUX
endif
if BUILD_X11
libcommon_la_SOURCES += src/x11/cc_x11_window.c
endif

lib_LTLIBRARIES = libccore.la
libccore_la_LDFLAGS = $(EXTRA_LDFLAGS) -version-info $(CCORE_VERSION) -no-undefined
libccore_la_LIBADD = $(EXTRA_LIBS)
libccore_la_SOURCES = src/cc_error.c

if BUILD_TESTS
check_PROGRAMS = check_ccore
check_ccore_SOURCES = test/test.c
check_ccore_LDADD = @CHECK_LIBS@ libccore.la

TESTS = check_ccore
endif

bin_PROGRAMS = bin/window

bin_window_SOURCES = examples/window.c
bin_window_LDADD = libcommon.la
