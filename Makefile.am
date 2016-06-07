ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = $(EXTRA_CPPFLAGS)
AM_LDFLAGS = $(EXTRA_LIBS) $(EXTRA_LDFLAGS)

CCORE_SHARED = src/cc_error.c
CCORE_SHARED += src/cc_event.c

if BUILD_LINUX
CCORE_SHARED += src/linux/cc_linux_file.c
CCORE_SHARED += src/linux/cc_linux_thread.c
CCORE_SHARED += src/linux/cc_linux_time.c
endif
if BUILD_X11
CCORE_SHARED += src/x11/cc_x11_display.c
CCORE_SHARED += src/x11/cc_x11_opengl.c
CCORE_SHARED += src/x11/cc_x11_window.c
endif

pkginclude_HEADERS = include/*.h

# Create a common library so the executables are real executables
noinst_LTLIBRARIES = libcommon.la
libcommon_la_LDFLAGS = $(EXTRA_LDFLAGS) -no-undefined -module -avoid-version
libcommon_la_LIBADD = $(EXTRA_LIBS)
libcommon_la_SOURCES = $(CCORE_SHARED)

lib_LTLIBRARIES = libccore.la
libccore_la_LDFLAGS = $(EXTRA_LDFLAGS) -version-info $(CCORE_VERSION)
libccore_la_LIBADD = $(EXTRA_LIBS)
libccore_la_SOURCES = $(CCORE_SHARED)

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
