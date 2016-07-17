#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([ccore], m4_esyscmd_s([cat VERSION]))
AC_PREREQ([2.53])

# Setup m4 folder for libtool
AC_CONFIG_MACRO_DIR([m4])
# Setup build dir to get rid of all the scripts in the root folder
AC_CONFIG_AUX_DIR([build-scripts])

CCORE_VERSION="$(cat VERSION)"
AC_SUBST(CCORE_VERSION)

# Set the programming language to 'C'
AC_LANG_C

AM_INIT_AUTOMAKE([foreign subdir-objects nostdinc silent-rules -Wall -Werror])
# Show less verbose output
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([disable])
AM_PROG_AR

# Initialize libtool
LT_INIT()
#AC_ENABLE_STATIC
AC_ENABLE_SHARED

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_CANONICAL_HOST

# Checks for the test library
PKG_CHECK_MODULES([CHECK], [check >= 0.8.2], [have_check=yes], [have_check=no])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/types.h stdio.h stdlib.h stddef.h stdarg.h malloc.h memory.h string.h strings.h inttypes.h stdint.h ctype.h math.h iconv.h signal.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC 
AC_CHECK_FUNCS([clock_gettime memset select strdup sysinfo])

build_linux=no
build_x11=no
build_opengl=no

SUMMARY="Configuration summary\n"
SUMMARY="${SUMMARY}ccore version: ${CCORE_VERSION}\n"

EXTRA_LDFLAGS="$BASE_LDFLAGS"
EXTRA_LIBS="$BASE_LIBS"
EXTRA_CFLAGS="$BASE_CFLAGS"
EXTRA_CPPFLAGS="-I$srcdir/include"

CheckX11()
{
	AC_CHECK_LIB([X11], [XOpenDisplay], [], [echo "X11 dev library is required" && exit -1])
	AC_CHECK_LIB([Xrandr], [XRRGetScreenResources], [], [echo "XRandr dev library is required" && exit -1])
#	AC_CHECK_LIB([Xi], [XISelectEvents], [], [echo "XInput dev library is required" && exit -1])
#	AC_CHECK_LIB([Xext], [XShmCreateImage], [], [echo "Xshm (Xext) dev library is required" && exit -1])
	AC_PATH_X

	EXTRA_CFLAGS="${EXTRA_CFLAGS} $X_CFLAGS"
	EXTRA_LDFLAGS="${EXTRA_LDFLAGS} $X_LIBS"

	build_x11=yes
	SUMMARY="${SUMMARY}Using X11 to handle window and input functions\n"
}

CheckOpenGL()
{
	AC_CHECK_LIB([GL], [glXChooseVisual], [], [echo "OpenGL dev library is required" && exit -1])

	build_opengl=yes
	SUMMARY="${SUMMARY}Using OpenGL\n"
}

CheckGLEW()
{
	build_examples_glew=yes
	AC_CHECK_LIB([GLEW], [glewInit], [], [build_examples_glew=no])

	if test x$build_examples_glew = xyes; then
		PKG_CHECK_MODULES([GLEW], [glew])
		AC_CHECK_HEADERS(GL/glew.h)

		EXTRA_CFLAGS="${EXTRA_CFLAGS} $GLEW_CFLAGS"
		EXTRA_LIBS="${EXTRA_LIBS} $GLEW_LIBS"

		EXTRA_CPPFLAGS="${EXTRA_CPPFLAGS} -DUSE_GLEW"

		SUMMARY="${SUMMARY}Using GLEW for the OpenGL example\n"
	fi
}

CheckEpoxy()
{
	build_examples_epoxy=yes
	AC_CHECK_LIB([epoxy], [glDrawArrays], [], [build_examples_epoxy=no])

	if test x$build_examples_epoxy = xyes; then
		PKG_CHECK_MODULES([EPOXY], [epoxy])
		AC_CHECK_HEADERS(epoxy/gl.h)

		EXTRA_CFLAGS="${EXTRA_CFLAGS} $EPOXY_CFLAGS"
		EXTRA_LIBS="${EXTRA_LIBS} $EPOXY_LIBS"

		EXTRA_CPPFLAGS="${EXTRA_CPPFLAGS} -DUSE_EPOXY"

		SUMMARY="${SUMMARY}Using Epoxy for the OpenGL example\n"
	else
		CheckGLEW
	fi
}

