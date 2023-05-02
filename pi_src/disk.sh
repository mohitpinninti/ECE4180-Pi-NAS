#!/bin/bash

echo m$(df /dev/sda1 --output=pcent | grep -Eo '[0-9]{1,4}')/
