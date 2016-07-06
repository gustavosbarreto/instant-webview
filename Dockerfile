FROM ubuntu:trusty

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    apt-add-repository -y ppa:beineri/opt-qt561-trusty && \
    apt-add-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-6 g++-6 build-essential mesa-common-dev libglu1-mesa-dev xvfb python && \
    apt-get install -y qt56tools qt56script qt56webengine qt56webchannel qt56websockets qt56declarative qt56x11extras

RUN rm -rf /var/lib/apt/lists/*

ENV QTDIR=/opt/qt56
ENV PATH=$QTDIR/bin:$PATH
ENV LD_LIBRARY_PATH=$QTDIR/lib/x86_64-linux-gnu:$QTDIR/lib:$LD_LIBRARY_PATH
ENV PKG_CONFIG_PATH=$QTDIR/lib/pkgconfig:$PKG_CONFIG_PATH

ADD . /usr/src/instant-webview

RUN mkdir /tmp/build

WORKDIR /tmp/build

RUN qmake PREFIX=/usr/local QMAKE_CXX="g++-6" /usr/src/instant-webview
RUN make -j$(nproc)
RUN make install

ENTRYPOINT ["/usr/src/instant-webview/scripts/docker-entrypoint.sh"]
