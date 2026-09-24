set dotenv-load := true

pio := "pio"

default:
    @just --list

build-probe:
    {{pio}} run -e probe

upload-probe:
    {{pio}} run -e probe --target upload

build-sequencer:
    {{pio}} run -e drum-step-sequencer

upload-sequencer:
    {{pio}} run -e drum-step-sequencer --target upload

monitor:
    {{pio}} device monitor --baud 115200
