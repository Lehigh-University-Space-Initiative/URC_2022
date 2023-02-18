#!/bin/bash

#first host is jetson and second is pi
# hostNames=("192.168.1.3" "192.168.1.5")
#use this if pi is on internet through ZeroTier vpn
hostNames=("192.168.1.3" "10.147.19.187")
hostUsers=("jimmy" "pi")
hostPwords=("lusi" "lusi")
hostDeployDirs=("/home/jimmy/urc_deploy" "/home/pi/urc_deploy")
hostPackages=("jetson_nodes" "pi_nodes")

localDirsToCpy=("/src")

# echo "Script executed from: ${PWD}"
cd "$(dirname $0)"
# ecoo "Script location: ${BASEDIR}"

initialRemoteSetup(){
    #only if the remote dir is empty, run initial catkin setup which includes:
    #  - mkdir src
    #  - catkin init
    #  - catkin build

    # echo "Current directory: ${PWD}"
    cd "urc_deploy"

    echo "Current directory: ${PWD}"

    echo "Checking if directory is empty"
    # echo "Directory contents: $(ls .)"
    if [ "$(ls .)" ]; then
        # echo "Directory is not empty"
        echo "Skipping initial remote setup"
    else
        echo "Running initial remote setup: $PWD"
        mkdir src
        echo "Running initial catkin setup"
        source /opt/ros/melodic/setup.bash
        # echo "$PWD"
        catkin init
        catkin build
    fi

}

rebootHost() {
    echo "Rebooting ${hostNames[$1]}"
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "echo ${hostPwords[$1]} | sudo -S reboot"
}

#rember when this func is run remotely it doesnt have acces to global vars
buildRemote(){
    echo "Current directory: ${PWD}"
    # cd "urc_deploy"

    # source the devel_cb/setup.bash file
    source /opt/ros/melodic/setup.bash
    source devel/setup.bash
    # source devel_cb/setup.bash
    

    # echo "$2 is the package, $1 is the machine"
    catkin build $2
}

#function to deploy to a single host given by index
deployToHost()
{
    # index = $1
    echo "Deploying to host ${hostNames[$1]}"

    #run the initialRemoteSetup function in the remote host
    code="$(typeset -f initialRemoteSetup); initialRemoteSetup "$1""
    # echo "${code}"
    # if ["$2" -eq "1"]; then
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "${code}"
    # fi

    # copy over source file
    #need to copy this catkin workspace to the host deploy directory
    #the workspace is the 2nd level up from the script location
    arg1="${PWD}/.."
    arg2="${hostUsers[$1]}@${hostNames[$1]}:${hostDeployDirs[$1]}"
    echo "Copying src files from ${arg1} to ${arg2}"

    # if sshpass command not found, need to install it on that machine
    sshpass -p "${hostPwords[$1]}" scp -r ${arg1}${localDirsToCpy[0]} ${arg2}
    
    #need to ssh into the host and run "catkin build <package>"
    #use ssh password to login
    # # echo $(typeset -g)
    if [ "$2" == "0" ]; then
        echo "Building package ${hostPackages[$1]} on host ${hostNames[$1]}"
        sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "cd ${hostDeployDirs[$1]}; $(typeset -f buildRemote); buildRemote "$1" ${hostPackages[$1]}"
    fi
}

checkIfEmpty=0
# if [$1 == "scratch"]; then
#     checkIfEmpty=1;
# fi
cpyOnly=0
if [ "$1" == "-cp" ]; then
    cpyOnly=1
fi

# hostsToUpdate=(1 0)

for I in 1
do
    deployToHost ${I} "${cpyOnly}"
    if [ "${cpyOnly}" == "0" ]; then
        rebootHost ${I}
    fi
done
