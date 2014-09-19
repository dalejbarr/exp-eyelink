#!/bin/bash -eu

#xrandr -s 1024x768
#./exp2 -expdb HelloWorld -dbdriver sqlite -session 1 -fullscreen
../../bin/expdebug2 -expdb TestAudio -dbdriver sqlite -session 1
#xrandr -s 0
