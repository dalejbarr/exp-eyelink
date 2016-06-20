OBJS = 	Experiment.o Template.o State.o \
  EXPConfig.o \
	ItemCell.o Trial.o MyDisplay.o Device.o Display_SDL.o \
	Audio.o Audio_SDL.o \
	InputDev.o Mouse_SDL.o \
	Webcam2.o \
	Stimulus.o StimulusDisplayOn.o StimulusImg.o \
	StimulusWav.o StimulusTxt.o \
	StimulusWebcam.o \
	Event.o EventGrabAOI.o EventSwapAOI.o EventUpdateAOI.o \
	EventUpdate.o EventRecord.o EventMsg.o StimulusShowAOI.o \
	EventRepeatIf.o EventResetCounter.o EventIncrementCounter.o \
  EventSocketSendMsg.o \
	Watch.o WatchKey.o WatchMouse.o WatchDone.o \
	WatchGamePadButton.o WatchGamePadMove.o \
	WatchSocketMsg.o WatchRemain.o \
	AlsaSoundIn.o \
	SoundInput.o RtAudio.o \
  Socket.o SocketListener.o \
	StartupInfo.o pastestr.o \
	Operation.o \
	Recordset.o RecordsetMySQL.o RecordsetSqlite.o helpers.o \
	EventAsync.o EventAsyncCtrl.o EventAnimation.o \
	EventClear.o \
	RespObj.o RespData.o \
	GamePad_SDL.o GPEvent.o ScrollTrackGP_SDL.o EventSTRecord.o \
	xy.o \
	EventLC1Display.o

W32L = win32/obj
CC = g++
W32CC = i586-mingw32msvc-g++
HDIR=$(shell cd;pwd)

W32OBJS = \
	$(W32L)/Experiment.o $(W32L)/Template.o $(W32L)/State.o \
	$(W32L)/ItemCell.o $(W32L)/Trial.o $(W32L)/MyDisplay.o $(W32L)/Device.o $(W32L)/Display_SDL.o \
	$(W32L)/Audio.o $(W32L)/Audio_SDL.o \
	$(W32L)/InputDev.o $(W32L)/Mouse_SDL.o \
	$(W32L)/Webcam2.o \
	$(W32L)/Stimulus.o $(W32L)/StimulusDisplayOn.o $(W32L)/StimulusImg.o \
	$(W32L)/StimulusWav.o $(W32L)/StimulusTxt.o \
	$(W32L)/StimulusWebcam.o \
	$(W32L)/Event.o $(W32L)/EventGrabAOI.o $(W32L)/EventSwapAOI.o $(W32L)/EventUpdateAOI.o \
	$(W32L)/EventUpdate.o $(W32L)/EventRecord.o $(W32L)/EventMsg.o $(W32L)/StimulusShowAOI.o \
	$(W32L)/EventRepeatIf.o $(W32L)/EventResetCounter.o $(W32L)/EventIncrementCounter.o \
	$(W32L)/EventSocketSendMsg.o \
	$(W32L)/Watch.o $(W32L)/WatchKey.o $(W32L)/WatchMouse.o $(W32L)/WatchDone.o \
	$(W32L)/WatchGamePadButton.o $(W32L).WatchGamePadMove.o \
	$(W32L)/WatchSocketMsg.o $(W32L)/WatchRemain.o \
	$(W32L)/PASoundIn.o \
  $(W32L)/Socket.o $(W32L)/SocketListener.o \
	$(W32L)/StartupInfo.o $(W32L)/pastestr.o \
	$(W32L)/Operation.o \
	$(W32L)/Recordset.o $(W32L)/RecordsetSqlite.o $(W32L)/RecordsetMySQL.o $(W32L)/helpers.o \
	$(W32L)/EventAsync.o $(W32L)/EventAsyncCtrl.o $(W32L)/EventAnimation.o \
	$(W32L)/EventClear.o \
	$(W32L)/RespObj.o $(W32L)/RespData.o \
	$(W32L)/GamePad_SDL.o $(W32L)/GPEvent.o $(W32L)/ScrollTrackGP_SDL.o $(W32L)/EventSTRecord.o \
	$(W32L)/xy.o \
	$(W32L)/EventLC1Display.o

