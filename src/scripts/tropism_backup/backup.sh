#!/bin/sh

# $Id: backup.sh 2 2007-07-19 13:00:48Z pdezwart $

IGNORE_PATH=/proc
TAPE=/dev/st0

tar --create --file $TAPE --preserve --sparse --totals --verbose --label "`date`" --exclude=$IGNORE_PATH /
