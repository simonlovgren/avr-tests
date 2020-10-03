# ATTiny85
Notes on ATTiny13A.

## Programming pinout
``` text
    ___
 CS/RST --+------+-- VCC
          |O     |
     NC --+      +-- CLK
          |      |
     NC --+      +-- MISO
          |      |
    GND --+------+-- MOSI
         ATTiny85
```

## Fuse bits
`1` means unprogrammed, `0` means programmed.

### Extended Byte (efuse)

| Bit(s) | Name        | Setting                                        | Default |
| ------ | ----------- | ---------------------------------------------- | :-----: |
| 7      | `N/A`       | Reserved                                       | `1`     |
| 6      | `N/A`       | Reserved                                       | `1`     |
| 5      | `N/A`       | Reserved                                       | `1`     |
| 4      | `N/A`       | Reserved                                       | `1`     |
| 3      | `N/A`       | Reserved                                       | `1`     |
| 2      | `N/A`       | Reserved                                       | `1`     |
| 1      | `N/A`       | Reserved                                       | `1`     |
| 0      | `SELFPRGEN` | Self-programming enabled                       | `1`     |

### High Byte

| Bit(s) | Name        | Setting                                   | Default |
| ------ | ----------- | ----------------------------------------- | :-----: |
| 7      | `RSTDISBL`  | External reset disabled                   | `1`     |
| 6      | `DWEN`      | DebugWIRE enabled                         | `1`     |
| 5      | `SPIEN`     | Serial program and data download enabled  | `0`     |
| 4      | `WDTON`     | Watchdog timer always on                  | `1`     |
| 3      | `EESAVE`    | EEPROM preserves chip erase               | `1`     |
| 2      | `BODLEVEL2` | Brown-out Detector trigger level          | `1`     |
| 1      | `BODLEVEL1` | Brown-out Detector trigger level          | `1`     |
| 0      | `BODLEVEL0` | Brown-out Detector trigger level          | `1`     |


### Low Byte

| Bit(s) | Name      | Setting                                        | Default |
| ------ | --------- | ---------------------------------------------- | :-----: |
| 7      | `CKDIV8`  | Clock divided by 8                             | `0`     |
| 6      | `CKOUT`   | Clock output enabled                           | `1`     |
| 5      | `SUT1`    | Start-up time setting                          | `1`     |
| 4      | `SUT0`    | Start-up time setting                          | `0`     |
| 3      | `CKSEL3`  | Clock source setting                           | `0`     |
| 2      | `CKSEL2`  | Clock source setting                           | `0`     |
| 1      | `CKSEL1`  | Clock source setting                           | `1`     |
| 0      | `CKSEL0`  | Clock source setting                           | `0`     |


### Read fuses

``` bash
avrdude -c buspirate -p attiny85 -P /dev/ttyUSB0 -U efuse:r:-:i -U hfuse:r:-:i -U lfuse:r:-:i
```

### Program fuses

#### Factory default settings
Internal 8MHz oscillator, Divide clock by 8, SPI Programming enabled.
``` bash
avrdude -c buspirate -p attiny85 -P /dev/ttyUSB0 -U efuse:w:0xFF:m -U hfuse:w:0xDF:m -U lfuse:w:0x62:m
```
#### Internal RC oscillator @ 8MHz with divider disabled
Internal 8MHz oscillator, No clock divider, rest is default.
``` bash
avrdude -c buspirate -p attiny13 -P /dev/ttyUSB0 -U efuse:w:0xFF:m -U hfuse:w:0xDF:m -U lfuse:w:0xE2:m
```
