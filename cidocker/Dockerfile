FROM debian:stretch AS clang_tmp
RUN apt-get update && apt-get install -y tar gzip
COPY clang_install.tar.gz /otest2/clang_install.tar.gz
RUN gunzip -c /otest2/clang_install.tar.gz | tar -x -v -f- -p -C/usr/local

FROM debian:stretch AS otest2-ci
RUN apt-get update && apt-get install -y scons libncurses5-dev g++ \
    gdbserver git openssh-client ca-certificates gnupg
COPY --from=clang_tmp /usr/local /usr/local
CMD ["sleep", "infinity"]
