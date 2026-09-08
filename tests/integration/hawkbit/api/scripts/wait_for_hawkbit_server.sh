#!/usr/bin/env bash
# This script is used inside CI to wait for HawkBit container to start

HOST="localhost"
PORT="8080"

for ((i = 0; i < 60; i++)); do
    if wget "$HOST:$PORT" -O /dev/null 2>/dev/null; then
        exit 0
    fi

    sleep 1
done

exit 1
