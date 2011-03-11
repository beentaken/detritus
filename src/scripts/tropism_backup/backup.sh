#!/bin/sh


IGNORE_PATH=/proc
TAPE=/dev/st0

tar --create --file $TAPE --preserve --sparse --totals --verbose --label "`date`" --exclude=$IGNORE_PATH /
