#!/usr/bin/env bash

cd "$(dirname "$0")"

cd ..

mkdir -p bin/linux/release
cp -r assets/data/. bin/linux/release/data
cp -r assets/logs/. bin/linux/release/logs
cp -r assets/res/. bin/linux/release/res
