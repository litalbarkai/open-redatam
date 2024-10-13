#!/bin/bash

make -f Makefile-debug

valgrind --leak-check=full --track-origins=yes --log-file=test/galapagos/log-dic-to-csv.txt ./redatam test/galapagos/cg15.dic test/galapagos/dic-to-csv

valgrind --leak-check=full --track-origins=yes --log-file=test/galapagos/log-dicx-to-csv.txt ./redatam test/galapagos/cg15.dicX test/galapagos/dicx-to-csv
