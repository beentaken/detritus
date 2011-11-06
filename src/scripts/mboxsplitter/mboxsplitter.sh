#!/bin/sh

SPLITSIZE=500

let SPLIT=$1
let SPLITCOUNT=0

FILES=""

MBOX=$2
OUTPUTDIR=$3

for FILE in $MBOX/cur/*
do
    if [ $SPLITCOUNT -lt $SPLITSIZE ]
    then
        let SPLITCOUNT++
        FILES="$FILES $FILE"
    else
        mkdir $OUTPUTDIR/$SPLIT
        cp $FILES $OUTPUTDIR/$SPLIT

        let SPLITCOUNT=0
        let SPLIT++
        FILES=""
    fi
done

if [ $SPLITCOUNT -ne 0 ]
then
    mkdir $OUTPUTDIR/$SPLIT
    cp $FILES $OUTPUTDIR/$SPLIT

    echo
fi
