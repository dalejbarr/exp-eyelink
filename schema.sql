CREATE TABLE `AOI` (
`AOIID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Link` TEXT COLLATE BINARY,
`ID` INTEGER COLLATE BINARY,
`Name` TEXT,
`Resource` TEXT,
`x1` INTEGER,
`y1` INTEGER,
`Layer` INTEGER);
CREATE TABLE `Block` (
`BlockID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Name` TEXT ,
`OrderConstraint` INTEGER 
);
CREATE TABLE `Condition` (
`CellID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Type` TEXT ,
`Value` TEXT 
);
CREATE TABLE `EXPEventArgs` (
`EvtCmdID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Default` TEXT,
`Obligatory` INTEGER,
`Description` TEXT
);
CREATE TABLE `EXPEventCmd` (
`EvtCmdID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Name` TEXT,
`Description` TEXT
);
CREATE TABLE `EXPWatchArgs` (
`WCmdID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Default` TEXT,
`Obligatory` INTEGER,
`Description` TEXT
);
CREATE TABLE EXPWatchCmd (WCmdID INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY, Name TEXT, Description TEXT);
CREATE TABLE `Event` (
`EventID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`StateID` INTEGER  COLLATE BINARY,
`EvtCmdID` INTEGER ,
`Msec` INTEGER
);
CREATE TABLE `EventArgs` (
`EventID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Info` TEXT
);
CREATE TABLE `EventTiming` (
`TrialID` INTEGER NOT NULL COLLATE BINARY,
`EventID` INTEGER NOT NULL COLLATE BINARY,
`Msec` INTEGER NOT NULL COLLATE BINARY
);
CREATE TABLE `Eye` (
`RespID` INTEGER NOT NULL COLLATE BINARY,
`Msec` INTEGER NOT NULL COLLATE BINARY,
`Xpos` INTEGER ,
`Ypos` INTEGER ,
`Pup` INTEGER ,
`tcHr` INTEGER ,
`tcMn` INTEGER ,
`tcSc` INTEGER ,
`tcFr` INTEGER 
);
CREATE TABLE `GamePad` (
`GPID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`RespID` INTEGER  COLLATE BINARY,
`SourceID` INTEGER ,
`Msec` INTEGER ,
`Button` INTEGER 
);
CREATE TABLE `Item` (
`ItemCellID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Item` TEXT ,
`Cell` INTEGER ,
`TemplateID` INTEGER 
);
CREATE TABLE `KeyTrack` (
`RespID` INTEGER  COLLATE BINARY,
`Msec` INTEGER ,
`Button` INTEGER 
);
CREATE TABLE `Keyboard` (
`KeyPressID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`RespID` INTEGER  COLLATE BINARY,
`SourceID` INTEGER ,
`Msec` INTEGER ,
`Button` INTEGER 
);
CREATE TABLE `List` (
`ListID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Name` TEXT NOT NULL default '',
`Excluded` INTEGER default 0
);
CREATE TABLE `ListOrder` (
`ListOrderID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`ListID` INTEGER  COLLATE BINARY,
`ItemCellID` INTEGER  COLLATE BINARY,
`OrderConstraint` INTEGER ,
`PhaseID` INTEGER 
);
CREATE TABLE `Mouse` (
`RespID` INTEGER NOT NULL COLLATE BINARY,
`Msec` INTEGER NOT NULL COLLATE BINARY,
`Xpos` INTEGER ,
`Ypos` INTEGER ,
`Buttons` INTEGER 
);
CREATE TABLE Msg (TrialID INTEGER NOT NULL COLLATE BINARY,
Msec INTEGER NOT NULL COLLATE BINARY,
Msg1 TEXT, Msg2 TEXT);
CREATE TABLE `Phase` (
`PhaseID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`BlockID` INTEGER  COLLATE BINARY,
`Name` TEXT ,
`OrderConstraint` INTEGER 
);
CREATE TABLE `Resource` (
`ResID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Code` TEXT ,
`ItemCellID` INTEGER  COLLATE BINARY,
`Data` TEXT 
);
CREATE TABLE `Response` (
`RespID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`SubjID` INTEGER  COLLATE BINARY,
`TrialID` INTEGER  COLLATE BINARY
);
CREATE TABLE `ResponsesExcluded` (
`RespID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`SubjID` INTEGER ,
`TrialID` INTEGER ,
`Reason` TEXT 
);
CREATE TABLE `Session` (
`SessionID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`ExpID` INTEGER ,
`ListID` INTEGER ,
`Date` timestamp NOT NULL default CURRENT_TIMESTAMP,
`Completion` TEXT ,
`Host` TEXT ,
`Comment` TEXT 
, Seed INTEGER);
CREATE TABLE `SessionLog` (
`SessionID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Comment` text
);
CREATE TABLE `SessionsExcluded` (
`SessionID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`ExpID` INTEGER ,
`ListID` INTEGER ,
`Date` timestamp NOT NULL default CURRENT_TIMESTAMP,
`Completion` TEXT ,
`Host` TEXT ,
`Comment` TEXT 
);
CREATE TABLE `State` (
`StateID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`TemplateID` INTEGER ,
`Sequence` INTEGER ,
`Name` TEXT 
);
CREATE TABLE `StateTiming` (
`TrialID` INTEGER NOT NULL COLLATE BINARY,
`StateID` INTEGER NOT NULL COLLATE BINARY,
`Msec` INTEGER NOT NULL COLLATE BINARY
);
CREATE TABLE SubjRole 
( SubjRoleID INTEGER,
	ExpID INTEGER,
	Data TEXT 
);
CREATE TABLE `SubjectsExcluded` (
`SubjID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`SessionID` INTEGER ,
`SubjRoleID` INTEGER ,
`Gender` TEXT ,
`Comment` TEXT ,
`Reason` TEXT 
);
CREATE TABLE `Template` (
`TemplateID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`Timeout` INTEGER ,
`Name` TEXT 
);
CREATE TABLE `Trial` (
`TrialID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`TrialOrder` INTEGER ,
`ItemCellID` INTEGER  COLLATE BINARY,
`SessionID` INTEGER  COLLATE BINARY,
`Begin` INTEGER ,
`End` INTEGER ,
`Status` TEXT default 'ABORTED',
`ListOrderID` INTEGER  COLLATE BINARY
);
CREATE TABLE `TrialsExcluded` (
`TrialID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`TrialOrder` INTEGER ,
`ItemCellID` INTEGER  COLLATE BINARY,
`SessionID` INTEGER  COLLATE BINARY,
`Begin` INTEGER ,
`End` INTEGER ,
`Status` TEXT default 'ABORTED',
`ListOrderID` INTEGER  COLLATE BINARY
);
CREATE TABLE `Watch` (
`WatchID` INTEGER PRIMARY KEY AUTOINCREMENT COLLATE BINARY,
`StateID` INTEGER ,
`WCmdID` INTEGER ,
`NextStateID` INTEGER
);
CREATE TABLE `WatchArgs` (
`WatchID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Info` TEXT
);
CREATE VIEW vAOI as 
SELECT AOI.AOIID, AOI.Link, ID, Name, Resource,
x1, y1, Layer, ArgID, Info 
FROM AOI 
LEFT JOIN AOIArgs ON AOI.AOIID=AOIArgs.AOIID 
ORDER BY AOI.ID, AOI.Name;

