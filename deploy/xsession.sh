#!/bin/sh
matchbox-window-manager -use_titlebar no -use_cursor no &
cd ~/gui
exec ./chibitronics-jig 2>> ~/chibitronics-jig.log
