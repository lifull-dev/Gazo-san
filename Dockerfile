FROM rhikimochi/opencv-docker:v0.14

COPY . /run
WORKDIR /run
RUN g++ -std=c++11 ./src/main.cpp -L./ -limageDiffCalc -o gazosan `pkg-config --libs opencv`
RUN mv gazosan /usr/local/bin
RUN rm -r /run/*