CC       = g++
DEFS     =   -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
CFLAGS   = -O2 -Wall -Wextra
CFLAGS  += -I$(INCLUDE) -I../include
LIBRARY  = -lpthread -lasound 

audioprobe : audioprobe.cpp ../src/RtAudio.o
	$(CC) $(CFLAGS) $(DEFS) -o audioprobe audioprobe.cpp ../src/RtAudio.o $(LIBRARY)
