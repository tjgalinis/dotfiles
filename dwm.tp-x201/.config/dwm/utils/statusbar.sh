#!/bin/bash

# Handle SIGTRAP signals sent by refresh-bar to update the status bar
# immediately.
trap 'update' 5

statusbar() {
	VOL="$(/usr/bin/amixer get Master | \
        grep 'Left:' | \
        grep -o '[0-9]*%\|\[on\]\|\[off\]' | \
        tr '\n' ' ' | \
        sed -e 's/\(.*\)\s\[on\]\s/ \1/' -e 's/\(.*\)\s\[off\]\s/ \1/')"

	# Wifi quality percentage and  icon if ethernet is connected.
	CON="$(grep "^\s*w" /proc/net/wireless | \
        awk '{ print "", int($3 * 100 / 70) "%" }')"

    CON="$CON$(sed "s/down//;s/up//" /sys/class/net/e*/operstate)"


	# Will show all batteries with approximate icon for remaining power.
    BAT=()

	for x in /sys/class/power_supply/BAT?;
	do
        capacity="$(cat $x/capacity)"
        state=""
        repr=""

        case "$(cat $x/status)" in
            Charging) state="" ;;
        esac

        case "$capacity" in
            100|9[0-9]|8[0-9])
                repr=" $capacity%" ;;
            7[0-9]|6[0-9])
                repr=" $capacity%" ;;
            5[0-9]|4[0-9])
                repr=" $capacity%" ;;
            3[0-9]|2[0-9])
                repr=" $capacity%" ;;
            *)
                repr=" $capacity%" ;;
        esac

        BAT+=("$state $repr")
	done

    echo -n "$VOL $CON${BAT[*]} $(date +'%a %I:%M %p')"
}

update() {
	/usr/bin/xsetroot -name " $(statusbar)" &
    wait
}


while :
do
    update

	sleep 1m &
    wait
done

