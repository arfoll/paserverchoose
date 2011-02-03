desktopdir = $(datadir)/applications

bin_PROGRAMS=paserverchoose

paserverchoose_SOURCES=paserverchoose.c x11prop.c x11prop.h avahibrowse.c avahibrowse.h

paserverchoose_LDADD=$(AM_LDADD) $(PULSE_LIBS)
paserverchoose_CFLAGS=$(AM_CFLAGS) $(PULSE_CFLAGS) 

