CC       = g++
DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__ 
# DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_PULSE__ 
CFLAGS   = -O2 -Wall -Wextra
# CFLAGS  += -I$(INCLUDE) -I../include
CFLAGS  += -I$(INCLUDE) 
# LIBRARY  = -lpthread -lpulse-simple -lpulse
LIBRARY  = -lpthread -lasound

clean : 
	rm *.o audioprobe

RtAudio.o : RtAudio.cpp
	$(CC) $(CFLAGS) $(DEFS) -c -o RtAudio.o RtAudio.cpp

audioprobe : audioprobe.cpp RtAudio.o
	$(CC) $(CFLAGS) $(DEFS) -o audioprobe audioprobe.cpp RtAudio.o $(LIBRARY)
