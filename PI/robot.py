#!/usr/bin/env python
#

from serial import Serial
from RPIO import setmode, setup as rSetup, BCM, IN
from RPIO import add_interrupt_callback, wait_for_interrupts
from Queue import Queue, Empty
from threading import Thread
from re import compile

PORT = '/dev/ttyAMA0'
BAUD = 57600
SERIAL_INTERRUPT = 17
LF = '\n'

setmode(BCM)


class Robot:

  CHECK = ('C', None)
  STRING = 'S'
  GET_R = 'R'
  GET_C = 'C'
  GET_M = 'M'
  GET_H = 'H'
  GET_D = 'D'
  READY_COUNTS = 0b00000001
  READY_MOTORS = 0b00000010
  READY_HEADING = 0b00000100
  READY_DISTANCES = 0b00001000
  ONE_RE = '^\s*(?P<cmd>[A-Z])[0-9]?\s(?P<v1>-?[0-9]+)\s*$'
  TWO_RE = '^\s*(?P<cmd>[A-Z])\s(?P<v1>-?[0-9]+)\s(?P<v2>-?[0-9]+)\s*$'


  def __init__(self):
    self.queue = Queue()
    self.serial = Serial(PORT, BAUD)
    # setup interrupt pin
    rSetup(SERIAL_INTERRUPT, IN)
    add_interrupt_callback(SERIAL_INTERRUPT, self.serialInterrupt)
    # regexes
    self.oneRE = compile(Robot.ONE_RE)
    self.twoRE = compile(Robot.TWO_RE)
    
    # init values
    self.counts = [0, 0]
    self.motors = [0, 0]
    self.heading = -1
    self.distance = [-1]
    return

  def run(self):
    thread = Thread(name='SerialMon', target=self.serialMonitor)
    thread.start()
    wait_for_interrupts()
    return
  
  def setupGUI(self):

    return

  def serialInterrupt(self, gpioID, value):
    print "Serial Interrupt"
    if gpioID != SERIAL_INTERRUPT:
      return
    self.queue.put(Robot.CHECK)
    return

  def handleInterrupt(self):
    print "Handle Interrupt"
    r = self.serialCmd(Robot.GET_R, self.oneRE)
    if not r:
      return
    v = int(r.group('v1'))
    print v
    if v & Robot.READY_COUNTS:
      print "Counts"
      r = self.serialCmd(Robot.GET_C, self.twoRE)
      if r:
	self.counts[0] = int(r.group('v1'))
	self.counts[1] = int(r.group('v2'))
	print "Counts: %s, %s" % (self.counts[0], self.counts[1])
    if v & Robot.READY_MOTORS:
      print "Motors"
      r = self.serialCmd(Robot.GET_M, self.twoRE)
      if r:
	self.motors[0] = int(r.group('v1'))
	self.motors[1] = int(r.group('v2'))
	print "Motors: %s, %s" % (self.motors[0], self.motors[1])
    if v & Robot.READY_HEADING:
      print "Heading"
      r = self.serialCmd(Robot.GET_H, self.oneRE)
      if r:
	self.heading = int(r.group('v1'))
	print "Heading: %s" % (self.heading)
    if v & Robot.READY_DISTANCES:
      print "Distances"
      r = self.serialCmd(Robot.GET_D, self.oneRE)
      if r:
	self.distance[0] = int(r.group('v1'))
	print "Distances: %s, %s" % (self.distance[0])
    return

  def serialCmd(self, cmd, regex):
    while self.serial.inWaiting() > 0:
      print "cleaning"
      self.serial.read()
    print "sending", cmd
    self.serial.write(cmd + LF)
    line = []
    c = self.serial.read()
    while c != LF:
      print "cc", c
      line.append(c)
      c = self.serial.read()
    print "lc", line
    r = regex.match(''.join(line))
    if r:
      if r.group('cmd') != cmd:
	print "cmd failed"
	return None
    else:
      print "no match"
    return r
    
  def serialMonitor(self):
    line = []
    print "Serial Monitor"
    while True:
      # reading
      while self.serial.inWaiting() > 0:
	c = self.serial.read()
	print "c", c
	if c == LF:
	  print "LINE", ''.join(line)
	  line = []
	line.append(c)
      try:
	while True:
	  item = self.queue.get(False)
	  if item[0] == Robot.CHECK[0]:
	    self.handleInterrupt()
	  elif item[0] == Robot.STRING:
	    self.serial.write(item[1])
	    self.serial.write(LF)
      except Empty:
	pass
    return

if __name__ == "__main__":
  robot = Robot()
  robot.run()
  
