: ${UPLOAD_SPEED:=921600}
: ${UPLOAD_PORT:=/dev/ttyUSB0}
: ${CHIP:=esp8266}
: ${UPLOAD_RESET:=}

esptool --baud=${UPLOAD_SPEED} --port ${UPLOAD_PORT} --chip ${CHIP} ${UPLOAD_RESET} write_flash 0x00000 "$@"
