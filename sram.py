#!/usr/bin/python

import serial
import time
import sys
import getopt
import string 
from xmodem import XMODEM

try:
	opts, args = getopt.getopt(sys.argv[1:], "f:d:")
except getopt.GetoptError, err:
	print str(err) 
	sys.exit(2)


for argument, value in opts:
	if argument == "-f":
		filename = value
	elif argument == "-d":
		serialdevice = value
	else:
		assert False, "Unhandled option"

print "Aria Loader - ISP utility for the Aria Module"
print "Version: 0.01"


try:
	print "--- Filename: " + filename
	print "--- Serial device: " + serialdevice
except:
	print "--- Example of use:"
	print "./loader.py -f filename -d /dev/ttyUSB0"
	sys.exit(1)


#Read the original executable file to send
try:
	f = open(filename,'rb')
	buffer = f.read()
	f.close()
except:
	print "Binary file not readable"

#Open the serial port 
ser = serial.Serial(
	port=serialdevice, 
	baudrate=115200, 
	timeout=1,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS
)  
ser.flushInput()

#Define the getc and putc function required from the
#xmodem module

txbyte=0

def getc(size, timeout=1):
	data = ser.read(size)
	return data

def putc(data, timeout=1):
	global txbyte

	ser.write(data)
	txbyte=txbyte+len(data)
	return len(data)

x = XMODEM(getc, putc)

while True:
	ser.flushInput()
	ser.write('#')
	rxchars = ser.read(3)
	print rxchars 
	if rxchars.find('>')>0:
		break

ser.flushInput()

cmdstring = "N#"
print "Send: [" + cmdstring + "]"
ser.write(cmdstring)
ser.read(2)

cmdstring = "S300000,#"
print "Send: [" + cmdstring + "]"
ser.write(cmdstring)

print "Send file %s in XModem" % filename
stream = open(filename, 'rb')
x.send(stream)
stream.close()

print "txbyte=%d" % txbyte

cmdstring = "G300000#"
print "Send: [" + cmdstring + "]"
ser.write(cmdstring)

print "Characters received from the Aria module (type Ctrl-C to exit)"

while True:
    s = ser.read(1) 
    sys.stdout.write(s)

ser.close()


