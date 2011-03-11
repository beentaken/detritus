#!/bin/sh

# $Id: IDR033.sh 2 2007-07-19 13:00:48Z pdezwart $

BASE=http://mirror.bom.gov.au/
IMG=`wget -q -O - http://mirror.bom.gov.au/products/IDR033.shtml|grep /radar/IDR033.gif|cut -d \" -f 2-2`

wget -q $BASE$IMG -O ~/.enlightenment/backgrounds/IDR033.gif
