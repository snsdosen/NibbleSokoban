# NibbleSokoban
![Screenshot](/readme/screenshot.png?raw=true "1")\
This is a clone of the popular Sokoban game for the CircuitMess Nibble console.\
It features original 50 levels plus additional 10 extra levels.\
Rewind feature is also available with memory of up to 1000 steps.

## Gameplay
The goal of this game is to place all the crates throughout the level to the chequered areas.\
Player can push only one crate at a time. Multiple crates can't be pushed at the same time.\
Crates that get stuck to a wall from at least 2 sides can't be pushed further.

## Rewind feature
If you mess up a move while playing press **B** button to enter "rewind mode".\
"**REW**" will appear at the top left of the screen and the player will retrace his steps.\
Release **B** button once you rewound to a satisfactory point.

## Controls
**Up/Down/Left/Right** - Move player and push crates\
**A** - Confirm options (in menus)\
**B** - Rewind\
**Menu** - Press once to restart the level or hold for 3 seconds to go to main menu

## Installing the game
### Compiling from the source:
Download Arduino IDE for your operating system:
>https://www.arduino.cc/en/software

After you are done installing it go to "File"->"Preferences" and paste the following in the "Additional Boards Manager URLs":

> https://raw.githubusercontent.com/CircuitMess/Arduino-Packages/master/package_circuitmess.com_esp8266_index.json

Now go to "Tools"->"Board"->"Boards manager" and search for "Nibble" and install "CircuitMess ESP8266 Boards".

Now clone this repository to your computer and open "NibbleSokoban.ino" with Arduino IDE.\
Select Nibble board ("Tools"->"Board"->"CircuitMess ESP8266 Boards"->"Nibble").\
Choose COM port for the console and start uploading by pressing Sketch->Upload.

Sketch will now compile and upload to your nibble console.

### Flashing compiled binary (Windows only)
Download "Flash Download Tools" from Espressif's download page:
>https://www.espressif.com/en/support/download/other-tools

Unpack data and open the tool. Select "Developer Mode" and then "ESP8266 DownloadTool".\
Check first checkbox and navigate to .bin file and select it.\
Enter "0x0" for the address (right of @). Select 4Mbit flash size and press "START".\
![Screenshot](/readme/Upload.png?raw=true "1")\
After upload is done press reset button on the console and the game should start.

## Making a screenshot
Although imagined as a debug feature for making screenshots to showcase the game\
I included a Python 3 script for capturing a 128x128px bitmaps from a device.

Before starting make sure to uncomment "#define DEBUG" in "NibbleSokoban.ino" otherwise that feature won't work.\
Also make sure that pySerial is installed. If it's not install it using the following command:
>python -m pip install pyserial

Now just type "python screenshot.py COMX" where "COMX" is your serial port Nibble is connected to.

Do note that the first time you run this command Nibble will reset.\
This is the way the board is designed to work in order to reset for code uploads when programming.\
Unfortunately I haven't found a workaround for this. All subsequent requests will not reset the board.

If done correctly there will now be a "screenshot.bmp" file in the directory you ran the script in.
