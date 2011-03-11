#!/bin/sh

# $Id: zcb.sh 2 2007-07-19 13:00:48Z pdezwart $

ZOUNDS=/mnt/stuff/Zounds
OGG=$ZOUNDS/ogg
TMP=/tmp/zcb

mkdir $TMP
mkdir $TMP/ogg
cd $TMP

let CDNUM=0
let CDSIZE=0
let MAXSIZE=700

for DIR in $OGG/*
do
	# Get the size of the Directory
	DIRSIZE=`du -sh $DIR | cut -d M -f 0-1`

	# Check to see if the current CD can fit this DIR in it
	let TMPSIZE=$DIRSIZE+$CDSIZE

	if [ $TMPSIZE -lt $MAXSIZE ]
	then
		ln -s $DIR $TMP/ogg
		ln -s $ZOUNDS/`basename $DIR`-*.m3u $TMP
		let CDSIZE+=$DIRSIZE
	else
		# Ok, let's create the ISO for the previous CD
		mkisofs -f -J -o /tmp/zcd-$CDNUM.iso -P "Froob Networks" -p "Pete de Zwart" -r -v -V "Zounds Backup $CDNUM" $TMP
		rm $TMP/*.m3u $TMP/ogg/*
		ln -s $DIR $TMP/ogg
		ln -s $ZOUNDS/`basename $DIR`-*.m3u $TMP
		let CDNUM+=1
		let CDSIZE=$DIRSIZE
	fi
done