CREATE VIEW vEvent AS 
SELECT State.TemplateID, Event.EventID, Event.StateID, State.Name AS State, 
Event.EvtCmdID, EXPEventCmd.Name AS cmd, Event.Msec,
EventArgs.ArgID, EventArgs.Info 
FROM Event 
LEFT JOIN EventArgs ON Event.EventID=EventArgs.EventID 
LEFT JOIN EventBlacklist ON Event.EventID=EventBlacklist.EventID
INNER JOIN EXPEventCmd ON Event.EvtCmdID=EXPEventCmd.EvtCmdID 
INNER JOIN State ON Event.StateID=State.StateID 
WHERE EventBlacklist.EventID IS NULL
ORDER BY State.TemplateID, Sequence, Msec, Event.EventID;

CREATE VIEW vEventAll AS 
SELECT State.TemplateID, Event.EventID, Event.StateID, State.Name AS State, 
Event.EvtCmdID, EXPEventCmd.Name AS cmd, Event.Msec,
EventArgs.ArgID, EventArgs.Info 
FROM Event 
LEFT JOIN EventArgs ON Event.EventID=EventArgs.EventID 
INNER JOIN EXPEventCmd ON Event.EvtCmdID=EXPEventCmd.EvtCmdID 
INNER JOIN State ON Event.StateID=State.StateID 
ORDER BY State.TemplateID, Sequence, Msec, Event.EventID;

