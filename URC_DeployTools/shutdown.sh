#!/bin/bash


hostNames=("192.168.1.3" "192.168.1.5")
#use this if pi is on internet through ZeroTier vpn
# hostNames=("192.168.1.3" "10.147.19.187")
hostUsers=("jimmy" "pi")
hostPwords=("lusi" "lusi")

shutdownHost() {
    echo "Commencing shutdown of ${hostNames[$1]}"
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "echo ${hostPwords[$1]} | sudo -S poweroff"
}

for I in 1 0
do
    shutdownHost $I
done