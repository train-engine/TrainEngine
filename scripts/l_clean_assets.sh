#!/bin/bash

cd "`dirname "$0"`"

cd ..

rm -rf bin/windows64/debug/data
rm -rf bin/windows64/debug/logs
rm -rf bin/windows64/debug/res
rm -rf bin/windows64/debug/openal32.dll

rm -rf bin/windows64/release/data
rm -rf bin/windows64/release/logs
rm -rf bin/windows64/release/res
rm -rf bin/windows64/release/openal32.dll

rm -rf bin/windows32/debug/data
rm -rf bin/windows32/debug/logs
rm -rf bin/windows32/debug/res
rm -rf bin/windows32/debug/openal32.dll

rm -rf bin/windows32/release/data
rm -rf bin/windows32/release/logs
rm -rf bin/windows32/release/res
rm -rf bin/windows32/release/openal32.dll

rm -rf bin/linux/debug/data
rm -rf bin/linux/debug/logs
rm -rf bin/linux/debug/res

rm -rf bin/linux/release/data
rm -rf bin/linux/release/logs
rm -rf bin/linux/release/res