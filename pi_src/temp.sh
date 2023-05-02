#!/bin/bash

echo t$(cat /sys/class/thermal/thermal_zone0/temp | awk '{printf "%d", $1/1000}')/
