# AVRDude FTDI programmer settings
The contents of this page is completely or largely taken from Darrel Tan's "[Programming the ATtiny10](http://irq5.io/2010/07/15/programming-the-attiny10/)" available at [irq5.io](http://irq5.io/2010/07/15/programming-the-attiny10/) (visited 2018-11-03).

## The FTDI programmer
A simple way of programming the ATtiny4/5/9/10 is to use a FTDI serial adapter together with an appropriate programmer definition for AVRDude, in order to bit-bang the serial programming protocol.

The programmer configuration is set up with the same pin configuration as a "dasa" programmer, with the main exception being that **on the FTDI-chip, all pins are inverted**. The programmer definition is posted below:
```text
programmer
  id    = "dasaftdi";
  desc  = "FTDI serial port banging, reset=rts sck=dtr mosi=txd miso=cts";
  type  = serbb;
  reset = ~7;
  sck   = ~4;
  mosi  = ~3;
  miso  = ~8;
;
```
*Darrel Tan's FTDI programmer definition for ATTiny10.*

## Connecting the FTDI programmer