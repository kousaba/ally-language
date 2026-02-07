#!/bin/bash

set -e
./build.sh

echo "Running ally compiler..."
echo ""
echo ""
echo "----------Result----------"
./build/ally-lang
echo "--------------------------"
echo ""
echo ""
