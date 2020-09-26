#!/usr/bin/env bash
#
# Copyright (c) 2018 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C

travis_retry pip install codespell==1.13.0
travis_retry pip install flake8==3.5.0
travis_retry pip install vulture==0.29

SHELLCHECK_VERSION=v0.6.0
 DOCKER_ID=$(docker run $DOCKER_ADMIN -idt --mount type=bind,src=$TRAVIS_BUILD_DIR,dst=$TRAVIS_BUILD_DIR --mount type=bind,src=$CCACHE_DIR,dst=$CCACHE_DIR --mount type=bind,src=$PARAMS_DIR,dst=$PARAMS_DIR -w $TRAVIS_BUILD_DIR --env-file /tmp/env $DOCKER_NAME_TAG)
export PATH="/tmp/shellcheck-${SHELLCHECK_VERSION}:${PATH}"
