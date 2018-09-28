# !/bin/bash

# Excute this script in pa2018-fall/
User=`whoami` #get the current user executing the script
sed -i 's/114.212.189.154/114.212.80.192/g' Makefile.git
sed -i 's/114.212.189.154/114.212.80.192/g' /home/$User/.pa-nemu/scripts/gitmonitor.sh

