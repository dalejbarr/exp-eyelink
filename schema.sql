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
INNER JOIN EXPEventCmd ON Event.EvtCmdID=EXPEventCmd.EvtCmdID 
INNER JOIN State ON Event.StateID=State.StateID 
ORDER BY State.TemplateID, Sequence, Msec, Event.EventID;
CREATE VIEW vWatch AS 
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
