#!/bin/bash

#kill -9 `pgrep exp2`
LASTSESS=`echo "select MAX(SessionID) from Session;" | sqlite3 HelloWorld.db`

exp-eyelink/expdebug2 -expdb HelloWorld -dbdriver sqlite -session $LASTSESS -resume
