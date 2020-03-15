#!/bin/bash

docker_args=--no-cache

develop_image=staon/otest2-ci:latest

# -- The development image
docker build "$docker_args" -t "$develop_image" -f dockerfile.ci . || exit 1

