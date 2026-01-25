#!/usr/bin/env bash

set -euo pipefail

CLIENTS="${1:-100}"
HOST="${HOST:-localhost}"
PORT="${PORT:-6667}"
PASS="${PASS:-123}"
CHANNEL="${CHANNEL:-#test}"
JOIN_TIMEOUT="${JOIN_TIMEOUT:-5}"
READY_TIMEOUT="${READY_TIMEOUT:-10}"
READ_TIMEOUT="${READ_TIMEOUT:-0.2}"
NC_QUIT_DELAY="${NC_QUIT_DELAY:-1}"

if ! [[ "$CLIENTS" =~ ^[0-9]+$ ]] || [ "$CLIENTS" -le 0 ]; then
  echo "Error: number_of_clients must be a positive integer"
  exit 1
fi

echo "Starting $CLIENTS clients to $HOST:$PORT"
echo "PASS=$PASS CHANNEL=$CHANNEL JOIN_TIMEOUT=${JOIN_TIMEOUT}s READY_TIMEOUT=${READY_TIMEOUT}s NC_QUIT_DELAY=${NC_QUIT_DELAY}s"

# Barriers for simultaneous login and send
LOGIN_FLAG="$(mktemp -u)"
SEND_FLAG="$(mktemp -u)"
READY_FIFO="$(mktemp -u)"
mkfifo "$READY_FIFO"
BARRIER_POLL="${BARRIER_POLL:-0.02}"

cleanup() {
  rm -f "$LOGIN_FLAG" "$SEND_FLAG" "$READY_FIFO"
}
trap cleanup EXIT

# Keep READY fifo open so writers don't block if reader isn't ready yet.
exec 9<> "$READY_FIFO"

for i in $(seq 1 "$CLIENTS"); do
(
  while [ ! -f "$LOGIN_FLAG" ]; do sleep "$BARRIER_POLL"; done
  coproc NC { nc -N -q "$NC_QUIT_DELAY" "$HOST" "$PORT"; }
  exec 3>&"${NC[1]}"
  exec 4<&"${NC[0]}"

  printf "PASS %s\r\n" "$PASS" >&3
  printf "NICK nc%03d\r\n" "$i" >&3
  printf "USER nc%03d 0 * :nc%03d\r\n" "$i" "$i" >&3
  printf "JOIN %s\r\n" "$CHANNEL" >&3

  joined=0
  deadline=$((SECONDS + JOIN_TIMEOUT))
  while [ $SECONDS -lt $deadline ]; do
    if read -r -t "$READ_TIMEOUT" line <&4; then
      printf '%s\n' "$line"
      if [[ "$line" == 366*" $CHANNEL"* ]] || [[ "$line" == *" 366 "*"$CHANNEL"* ]]; then
        joined=1
        break
      fi
    fi
  done

  if [ "$joined" -ne 1 ]; then
    echo "WARN: nc$(printf '%03d' "$i") JOIN timeout" >&2
  fi
  echo "READY" > "$READY_FIFO"

  # Drain output so server sends don't block on full socket buffers.
  cat <&4 &
  DRAIN_PID=$!

  while [ ! -f "$SEND_FLAG" ]; do sleep "$BARRIER_POLL"; done
  printf "PRIVMSG %s :hello from nc%03d\r\n" "$CHANNEL" "$i" >&3

  # Close write end so nc can exit cleanly.
  exec 3>&-
  sleep 0.1
  kill "$NC_PID" "$DRAIN_PID" 2>/dev/null || true
  wait "$NC_PID" 2>/dev/null || true
  wait "$DRAIN_PID" 2>/dev/null || true
) &
done

# Release all clients to login at once
sleep 1
touch "$LOGIN_FLAG"

# Wait until all clients report JOIN (or timeout) before send
ready_count=0
ready_deadline=$((SECONDS + READY_TIMEOUT))
while [ "$ready_count" -lt "$CLIENTS" ]; do
  if read -r -t "$READ_TIMEOUT" _ <&9; then
    ready_count=$((ready_count + 1))
  else
    if [ $SECONDS -ge $ready_deadline ]; then
      echo "WARN: only $ready_count/$CLIENTS clients ready after ${READY_TIMEOUT}s"
      break
    fi
  fi
done

# Release all clients to write at once
sleep 1
touch "$SEND_FLAG"

echo "All clients logged in and sent their message simultaneously."
wait
