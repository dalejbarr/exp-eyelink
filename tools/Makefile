CC       = g++
DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__ 
# DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_PULSE__ 
CFLAGS   = -O2 -Wall -Wextra
# CFLAGS  += -I$(INCLUDE) -I../include
CFLAGS  += -I$(INCLUDE) 
# LIBRARY  = -lpthread -lpulse-simple -lpulse
LIBRARY  = -lpthread -lasound

audioprobe : audioprobe.cpp RtAudio.o
	$(CC) $(CFLAGS) $(DEFS) -o audioprobe audioprobe.cpp RtAudio.o $(LIBRARY)

RtAudio.o : RtAudio.cpp
	$(CC) $(CFLAGS) $(DEFS) -c -o RtAudio.o RtAudio.cpp

clean : 
	rm -f *.o audioprobe


