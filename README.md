# Rotom

Pokemon automated egg hatcher without hacking 
by emulating a Nintendo switch controller over USB.

## Requirements

### Hardware
* An STM32F103C8 blupill
* STLinkV2
* Micro USB to USB cable adapter

### Software
* Platform IO

## Usage

You may run the project via the Platform IO Upload button in the VSCode status bar 
or through the CLI command.

```
$ platformio run --target upload
```

Normal expected output should look somewhat similar to

```
xPack OpenOCD, x86_64 Open On-Chip Debugger 0.10.0+dev-00378-ge5be992df (2020-06-26-12:31)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
debug_level: 1

hla_swd
none separate

target halted due to debug-request, current mode: Thread 
xPSR: 0x01000000 pc: 0x0800197c msp: 0x20005000
** Programming Started **
** Programming Finished **
** Verify Started **
** Verified OK **
** Resetting Target **
shutdown command invoked
```

You may configure the spin duration and number
of iterations to perform before halting by adjusting the following variables

```
#define SCRIPT_ITERATIONS 100
#define SPIN_DURATION_MS 720000
```

## Notes

This is based on [libOpenCM3](https://libopencm3.org/) and uses a [FreeRTOS](https://www.freertos.org/)
scheduler to run USB communication in parallel with script execution. This is sub optimal in
execution speed but was decided on in order to make it easier to maintain. As a result button press durations are computed in MS rather than ticks.