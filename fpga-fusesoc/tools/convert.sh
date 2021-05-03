#!/bin/bash
hexdump -ve '16/2 "%04x " "\n"' $1 > $2
