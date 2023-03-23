#!/bin/bash

# cd "$(dirname $0)"

csv_file="config/hosts.csv"

# Read the CSV file and remove the header
mapfile -t lines < <(tail -n +2 "$csv_file")

# Initialize arrays
hostNames=()
hostUsers=()
hostPwords=()
hostDeployDirs=()
hostPackages=()
hostEXENames=()

# Loop through the lines and populate the arrays
for line in "${lines[@]}"; do
    IFS=',' read -ra fields <<< "$line"
    hostNames+=("${fields[0]}")
    hostUsers+=("${fields[1]}")
    hostPwords+=("${fields[2]}")
    hostDeployDirs+=("${fields[3]}")
    hostPackages+=("${fields[4]}")
    hostEXENames+=("${fields[5]}")
done

export hostNames
export hostUsers
export hostPwords
export hostDeployDirs
export hostPackages
export hostEXENames