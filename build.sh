#!/usr/bin/env bash

cd server
make clean; make
mv server.out ..
cd ../client
make clean; make
mv client.out ..

