PACKAGE=r

R_MAJOR_VERSION=0
R_MINOR_VERSION=1

VERSION=$R_MAJOR_VERSION.$R_MINOR_VERSION

AC_INIT([r], [0.1], [danilov@gmail.com])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AH_TEMPLATE([PACKAGE], [Package name.])
AH_TEMPLATE([VERSION], [Version of the package.])
AH_TEMPLATE([ENABLE_DEBUG], [Enable assertion and invariant checking.])

AM_INIT_AUTOMAKE([$PACKAGE], [$VERSION])

LT_RELEASE=$VERSION
AC_SUBST([LT_RELEASE])
AC_PROG_CC

SRCDIR=$PWD
AC_SUBST([SRCDIR])
AC_TYPE_SIZE_T
AC_FUNC_MALLOC

AC_ARG_ENABLE([debug],
        [AC_HELP_STRING([--disable-debug], 
	[disable assertions and invariants])], [],
        [enable_debug=yes])

if test x$enable_debug = xyes; then
        CFLAGS="-O0 -g"
        AC_DEFINE([ENABLE_DEBUG])
fi

CFLAGS="-Wall -Werror -D_REENTRANT -fno-strict-aliasing $CFLAGS $USER_CFLAGS"
LDFLAGS="-rdynamic"
# trim spaces at the end of flags
CFLAGS=$(echo $CFLAGS | sed 's/[[:blank:]]*$//')
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo ""
echo "CFLAGS :  \"$CFLAGS\""
echo "LDFLAGS:  \"$LDFLAGS\""
echo ""

echo "Type 'make' to build r"

