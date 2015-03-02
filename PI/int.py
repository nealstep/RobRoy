#!/usr/bin/env python
#

import RPIO

RPIO.setmode(RPIO.BCM)

def getValue(gpio_id, val):
  print "%d: %d" % (gpio_id, val)

RPIO.setup(17, RPIO.IN)
RPIO.add_interrupt_callback(17, getValue)
RPIO.wait_for_interrupts()
