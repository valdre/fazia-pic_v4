# FAZIA PIC ModernXC8 Firmware

This repository contains the MPLAB X firmware for the FAZIA PIC board, migrated from legacy C18-style code to a modern XC8-compatible implementation.
The target device is a PIC18F46K20 microcontroller.

The firmware configures board peripherals, communicates with two FPGA devices, handles UART framed commands, drives high-voltage modules, performs ADC/DAC operations, reads temperature sensors, and stores persistent calibration data in EEPROM.

## Technical Scope

- Target MCU: PIC18F46K20.
- Toolchain: MPLAB X + XC8 (validated with XC8 v3.10).
- Main entry point: src/main.c.
- Main communication interface: USART with framed protocol.
- Main control domains: high voltage, leakage current, preamplifier offset, temperature, FPGA access, EEPROM calibration.
- Main buses/peripherals: USART, SPI, internal timers, internal EEPROM, ADC.

## Migration Context

This branch modernizes the historical C18 codebase and preserves runtime behavior where possible:

- Replaced legacy C18 dependencies with XC8-compatible code paths.
- Kept peripheral-level behavior (timers, SPI, USART) aligned with original hardware usage.
- Added compatibility wrappers where old APIs were removed.
- Updated type usage (uint8_t/uint16_t/uint32_t) and memory qualifiers to modern C conventions.
- Integrated NewCalibration high-voltage calibration logic improvements (linear calibration validation and safer fallback behavior).

## Architecture Overview

The firmware follows a layered structure:

- Application layer: startup, scheduler loop, high-voltage state handling.
- Service layer: UART command dispatch and business handlers.
- Driver layer: SPI, ADC, DAC, EEPROM, temperature, setup.
- Low-level layer: PIC registers and XC8 peripheral handling.

## Runtime Flow

1. Reset and board startup:
   - MCU setup, peripherals, memory state, command table.
2. Initial checks:
   - FPGA communication, temperature subsystem, EEPROM-based parameters.
3. Continuous loop:
   - UART frame parsing and command execution.
   - Time-scheduled periodic tasks (HV ramp, leak current processing, offsets, safety checks).
4. ISR responsibilities:
   - Receive USART bytes into RX circular buffer.
   - Maintain scheduler timing based on Timer2 ticks.

## Main Software Components

- src/main.c: startup sequence, scheduler loop, HV control state machine.
- src/functions.c: command table, handlers, compatibility helper functions.
- src/isr.c: USART RX and scheduling tick interrupt logic.
- src/setup.c: peripheral initialization and runtime parameter loading.
- src/uartbuf.c: UART TX/RX buffering.
- src/cbuffer.c: generic circular buffer support.
- src/frame.c: protocol parsing, CRC and frame extraction.
- src/myfunc/spi.c: FPGA SPI register read/write helpers.
- src/myfunc/dac8568.c: DAC control and HV slope setup.
- src/myfunc/ads8332.c: ADC acquisition and leakage-current computation.
- src/myfunc/analog.c: analog measurements and conversions.
- src/myfunc/Tsensor.c: sensor timing/temperature acquisition.
- src/myfunc/wr_eeprom.c: EEPROM read/write routines.
- src/myfunc/maths.c: parsing and numeric helpers.
- src/myfunc/display.c: formatting and conversion helpers.

## High-Voltage Calibration Logic

The current implementation supports two calibration paths:

- Linear calibration path (EEPROM-based coefficient + constant).
- Legacy fallback path (historical model) when linear calibration is unavailable or invalid.

### Key behavior in this branch

- Linear calibration parameters are stored as signed 16-bit values in EEPROM.
- Linear calibration is validated before use.
- If calibration data is invalid (empty/erased pair), firmware falls back to the legacy transfer model.
- DAC conversion now uses signed 32-bit intermediate arithmetic to avoid overflow artifacts.

### Practical implication

Compared to older logic, outputs are not strictly identical in every numeric case.
This is expected and intentional in ranges where old 16-bit intermediate arithmetic could overflow.
The new logic is safer and more deterministic for calibration-driven values.

## Build Instructions

### Option A: MPLAB X IDE

1. Open the project folder in MPLAB X.
2. Select target device PIC18F46K20.
3. Select XC8 compiler (tested with v3.10).
4. Build the default configuration.

### Option B: Command line

From repository root:

make -f nbproject/Makefile-default.mk SUBPROJECTS= .build-conf

or simply:

make

Expected outputs are generated under build/default/production and dist/default/production.

## Flashing and Validation

1. Program the generated HEX file to the board.
2. Verify UART communication with a known command sequence.
3. Check FPGA read/write command behavior.
4. Verify HV ramp behavior and module state transitions.
5. Validate leak-current readouts on representative channels.
6. Confirm EEPROM read/write persistence across reset.

## Repository Layout

- include: public headers and constants.
- src: firmware sources.
- src/myfunc: hardware/domain-specific modules.
- nbproject: MPLAB project metadata and generated makefiles.
- build, dist: generated artifacts.

## Known Build Notes

- Some warnings (unused local helper functions) may appear in non-critical utility modules.
- Build artifacts in dist/ and build/ can produce large diffs; avoid committing them unless explicitly required for release packaging.

## Safety and Maintenance Notes

- Keep EEPROM address maps synchronized with calibration data format updates.
- Validate calibration data before enabling linear transfer functions.
- For timing-sensitive changes (ISR, sensor timing, SPI sequences), validate on real hardware.
- When modifying HV logic, test all four channels (A1, A2, B1, B2) and both modules.

## Suggested Regression Checklist

- Command parser ACK/NAK behavior.
- UART stability under burst traffic.
- HV setpoint ramp convergence for all channels.
- Leak current computation with valid and invalid calibration data.
- Temperature readout and timing robustness.
- FPGA communication after repeated resets.
