#!/bin/bash
echo "Починаємо білд проекту..."
mkdir -p build
cd build
cmake ..
make
echo "Білд завершено!"
