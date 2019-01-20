#!/usr/bin/python
#---------------------------------------------------------------------
#    ___  ___  _ ____
#   / _ \/ _ \(_) __/__  __ __
#  / , _/ ___/ /\ \/ _ \/ // /
# /_/|_/_/  /_/___/ .__/\_, /
#                /_/   /___/
#
#           bh1750.py
# Read data from a BH1750 digital light sensor.
#
# Author : Matt Hawkins
# Date   : 26/06/2018
#
# For more information please visit :
# https://www.raspberrypi-spy.co.uk/?s=bh1750
#
#---------------------------------------------------------------------
import smbus
import time
import csv
import os.path

# Define some constants from the datasheet
headers = ['time', 'lumens']
DEVICE     = 0x23 # Default device I2C address

POWER_DOWN = 0x00 # No active state
POWER_ON   = 0x01 # Power on
RESET      = 0x07 # Reset data register value

# Start measurement at 4lx resolution. Time typically 16ms.
CONTINUOUS_LOW_RES_MODE = 0x13
# Start measurement at 1lx resolution. Time typically 120ms
CONTINUOUS_HIGH_RES_MODE_1 = 0x10
# Start measurement at 0.5lx resolution. Time typically 120ms
CONTINUOUS_HIGH_RES_MODE_2 = 0x11
# Start measurement at 1lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_HIGH_RES_MODE_1 = 0x20
# Start measurement at 0.5lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_HIGH_RES_MODE_2 = 0x21
# Start measurement at 1lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_LOW_RES_MODE = 0x23

#bus = smbus.SMBus(0) # Rev 1 Pi uses 0
bus = smbus.SMBus(1)  # Rev 2 Pi uses 1

def convertToNumber(data):
  # Simple function to convert 2 bytes of data
  # into a decimal number. Optional parameter 'decimals'
  # will round to specified number of decimal places.
  result=(data[1] + (256 * data[0])) / 1.2
  return (result)

def readLight(addr=DEVICE):
  # Read data from I2C interface
  data = bus.read_i2c_block_data(addr,ONE_TIME_HIGH_RES_MODE_1)


  return convertToNumber(data)

def main():

  while True:
    lightLevel=readLight()
#    print("Light Level : " + format(lightLevel,'.2f') + " lx")
    curdttm = time.strftime("%m/%d/%Y %H:%M:%S")
    file_exists = os.path.isfile('/home/pi/lightlog.csv')
    with open('/home/pi/lightlog.csv','a') as csvfile:
      csvwriter = csv.writer(csvfile,delimiter=',',
      quotechar='|', quoting=csv.QUOTE_MINIMAL)
      headers = ['time','lumens']
      writer = csv.DictWriter(csvfile, delimiter=',', lineterminator='\n',fieldnames=headers)
      if not file_exists:
        writer.writeheader()
      csvwriter.writerow([curdttm,lightLevel])
    time.sleep(1)

if __name__=="__main__":
   main()
