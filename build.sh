#!/bin/bash

make clean
rm server
make EXEC=server

rm client
make clean
make EXEC=client
