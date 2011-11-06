#!/bin/sh

LOGFILE=/tmp/web.log
TMPLATE=/tmp/`basename $0`.XXXXXX
TMP=`mktemp $TMPLATE`
URL='ftp://dezwart:REDACTED@froob.net/logs/web.log'
WGET="/usr/bin/wget -q"
WEBALIZER=/usr/bin/webalizer

function _exit {
    rm -f $LOGFILE $TMP
    exit $1
}

trap _exit 15
trap _exit 2

$WGET -O $LOGFILE $URL
if [ $? ]
then
    # Set the correct mask
    umask 022
    # Process the logfile
    cat $LOGFILE | /usr/local/sbin/apache-log-ip2dns > $TMP
    $WEBALIZER -p -T -c /usr/local/etc/froob.net-webalizer.conf $TMP
else
    echo "Download of the logfile failed."
    _exit 1
fi

_exit
