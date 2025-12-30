#!/usr/bin/env bash

make clean &> /dev/null
make &> /dev/null
for file in ./day*.c;
do
  day=$(basename "$file" .c | grep -o '[0-9]\+')
  echo "testing File: $file, Day: $day"
  diff <(./aoc $day "data/day${day}.txt") "results/day${day}.txt" &> /dev/null
  if [[ $? != 0 ]]
  then
    echo "Test failed for Day $day. Diff found"
    diff <(./aoc $day "data/day${day}.txt") "results/day${day}.txt"
    exit 1
  else
    echo "Test passed for Day $day"
  fi
done
