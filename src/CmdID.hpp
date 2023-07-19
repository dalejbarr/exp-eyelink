#ifndef SBX_CMDID_INCLUDED
#define SBX_CMDID_INCLUDED

#define SBX_EVENT_SHOW_BITMAP 1
#define SBX_EVENT_PLAY_SOUND 2
//#define HIGHLIGHTOFF 3
//#define HIGHLIGHTON 4
#define SBX_EVENT_CLEARREGION 5
//#define SBX_EVENT_MOVEAOI 6
//#define FLIPAOI 7
#define SBX_EVENT_SHOWTEXT 8
//#define FLIPREGION 9
#define SBX_EVENT_RECSOUND 10
//#define MOVIEPLAY 11
#define SBX_EVENT_DISPLAY_ON 12
//#define ISCAN_REC 13
//#define ISCAN_STOP 14
#define SBX_EVENT_MOUSE_REC 15
//#define MOUSE_STOP 16
//#define MOUSE_ECHO 17
//#define MOUSE_LISTENER 18
//#define SENDCMD 19
//#define BREAKAFTER 20
//#define DRAWBORDER 21
//#define GPFEEDBACK 22
//#define WHEEL 23
//#define GPCURSOR 24
//#define HIGHLIGHTGPAOI 25
//#define GRID 26
#define SBX_EVENT_SWAP_AOI 27
#define SBX_EVENT_SHOW_AOI 28
//#define SHOWAOI 28
//#define HIGHLIGHTAOI 29
//#define RECSOUND_STOP 30
//#define MOVIEPAUSE 31
//#define PLAYSOUND_STOP 32
//#define INCR_TCTR 33
//#define INCR_CCTR 34
//#define GIVE_FEEDBACK 35
//#define RESET_TCTR 36
//#define RESET_CCTR 37
//#define PLAYSOUNDRAND 38
//#define TEXTBOX 39
//#define PLAYSOUNDLOOPED 40
//#define KEY_REC 41
//#define STOREAOI 42
//#define MOUSEECHO 43
//#define LINKAOI 44
//#define REPLACEAOI 45
//#define SHOWBMPTRANS 46
//#define OVERLAYAOI 47
//#define NUMLOCKON 48
//#define ISCAN2_REC 49
//#define REPEAT_TRIAL 50
#define SBX_EVENT_GRAB_AOI 51
#define SBX_EVENT_UPDATE_AOI 52
#define SBX_EVENT_UPDATE 53
#define SBX_EVENT_EYELINK_PRINTMSG 54
#define SBX_EVENT_MSG 55
#define SBX_EVENT_GAMEPADREC 56
#define SBX_EVENT_SCROLLTRACKGP 57
#define SBX_EVENT_REPEATIF 58
#define SBX_EVENT_INCREMENT_COUNTER 59
#define SBX_EVENT_RESET_COUNTER 60
#define SBX_EVENT_SET_FLAG 61
#define SBX_EVENT_ASYNCSTART 200
#define SBX_EVENT_ASYNCSTOP 201
#define SBX_EVENT_SOCKET_SEND_MSG 300
#define SBX_EVENT_WEBCAM_START 400
#define SBX_EVENT_WEBCAMCV_START 401

// experiment-specific events
#define SBX_EVENT_GSC1FEEDBACK 10001
#define SBX_EVENT_GSC1DRAWGRID 10002

#define SBX_EVENT_LC1DISPLAY 10003
#define SBX_EVENT_SWAP_AOI2 10004
//
//#define TOBII_REC 130
//#define TOBII_STOP 140
//
//#define LC2FEEDBACK 100000
//#define PATDRAW 100001
//#define SHOWDOT 100002
//#define CONV1GRID 100003

//#define WATCHNULL 0
//#define EYEENTER 1
//#define MOUSEENTER 2
//#define MOUSELUP 3
//#define MOUSELDOWN 4
//#define MOUSERUP 5
//#define MOUSERDOWN 6
#define SBX_WATCH_DONE 7
#define SBX_WATCH_TIMEOUT 8
#define SBX_WATCH_GAMEPAD_BUTTONDOWN 9
//#define CMDLISTEN 10
//#define GAMEBUTTONRESP 11
#define SBX_WATCH_KEYDOWN 12
//#define GBAOIRESP 13
//#define GBRESPELSE 14
//#define MCLAOIELSE 15
//#define TEXTDONE 16
//#define MOUSELDOWNAOI 17
//#define ISCAN2_EYEIN 18
#define SBX_WATCH_MOUSEBUTTON 19
#define SBX_WATCH_MOUSEMOVE 20
#define SBX_WATCH_GAMEPAD_MOVE 80
#define SBX_WATCH_SOCKET_MSG 300
#define SBX_WATCH_REMAIN 30
#define SBX_WATCH_TRIALTIMEOUT 31

// experiment specific watches
#define SBX_WATCH_GSC1BUTTON 20001
#define SBX_WATCH_GSC1MOVE 20002
#define SBX_WATCH_GSC1SOUNDBUTTON 20003
#define SBX_WATCH_GSC1TOGGLEBUTTON 20004

#endif