CREATE VIEW vWatch AS 
SELECT S1.TemplateID, Watch.WatchID, Watch.StateID AS S1ID,
S1.Name AS S1, Watch.NextStateID AS S2ID, S2.Name AS S2, Watch.WCmdID,
EXPWatchCmd.Name AS cmd, WatchArgs.ArgID, WatchArgs.Info 
FROM Watch
LEFT JOIN WatchArgs ON Watch.WatchID=WatchArgs.WatchID 
LEFT JOIN WatchBlacklist ON Watch.WatchID=WatchBlacklist.WatchID
INNER JOIN EXPWatchCmd ON Watch.WCmdID=EXPWatchCmd.WCmdID 
INNER JOIN State AS S1 ON Watch.StateID=S1.StateID 
LEFT JOIN State AS S2 ON Watch.NextStateID=S2.StateID 
WHERE WatchBlacklist.WatchID IS NULL
ORDER BY S1.TemplateID, S1.Sequence, Watch.WatchID;

CREATE VIEW vWatchAll AS 
SELECT S1.TemplateID, Watch.WatchID, Watch.StateID AS S1ID,
S1.Name AS S1, Watch.NextStateID AS S2ID, S2.Name AS S2, Watch.WCmdID,
EXPWatchCmd.Name AS cmd, WatchArgs.ArgID, WatchArgs.Info 
FROM Watch
LEFT JOIN WatchArgs ON Watch.WatchID=WatchArgs.WatchID 
INNER JOIN EXPWatchCmd ON Watch.WCmdID=EXPWatchCmd.WCmdID 
INNER JOIN State AS S1 ON Watch.StateID=S1.StateID 
LEFT JOIN State AS S2 ON Watch.NextStateID=S2.StateID 
ORDER BY S1.TemplateID, S1.Sequence, Watch.WatchID;

CREATE TABLE Subject (
SubjID INTEGER PRIMARY KEY AUTOINCREMENT,
SessionID INTEGER,
SubjRoleID INTEGER,
Gender TEXT,
Comment TEXT);
CREATE TABLE EXPConfig (Var TEXT PRIMARY KEY, Data TEXT);
CREATE TABLE TrialsTodo (SessionID INTEGER, ItemCellID INTEGER, BlockID INTEGER, PhaseID INTEGER, Ord INTEGER);
CREATE TABLE SessionRuns (SessionID INTEGER, RunID INTEGER);
CREATE TABLE `AOIArgs` (
`AOIID` INTEGER COLLATE BINARY,
`ArgID` TEXT,
`Info` TEXT);
CREATE TABLE EventBlacklist(EventID INTEGER PRIMARY KEY);
CREATE TABLE WatchBlacklist(WatchID INTEGER PRIMARY KEY);

-- now put in the data
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
INSERT INTO "EXPEventCmd" VALUES(400,'WEBCAM_START','start recording and displaying webcam');
INSERT INTO "EXPEventCmd" VALUES(10003,'LC1DISPLAY','display on for experiment LexCompCV1');
INSERT INTO "EXPWatchArgs" VALUES(8,'Msec',NULL,1,'time in milliseconds to wait');
INSERT INTO "EXPWatchArgs" VALUES(19,'Button','left',0,'which button (''left'',''center'',''right'')');
INSERT INTO "EXPWatchArgs" VALUES(19,'State','down',0,'state of button (''down'', ''up'')');
INSERT INTO "EXPWatchArgs" VALUES(9,'Button','ANY',0,'watch for a specific button (default ANY)');
INSERT INTO "EXPWatchArgs" VALUES(30,'Total_Msec',NULL,1,'max time to wait');
INSERT INTO "EXPWatchArgs" VALUES(300,'SocketID',NULL,1,'id of the socket channel');
INSERT INTO "EXPWatchArgs" VALUES(300,'Message',NULL,0,'message to wait for (omit to wait for any message)');
INSERT INTO "EXPWatchCmd" VALUES(7,'DONE','wait until all events (or a specific event) completes');
INSERT INTO "EXPWatchCmd" VALUES(8,'TIMEOUT','delay for a specified number of milliseconds');
INSERT INTO "EXPWatchCmd" VALUES(9,'GAMEBUTTON_DOWN','watch for game button to be pressed');
INSERT INTO "EXPWatchCmd" VALUES(12,'KEY_DOWN','watch for a specific (or any) key to be pressed');
INSERT INTO "EXPWatchCmd" VALUES(19,'MOUSE_BUTTON','watch for a specific (or any) mouse button to be pressed/released');
INSERT INTO "EXPWatchCmd" VALUES(20,'MOUSE_MOVE','watch for a movement of the mouse cursor');
INSERT INTO "EXPWatchCmd" VALUES(30,'REMAIN','watch until at least X amount of time elapses in trial');
INSERT INTO "EXPWatchCmd" VALUES(300,'SOCKET_MSG','wait for message over network socket');
