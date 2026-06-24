#!/bin/bash
cd "/c/CODE/New folder/StudentRecordSystem"
/ucrt64/bin/g++ -std=c++17 -O2 -o StudentRecordSystem.exe Student.o StudentManager.o Utils.o main.o > link_out.txt 2>&1
echo "EXIT: $?" >> link_out.txt
