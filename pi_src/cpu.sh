#!/bin/bash
# Outputs the total CPU usage from the Raspberry PI.

# [vmstat]: Outputs CPU, RAM and various other metrics
# [tail -1]: Grabs the last row (which is the first row of metrics for the PI)
# [awk '{printf "%d", (100-$15)}']: $15 is the idle CPU usage, this is subtracted with 100 to get the total cpu usage as output
echo c$(vmstat | tail -1 | awk '{printf "%d", (100-$15)}')/
