#!/bin/bash -eu

echo 'DROP TABLE IF EXISTS EXPEventCmd;' > $2
echo 'DROP TABLE IF EXISTS EXPEventArgs;' >> $2
echo 'DROP TABLE IF EXISTS EXPWatchCmd;' >> $2
echo 'DROP TABLE IF EXISTS EXPWatchArgs;' >> $2
echo '.dump EXPEventCmd' | sqlite3 $1 >> $2
echo '.dump EXPEventArgs' | sqlite3 $1 >> $2
echo '.dump EXPWatchCmd' | sqlite3 $1 >> $2
echo '.dump EXPWatchArgs' | sqlite3 $1 >> $2
