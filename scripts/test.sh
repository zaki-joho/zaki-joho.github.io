#!/bin/bash

set -eux

DIR=${PWD}

function download_aoj() {
    echo download: ${1}
    URL=http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=${1}
    if [ ! -d ${1} ]; then
        oj download --system -s -d ${1} ${URL} || true
    fi
}

declare -A TESTS

TESTS=(
    ["dinic_test.cpp"]="GRL_6_A"
    ["dinic_with_karzanov_test.cpp"]="GRL_6_A"
    ["edmonds_karp_test.cpp"]="GRL_6_A"
    ["ford_fulkerson_test.cpp"]="GRL_6_A"
    ["fujishige_maxflow_test.cpp"]="GRL_6_A"
    ["push_relabel_test.cpp"]="GRL_6_A"
)

for TEST in ${!TESTS[@]}; do
    ID=${TEST:0:-4}
    echo ${ID} ${TESTS[$TEST]}
    mkdir -p ${TESTS[$TEST]}
    pushd ${TESTS[$TEST]}
    download_aoj ${TESTS[$TEST]}
    g++ -std=c++17 -O2 -Wall -Wextra -Wshadow ${DIR}/test/$TEST -o a.out
    oj test --tle 10 -d ${TESTS[$TEST]}
    popd
done
