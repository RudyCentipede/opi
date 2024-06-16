#!/bin/bash

if [ $# -ne 2 ]; then
	exit 1
fi

in_file="$1"
data_file="$2"
out="../data/out.txt"
command="../../app.exe"

if ! $command "$data_file" < "$in_file" > "$out"; then
	exit 0
else
	exit 1
fi

