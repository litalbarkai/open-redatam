#!/bin/bash

for file in downloads/*.zip; do
  filename=$(basename "$file" .zip)
  if [ ! -d "downloads/redatam/$filename" ]; then
    mkdir -p "downloads/redatam/$filename"
    unzip "$file" -d "downloads/redatam/$filename"
  fi
done

unzip downloads/redatam/CP1991ARG/datos.zip -d downloads/redatam/CP1991ARG/
rm downloads/redatam/CP1991ARG/datos.zip
rm downloads/redatam/CP1991ARG/*.exe

for file in $(find downloads -type f -name "*.dicx"); do
  dirname=$(dirname "$file")  
  dirname=$(echo "$dirname" | cut -d'/' -f1,2,3)
  dirname="$dirname/open-redatam-dicx-to-csv/"
  mkdir -p "$dirname"
  
  if [ $(ls -1 "$dirname" | wc -l) -eq 0 ]; then
    echo "Converting $file..."
    ./redatam "$file" "$dirname"

    for csv in $(find "$dirname" -type f -name "*.csv"); do
      gzip "$csv"
    done
  fi
done

for file in $(find downloads -type f -name "*.dic"); do
  dirname=$(dirname "$file")  
  dirname=$(echo "$dirname" | cut -d'/' -f1,2,3)
  dirname="$dirname/open-redatam-dic-to-csv/"
  mkdir -p "$dirname"
  
  if [ $(ls -1 "$dirname" | wc -l) -eq 0 ]; then
    echo "Converting $file..."
    ./redatam "$file" "$dirname"

    for csv in $(find "$dirname" -type f -name "*.csv"); do
      gzip "$csv"
    done
  fi
done
