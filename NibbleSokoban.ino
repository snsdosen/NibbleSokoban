//Sokoban clone for the Nibble gaming console
//Sinisa Dosen, Feb. 2021

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>

//Binary data for all of the 50 original levels
#include "src/levels.h"

//Binary data for 10 extra levels
#include "src/extra_levels.h"

//Binary data for sprites
#include "src/sprites.h"

//Game version
#define GAME_VERSION    "1.0"

//For debugging purposes
//#define DEBUG
//#define LIFO_DEBUG

#define LIFO_SIZE         1000  //Number of available rewind steps

#define LEVEL_COUNT       50    //Number of available levels in the game
#define EXTRA_LEVEL_COUNT 10    //Number of available extra levels
#define LEVEL_SIZE        304   //Level size in bytes
#define LEVEL_WIDTH       19    //Width in blocks
#define LEVEL_HEIGHT      16    //Height in blocks
#define LEVEL_X_OFFSET    6     //X Level offset in pixels on screen
#define LEVEL_Y_OFFSET    26    //Y Level offset in pixels on screen

//Lower layer tiles
#define TILE_NOTHING    0x0
#define TILE_FLOOR      0x1
#define TILE_WALL       0x2
#define TILE_GOAL       0x3

//Upper layer tiles
#define TILE_CRATE      0x1
#define TILE_PLAYER     0x2

//Movement directions
#define DIR_UP      0
#define DIR_DOWN    1
#define DIR_LEFT    2
#define DIR_RIGHT   3

//Scenes throughout the game
#define SCENE_INTRO   0
#define SCENE_MENU    1
#define SCENE_LOAD    2
#define SCENE_GAME    3

//Game modes
#define MODE_ORIGINAL 0
#define MODE_EXTRA    1

//Timeouts
#define D_PAD_TIMEOUT 4

//LIFO rewind stack
byte* rewStack = NULL;
int rewIndex = 0;
int lifoCount = 0;

//D-pad flags
bool upFlag = false;
bool downFlag = false;
bool leftFlag = false;
bool rightFlag = false;

//Button flags
bool aFlag = false;
bool bFlag = false;
bool cFlag = false;

//D-pad timeouts
int dPadTimeout = 0;

//Time tracking for timed events
unsigned long previousMillis = 0;
const long interval = 30;

int CurrentScene = SCENE_INTRO;
int CurrentLevel = 0;       //Currently active level
int CurrentExtraLevel = 0;  //Currently active extra level
char* levelString = "XX:XX";

int x_coordinate = 0;
int y_coordinate = 0;

//Player variables
int playerX = 0;
int playerY = 0;

//Player coordinates on the screen
int playerScreenX = 64;
int playerScreenY = 64;

bool playerInRewind = false;
bool playerIsMoving = false;
bool crateIsMoving = false;
int crateMovementDirection = 0;

int crateX = 0;
int crateY = 0;

//Crates
int cratesLeft = 0;

Display* display;
Sprite* sprite;

uint16_t clrColor = 0;
uint16_t floorColor = 0;
uint16_t wallColor = 0;
uint16_t crateColor = 0;
uint16_t goalColor = 0;
uint16_t playerColor = 0;

//Playing field is 19 blocks wide and 16 blocks tall
//And consists of lower and upper level
byte WorkAreaUpper[19][16];
byte WorkAreaLower[19][16];

//Main menu text
char* menuItemOriginal = "Original: <XX>";
char* menuItemExtra = "Extra: <XX>";
char* BCD = "XX";
int selectedItemIndex = 0;

void setup() {

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println();
  Serial.println("Debugging started.");
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
#endif

  SetDisplayColors();
  
  Nibble.begin();
  display = Nibble.getDisplay();
  sprite = display->getBaseSprite();

  //Stop execution if rewind is not available (no memory!)
  if(!LifoCreate()){
    while(true)yield();
  }
  
  RegisterButtonCallbacks();

#ifdef DEBUG
  Serial.print("Free heap after init: ");
  Serial.println(ESP.getFreeHeap());
  Serial.flush();
#endif
}

void loop() {
  unsigned long currentMillis = millis();
  
  Input::getInstance()->loop(0);

#ifdef DEBUG
  ScreenCapHandler();
#endif

  //Frame counting
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    RunScene();
    
    display->commit();
  }
}
