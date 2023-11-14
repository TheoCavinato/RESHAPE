FROM ubuntu:20.04

LABEL org.opencontainers.image.created="2022-11-30"
LABEL org.opencontainers.image.url="https://github.com/odelaneau/GLIMPSE"
LABEL org.opencontainers.image.version="2.0.0"
LABEL org.opencontainers.image.licences="MIT"
LABEL org.opencontainers.image.title="RESHAPE"
LABEL org.opencontainers.image.authors="theo.cavinato@unil.ch"

#COPY RESHAPE RESHAPE_DOCKER/
COPY RESHAPE/bin/haploshuffling_static RESHAPE_DOCKER/bin/

RUN mv RESHAPE_DOCKER/bin/haploshuffling_static /bin
RUN chmod +x /bin/*
RUN rm -rf RESHAPE_DOCKER

WORKDIR /root/

