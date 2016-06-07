#!/bin/bash -eu
REMOTE=yap
cmd="export DISPLAY=:0; cd socket; ./helloworld.sh $1"

echo $cmd
ssh -X $REMOTE bash -c "'
export DISPLAY=:0
cd socket_remote
./helloworld.sh $1
'" > log_remote.txt &
sleep 2
./helloworld.sh $1 > log_local.txt
