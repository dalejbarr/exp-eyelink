DROP TABLE IF EXISTS EXPEventCmd;
DROP TABLE IF EXISTS EXPEventArgs;
DROP TABLE IF EXISTS EXPWatchCmd;
DROP TABLE IF EXISTS EXPWatchArgs;
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE `EXPEventCmd` (
`EvtCmdID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Name` TEXT,
`Description` TEXT
);
INSERT INTO "EXPEventCmd" VALUES(1,'SHOW_BITMAP','show a bitmap');
INSERT INTO "EXPEventCmd" VALUES(2,'PLAY_SOUND','play a WAV file');
INSERT INTO "EXPEventCmd" VALUES(5,'CLEAR_REGION','clear screen (or region of screen)');
INSERT INTO "EXPEventCmd" VALUES(6,'MOVE_AOI','move an AOI');
INSERT INTO "EXPEventCmd" VALUES(8,'SHOW_TEXT','show text at a given coordinate');
INSERT INTO "EXPEventCmd" VALUES(10,'RECSOUND','record audio');
INSERT INTO "EXPEventCmd" VALUES(12,'DISPLAY_ON','show all of the stimuli in the AOI table simultaneously');
INSERT INTO "EXPEventCmd" VALUES(15,'MOUSE_REC','start recording mouse data (goes into Mouse table)');
INSERT INTO "EXPEventCmd" VALUES(27,'SWAP_AOI','swap locations of two AOIs');
INSERT INTO "EXPEventCmd" VALUES(28,'SHOW_AOI','make an AOI appear');
INSERT INTO "EXPEventCmd" VALUES(51,'GRAB_AOI','grab an AOI with the mouse');
INSERT INTO "EXPEventCmd" VALUES(52,'UPDATE_AOI','change aspects of an AOI');
INSERT INTO "EXPEventCmd" VALUES(53,'UPDATE','change a TARGET value to a SOURCE value');
INSERT INTO "EXPEventCmd" VALUES(54,'EYELINK_PRINTMSG','prints a message to the eyelink file');
INSERT INTO "EXPEventCmd" VALUES(300,'SOCKET_SEND_MSG','Send a message to another computer through socket');
INSERT INTO "EXPEventCmd" VALUES(10003,'LC1DISPLAY','display on for experiment LexCompCV1');
COMMIT;
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE `EXPEventArgs` (
`EvtCmdID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Default` TEXT,
`Obligatory` INTEGER,
`Description` TEXT
);
INSERT INTO "EXPEventArgs" VALUES(6,'x',NULL,1,'x position (or offset) to move to');
INSERT INTO "EXPEventArgs" VALUES(6,'y',NULL,1,'y position (or offset) to move to');
INSERT INTO "EXPEventArgs" VALUES(1,'Resource',NULL,1,'filename of bitmap (or $resource variable)');
INSERT INTO "EXPEventArgs" VALUES(1,'x',NULL,1,'x position of upper left corner of bitmap');
INSERT INTO "EXPEventArgs" VALUES(1,'y',NULL,1,'y position of upper left corner of bitmap');
INSERT INTO "EXPEventArgs" VALUES(1,'Layer','0',0,'layer on which to show bitmap');
INSERT INTO "EXPEventArgs" VALUES(6,'AOI',NULL,1,'name of AOI (or $resource or @watch[x].selectedAOI)');
INSERT INTO "EXPEventArgs" VALUES(51,'AOI',NULL,1,'name of AOI (or $resource or @watch[x].selectedAOI)');
INSERT INTO "EXPEventArgs" VALUES(52,'Layer',NULL,0,'change AOI layer');
INSERT INTO "EXPEventArgs" VALUES(52,'ResetLoc',NULL,0,'reset AOI location to last values (before grab)');
INSERT INTO "EXPEventArgs" VALUES(52,'AOI',NULL,1,'name of AOI(or @watch[x].selectedAOI)');
INSERT INTO "EXPEventArgs" VALUES(53,'TARGET',NULL,1,'name of variable to be updated');
INSERT INTO "EXPEventArgs" VALUES(53,'SOURCE',NULL,1,'value with which to update TARGET');
INSERT INTO "EXPEventArgs" VALUES(2,'Resource',NULL,1,'filename of WAV file (or $resource variable)');
INSERT INTO "EXPEventArgs" VALUES(52,'Highlight',NULL,0,'Highlight (border width in pixels, R, G, B)');
INSERT INTO "EXPEventArgs" VALUES(54,'MSG',NULL,1,'message to send to eyelink');
INSERT INTO "EXPEventArgs" VALUES(8,'Text',NULL,1,'text to be displayed');
INSERT INTO "EXPEventArgs" VALUES(8,'x',NULL,1,'x coordinate (center of string)');
INSERT INTO "EXPEventArgs" VALUES(8,'y',NULL,1,'y coordinate (center of string)');
INSERT INTO "EXPEventArgs" VALUES(5,'x1',NULL,0,'top left x coordinate of rectangle to clear');
INSERT INTO "EXPEventArgs" VALUES(5,'y1',NULL,0,'top left y coordinate of rectangle to clear');
INSERT INTO "EXPEventArgs" VALUES(5,'x2',NULL,0,'bottom right x coordinate fo rectangle to clear');
INSERT INTO "EXPEventArgs" VALUES(5,'y2',NULL,0,'bottom right y coordinate of rectangle to clear');
INSERT INTO "EXPEventArgs" VALUES(28,'AOI',NULL,1,'name of AOI to appear');
INSERT INTO "EXPEventArgs" VALUES(10,'Soundfile',NULL,1,'name of soundfile ($resource)');
INSERT INTO "EXPEventArgs" VALUES(300,'SocketID',NULL,1,'integer ID of socket specified in EXPConfig');
INSERT INTO "EXPEventArgs" VALUES(300,'Message',NULL,1,'message to send');
COMMIT;
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE EXPWatchCmd (WCmdID INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY, Name TEXT, Description TEXT);
INSERT INTO "EXPWatchCmd" VALUES(7,'DONE','wait until all events (or a specific event) completes');
INSERT INTO "EXPWatchCmd" VALUES(8,'TIMEOUT','delay for a specified number of milliseconds');
INSERT INTO "EXPWatchCmd" VALUES(9,'GAMEBUTTON_DOWN','watch for game button to be pressed');
INSERT INTO "EXPWatchCmd" VALUES(12,'KEY_DOWN','watch for a specific (or any) key to be pressed');
INSERT INTO "EXPWatchCmd" VALUES(19,'MOUSE_BUTTON','watch for a specific (or any) mouse button to be pressed/released');
INSERT INTO "EXPWatchCmd" VALUES(20,'MOUSE_MOVE','watch for a movement of the mouse cursor');
INSERT INTO "EXPWatchCmd" VALUES(300,'SOCKET_MSG','wait for a message over internet socket');
COMMIT;
PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE `EXPWatchArgs` (
`WCmdID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Default` TEXT,
`Obligatory` INTEGER,
`Description` TEXT
);
INSERT INTO "EXPWatchArgs" VALUES(8,'Msec',NULL,1,'time in milliseconds to wait');
INSERT INTO "EXPWatchArgs" VALUES(19,'Button','left',0,'which button (''left'',''center'',''right'')');
INSERT INTO "EXPWatchArgs" VALUES(19,'State','down',0,'state of button (''down'', ''up'')');
INSERT INTO "EXPWatchArgs" VALUES(9,'Button','ANY',0,'watch for a specific button (default ANY)');
INSERT INTO "EXPWatchArgs" VALUES(300,'SocketID',NULL,1,'id of the socket channel');
INSERT INTO "EXPWatchArgs" VALUES(300,'Message',NULL,0,'message to wait for (omit to wait for any message)');
COMMIT;