CFLAGS = -c -I. 

W32CF = -c -I. -I/home/daleb/src/boost -D WIN32 $(shell /opt/SDL-win32/bin/sdl-config --cflags)
#-I/usr/i586-mingw32msvc/include/SDL -I/home/daleb/src/boost -D WIN32
#W32LF = -L/usr/i586-mingw32msvc/lib -lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_mixer
W32LF = $(shell /opt/SDL-win32/bin/sdl-config --libs) -lSDL_mixer -lSDL_ttf

#MYFLAGS = -I/usr/include/mysql

W32MYF = -Iwin32/mysql/include

MYFLAGS = -I/usr/include/mysql
MYLINK=-L/usr/lib/mysql -lmysqlclient 
SDL_CFLAGS := $(shell sdl-config --cflags)
RTAFLAGS = -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
#SDL_LDFLAGS := $(shell sdl-config --libs) -lSDL_ttf -lSDL_mixer
#SDL_LDFLAGS := $(shell sdl-config --libs) -lSDL_mixer -lSDL_ttf
SDL_LDFLAGS = $(shell sdl-config --libs) -lSDL -lpthread -lSDL_image -lSDL_mixer -lSDL_ttf -lX11 -ljpeg
ELINK_CFLAGS := -I$(HDIR)/include
#ELINK_LDFLAGS := -L$(HDIR)/libs -leyelink_core -leyelink_core_graphics  -lsdl_util `sdl-config --libs`   -lSDL_image -lSDL_mixer -lrt -lSDL_ttf -lSDL_gfx -lSDL_image
ELINK_LDFLAGS := -L$(HDIR)/libs -leyelink_core -leyelink_core_graphics  -lsdl_util `sdl-config --libs`   -lSDL_image -lSDL_mixer -lrt -lSDL_gfx -lSDL_image -lSDL_ttf

sblib-debug.a : $(OBJS) ReportErrorDebug.o sqlite3.o global.hpp
	ar -rv sblib-debug.a $(OBJS) sqlite3.o ReportErrorDebug.o

sblib.a : $(OBJS) ReportError.o sqlite3.o global.hpp
	ar -rv sblib.a $(OBJS) sqlite3.o ReportError.o

main.o : main.cpp picture.h
	$(CC) $(CFLAGS) $(ELINK_CFLAGS) $(SDL_CFLAGS) main.cpp

pp3main.o : pp3main.cpp picture.h
	$(CC) $(CFLAGS) $(ELINK_CFLAGS) $(SDL_CFLAGS) pp3main.cpp

exp2 : $(OBJS) sblib.a expshell2.o ClockFn.o StimulusEyeLinkMsg-notrack.o
	$(CC) $(RTAFLAGS) -o exp2 expshell2.o sblib.a ClockFn.o StimulusEyeLinkMsg-notrack.o $(SDL_LDFLAGS) -ldl -lrt -lpthread -lasound $(MYLINK) 

expdebug2 : sblib-debug.a expshell2.o ClockFn.o StimulusEyeLinkMsg-notrack.o
	$(CC) $(RTAFLAGS) -o expdebug2 expshell2.o sblib-debug.a ClockFn.o StimulusEyeLinkMsg-notrack.o $(SDL_LDFLAGS) -ldl -lrt -lpthread -lasound $(MYLINK) 

install : exp2 expdebug2
	cp exp2 ../bin/exp2; cp expdebug2 ../bin/expdebug2

expdebug2pp3 : sblib-debug.a pp3main.o ClockFn.o StimulusEyeLinkMsg-notrack.o
	$(CC) -o expdebug2 pp3main.o sblib-debug.a ClockFn.o StimulusEyeLinkMsg-notrack.o $(SDL_LDFLAGS) -ldl -lrt -lpthread -lasound $(MYLINK)

