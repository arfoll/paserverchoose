AC_PREREQ(2.57)
AC_INIT([paserverchoose],[0.9.4],[brendan (at) fridu (dot) net])
AC_CONFIG_SRCDIR([src/paserverchoose.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign 1.9 -Wall])

AC_SUBST(PACKAGE_URL, [http://www.madeo.co.uk])

if type -p stow > /dev/null && test -d /usr/local/stow ; then
   AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
   ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
AC_TYPE_SIGNAL
AC_HEADER_STDC

PKG_CHECK_MODULES(GUILIBS, [ gtk+-2.0 >= 2.10 libnotify libglade-2.0 gconf-2.0 libgnomeui-2.0 gnome-desktop-2.0])
AC_SUBST(GUILIBS_CFLAGS)
AC_SUBST(GUILIBS_LIBS)

if test -d ../pulseaudio ; then
   PULSE_CFLAGS='-I$(top_srcdir)/../pulseaudio/src'
   PULSE_LIBS='-L$(top_srcdir)/../pulseaudio/src/.libs -lpulse -lpulse-mainloop-glib -lpulse-browse'
   echo "*** Found pulseaudio in ../pulseaudio, using that version ***"
else
   PKG_CHECK_MODULES(PULSE, [ libpulse >= 0.9.2 libpulse-browse >= 0.9.2 libpulse-mainloop-glib >= 0.9.2 ])
fi

AC_SUBST(PULSE_LIBS)
AC_SUBST(PULSE_CFLAGS)

# If using GCC specifiy some additional parameters
if test "x$GCC" = "xyes" ; then
   CFLAGS="$CFLAGS -pipe -Wall -W -Wno-unused-parameter"
   CXXFLAGS="$CXXFLAGS -pipe -Wall -W"
fi

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
