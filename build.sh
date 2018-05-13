#!/usr/bin/env bash

cd server
make clean; make
mv server ..
cd ../client
make clean; make
mv client ..

