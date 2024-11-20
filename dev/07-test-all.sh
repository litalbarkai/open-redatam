#!/bin/sh

# cmd

echo "====================================="
echo "===== TESTING COMMAND LINE TOOL ====="
echo "====================================="

make clean

make

./redatam test/uru2011mini/uru2011mini.dic test/uru2011mini/dic-to-csv

./redatam test/uru2011mini/uru2011mini.dicx test/uru2011mini/dicx-to-csv

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

# r

echo "========================="
echo "===== TESTING R PKG ====="
echo "========================="

cd rpkg

Rscript -e "devtools::clean_dll(); devtools::check()"

cd ..

# py

echo "=========================="
echo "===== TESTING PY PKG ====="
echo "=========================="

cd pypkg

source venv/bin/activate
rm -rf build dist redatam.egg-info
pip install --use-pep517 .
python tests/basic-test.py
deactivate

cd ..
