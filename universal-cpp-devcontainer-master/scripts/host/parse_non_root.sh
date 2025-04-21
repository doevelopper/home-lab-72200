#!/bin/sh

# change pwd to directory, where this script is stored
: ${curdir:="$(dirname -- $(readlink -f -- "$0"))"}

# parse IDs to non-root target of Dockerfile
sed --in-place \
    -e"s/^ARG USER_ID.*/ARG USER_ID=$(id -u)/g" \
    -e"s/^ARG USER_NAME.*/ARG USER_NAME=$(id -un)/g" \
    -e"s/^ARG USER_GID.*/ARG USER_GID=$(id -g)/g" \
    -e"s/^ARG USER_GROUP.*/ARG USER_GROUP=$(id -gn)/g" \
    $curdir/../../.devcontainer/Dockerfile
