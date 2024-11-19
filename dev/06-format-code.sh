#!/bin/bash

# Find clang-format
clang_format=$(which clang-format-19)

# Check if clang-format is installed
if [ -z "$clang_format" ]; then
  echo "clang-format-19 is not installed. Please install it first."
  exit 1
fi

# Find all .h, .hpp, and .cpp files, excluding those in the vendor/ directory
files=$(find . -path ./vendor -path ./rpkg/src/vendor -path ./pypkg/redatam/vendor -prune -o -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -print)

# Format each file
for file in $files; do
  echo "Formatting $file"
  $clang_format -i "$file"
done

echo "Formatting complete."
