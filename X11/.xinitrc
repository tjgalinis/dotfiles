#!/bin/bash

#-------------------------------------------------------------------------------
# File: ~/.xinitrc
# Author: Thomas Galinis <tjgalinis@gmail.com>
#-------------------------------------------------------------------------------

# Turn off the terminal bell
xset b off

# reload x resources database
xrdb -merge ./.Xresources

# launch xscreensaver daemon
# TODO: create a systemd user unit perhaps?
xscreensaver -no-splash &

if [ -f ~/.config/dwm/utils/statusbar.sh ]
then
    bash ~/.config/dwm/utils/statusbar.sh &
fi

exec /usr/local/bin/dwm

