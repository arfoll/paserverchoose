desktopdir = $(datadir)/applications

bin_PROGRAMS=paserverchoose

paserverchoose_SOURCES=paserverchoose.c x11prop.c x11prop.h

paserverchoose_LDADD=$(AM_LDADD) $(GUILIBS_LIBS) $(PULSE_LIBS)
paserverchoose_CFLAGS=$(AM_CFLAGS) $(GUILIBS_CFLAGS) $(PULSE_CFLAGS) 

