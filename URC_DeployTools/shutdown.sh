#!/bin/bash

#DOCS:
#this program will shutdown or reboot all or one of the rover computers
#Parameters: -r will reboot the desired hosts
#            -o0 will only shutdown the jetson
#            -o1 will only shutdown the pi

#set working directory to the directory of this script
cd "$(dirname $0)"
BASEDIR=$(pwd)

source helpers/loadHosts.sh


#restore pwd
cd $BASEDIR

source helpers/onlyFlagParse.sh

#check if first flag is -r for reboot
reboot=0
if [ "$1" == "-r" ]; then
    reboot=1
    shift
else
    reboot=0
fi

shutdownHost() {
    echo "Commencing shutdown of ${hostNames[$1]}"
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "echo ${hostPwords[$1]} | sudo -S poweroff"
}

rebootHost() {
    echo "Commencing reboot of ${hostNames[$1]}"
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "echo ${hostPwords[$1]} | sudo -S reboot"
}

#if reboot echo 
if [ $reboot -eq 1 ]; then
    echo "Rebooting desired hosts"
else
    echo "Shutting down desired hosts"
fi

if [ -n "$only_host" ]; then
    echo "Only $only_host selected"
    if [ $reboot -eq 1 ]; then
        rebootHost $only_host
    else
        shutdownHost $only_host
    fi
else
    echo "All hosts selected"
    for i in "${!hostNames[@]}"; do
        if [ $reboot -eq 1 ]; then
            rebootHost $i
        else
            shutdownHost $i
        fi
    done
fi