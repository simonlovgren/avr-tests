# ATTiny13A
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
         ATTiny13A
```

## Fuse bits
`1` means unprogrammed, `0` means programmed.

### High Byte

| Bit(s) | Name        | Setting                  | Default |
| ------ | ----------- | ------------------------ | :-----: |
| 7      | `N/A`       | Reserved                 | `1`     |
| 6      | `N/A`       | Reserved                 | `1`     |
| 5      | `N/A`       | Reserved                 | `1`     |
| 4      | `SELFPRGEN` | Self programming enable  | `1`     |
| 3      | `DWEN`      | debugWire enable         | `1`     |
| 2      | `BODLEVEL1` | Brown-out detector level | `1`     |
| 1      | `BODLEVEL0` | Brown-out detector level | `1`     |
| 0      | `RSTDISBL`  | External reset disable   | `1`     |


### Low Byte

| Bit(s) | Name     | Setting                                        | Default |
| ------ | -------- | ---------------------------------------------- | :-----: |
| 7      | `SPIEN`  | Enable serial programming and data downloading | `0`     |
| 6      | `EESAVE` | Preserve EEPROM memory through Chip Erase      | `1`     |
| 5      | `WDTON`  | Watchdog Timer always on                       | `1`     |
| 4      | `CKDIV8` | Divide clock by 8                              | `0`     |
| 3      | `SUT1`   | Select start-up time                           | `1`     |
| 2      | `SUT0`   | Select start-up time                           | `0`     |
| 1      | `CKSEL1` | Select clock source                            | `1`     |
| 0      | `CKSEL0` | Select clock source                            | `0`     |

### Program fuses

#### Factory default settings
Internal 9.6MHz oscillator, Divide clock by 8, SPI Programming enabled.
``` bash
avrdude -c buspirate -p attiny13 -P /dev/ttyUSB0 -U hfuse:w:0xFF:m -U lfuse:w:0x6A:m
```

#### Internal clock @ 9.6MHz with divider disabled
Internal 9.6MHz oscillator, No clock divider, rest is default.
``` bash
avrdude -c buspirate -p attiny13 -P /dev/ttyUSB0 -U hfuse:w:0xFF:m -U lfuse:w:0x7A:m
```

