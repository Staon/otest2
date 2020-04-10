#!/bin/bash

docker_args=--no-cache

develop_image=staon/otest2-ci:latest

# -- The development image
cp ../otest2/otest2_discovery.sh .
docker build "$docker_args" -t "$develop_image" -f dockerfile.ci . || exit 1

