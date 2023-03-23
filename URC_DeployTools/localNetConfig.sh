#!/bin/bash

#DOCS:
#this program will update your netplan config file to either use the internet or the rover network
#will ask for sudo password
#Parameters: -int will switch to the internet
#            default will switch to the rover network
#NOTE: might not work on all computers as the netplan config file might be named differently

toInternet=0
if [ "$1" == "-int" ]; then
    toInternet=1
fi

#this program will either set the /etc/netplan/01-netcfg.yaml file to use the internet or the local rover network
#in the /etc/netplan/templates folder there are 2 files _internet_01-netcfg.yaml and _roverNet_01-netcfg.yaml

#if the toInternet variable is set to 1 then the internet file will be copied to the /etc/netplan/01-netcfg.yaml file
#if the toInternet variable is set to 0 then the roverNet file will be copied to the /etc/netplan/01-netcfg.yaml file
#then run sudo netplan apply

if [ $toInternet -eq 1 ]; then
    echo "Switching to Internet"
    sudo cp /etc/netplan/templates/_internet_01-netcfg.yaml /etc/netplan/01-netcfg.yaml
else
    echo "Switching to RoverNet"
    sudo cp /etc/netplan/templates/_roverNet_01-netcfg.yaml /etc/netplan/01-netcfg.yaml
fi

sudo netplan apply