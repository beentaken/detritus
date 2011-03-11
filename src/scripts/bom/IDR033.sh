#!/bin/sh


BASE=http://mirror.bom.gov.au/
IMG=`wget -q -O - http://mirror.bom.gov.au/products/IDR033.shtml|grep /radar/IDR033.gif|cut -d \" -f 2-2`

wget -q $BASE$IMG -O ~/.enlightenment/backgrounds/IDR033.gif
