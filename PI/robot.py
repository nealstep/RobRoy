#!/usr/bin/env python
#

from serial import Serial
from RPIO import setmode, setup as rSetup, BCM, IN
from RPIO import add_interrupt_callback, wait_for_interrupts

PORT = '/dev/ttyAMA0'
BAUD = 57600

setmode(BCM)

def serialInterrupt(gpio_id, val):
  print "==> %d: %d" % (gpio_id, val)
  return

s = Serial(PORT, BAUD)
rSetup(17, IN)
add_interrupt_callback(17, serialInterrupt)
wait_for_interrupts(threaded=True)

while True:
  b = s.inWaiting()
  if b:
    data = s.read(b)
    print data,
  inp = raw_input("")
  s.write(inp)
  s.write("\n")
