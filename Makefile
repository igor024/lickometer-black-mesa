# Codex trash but it works (sort of)

PROJECT := lickometer
BUILD_DIR := .build

ARDUINO_SKETCHBOOK ?= $(HOME)/Arduino
ARDUINO_DATA_DIR ?= $(HOME)/.arduino15
ARDUINO_AVR_VERSION ?= 1.8.7

ARDUINO_AVR_DIR := $(ARDUINO_DATA_DIR)/packages/arduino/hardware/avr/$(ARDUINO_AVR_VERSION)
AVR_TOOLS_DIR := $(ARDUINO_DATA_DIR)/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7
AVRDUDE_DIR := $(ARDUINO_DATA_DIR)/packages/arduino/tools/avrdude/8.0.0-arduino1

CC := $(AVR_TOOLS_DIR)/bin/avr-gcc
CXX := $(AVR_TOOLS_DIR)/bin/avr-g++
AR := $(AVR_TOOLS_DIR)/bin/avr-gcc-ar
OBJCOPY := $(AVR_TOOLS_DIR)/bin/avr-objcopy
SIZE := $(AVR_TOOLS_DIR)/bin/avr-size
AVRDUDE := $(AVRDUDE_DIR)/bin/avrdude

BOARD_TAG ?= uno
MCU ?= atmega328p
F_CPU ?= 16000000L
VARIANT ?= standard
PORT ?= /dev/ttyACM0
UPLOAD_SPEED ?= 115200
PROGRAMMER ?= arduino

CORE_DIR := $(ARDUINO_AVR_DIR)/cores/arduino
VARIANT_DIR := $(ARDUINO_AVR_DIR)/variants/$(VARIANT)
WIRE_SRC := $(ARDUINO_AVR_DIR)/libraries/Wire/src
MPR121_SRC := $(ARDUINO_SKETCHBOOK)/libraries/MPR121/src

# Add project libraries here. User libraries should point at their src directory.
USER_LIB_DIRS := $(MPR121_SRC)
PLATFORM_LIB_DIRS := $(WIRE_SRC)

PROJECT_SRCS := main.cpp cap_sensor/cap_sensor.cpp
USER_LIB_SRCS := $(shell find $(USER_LIB_DIRS) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.S' \) 2>/dev/null)
PLATFORM_LIB_SRCS := $(shell find $(PLATFORM_LIB_DIRS) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.S' \) 2>/dev/null)
CORE_SRCS := $(filter-out $(CORE_DIR)/main.cpp,$(wildcard $(CORE_DIR)/*.c $(CORE_DIR)/*.cpp $(CORE_DIR)/*.S))

PROJECT_OBJS := $(addprefix $(BUILD_DIR)/project/,$(addsuffix .o,$(PROJECT_SRCS)))
USER_LIB_OBJS := $(patsubst $(MPR121_SRC)/%,$(BUILD_DIR)/libraries/MPR121/%.o,$(USER_LIB_SRCS))
PLATFORM_LIB_OBJS := $(patsubst $(WIRE_SRC)/%,$(BUILD_DIR)/libraries/Wire/%.o,$(PLATFORM_LIB_SRCS))
CORE_OBJS := $(patsubst $(CORE_DIR)/%,$(BUILD_DIR)/core/%.o,$(CORE_SRCS))

OBJS := $(PROJECT_OBJS) $(USER_LIB_OBJS) $(PLATFORM_LIB_OBJS) $(CORE_OBJS)
DEPS := $(OBJS:.o=.d)

DEFINES := -DF_CPU=$(F_CPU) -DARDUINO=10819 -DARDUINO_AVR_$(shell echo $(BOARD_TAG) | tr '[:lower:]' '[:upper:]') -DARDUINO_ARCH_AVR
WARNINGS := -Wall -Wextra
COMMON_FLAGS := -mmcu=$(MCU) $(DEFINES) -Os -ffunction-sections -fdata-sections -MMD -MP -flto
CFLAGS := $(COMMON_FLAGS) $(WARNINGS) -std=gnu11
CXXFLAGS := $(COMMON_FLAGS) $(WARNINGS) -std=gnu++11 -fpermissive -fno-exceptions -fno-threadsafe-statics
ASFLAGS := $(COMMON_FLAGS) -x assembler-with-cpp
LDFLAGS := -mmcu=$(MCU) -Os -Wl,--gc-sections -flto

# Arduino include paths come before local paths so real library headers win over
# the local stubs MPR121.h and Wire.h.
INCLUDES := \
	-Iarduino_compat \
	-I$(MPR121_SRC) \
	-I$(WIRE_SRC) \
	-I$(CORE_DIR) \
	-I$(VARIANT_DIR) \
	-I. \
	-Icap_sensor

ELF := $(BUILD_DIR)/$(PROJECT).elf
HEX := $(BUILD_DIR)/$(PROJECT).hex

.DEFAULT_GOAL := all

all: $(HEX)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	$(SIZE) --mcu=$(MCU) -C $<

$(ELF): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/project/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/project/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/project/%.S.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/MPR121/%.cpp.o: $(MPR121_SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/MPR121/%.c.o: $(MPR121_SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/MPR121/%.S.o: $(MPR121_SRC)/%.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/Wire/%.cpp.o: $(WIRE_SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/Wire/%.c.o: $(WIRE_SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/libraries/Wire/%.S.o: $(WIRE_SRC)/%.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/core/%.cpp.o: $(CORE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/core/%.c.o: $(CORE_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/core/%.S.o: $(CORE_DIR)/%.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

upload: $(HEX)
	$(AVRDUDE) -C$(AVRDUDE_DIR)/etc/avrdude.conf -v -p$(MCU) -c$(PROGRAMMER) -P$(PORT) -b$(UPLOAD_SPEED) -D -Uflash:w:$(HEX):i

lsp:
	printf '[\n' > compile_commands.json
	printf '  {\n' >> compile_commands.json
	printf '    "directory": "$(CURDIR)",\n' >> compile_commands.json
	printf '    "file": "$(CURDIR)/main.cpp",\n' >> compile_commands.json
	printf '    "command": "$(CXX) $(CXXFLAGS) $(INCLUDES) -c main.cpp -o $(BUILD_DIR)/project/main.cpp.o"\n' >> compile_commands.json
	printf '  },\n' >> compile_commands.json
	printf '  {\n' >> compile_commands.json
	printf '    "directory": "$(CURDIR)",\n' >> compile_commands.json
	printf '    "file": "$(CURDIR)/cap_sensor/cap_sensor.cpp",\n' >> compile_commands.json
	printf '    "command": "$(CXX) $(CXXFLAGS) $(INCLUDES) -c cap_sensor/cap_sensor.cpp -o $(BUILD_DIR)/project/cap_sensor/cap_sensor.cpp.o"\n' >> compile_commands.json
	printf '  }\n' >> compile_commands.json
	printf ']\n' >> compile_commands.json

clean:
	rm -rf $(BUILD_DIR) compile_commands.json

.PHONY: all upload lsp clean

-include $(DEPS)
