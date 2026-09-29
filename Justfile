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

package-m5burner:
    tools/package-m5burner.sh

test-native:
    {{pio}} test -d packages/step-trigger -e native
    {{pio}} test -e native
    tools/test-step-trigger-consumer.sh

upload-sequencer:
    {{pio}} run -e drum-step-sequencer --target upload

monitor:
    {{pio}} device monitor --baud 115200
