#!/bin/bash
# Outputs the ram available on the PI as a percentage.

# [awk '$3=="kb"{$2=$2/1024;$3="MB"} 1' /proc/meminfo]: Converts the output of /proc/meminfo from kb to mb
# [grep MemAvailable] && [grep MemTotal]: Selects the row with the metric of interest
# [grep -Eo '[0-9.]{1, 10}']: Extracts the number from the input, in this case the ram metric as a float.
ram_free=$(awk '$3=="kB"{$2=$2/1024;$3="MB"} 1' /proc/meminfo | grep MemAvailable | grep -Eo '[0-9.]{1,10}')
ram_total=$(awk '$3=="kB"{$2=$2/1024;$3="MB"} 1' /proc/meminfo | grep MemTotal | grep -Eo '[0-9.]{1,10}')

# returns (ram available/total) * 100 ram as output.
echo r$(echo "$ram_free $ram_total" | awk '{printf "%d", ($1/$2)*100}')/

