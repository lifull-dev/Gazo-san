FROM rhikimochi/opencv-docker:v0.14

COPY . /run
WORKDIR /run
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make
RUN mv bin/gazosan /usr/local/bin
RUN rm -r /run/*