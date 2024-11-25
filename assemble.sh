#!/bin/bash

MARS=Mars4_5.jar
TMPFILE_IN=$(mktemp)
cat $(dirname "${1}")/startup.s $(dirname "${1}")/libio.s $1 > $TMPFILE_IN 
TMPFILE_TEXT=$(mktemp)
TMPFILE_DATA=$(mktemp)
java -jar $MARS a we mc Default $TMPFILE_IN dump .text HEX $TMPFILE_TEXT dump .data HEX $TMPFILE_DATA
echo ".text" > hexcode.txt
cat $TMPFILE_TEXT >> hexcode.txt
echo ".data" >> hexcode.txt
cat $TMPFILE_DATA >> hexcode.txt
rm $TMPFILE_IN $TMPFILE_TEXT $TMPFILE_DATA
