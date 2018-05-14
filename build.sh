#!/usr/bin/env bash

cd server_src
make clean; make
mv server ..
cd ../client_src
make clean; make
mv client ..

