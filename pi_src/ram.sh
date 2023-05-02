#!/bin/bash

ram_free=$(awk '$3=="kB"{$2=$2/1024;$3="MB"} 1' /proc/meminfo | grep MemAvailable | grep -Eo '[0-9.]{1,10}')
ram_total=$(awk '$3=="kB"{$2=$2/1024;$3="MB"} 1' /proc/meminfo | grep MemTotal | grep -Eo '[0-9.]{1,10}')

echo r$(echo "$ram_free $ram_total" | awk '{printf "%d", ($1/$2)*100}')/

