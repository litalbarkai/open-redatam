#!/bin/bash

# Find clang-format and clang-tidy
clang_format=$(which clang-format-19)
clang_tidy=$(which clang-tidy-19)

# Check if clang-format is installed
if [ -z "$clang_format" ]; then
  echo "clang-format-19 is not installed. Please install it first."
  exit 1
fi

# Check if clang-tidy is installed
if [ -z "$clang_tidy" ]; then
  echo "clang-tidy-19 is not installed. Please install it first."
  exit 1
fi

# Find relevant files, excluding specific directories
files=$(find . \( -path './dev' -o -path './vendor' -o -path './rpkg' -o -path './pypkg' -o -path './gui' \) -prune -o \
        -type f \( -name '*.hpp' -o -name '*.cpp' \) -print)

# Format each file with clang-format
for file in $files; do
  echo "Formatting $file"
  $clang_format -i "$file"
done

# Include directories (using absolute paths)
include_dirs=(
  "-I$(realpath ./include)"
  "-I$(realpath ./include/database)"
  "-I$(realpath ./include/entities)"
  "-I$(realpath ./include/exporters)"
  "-I$(realpath ./include/readers)"
  "-I$(realpath ./include/utils)"
  "-I$(realpath ./src)"
  "-I$(realpath ./src/database)"
  "-I$(realpath ./src/entities)"
  "-I$(realpath ./src/exporters)"
  "-I$(realpath ./src/readers)"
  "-I$(realpath ./src/utils)"
  "-I$(realpath ./vendor/pugixml)"
)

# Apply clang-tidy to each .cpp file
for file in $files; do
  if [[ $file == *.cpp ]]; then
    echo "Running clang-tidy on $file"
    $clang_tidy "$file" --extra-arg=-v -- -std=c++11 ${include_dirs[@]}
  fi
done

echo "Formatting and clang-tidy checks complete."
