desktopdir = $(datadir)/applications

bin_PROGRAMS=paserverchoose

paserverchoose_SOURCES=x11prop.c avahibrowse.c paserverchoose.c
noinst_HEADERS=x11prop.h avahibrowse.h

paserverchoose_LDADD=$(AM_LDADD) $(PASC_LIBS)
paserverchoose_CFLAGS=$(AM_CFLAGS) $(PASC_CFLAGS) 

