#!/bin/bash

make clean

make -f Makefile-debug

mkdir -p test/uru2011mini

valgrind --leak-check=full --track-origins=yes --log-file=test/uru2011mini/log-dic-to-csv.txt ./redatam test/uru2011mini/uru2011mini.dic test/uru2011mini/dic-to-csv

valgrind --leak-check=full --track-origins=yes --log-file=test/uru2011mini/log-dicx-to-csv.txt ./redatam test/uru2011mini/uru2011mini.dicx test/uru2011mini/dicx-to-csv
