MAKEFILE_DIR    := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
EXAMPLES_DIR    := $(MAKEFILE_DIR)examples
BUILD_DIR       := $(MAKEFILE_DIR)build
TEST_BUILD_DIR  := $(MAKEFILE_DIR)test/build

ARDUINO_CLI ?= arduino-cli
PYTHON      ?= python
CMAKE       ?= cmake
CTEST       ?= ctest

FQBN    ?= SPRESENSE:spresense:spresense
EXAMPLE ?= Devices
PORT    ?= /dev/ttyUSB0
BAUD    ?= 115200

.PHONY: all build upload monitor list validate test clean

all: build

# 例: make build EXAMPLE=Imu
build:
	$(ARDUINO_CLI) compile --fqbn $(FQBN) --library $(MAKEFILE_DIR) \
		--build-path $(BUILD_DIR)/$(EXAMPLE) \
		$(EXAMPLES_DIR)/$(EXAMPLE)

# 例: make upload EXAMPLE=Imu PORT=/dev/ttyUSB0
upload: build
	$(ARDUINO_CLI) upload --fqbn $(FQBN) --port $(PORT) \
		--input-dir $(BUILD_DIR)/$(EXAMPLE) \
		$(EXAMPLES_DIR)/$(EXAMPLE)

# 例: make monitor PORT=/dev/ttyUSB0 BAUD=115200
monitor:
	$(ARDUINO_CLI) monitor --port $(PORT) --config baudrate=$(BAUD)

# ビルド可能なexample一覧を表示する
list:
	@ls $(EXAMPLES_DIR)

validate:
	$(PYTHON) -m json.tool $(MAKEFILE_DIR)library.json > /dev/null

# Fsmのホストユニットテストをビルドして実行する
test:
	$(CMAKE) -B $(TEST_BUILD_DIR) -S $(MAKEFILE_DIR)test -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build $(TEST_BUILD_DIR) --parallel 4
	$(CTEST) --test-dir $(TEST_BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)
