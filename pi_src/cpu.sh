#!/bin/bash

echo c$(vmstat | tail -1 | awk '{printf "%d", (100-$15)}')/
