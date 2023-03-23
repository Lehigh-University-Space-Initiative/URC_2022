#!/bin/bash

#flage usage: -01 or -02

# Initialize the variable for the --only flag
only_host=""

# Parse the flags
while getopts ":o:" opt; do
  case $opt in
    o)
      only_host="$OPTARG"
      ;;
    \?)
    #   echo "Invalid option: -$OPTARG" >&2
    #   exit 1
    #   ignore it
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

export only_host