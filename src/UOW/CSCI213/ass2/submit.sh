#!/bin/sh


tar -cvf assign2.tar *.java README.txt
gzip -9v assign2.tar
submit -u $USER -c CSCI213 -a 2 assign2.tar.gz
