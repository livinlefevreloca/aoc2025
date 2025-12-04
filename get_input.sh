#! /bin/bash
#
# This script is used to get input for the problems
#
mkdir -p input

YEAR=$1
DAY=$2
COOKIE=$3

curl -H "Cookie: session=${COOKIE}" \
 "https://adventofcode.com/${YEAR}/day/${DAY}/input" > "data/day${DAY}.txt"
