#!/bin/bash
printf "\033[1;35m\nMatrix size: 1000 X 1000\033[0m\n\n"

g++ ./shortest-path.cpp -o shortest-path 
printf "\033[33moriginal version: \033[0m\n"
./shortest-path

g++ ./shortest-path-mpi-bcast.cpp -o shortest-path -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version 1 (Bcast): \033[0m\n"
mpiexec -n 4 ./shortest-path

g++ ./shortest-path-mpi-scatter.cpp -o shortest-path -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version 2 (Scatter): \033[0m\n"
mpiexec -n 4 ./shortest-path

g++ ./shortest-path-mpi-sr.cpp -o shortest-path -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version 3 (Blocking send & receive): \033[0m\n"
mpiexec -n 4 ./shortest-path

g++ ./shortest-path-mpi-isr.cpp -o shortest-path -I"$MSMPI_INC" -L"$MSMPI_LIB64" -lmsmpi
printf "\n\033[33mmpi version 4 (Non-Blocking send & receive): \033[0m\n"
mpiexec -n 4 ./shortest-path

rm -rf ./shortest-path