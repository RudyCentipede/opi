#!/bin/bash

if [ $# -ne 3 ]; then
	exit 1
fi

in_file="$1"
data_file="$3"
out="../data/out.txt"
command="../../app.exe"

num=$(head -n 1 "$in_file" | grep -o -E '^[0-9]+')

if [ "$num" -eq 1 ] || [ "$num" -eq 4 ]; then
    $command "$data_file" < "$in_file" > "$out"
  else
    cp -f "$data_file" "$out"
    $command "$out" < "$in_file"
fi

if ./comparator.sh "$out" "$2"; then
	exit 0
else
	exit 1
fi

