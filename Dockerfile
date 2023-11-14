FROM ubuntu:20.04

LABEL org.opencontainers.image.created="2022-11-30"
LABEL org.opencontainers.image.url="https://github.com/odelaneau/GLIMPSE"
LABEL org.opencontainers.image.version="2.0.0"
LABEL org.opencontainers.image.licences="MIT"
LABEL org.opencontainers.image.title="RESHAPE"
LABEL org.opencontainers.image.authors="theo.cavinato@unil.ch"

RUN apt-get update && apt-get install -y wget

RUN wget https://github.com/TheoCavinato/RESHAPE/releases/download/v1.0.0/haploshuffling_static && \
cp haploshuffling_static bin/ && \
chmod +x /bin/haploshuffling_static && \
rm haploshuffling_static

WORKDIR /root/

