#!/bin/bash

docker_args=--no-cache

develop_image=otest2-develop-env

# -- The development image
docker build "$docker_args" -t "$develop_image" -f dockerfile.devel \
	--build-arg UID=$(id -u) --build-arg GID=$(id -g) \
	--build-arg DOCKER_GID=$(getent group docker | cut -d: -f3) \
	--build-arg UNAME="$USER" . || exit 1

