@echo off

cmake -B build -G Ninja

cmake --build build

cd build

echo =========================RUN=========================

main.exe