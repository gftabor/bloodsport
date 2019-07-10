#!/bin/bash

# Just in case these directories don't exist
mkdir data 2> /dev/null
mkdir autogen_model 2> /dev/null

# Kick off tests
for i in `seq 0 7`
do
  for j in `seq 0 8`
  do
    id=$((i*100+j))
    mass=23.5
    rad_per_sec=130
    ixx="$(echo print 0.005+$i/20.0 | python2)"
    iyy=2.5
    izz="$(echo print 2.5-$j/10.0 | python2)"
    min_force=1000
    max_force=50000

    echo "Test params:"
    echo $id
    echo $ixx
    echo $iyy
    echo $izz

    sh single_test.sh $id $mass $rad_per_sec $ixx $iyy $izz $min_force $max_force &
  done
done
