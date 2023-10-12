#!/bin/bash

# Define the source and destination directories
src_dir="./"

# Get the current directory's name
current_dir_name=$1

# Get the username
username=$(whoami)

# Define the destination directory
dest_dir="../${username}-${current_dir_name}"

rm -rf $dest_dir

# Create the destination directory if it doesn't exist
mkdir -p $dest_dir

# Use rsync to copy the files, excluding the specified directories
rsync -av --progress $src_dir $dest_dir \
    --exclude venv \
    --exclude docs \
    --exclude 'cmake-build-*' \
    --exclude .git \
    --exclude .github \
    --exclude .DS_Store \
    --exclude .idea

zip -r $dest_dir.zip $dest_dir
echo "Sanitized copy created successfully."
