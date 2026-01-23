#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <number_of_clients>"
  exit 1
fi

CLIENTS=$1
HOST="localhost"
PORT="6667"

if ! [[ "$CLIENTS" =~ ^[0-9]+$ ]] || [ "$CLIENTS" -le 0 ]; then
  echo "Error: number_of_clients must be a positive integer"
  exit 1
fi

echo "Starting $CLIENTS clients to $HOST:$PORT"

# Barrier for simultaneous send
BARRIER=$(mktemp -u)
mkfifo "$BARRIER"

for i in $(seq 1 "$CLIENTS"); do
(
  {
    # wait until all clients are ready
    read _ < "$BARRIER"

    FD=$$
    echo "hall von client No. $i client_fd No. $FD"
  } | nc "$HOST" "$PORT"
) &
done

# Release all clients at once
sleep 1
for i in $(seq 1 "$CLIENTS"); do
  echo "GO" > "$BARRIER"
done

rm "$BARRIER"

echo "All clients sent their message simultaneously."
wait

