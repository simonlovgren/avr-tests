# ATTiny4/5/9/10
Notes on ATTiny4/5/9/10, a tiny microcontroller with 512/1024 Bytes flash and 32 Bytes SRAM.

**Note: the ATtiny4/5/9/10 can only be programmed with 5V VCC**.

## Programmer
The ATtiny4/5/9/10 series is programmed via TPI. **It is possible to program these devices using an ordinary FTDI serial cable** (even one with 3.3V I/O levels, as long as the micro is fed 5V).

For more information about using a FTDI serial cable as a programmer, [see this readme](avrdude).

## Programming pinout
``` text

         (PCINT0/TPIDATA /OC0A/ADC0/AIN0) PB0 --+--  --+-- PB3 (RESET/PCINT3/ADC3)
                                                |O ‾‾  |
                                          GND --+      +-- VCC
                                                |      |
 (PCINT1/TPICLK/CLKI/ICP0/OC0B/ADC1/AIN1) PB1 --+------+-- PB2 (T0/CLKO/PCINT2/INT0/ADC2)

                                             ATTiny4/5/9/10
```

## Fuse bits
`1` means unprogrammed, `0` means programmed.

### BYTE0

| Bit(s) | Name       | Setting                                        | Default |
| ------ | ---------- | ---------------------------------------------- | :-----: |
| 7      | `N/A`      | Reserved                                       | `1`     |
| 6      | `N/A`      | Reserved                                       | `1`     |
| 5      | `N/A`      | Reserved                                       | `1`     |
| 4      | `N/A`      | Reserved                                       | `1`     |
| 3      | `N/A`      | Reserved                                       | `1`     |
| 2      | `CKOUT`    | Output external clock                          | `1`     |
| 1      | `WDTON`    | Watchdog timer always on                       | `1`     |
| 0      | `RSTDISBL` | Disable external reset                         | `1`     |

### Read fuses

``` bash
avrdude -c dasaftdi -p attiny10 -P /dev/ttyUSB0 -U fuse:r:-:i
```

### Program fuses

#### Factory default settings
``` bash
avrdude -c dasaftdi -p attiny10 -P /dev/ttyUSB0 -U fuse:w:0xFF:m
```
