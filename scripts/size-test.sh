#!/bin/bash

# size-test.sh -- write size of file into .h file
#
# Copyright (C) 2021 - 2025 andres26
#
# This file is distributed under the terms of the MIT license.
#

# check the file size and write to .h file

PROGNAME=$0
HEADER=$1
FILE_TO_CHECK=$2
OVERWRITE=$3


PROGNAME=$(basename $FILE_TO_CHECK)
DEFINE_NAME=$(echo "$PROGNAME" | awk '{print toupper($0)}')
NAME=$(echo "$DEFINE_NAME" | sed -r 's/[.]+/_/g')_SIZE

print_info() {
	echo "$PROGNAME info - $1"
}

print_error() {
	echo "$PROGNAME error - $1"
}

print_help() {
	echo "$PROGNAME -- test size of file and write to .h file"
	echo "usage: $PROGNAME (first) (second) (thrird)"
	echo "(first) output .h file"
	echo "(second) file to check"
	echo "(thrird) overwrite .h file [default=no]"
	exit 0;
}

if [[ -z $HEADER || $HEADER == "help" ]]; then
	print_help;
fi

if [[ -z $FILE_TO_CHECK ]]; then
	print_error "file not found"
fi

if [[ $OVERWRITE == "yes" || $OVERWRITE == "1" ]]; then
	print_info "enabling file overwriting"

	cat > $HEADER << EOF

/* size of file $FILE_TO_CHECK */
#define $NAME $(stat -c%s $FILE_TO_CHECK)

EOF

exit 0

else
	cat >> $HEADER << EOF

/* size of file $FILE_TO_CHECK */
#define $NAME $(stat -c%s $FILE_TO_CHECK)

EOF

exit 0
fi


exit 0;

