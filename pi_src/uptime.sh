#!/bin/bash
# Outputs the uptime of the Raspberry PI in minutes

# [echo $(awk {'print $1'} /proc/uptime) / 60]: Outputs a string of uptime of the PI in seconds divided by 60
# [bc]: Performs the math operation from the input
echo u$(echo $(awk '{print $1}' /proc/uptime) / 60 | bc)/ # remember to include to install bc in website
