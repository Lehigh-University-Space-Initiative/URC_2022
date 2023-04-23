#!/bin/bash

#DOCS:
#this program will deploy the urc code to the rover computers
#it will also run the initial catkin setup on the remote computers if needed
#Parameters: -o0 will only deploy to the jetson
#            -o1 will only deploy to the pi
#            -cp will only copy the local files to the specified remote computers
#               this param must be first if desired


#color constants
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
LIGHTCYAN='\033[1;36m'
YELLOW='\033[0;33m'
NOCOLOR='\033[0m' # No Color

# ecoo "Script location: ${BASEDIR}"

# #first host is jetson and second is pi
# hostNames=("192.168.1.3" "192.168.1.5")
# #use this if pi is on internet through ZeroTier vpn
# # hostNames=("192.168.1.3" "10.147.19.187")
# hostUsers=("jimmy" "pi")
# hostPwords=("lusi" "lusi")
# hostDeployDirs=("/home/jimmy/urc_deploy" "/home/pi/urc_deploy")
# hostPackages=("jetson_nodes" "pi_nodes")

#the above data is in a csv file now

#set working directory to the directory of this script
cd "$(dirname $0)"
BASEDIR=$(pwd)

source helpers/loadHosts.sh



localDirsToCpy=("/src")

#restore pwd
cd $BASEDIR

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

sendDebugInfo() {
    #build the debugInfo file on the remote host
    #the file will have:
    #   - the date and time the code was deployed
    #   - the git commit hash
    #   - the git commit message
    #   - the git branch
    #   - the git status
    #   - the git author (currnet computer owner) name and email
    
    #get the git commit hash
    gitCommitHash=$(git rev-parse HEAD)
    #get the git commit message
    gitCommitMsg=$(git log -1 --pretty=%B)
    #escape commas in commit message
    gitCommitMsg=$(echo $gitCommitMsg | sed 's/,/\\,/g')
    #get the git branch
    gitBranch=$(git rev-parse --abbrev-ref HEAD)
    #get the git status
    # gitStatus=$(git status)
    #get the git user name and email
    gitUserName=$(git config user.name)
    gitUserEmail=$(git config user.email)

    #get the date and time (utc)
    dateTime=$(date -u)

    #build the debugInfo file: a csv file with the above data and a header
    #place it on the remote host at the deploy dir

    #build the header
    debugInfoHeader="Date and Time, Git Commit Hash, Git Commit Message, Git Branch, Git User Name, Git User Email"
    #build the data
    debugInfoData="${dateTime}, ${gitCommitHash}, ${gitCommitMsg}, ${gitBranch}, ${gitUserName}, ${gitUserEmail}"
    #build the file
    debugInfoFile="${debugInfoHeader} \n ${debugInfoData}"

    echo -e "sending debug info to ${hostNames[$1]}"
    #write the file to the remote host
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "echo -e '${debugInfoFile}' > ${hostDeployDirs[$1]}/debugInfo.csv"
}

#rember when this func is run remotely it doesnt have acces to global vars
buildRemote(){
    echo "Current directory: ${PWD}"
    # cd "urc_deploy"

    # source the devel_cb/setup.bash file
    source /opt/ros/melodic/setup.bash
    source devel/setup.bash
    # source devel_cb/setup.bash
    
    # make the launch script executable (if there is one)

    # check if it exists (src/$2/launch/localStartCMD.bash)
    if [ -f "src/$2/launch/localStartCMD.bash" ]; then 
        echo -e "${CYAN}Making launch script executable${NOCLOR}"
        chmod +x "src/$2/launch/localStartCMD.bash" 
    else
        echo -e "${LIGHTCYAN}No launch script found ${NOCLOR}"
    fi

    # echo "$2 is the package, $1 is the machine"
    catkin build $2
}

