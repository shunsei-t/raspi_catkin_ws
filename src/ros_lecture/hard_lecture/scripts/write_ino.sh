echo "writing arduino"
echo "connect arduino on USB"

cd `dirname ${0}`/../platformio/uno
platformio run
