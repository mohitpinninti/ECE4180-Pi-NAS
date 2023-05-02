#!/bin/bash
# Outputs the temperature of the PI's CPU in celcius

# [cat /sys/class/thermal/thermal_zone0/temp]: Outputs the temperature in a specific format
# [awk '{printf "%d", $1/1000}']: Converts the input temperature into celcius
echo t$(cat /sys/class/thermal/thermal_zone0/temp | awk '{printf "%d", $1/1000}')/
