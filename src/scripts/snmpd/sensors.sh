#!/bin/sh

VCORE_1_TAG="VCore 1:"
VCORE_2_TAG="VCore 2:"
P33V_TAG="+3.3V:"
P5V_TAG="+5V:"
P12V_TAG="+12V:"
N12V_TAG="-12V:"
N5V_TAG="-5V:"
FAN_1_TAG="fan1:"
FAN_2_TAG="fan2:"
FAN_3_TAG="fan3:"
TEMP_1_TAG="temp1:"
TEMP_2_TAG="temp2:"
TEMP_3_TAG="temp3:"
VID_TAG="vid:"

BASENAME=`basename $0`
TMP=`mktemp -p /tmp $BASENAME.XXXXXX`

/usr/bin/sensors > $TMP

for VOLTAGE in "$VCORE_1_TAG" "$VCORE_2_TAG"
do
	grep "$VOLTAGE" < $TMP | awk '{print $3}'
	grep "$VOLTAGE" < $TMP | awk '{print $7}'
	grep "$VOLTAGE" < $TMP | awk '{print $11}'
done

for VOLTAGE in "$P33V_TAG" "$P5V_TAG" "$P12V_TAG" "$N12V_TAG" "$N5V_TAG"
do
	grep -e "$VOLTAGE" < $TMP | awk '{print $2}'
	grep -e "$VOLTAGE" < $TMP | awk '{print $6}'
	grep -e "$VOLTAGE" < $TMP | awk '{print $10}'
done

for FAN in "$FAN_1_TAG" "$FAN_2_TAG" "$FAN_3_TAG"
do
	grep "$FAN" < $TMP | awk '{print $2}'
	grep "$FAN" < $TMP | awk '{print $6}'
done

for TEMP in "$TEMP_1_TAG" "$TEMP_2_TAG" "$TEMP_3_TAG"
do
	grep "$TEMP" < $TMP | awk '{print $2}' | cut -d ° -f 0-1
	grep "$TEMP" < $TMP | awk '{print $5}' | cut -d ° -f 0-1
	grep "$TEMP" < $TMP | awk '{print $8}' | cut -d ° -f 0-1
done

grep "$VID_TAG" < $TMP | awk '{print $2}'

rm -f $TMP
