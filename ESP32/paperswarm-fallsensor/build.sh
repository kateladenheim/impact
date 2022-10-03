: ${CHIP:=esp8266}
: ${BOARD:=generic}
: ${SRC:=src}

podman run --rm \
    --userns keep-id \
    -v ./lib:/esp/libs \
    -v ./"$SRC":/esp/src \
    -v ./bin:/esp/bin \
    -e BIN_PATH=/esp/bin \
    -e CHIP=$CHIP \
    -e BOARD=$BOARD \
    git.uclalemur.com:5050/mehtank/espmake:latest \
    "$@"