expeyelink : sblib.a main.o ClockFn-eyelink.o StimulusEyeLinkMsg.o
	$(CC) -o expeyelink main.o sblib.a ClockFn-eyelink.o StimulusEyeLinkMsg.o $(MYLINK) $(ELINK_LDFLAGS) -ldl -pthread -lrt -lpthread -lasound  

expeyelinkpp3 : sblib.a pp3main.o ClockFn-eyelink.o StimulusEyeLinkMsg.o
	$(CC) -o expeyelinkpp3 pp3main.o sblib.a ClockFn-eyelink.o StimulusEyeLinkMsg.o $(MYLINK) $(ELINK_LDFLAGS) -ldl -pthread -lrt -lpthread -lasound  

expeyelinkdebug : sblib-debug.a main.o ClockFn-eyelink.o StimulusEyeLinkMsg.o
	$(CC) -o expeyelinkdebug main.o sblib-debug.a ClockFn-eyelink.o StimulusEyeLinkMsg.o $(MYLINK) $(ELINK_LDFLAGS) -ldl -pthread -lrt -lpthread -lasound 

expeyelinkpp3debug : sblib-debug.a pp3main.o ClockFn-eyelink.o StimulusEyeLinkMsg.o
	$(CC) -o expeyelinkpp3debug pp3main.o sblib-debug.a ClockFn-eyelink.o StimulusEyeLinkMsg.o $(MYLINK) $(ELINK_LDFLAGS) -ldl -pthread -lrt -lpthread -lasound 

assigncontroller : assigncontroller.o sblib-debug.a ClockFn.o
	$(CC) -ldl -pthread $(MYLINK) $(SDL_LDFLAGS) -o assigncontroller assigncontroller.o sblib-debug.a ClockFn.o

assigncontroller.o : assigncontroller.cpp global.hpp
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(MYFLAGS) $<

RecordsetMySQL.o : RecordsetMySQL.cpp RecordsetMySQL.hpp
	$(CC) $(CFLAGS) $(MYFLAGS) $<

StimulusBmp.o : StimulusBmp.cpp StimulusBmp.hpp Stimulus.hpp Stimulus.cpp
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $<

Experiment.o : Experiment.cpp Experiment.hpp
	$(CC) $(CFLAGS) $(MYFLAGS) $(BOOST) $(SDL_CFLAGS) $<

expshell.o : expshell.cc
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(MYFLAGS) $<

expshell2.o : expshell2.cc
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(MYFLAGS) $<

sqlite3.o : sqlite3.c sqlite3.h
	gcc $(CFLAGS) $<

ReportError.o : ReportError.cpp ReportError.hpp
	g++ $(CFLAGS) ReportError.cpp

ReportErrorDebug.o : ReportError.cpp ReportError.hpp
	g++ -o ReportErrorDebug.o $(CFLAGS) -D DEBUG_MODE ReportError.cpp

ClockFn.o : ClockFn.cpp global.hpp
	g++ $(CFLAGS) $(SDL_CFLAGS) ClockFn.cpp

ClockFn-eyelink.o : ClockFn.cpp global.hpp
	g++ $(CFLAGS) $(SDL_CFLAGS) $(ELINK_CFLAGS) -DEXP_EYELINK_VERS -o ClockFn-eyelink.o ClockFn.cpp

StimulusEyeLinkMsg-notrack.o : StimulusEyeLinkMsg.cpp StimulusEyeLinkMsg.hpp Template.hpp global.hpp
	g++ $(CFLAGS) $(SDL_CFLAGS) -o StimulusEyeLinkMsg-notrack.o StimulusEyeLinkMsg.cpp

StimulusEyeLinkMsg.o : StimulusEyeLinkMsg.cpp StimulusEyeLinkMsg.hpp Template.hpp global.hpp
	g++ $(CFLAGS) $(SDL_CFLAGS) $(ELINK_CFLAGS) -DEXP_EYELINK_VERS -o StimulusEyeLinkMsg.o StimulusEyeLinkMsg.cpp

