#!/bin/bash
printf "\033[1;35m\nMatrix size: 1000 X 1000\033[0m\n\n"

g++ ./matrix-calculation.cpp -o matrix-calculation 
printf "\033[33moriginal version: \033[0m\n"
./matrix-calculation

g++ ./matrix-calculation-mpi-row.cpp -o matrix-calculation -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version 1 (row): \033[0m\n"
mpiexec -n 4 ./matrix-calculation

rm -rf ./matrix-calculation