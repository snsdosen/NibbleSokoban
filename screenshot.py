#!/usr/bin/python3
#Screen capture script for Nibble applications

import sys
import serial
import time

bmpHeader = bytearray([0x42, 0x4D, 0x36, 0xC0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])

bmpR = 0
bmpG = 0
bmpB = 0

#Frame of data
lineLength = 256    #In bytes
linePixels = 128    #In pixels
lineNum = 128       #Number of available lines

#Check if required number of arguments was sent
if(len(sys.argv) < 2):
    print("Serial port was not defined")
    exit()

comPort = sys.argv[1]

try:
    ser = serial.Serial(comPort, 115200, timeout=1)
except serial.SerialException as e:
    print("Unable to open", sys.argv[1])
    exit()

#Open file for writing and place a bmp header
outFile = open("screenshot.bmp", "wb")
outFile.write(bmpHeader)

print("Screenshot reqest sent to", sys.argv[1])

#Nibble resets first time port is open, so wait untill it reboots
time.sleep(2)

#Dummy read request to flush buffers
ser.write(bytes([115, 255]))
ser.read(lineLength)

#Read all available buffer data
for i in range(lineNum):
    ser.write(bytes([115])) #'s' char - screenshot request
    ser.write(bytes([lineNum - i - 1])) # line to fetch (0 - 128), flipped because of BMP format

    #Get data from single line
    lineData = ser.read(lineLength)

    #check if complete frame transfered
    if(len(lineData) < lineLength):
        print(len(lineData))
        print("Timeout occured. Make sure correct port is selected.")
        print("Also make sure device is connected and DEBUG defined.")
        ser.close()
        exit()

    #Report progress
    print('Saving screenshot [%d%%]\r'%(i / (linePixels - 1) * 100), end="")

    #Go through each pixel and convert it to 24bit RGB
    for px in range(0, lineLength, 2):
        bmpR = (lineData[px] & 0xF8)
        bmpG = (lineData[px] & 0x7) << 5 | (lineData[(px) + 1] & 0xE0 ) >> 3
        bmpB = (lineData[(px) + 1] & 0x1F) << 3
        outFile.write(bytes([bmpB, bmpG, bmpR]))


outFile.close()
ser.close()
