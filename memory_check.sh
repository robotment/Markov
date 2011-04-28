#!/bin/sh
foo=1

while [ "$foo" -le 2000 ]
do
    ./markov < test
    foo=$(($foo+1))
done
