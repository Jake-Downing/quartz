#!/usr/bin/bash

nasm -felf64 output.asm
wait 

ld output.o -o test
wait 

./test
echo $?