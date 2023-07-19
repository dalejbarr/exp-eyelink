#!/bin/bash -eu
REMOTE=yap
#cmd="export DISPLAY=:0; cd socket; ./helloworld.sh $1"

#echo $cmd
ssh -X $REMOTE bash -c "'
export DISPLAY=:0
cd Experiments/EESP4_Listener
./run_listener.sh $1
'" > log_listener.txt &
sleep 2
./run.sh $1 > log_speaker.txt