# AC_CANONICAL_HOST is needed to acces the 'host_os' variable
SUMMARY="${SUMMARY}Target host: ${host_os}\n"
case "${host_os}" in
	linux*)
		# Check Linux headers & libraries
		AC_CHECK_HEADERS([sys/ioctl.h sys/param.h sys/types.h unistd.h])
		AC_CHECK_LIB([m], [cos], [], [echo "Math library is required" && exit -1])
		AC_CHECK_LIB([pthread], [pthread_create], [], [echo "Pthread dev library is required" && exit -1])

		CheckX11
		CheckOpenGL
		CheckEpoxy

		build_linux=yes
		;;
	*mingw32* | *cygwin* | *wince* |mingwce*)
    AC_CHECK_TOOL(WINDRES, windres, :)
    AC_CHECK_TOOL(OBJCOPY, objcopy, :) 
		
		case "${host_os}" in
      *mingw32*)
        host_sys=mingw32
        ;;
      *cygwin*)
        # Check if we are using the mno-cygwin mode in which case we are actually dealing with a mingw32 compiler
        AC_EGREP_CPP(yes,
            [#ifdef _WIN32
             yes
             #endif],
            host_sys=mingw32, AC_MSG_ERROR([ccore requires -mno-cygwin]))
        ;;
    esac

    if test x$host_sys = xmingw32; then
    	EXTRA_LDFLAGS="${EXTRA_LDFLAGS} -Wl,--nxcompat -Wl,--no-seh -Wl,--dynamicbase"
			EXTRA_LIBS="${EXTRA_LIBS} -lopengl32"
		fi

		build_windows=yes
		;;
	*)
		AC_MSG_ERROR(["OS $host_os is not supported"])
		;;
esac

# Pass the building booleans to Makefile.am
AM_CONDITIONAL([BUILD_LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([BUILD_X11], [test "$build_x11" = "yes"])
AM_CONDITIONAL([BUILD_WIN32], [test "$build_windows" = "yes"])
AM_CONDITIONAL([BUILD_OPENGL], [test "$build_opengl" = "yes"])

AM_CONDITIONAL([BUILD_TESTS], [test "$have_check" = "yes"])

if test x$have_check != xyes; then
	AC_DEFINE([HAVE_CHECK], [1], [Use check])
	SUMMARY="${SUMMARY}Not building tests (test/test.c) because the 'check' package could not be found\n"
	SUMMARY="${SUMMARY}Please install the check package to be able to run the tests\n"
fi

# Check if we want to build ccore with the debug flag
AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [compile with assertions and other debug options [default=auto]])], [enable_debug=$enableval],	[enable_debug=auto])
if test x$enable_debug = xyes; then
	EXTRA_CPPFLAGS="${EXTRA_CPPFLAGS} -DCC_DEBUG"
	SUMMARY="${SUMMARY}Building the library with debug functionality (#define CC_DEBUG)\n"
fi

AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(EXTRA_LIBS)
AC_SUBST(EXTRA_CFLAGS)
AC_SUBST(EXTRA_CPPFLAGS)

SUMMARY="${SUMMARY}Building the library with the following flags:\n"
SUMMARY="${SUMMARY}- LDFLAGS:\n\t${EXTRA_LDFLAGS}\n"
SUMMARY="${SUMMARY}- LIBS:\n\t${EXTRA_LIBS}\n"
SUMMARY="${SUMMARY}- CPPFLAGS:\n\t${EXTRA_CPPFLAGS}\n"
SUMMARY="${SUMMARY}- CFLAGS:\n\t${EXTRA_CFLAGS}\n"

# Check if the source files are in the right location
AC_CONFIG_SRCDIR([src/cc_error.c])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_LINKS([bin/icon.data:examples/icon.data])

# Display the output summary and show the ccore logo
AC_CONFIG_COMMANDS([summary], [cat logo.txt ; echo -en "$SUMMARY"], [SUMMARY="$SUMMARY"])

AC_OUTPUT()
