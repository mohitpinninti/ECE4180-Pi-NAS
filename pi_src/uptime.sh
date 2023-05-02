#!/bin/bash

# remember to include to install bc in website

echo u$(echo $(awk '{print $1}' /proc/uptime) / 60 | bc)/
