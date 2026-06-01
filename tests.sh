#!/bin/bash

rootDir=$(pwd)

cd build/src/metric_impl || exit 1
./metric_test || exit 1

cd ${rootDir}
cd build/src/metric_accumulator_impl || exit 1
./metric_accumulator_test || exit 1


