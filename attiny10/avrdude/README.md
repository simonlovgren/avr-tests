# AVRDude FTDI programmer settings
This information was taken from [NYCResistor](https://www.nycresistor.com/2011/11/12/attiny10/), visited on *2013-11-04*.

## AVRDude programmer configuration
A simple way of programming the ATtiny4/5/9/10 is to use a FTDI serial adapter together with an appropriate programmer definition for AVRDude, in order to bit-bang the serial programming protocol.

The programmer configuration below need to be added to `avrdude.conf`.
```text
# ATtiny4/5/9/10 FTDI configuration for 6-pin FTDI cable
# Manual pull-low of RST-line needed when programming.
programmer
  id    = "dasaftdi";
  desc  = "Tiny10 with no reset, sck=!rts mosi=!txd miso=!cts";
  type  = "serbb";
  reset = ~4;
  sck   = ~7;
  mosi  = ~3;
  miso  = ~8;
;
```

## Connecting the FTDI programmer
``` text
                                1k ohm
               TXD (orange) ----/\/\/\/----+
                                            \
                 CTS (brown)  ---------------+--+--  --+-- GND (black) [during programming]
                                                |O ‾‾  |
                                          GND --+      +-- VCC (red)
                                                |      |
                                  RTS (Green) --+------+-- NC

                              [ FTDI connections for programming ATTiny4/5/9/10 ]
```

## Notes
When programming, I had many issues with `MOSI->MISO` communication. The first (and so far only) times I got past that issue was on Windows, the mac (OSX high sierra) never got further. Using ubuntu on Windows seems to not work either, but rather due to issues with the serial drivers (as windows is masquerading the ports) some IOCTL calls do not work properly.

The issue with `MOSI->MISO` may be down to bad connections, as I got it on Windows after a while as well (and pressing down on the cable/connections fixed the issue). With a better breakout it works better, but OSX still does not work.