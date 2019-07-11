#!/bin/bash

# 1st arg is ID
export GAZEBO_MASTER_URI=http://localhost:`expr 11345 + $1`

# Generates new world object
# 1st arg is ID
# 2nd arg is weapon mass
# 3rd arg is weapon min rad_per_sec
# 4th arg is weapon max rad_per_sec
# 5th arg is weapon ixx
# 6th arg is weapon iyy
# 7th arg is weapon izz
# 8th arg is min force
# 9th arg is max force
python program_stability_tests.py $1 $2 $3 $4 $5 $6 $7 $8 $9

# Kicks off gazebo simulation
gzserver autogen_model/world$1.world --verbose > data/test$1.txt
