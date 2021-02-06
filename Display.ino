//This file contains display routines

//Convert 24bit color to 16bit
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

int DrawScreen() {
  sprite->clear(clrColor);
  PrintCurrentLevel();
  DrawBoard();
  DrawPlayer();
  PrintRewindText();
}

void SetDisplayColors(){
  clrColor = TFT_BLACK;
  wallColor = TFT_MAROON;
  floorColor = TFT_DARKGREY;
  crateColor = TFT_RED;
  goalColor = TFT_LIGHTGREY;
  playerColor = TFT_GREEN;
}
