FROM ubuntu:24.04

RUN $(grep -qi 'avx2' /proc/cpuinfo) || (echo 'AVX2 not supported by this machine. Aborting docker build.'>&2; exit 1)

RUN apt-get update && apt-get -y upgrade
RUN apt-get install --assume-yes --no-install-recommends ca-certificates build-essential cmake git autoconf
RUN apt-get install --assume-yes --no-install-recommends libboost-regex-dev libsqlite3-dev
RUN apt-get install --assume-yes --no-install-recommends texlive-latex-extra texlive-fonts-recommended texlive-latex-recommended texlive-fonts-extra
RUN apt-get install --assume-yes --no-install-recommends libtbb-dev libxxhash-dev curl

RUN curl https://sh.rustup.rs -sSf | bash -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

# Build sqlplot-tools
RUN git clone https://github.com/bingmann/sqlplot-tools.git /opt/sqlplot-tools
RUN mkdir /opt/sqlplot-tools/build
WORKDIR /opt/sqlplot-tools/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DWITH_POSTGRESQL=OFF -DWITH_MYSQL=OFF ..
RUN cmake --build . -j 8

# Build Experiments
COPY . /opt/mphf_experiments
RUN mkdir /opt/mphf_experiments/build
WORKDIR /opt/mphf_experiments/build
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cmake --build . -j 8

# Actual benchmark
CMD bash /opt/dockerVolume/pareto.sh
