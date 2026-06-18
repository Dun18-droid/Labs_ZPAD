#!/bin/bash
echo "Встановлення залежностей..."
sudo apt update
sudo apt install libopencv-dev cmake gcc g++ make wget -y
chmod +x build.sh run.sh

echo "Завантаження моделей для Face Detection..."
wget -qnc https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -qnc https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "Готово!"