FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y gcc-avr cmake python3-pip
RUN pip3 install conan
RUN apt install -y libgl-dev pkg-config  libx11-dev libx11-xcb-dev libfontenc-dev \
    libice-dev libsm-dev libxau-dev libxaw7-dev libxcomposite-dev libxcursor-dev \
    libxdamage-dev libxdmcp-dev libxext-dev libxfixes-dev libxft-dev libxi-dev \
    libxinerama-dev libxkbfile-dev libxmu-dev libxmuu-dev libxpm-dev libxrandr-dev \
    libxrender-dev libxres-dev libxss-dev libxt-dev libxtst-dev libxv-dev libxvmc-dev \
    libxxf86vm-dev xtrans-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev \
    libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev \
    libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev \
    xkb-data libxcb-dri3-dev uuid-dev libxcb-util-dev
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --parallel 12
