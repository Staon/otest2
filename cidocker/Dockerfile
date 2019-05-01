FROM debian:stretch
WORKDIR /otest2
RUN apt-get update && apt-get install -y scons libncurses5-dev g++ \
    tar gzip git openssh-client ca-certificates gnupg
COPY . /otest2
RUN gunzip -c /otest2/clang_install.tar.gz | tar -x -v -f- -p -C/usr/local
RUN rm -Rf /otest2
CMD ["sleep", "infinity"]
