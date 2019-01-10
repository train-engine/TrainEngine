#!/bin/bash

cd "`dirname "$0"`"

cd ..

rm -rf bin/windows64/debug/*.exe
rm -rf bin/windows32/debug/*.exe
rm -rf bin/macos/Debug/*
find bin/linux/debug -type f  ! -name "."  -delete
rm -rf bin/ios/Debug/*
rm -rf bin/ios/Debug-iphonesimulator/*
rm -rf bin/android/TrainEngine-debug-unaligned.apk
rm -rf bin/android/TrainEngine-debug-unaligned.apk.d
rm -rf bin/android/TrainEngine-debug.apk
