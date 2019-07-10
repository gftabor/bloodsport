#!/bin/bash
for i in `seq 1 5`
do
  id=$i
  mass=23.5
  rad_per_sec=130
  ixx=0.057
  iyy=2.757
  izz=2.483
  min_force=1000
  max_force=50000

  sh single_test.sh $id $mass $rad_per_sec $ixx $iyy $izz $min_force $max_force &
done
