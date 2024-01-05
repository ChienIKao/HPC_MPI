#!/bin/bash

g++ prime.cpp -o prime
printf "\033[33moriginal version: \033[0m\n"
./prime

g++ prime_mpi.cpp -o prime -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version:\033[0m\n"
mpiexec -n 4 ./prime
