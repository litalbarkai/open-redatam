#!/bin/bash

make clean

make -f Makefile-debug

valgrind --leak-check=full --track-origins=yes --log-file=test/uru2011mini/log-dic-to-csv.txt ./redatam test/uru2011mini/uru2011mini.dic test/uru2011mini/dic-to-csv

valgrind --leak-check=full --track-origins=yes --log-file=test/uru2011mini/log-dicx-to-csv.txt ./redatam test/uru2011mini/uru2011mini.dicx test/uru2011mini/dicx-to-csv

validate_csv() {
  local file=$1
  local expected_lines=$2
  local expected_columns=$3

  # Get the number of lines
  actual_lines=$(wc -l < "$file")
  # Get the number of columns (assuming the first line contains the headers)
  actual_columns=$(head -n 1 "$file" | awk -F';' '{print NF}')

  if [[ "$actual_lines" -eq "$expected_lines" && "$actual_columns" -eq "$expected_columns" ]]; then
    echo "Validation passed for $file"
  else
    echo "Validation failed for $file: expected $expected_lines lines and $expected_columns columns, but got $actual_lines lines and $actual_columns columns"
    exit 1
  fi
}

validate_csv "test/uru2011mini/dic-to-csv/SEXO.csv" 38 4
validate_csv "test/uru2011mini/dicx-to-csv/SEXO.csv" 38 4
