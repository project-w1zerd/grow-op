import wiringpi
from time import sleep

gpio = wiringpi.GPIO(wiringpi.GPIO.WPI_MODE_GPIO)  
shutterpin = 17
solenoidpin = 18
gpio.pinMode(shutterpin,gpio.OUTPUT)  
gpio.pinMode(solenoidpin,gpio.OUTPUT)
wiringpi.pinMode(shutterpin,1)
wiringpi.pinMode(solenoidpin,1)

gpio.digitalWrite(solenoidpin,gpio.HIGH)
sleep(0.06)
gpio.digitalWrite(solenoidpin,gpio.LOW)

sleep(0.1)

gpio.digitalWrite(solenoidpin,gpio.HIGH)
sleep(0.05)
gpio.digitalWrite(solenoidpin,gpio.LOW)

sleep(0.12)
gpio.digitalWrite(shutterpin,gpio.HIGH)
sleep(0.1)
gpio.digitalWrite(shutterpin,gpio.LOW)