RtAudio.o : RtAudio.cpp RtAudio.h
	g++ $(CFLAGS) $(RTAFLAGS) -O2 -Wall -Wextra -fPIC -o RtAudio.o RtAudio.cpp

%.o : %.cpp %.hpp
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $<

# win 32

$(W32L)/%.o : %.cpp %.hpp
	$(W32CC) $(W32CF) -o $@ $<

win32/obj/helpers.o : helpers.cpp global.hpp
	$(W32CC) $(W32CF) -o $@ $<

win32/obj/assigncontroller.o : assigncontroller.cpp global.hpp
	$(W32CC) $(W32CF) $(W32MYF) -o $@ $<

win32/obj/ClockFn.o : ClockFn.cpp global.hpp
	$(W32CC) $(W32CF) -o $@ ClockFn.cpp

win32/obj/sqlite3.o : sqlite3.c sqlite3.h
	i586-mingw32msvc-gcc $(CFLAGS) -o win32/obj/sqlite3.o sqlite3.c

win32/obj/Experiment.o : Experiment.cpp Experiment.hpp
	$(W32CC) $(W32CF) $(BOOST) -o $@ $<

win32/obj/RecordsetMySQL.o : RecordsetMySQL.hpp RecordsetMySQL.cpp
	$(W32CC) $(W32CF) $(W32MYF) $(BOOST) -o $@ RecordsetMySQL.cpp

win32/obj/ReportErrorDebug.o : ReportError.cpp ReportError.hpp
	$(W32CC) $(W32CF) -o win32/obj/ReportErrorDebug.o -D DEBUG_MODE ReportError.cpp

win32/sblib-w32d.a : $(W32OBJS) $(W32L)/ReportErrorDebug.o $(W32L)/sqlite3.o global.hpp
	i586-mingw32msvc-ar -csvq win32/sblib-w32d.a $(W32OBJS) $(W32L)/ReportErrorDebug.o $(W32L)/sqlite3.o

win32/sblib-w32.a : $(W32OBJS) $(W32L)/ReportError.o $(W32L)/sqlite3.o global.hpp
	i586-mingw32msvc-ar -csvq win32/sblib-w32.a $(W32OBJS) $(W32L)/ReportError.o $(W32L)/sqlite3.o

win32/obj/expshell.o : expshell.cc
	$(W32CC) $(W32CF) $(W32MYF) -o $@ $<

win32/obj/expshell2.o : expshell2.cc
	$(W32CC) $(W32CF) $(W32MYF) -o $@ $<

win32/stimbot.exe : win32/sblib-w32.a win32/obj/expshell.o win32/obj/ClockFn.o
	$(W32CC) -o win32/stimbot.exe \
	win32/obj/expshell.o \
	win32/sblib-w32.a \
	$(W32LF) 

win32/expdebug.exe : win32/sblib-w32d.a win32/obj/expshell2.o win32/obj/ClockFn.o
	$(W32CC) -o win32/expdebug.exe \
	win32/obj/expshell2.o \
	win32/obj/ClockFn.o \
	win32/sblib-w32.a \
	$(W32LF) 

win32/exp.exe : win32/sblib-w32.a win32/obj/expshell2.o win32/obj/ClockFn.o
	$(W32CC) -o win32/exp.exe \
	win32/obj/expshell2.o \
	win32/obj/ClockFn.o \
	win32/sblib-w32.a \
	$(W32LF) 

win32/actrl.exe : win32/sblib-w32d.a win32/obj/assigncontroller.o win32/obj/ClockFn.o
	$(W32CC) -o win32/actrl.exe \
	win32/obj/assigncontroller.o \
	win32/obj/ClockFn.o \
	win32/sblib-w32d.a \
	$(W32LF) 

clean :
	rm -f $(OBJS) main.o ReportError.o ReportErrorDebug.o *~ exp expdebug expdebug2 assigncontroller sblib.a sblib-debug.a StimulusEyeLinkMsg.o StimulusEyeLinkMsg-notrack.o

clean32 :
	rm -f $(W32L)/*.o $(W32L)/sblib-w32.a $(W32L)/sblib-w32d.a
