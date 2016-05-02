#!/bin/sh
a=0


    `make all`
while [ $a -lt 1000 ]
do

    `./main < input >> output`
    echo "
    
    ====================================================================

    " >> output
#    echo `./prog1 64 100 1000000000 2000000000 >> output`
#    `make clean`
#    echo "
#
#    ====================================================================
#
#    ">>output
    echo $a
    a=`expr $a + 1`
done
