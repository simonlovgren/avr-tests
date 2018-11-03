# ATMega8
Notes on ATMega8.

## Programming pinout
``` text
    ___
 CS/RST --+--  --+-- NC
          |O ‾‾  |
     NC --+      +-- NC
          |      |
     NC --+      +-- NC
          |      |
     NC --+      +-- NC
          |      |
     NC --+      +-- NC
          |      |
     NC --+      +-- NC
          |      |
    VCC --+      +-- GND
          |      |
    GND --+      +-- AREF
          |      |
  XTAL1 --+      +-- NC
          |      |
  XTAL2 --+      +-- CLK
          |      |
     NC --+      +-- MISO
          |      |
     NC --+      +-- MOSI
          |      |
     NC --+      +-- NC
          |      |
     NC --+------+-- NC
         ATMega8
```

## Fuse bits
`1` means unprogrammed, `0` means programmed.

The default setting uses the internal RC oscillator at 1MHz.

### Extended Byte (efuse)
Not used for ATMega8

| Bit(s) | Name        | Setting                                        | Default |
| ------ | ----------- | ---------------------------------------------- | :-----: |
| 7      | `N/A`       | Reserved                                       | `1`     |
| 6      | `N/A`       | Reserved                                       | `1`     |
| 5      | `N/A`       | Reserved                                       | `1`     |
| 4      | `N/A`       | Reserved                                       | `1`     |
| 3      | `N/A`       | Reserved                                       | `1`     |
| 2      | `N/A`       | Reserved                                       | `1`     |
| 1      | `N/A`       | Reserved                                       | `1`     |
| 0      | `N/A`       | Reserved                                       | `1`     |



### High Byte (hfuse)

| Bit(s) | Name        | Setting                                        | Default |
| ------ | ----------- | ---------------------------------------------- | :-----: |
| 7      | `RSTDISBL`  | External reset disable                         | `1`     |
| 6      | `WDTON`     | Enable watchdog timer                          | `1`     |
| 5      | `SPIEN`     | Enable serial programming and data downloading | `0`     |
| 4      | `CKOPT`     | Oscillator options                             | `1`     |
| 3      | `EESAVE`    | Preserve EEPROM memory through Chip Erase      | `1`     |
| 2      | `BOOTSZ1`   | Select Boot Size                               | `0`     |
| 1      | `BOOTSZ0`   | Select Boot Size                               | `0`     |
| 0      | `BOOTRST`   | Select Reset Vector                            | `1`     |


### Low Byte (lfuse)

| Bit(s) | Name       | Setting                                        | Default |
| ------ | ---------- | ---------------------------------------------- | :-----: |
| 7      | `BODLEVEL` | Brown out detector trigger level               | `1`     |
| 6      | `BODEN`    | Brown out detector enable                      | `1`     |
| 5      | `SUT1`     | Select start-up time                           | `1`     |
| 4      | `SUT0`     | Select start-up time                           | `0`     |
| 3      | `CKSEL3`   | Select clock source                            | `0`     |
| 2      | `CKSEL2`   | Select clock source                            | `0`     |
| 1      | `CKSEL1`   | Select clock source                            | `0`     |
| 0      | `CKSEL0`   | Select clock source                            | `1`     |

### Read fuses

``` bash
avrdude -c buspirate -p atmega8 -P /dev/ttyUSB0 -U efuse:r:-:i -U hfuse:r:-:i -U lfuse:r:-:i
```

### Program fuses

#### Factory default settings
Internal 1MHz RC oscillator
``` bash
avrdude -c buspirate -p atmega8 -P /dev/ttyUSB0 -U hfuse:w:0xD9:m -U lfuse:w:0xE1:m
```

#### Internal clock @ 8MHz with divider disabled
Internal 8MHz RC oscillator.
``` bash
avrdude -c buspirate -p atmega8 -P /dev/ttyUSB0 -U efuse:w:0xFF:m -U hfuse:w:0xD9:m -U lfuse:w:0xE4:m
```

