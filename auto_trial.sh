#!/bin/sh

if [ $# -ne 1 ]; then
    echo "usage: $0 inputfile"
    exit 1
fi

if !(test -f $1); then
    echo "$1 is not a file"
    exit 2
fi

index=1
line_count=$(sed -n '$=' $1)

while :;do
    if [ ${index} -gt $line_count ]; then
        break
    fi

    arg1=$(sed -n ''$index'p' $1 | cut -d":" -f 1)
    arg2=$(sed -n ''$index'p' $1 | cut -d":" -f 2)
    arg3=$(sed -n ''$index'p' $1 | cut -d":" -f 3)
    ./test $arg1 $arg2 $arg3

    # ((++index))  ;shell of busybox can't execute this line
    index=`expr $index + 1`
done
