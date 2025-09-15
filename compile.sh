if [[ "$1" == "clean" ]]; then
    rm -rf build
fi
cmake -S . -B build
cmake --build build