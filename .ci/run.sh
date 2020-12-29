#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add cyberduckninja https://api.bintray.com/conan/cyberduckninja/conan
#- mkdir build && cd build
conan install . --build=missing #-s build_type=Release --build=missing #--install-folder=build
cmake . -DCMAKE_BUILD_TYPE=Release -DHTTP_COMPONENT=ON -DALLOW_EXAMPLES=ON
cmake --build . -- VERBOSE=1