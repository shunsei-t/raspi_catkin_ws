echo "writing arduino"
echo "connect arduino on USB"

cd `dirname ${0}`/../platformio/arduino_uno
platformio run
