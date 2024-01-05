#!/bin/bash

g++ circuit.cpp -o circuit
printf "\033[33moriginal version: \033[0m\n"
./circuit

g++ circuit_mpi.cpp -o circuit -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version:\033[0m\n"
mpiexec -n 4 ./circuit

