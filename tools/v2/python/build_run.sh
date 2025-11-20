#!/bin/bash

build_image() {
    docker build --no-cache -t robotgl_python .
}

run_container() {
    docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v "$PWD"/app:/app robotgl_python
}

if [[ "$1" == "clean" ]]; then
    docker builder prune -af
    docker image prune -af
elif [[ "$1" == "build" ]]; then
    build_image
elif [[ "$1" == "run" ]]; then
    run_container
else
    build_image
    run_container
fi
