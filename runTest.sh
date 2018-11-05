#! /bin/bash
#Run me in the top level submission directory
echo "Cleaning executables and other files"
make clean
echo "Remaking executable"
make circuits
echo "Moving into testfiles"
cd testfiles/

echo "--Testing default Zero value wire--"
../circuits < ZeroTest

echo "--Testing default One value wire--"
../circuits < OneTest

echo "--NOT test--"
../circuits < NOTtest

echo "--AND test--"
../circuits < ANDtest

echo "--NAND test--"
../circuits < NANDtest

echo "--OR test--"
../circuits < ORtest

echo "--NOR test--"
../circuits < NORtest

echo "--XOR test--"
../circuits < XORtest

echo "--EQ test--"
../circuits < EQtest

echo "--NOT & 3 XOR, unstable--"
../circuits < NotXor