#function to deploy to a single host given by index
deployToHost()
{
    # index = $1
    echo -e "${LIGHTCYAN}Deploying to host ${hostNames[$1]}${NOCOLOR}"

    #run the initialRemoteSetup function in the remote host
    code="$(typeset -f initialRemoteSetup); initialRemoteSetup "$1""
    # echo "${code}"
    # if ["$2" -eq "1"]; then
    sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "${code}"
    # fi

    #rm the debugInfo file on the remote host
    # sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "rm ${hostDeployDirs[$1]}/debugInfo.csv" 

    # copy over source file
    #need to copy this catkin workspace to the host deploy directory
    #the workspace is the 2nd level up from the script location
    arg1="${PWD}/.."
    arg2="${hostUsers[$1]}@${hostNames[$1]}:${hostDeployDirs[$1]}"
    echo "Copying src files from ${arg1} to ${arg2}"

    #clean the local dir
    # sshpass -p "${hostPwords[$1]}" rm -rf ${arg2}${localDirsToCpy[0]}
    # if sshpass command not found, need to install it on that machine
    # sshpass -p "${hostPwords[$1]}" scp -r ${arg1}${localDirsToCpy[0]} ${arg2}
    #instead of scp use rsync with the -a: archive mode, -v: verbose, -z: compress file data during the transfer, -e: specify the remote shell to use (ssh), -P: to show progress

    rsync -avzP --delete -e "sshpass -p ${hostPwords[$1]} ssh" ${arg1}${localDirsToCpy[0]} ${arg2}
    
    #need to ssh into the host and run "catkin build <package>"
    #use ssh password to login
    # # echo $(typeset -g)
    if [ "$2" == "1" ]; then
        buildSuccess=0
        nowDate=$(date -u)
        echo "Building package ${hostPackages[$1]} on host ${hostNames[$1]}"
        sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "cd ${hostDeployDirs[$1]}; $(typeset -f buildRemote); buildRemote "$1" ${hostPackages[$1]}"

        #now check if build was successful by checking if the executable modifed date is newer than the nowDate

        #get the modified date of the executable
        exeName=${hostEXENames[$1]} 
        #exe is stored in the devel/lib/<package> dir
        exeDir=${hostDeployDirs[$1]}/devel/lib/${hostPackages[$1]}

        #get the modified date of the executable
        modDate=$(sshpass -p "${hostPwords[$1]}" ssh "${hostUsers[$1]}@${hostNames[$1]}" "stat -c %y ${exeDir}/${exeName}")

        #convert the modDate to a date object
        #if modDate is empty, then the build failed so set buildSuccess to 0 and return 0
        if [ -z "${modDate}" ]; then
            buildSuccess=0
        else
            modDate=$(date -d "${modDate}" -u)

            echo "modDate: ${modDate}"

            #compare the modified date of the executable to the nowDate

            #if the modDate is newer than the nowDate, then the build was successful
            if [ "${modDate}" \> "${nowDate}" ]; then
                buildSuccess=1
                echo -e "${GREEN}Build Successful!${NOCOLOR}"

                #send the debugInfo file to the remote host
                sendDebugInfo $1
            fi
        fi

        if [ "${buildSuccess}" == "0" ]; then
            echo -e "${RED}Build Failed!${NOCOLOR}"
            echo -e "${YELLOW}This could be becasue no changes were made since the last build, or the build failed.${NOCOLOR}"
        fi 

        return $buildSuccess

    fi
}

checkIfEmpty=0
# if [$1 == "scratch"]; then
#     checkIfEmpty=1;
# fi
deploy=1
reboot=1

if [ "$1" == "-cp" ]; then
    echo -e "${CYAN}Copying files only${NOCOLOR}"
    deploy=0
    reboot=0
fi

#if first flag is -dep, then deploy to the hosts
if [ "$1" == "-dep" ]; then
    echo -e "${CYAN}Copying files and building code on remote hosts${NOCOLOR}"

    deploy=1
    reboot=0
fi

#if no flags are given, then deploy and reboot
if [ "$1" == "-depR" ]; then
    echo -e "${CYAN}Running full deploy: copy, build, and reboot${NOCOLOR}"

    deploy=1
    reboot=1
fi


# TODO: add back pi:
# for I in 0 1
# do
#     deployToHost ${I} "${cpyOnly}"
#     if [ "${cpyOnly}" == "0" ]; then
#         rebootHost ${I}
#     fi
# done

source helpers/onlyFlagParse.sh

if [ -n "$only_host" ]; then
    echo "Deploying to host ${only_host}"
    deployToHost ${only_host} "${deploy}"
    built=$?

    if [ "${reboot}" == "1" ]; then
        if [ "${buildSuccess}" == "1" ]; then
            rebootHost ${only_host}
        else
            #tell user not rebooting because build failed (in yellow)
            echo -e "${YELLOW}Not rebooting because build failed.${NOCOLOR}"
        fi
        
    fi
else
    #for each host in the hostsToUpdate array
    for i in "${!hostNames[@]}"; do
        deployToHost $i "${deploy}"
        built=$?

        if [ "${buildSuccess}" == "1" ]; then
            if [ "${reboot}" == "1" ]; then
                rebootHost $i
            fi
        fi
    done
fi
