pio := "pio"

build-probe:
    {{pio}} run -e probe

upload-probe:
    {{pio}} run -e probe --target upload

monitor:
    {{pio}} device monitor --baud 115200
