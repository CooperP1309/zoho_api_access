#!/bin/bash

echo "Building project..."

g++ src/main.cpp -o app.exe -lcurl -lws2_32

echo "

Compiler returned
"