#!/bin/sh
COUNT=`cat $1`
let COUNT=COUNT+1
echo -n $COUNT | tee $1
