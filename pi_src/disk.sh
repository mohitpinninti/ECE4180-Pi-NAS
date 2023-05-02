#!/bin/bash
# Outputs the available memory left as a percentage of /dev/sda1 (External Drive 1 connected to the PI)

# [df /dev /sda1 --output=pcent]: Outputs available memeory left as percentage
# [grep -Eo '[0-9]{1,4}']: Uses regex to extract numbers from input
echo m$(df /dev/sda1 --output=pcent | grep -Eo '[0-9]{1,4}')/
