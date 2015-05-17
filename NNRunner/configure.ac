dnl Process this file with autoconf to produce a configure script.
AC_INIT(NNRunner)

AC_DEFUN([CONFIG_FAILED],
[
  echo 
  echo 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
  echo 'NNRunner configuration failed with ERROR:'
  echo $1
  echo 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
  echo 
  AC_ERROR(Fix the problem and run the configuration again)
])

dnl keep track of current dir  -------------------------------------

NNR_HOME=`pwd`
AC_SUBST(NNR_HOME)

dnl C++ compiler

AC_PROG_CXX()
AC_SUBST(CXX)

dnl root -----------------------------------------------------------

AC_CHECK_FILE($ROOTSYS,,CONFIG_FAILED([No ROOTSYS defined]))
ROOT_CXXFLAGS=`root-config --cflags`
ROOT_LIBS=`root-config --glibs`
ROOT_LDFLAGS=`root-config --ldflags`

dnl BOOST -----------------------------------------------------------

dnl Fix for noncompatible system BOOST
AC_ARG_WITH(min_boost, 
            [  --with-min-boost           Minimal BOOST version to use. Can be used to exclude system one e.g.: SLC6 comes with BOOST 1.41 which does not compile with GCC 4.7. Default value is  1.41.0],
MIN_BOOST_VERSION=$with_min_boost,
MIN_BOOST_VERSION=1.41.0
)
AX_BOOST_BASE([$MIN_BOOST_VERSION],,CONFIG_FAILED([No BOOST found]))

dnl muParser

PKG_CHECK_MODULES([MUPARSER], [muparser >= 2.0.0])

dnl Consolidate flags

CXXFLAGS="$ROOT_CXXFLAGS $BOOST_CPPFLAGS $MUPARSER_CFLAGS"
LIBS="$ROOT_LIBS $MUPARSER_LIBS"
LDFLAGS="$ROOT_LDFLAGS $BOOST_LDFLAGS"
AC_SUBST(CXXFLAGS)
AC_SUBST(LIBS)
AC_SUBST(LDFLAGS)
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(MUPARSER_CFLAGS)

AC_CONFIG_FILES([Makefile setup.sh])
AC_OUTPUT

echo 
echo "configuration success. just do :"
echo "make"

