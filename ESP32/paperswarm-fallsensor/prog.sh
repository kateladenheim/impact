: ${UPLOAD_PORT:=/dev/ttyUSB0}

INO="$1.ino"
BIN="bin/$1.bin"

shift

build.sh "$INO" "$@" && \
flash.sh "$BIN" && \
echo "=== SERIAL MONITOR DUMP ===" && \
miniterm.py "$UPLOAD_PORT" 115200
