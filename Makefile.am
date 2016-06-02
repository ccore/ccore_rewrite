ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = $(EXTRA_CPPFLAGS)
AM_LDFLAGS = $(EXTRA_LIBS) $(EXTRA_LDFLAGS)

pkginclude_HEADERS = include/*.h

# Create a common library so the executables are real executables
noinst_LTLIBRARIES = libcommon.la
libcommon_la_LDFLAGS = $(EXTRA_LDFLAGS) -no-undefined
libcommon_la_LIBADD = $(EXTRA_LIBS)
libcommon_la_SOURCES = src/cc_error.c
libcommon_la_SOURCES += src/cc_event.c

if BUILD_LINUX
libcommon_la_SOURCES += src/linux/cc_linux_file.c
libcommon_la_SOURCES += src/linux/cc_linux_thread.c
endif
if BUILD_X11
libcommon_la_SOURCES += src/x11/cc_x11_display.c
libcommon_la_SOURCES += src/x11/cc_x11_opengl.c
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

bin_PROGRAMS = bin/display bin/event bin/mouse bin/opengl bin/thread bin/window_icon bin/window_simple

bin_display_SOURCES = examples/display.c
bin_display_LDADD = libcommon.la

bin_event_SOURCES = examples/event.c
bin_event_LDADD = libcommon.la

bin_mouse_SOURCES = examples/mouse.c
bin_mouse_LDADD = libcommon.la

bin_opengl_SOURCES = examples/opengl.c
bin_opengl_LDADD = libcommon.la

bin_thread_SOURCES = examples/thread.c
bin_thread_LDADD = libcommon.la

bin_window_simple_SOURCES = examples/window_simple.c
bin_window_simple_LDADD = libcommon.la

bin_window_icon_SOURCES = examples/window_icon.c
bin_window_icon_LDADD = libcommon.la
