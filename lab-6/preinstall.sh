#!/bin/bash
echo "Встановлення залежностей..."
sudo apt update
sudo apt install libopencv-dev cmake gcc g++ make -y
chmod +x build.sh run.sh
echo "Готово!"
