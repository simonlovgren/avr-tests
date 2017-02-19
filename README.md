# AVR Tests
Tests for programming avr/atmega, for learning and as a future resource.

## Installing the toolchain
### AtmelAVR Toolchain (Ububtu/Debian)
``` bash
sudo apt-get update
sudo apt-get upgrade all
sudo apt-get install -y gcc-avr binutils-avr avr-libc
sudo apt-get install -y gdb-avr # Optional
```

### AVRDude (Ububtu/Debian)
``` bash
sudo apt-get install -y avrdude
```

## Using the toolchain
### avr-gcc
`avr-gcc` is the compiler with which the code is compiled to the final binary
and it works the same way as GCC. It does, however, have some special arguments. In the table
below, some arguments/flags are listed and explained.

| Flag   | Value | Description |
| ------ | ------------------------------ | ----------- |
| `-mmcu=` | `help`,`atmega328`, `atmega32u4`, `...` | Microcontroller to compile for. Use `help` to list available values. |
| `-c`     | `N/A` | Compile to object file (same as GCC). |
| `-Os`    | `N/A` | Optimize for minimum size. |
| `-o`     | Name of file | Specify name- and (optionally) location of output file |

#### Some simple examples:
``` bash
# List available microcontrollers
avr-gcc -mmcu=help
```
``` bash
# Compile a file to object-file (.o), optimized for size
avr-gcc -Os -mmcu=atmega328 -c program.c
```
``` bash
# Compile a file to binary (elf)
avr-gcc -mmcu=atmega328 -o program.elf program.o
```

### avr-objcopy
`avr-objcopy` us used to copy the contents of a binary file to a hex-file, which
then can be used for uploading to a mcu.

``` bash
avr-objcopy -j .text -j .data -O ihex program.elf program.hex 
```
### avrdude
AVRDude is a tool for programming avr mcus, set their fuse-bits as well as read
data/settings from the chip. Some of its options are explained below.

| Argument           | Important   | Description                                             |
| ------------------ | ----------- | ---------------------------------------------- |
| `-p <partno>`      | ![](https://raw.githubusercontent.com/simonlovgren/avr-tests/master/res/tick.png)   | This is to tell avrdude which mcu it's programming. |
| `-b <baudrate>`    |    | This is for overriding the default serial baud rate for programmers like STK500. Don't use this switch, the default is (most often) correct. |
| `-B <bitrate>`     |    | This is for changing the bitrate, aka. the speed, at which the programmer talks to the mcu.  |
| `-C <config-file>` |    | The config file tells avrdude about all different ways it can talk to the programmer. Usually the default config works fine, so don't use this if not needed. |
| `-c <programmer>`  | ![](https://raw.githubusercontent.com/simonlovgren/avr-tests/master/res/tick.png) | This argument specifies which programmer is used (ex. `stk500`, `buspirate`, etc.). |
| `-D`               |    | This disables erasing the chip before programming. Don't use this unless needed in special cases. |
| `-P <port>`        | ![](https://raw.githubusercontent.com/simonlovgren/avr-tests/master/res/tick.png) | The port at which the programmer is connected (ex. `/dev/ttyUSB0`) |
| `-F`               |    | This overrides the signature check that makes sure the chip to be programmed is the same as specified with the `-p` flag. **The signature check is recommended, so do not use unless really needed!** |
| `-e`               |    | This erases the chip. Usually not needed as the chip usually is auto-erased before flashing. |
| `-U <memtype>:r|w|v:<filename>[:format]` | ![](https://raw.githubusercontent.com/simonlovgren/avr-tests/master/res/important.png) ![](https://raw.githubusercontent.com/simonlovgren/avr-tests/master/res/important.png) | This is the most importent argument and it has a lot of parts to explain, such that its informaiton merits [its own sub-section](#-u-rwvformat). |
| `-n`               |    | This means nothing is actually written to the chip and is good for verifying that the command is actually the one you want to perform (like apt-get's `--dry-run`). |
| `-V`               |    | This turns **off** auto-verifying after writing. |
| `-u`               |    | If you want to modify the fuse-bits, use this flag to tell avrdude you really want it (usually not needed for such things as clock configuration etc.). |
| `-t`               |    | This is a terminal-mode where you can type out commands in a row. |
| `-E`               |    | This lists some programmer specifications. |
| `-v`               |    | This is to enable verbose output. |
| `-q`               |    | This is to enable quiet mode, the opposite of `-v`. |

*[Taken from ladyada.net](http://www.ladyada.net/learn/avr/avrdude.html)*

#### -U <memtype>:r|w|v:<filename>[:format]
This flag is the most important one, as it specifies what memory on the mcu/chip to use,
what action to perform- as well as what file to use and its format. Let's break down the
argument to its components:

`<memtype>` is the part where you tell avrdude which part of the MCU you want to apply the
command to. This can either be `flash` or `eeprom` for ordinary programming/data transfer *or*
`efuse`, `lfuse` or `hfuse` for the chip configuration fuses (fuse bits).

`r|w|v` is the mode of the command. `r` for *read*, `w` for *write* and `v` for *verify*.

`<filename>` is the file you want to write to- or read from.

`format` is an optional flag that specifies the format of the input- or output file. Usually
*Intel Hex* is used with the flag `i`, but others exist like `m` that interprets the `<filename>` directly as the data (see [Fuse bits](#fuse-bits)).

##### Examples
``` bash
# dump eeprom to file
-U eeprom:r:eeprom_dump.hex:i

# Upload prog.hex to flash
-U flash:w:prog.hex:i

# Write lower fuse byte to 0x6A
-U lfuse:w:0x6A:m
```

## Configuring the MCU
### Fuse bits
The fuse bits are what configures how the mcu is to function, and it is very important
to know how they work before trying to configure the mcu/set the fuse bits. **If the wrong
bits are set, the mcu can be bricked/locked from programming.**

What fuse bits are available depends on the model of the mcu. Under each directory for
a specific mcu, the fuse bits are explained in the README.

Example of setting the fuse bits using avrdude:
``` bash
# ATTiny13A Default: Internal 9.6MHz oscillator, Divide clock by 8, SPI Programming enabled.
avrdude -c buspirate -p attiny13 -P /dev/ttyUSB0 -U hfuse:w:0xFF:m -U lfuse:w:0x6A:m
```

***

*Icons made by [Pixel Buddha](http://www.flaticon.com/authors/pixel-buddha)*
