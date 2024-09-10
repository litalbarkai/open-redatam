#!/bin/bash

for file in downloads/*.zip; do
  filename=$(basename "$file" .zip)
  mkdir -p "downloads/$filename"
  if [ ! -d "downloads/$filename" ]; then
    unzip "$file" -d "downloads/$filename"
  fi
done

for file in $(find downloads -type f -name "*.dicx"); do
  dirname=$(dirname "$file")
  dirname=$(echo "$dirname" | cut -d'/' -f1,2)
  dirname="$dirname/redatam-converter-cpp/"
  mkdir -p "$dirname"
  
  # if the directory has 0 files, then convert the file
  if [ $(ls -1 "$dirname" | wc -l) -eq 0 ]; then
    echo "Converting $file..."
    ./redatam "$file" "$dirname"
  fi
done
