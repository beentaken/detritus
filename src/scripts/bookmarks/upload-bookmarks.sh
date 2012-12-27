#!/bin/sh

USER=dezwart
PASS=REDACTED
HOST=froob.net
DIR=web
LDIR=$HOME/public_html
FILE=bookmarks.html

function _exit {
  rm -f $LOGFILE $TMP
  exit $1
}

trap _exit 15
trap _exit 2

cd $LDIR
echo -e "$USER\n$PASS\ncd $DIR\nput $FILE\nquit" | ftp $HOST

_exit
