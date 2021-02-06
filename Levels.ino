//This code contains routines for level handling

//Grab level data and decompress it to playing field
int DecompressLevel(){
  int level = 0;
  int counter = 0;
  byte blockData = 0;

  crateIsMoving = false;
  rewIndex = 0;
  LifoReset();

  if(selectedItemIndex == 0) level = CurrentLevel;
  else level = CurrentExtraLevel;

  sprintf (levelString, "LV:%02d", level + 1);

  //Place player in the center of the screen
  int playerScreenX = 64;
  int playerScreenY = 64;
  
  for(int LVLy = 0; LVLy < LEVEL_HEIGHT; LVLy++){
    for(int LVLx = 0; LVLx < LEVEL_WIDTH; LVLx++){
      if(selectedItemIndex == 0) blockData = pgm_read_byte(levels_bin + (counter + (level * LEVEL_SIZE)));
      else blockData = pgm_read_byte(extra_levels_bin + (counter + (level * LEVEL_SIZE)));
      //if(selectedItemIndex == 0) blockData = levels_bin[counter + (level * LEVEL_SIZE)];
      //else blockData = extra_levels_bin[counter + (level * LEVEL_SIZE)];
      WorkAreaLower[LVLx][LVLy] = blockData & 0xF;
      WorkAreaUpper[LVLx][LVLy] = (blockData >> 4) & 0xF;
      counter++;
    }
  }
}

int DrawPlayer() {
  int expectedPlayerX = playerX*6 + LEVEL_X_OFFSET;
  int expectedPlayerY = playerY*6 + LEVEL_Y_OFFSET;

  //Check if player should be allowed to move
  if(expectedPlayerX != playerScreenX || expectedPlayerY != playerScreenY){
    playerIsMoving = true;
  } else{
    playerIsMoving = false;
    if(crateIsMoving){
      crateIsMoving = false;

      //Draw crate in the stopped position (fixes flickering)
      DrawCrate(LEVEL_X_OFFSET + crateX * 6, LEVEL_Y_OFFSET + crateY * 6);
      //sprite->fillRect(LEVEL_X_OFFSET + crateX * 6, LEVEL_Y_OFFSET + crateY * 6, 6, 6, crateColor);
      
    }
  }
  
  //Check if the player is in the proper screen position and animate if not
  if(playerScreenX < expectedPlayerX) playerScreenX+=2;
  else if(playerScreenX > expectedPlayerX) playerScreenX-=2;

  if(playerScreenY < expectedPlayerY) playerScreenY+=2;
  else if(playerScreenY > expectedPlayerY) playerScreenY-=2;

  sprite->fillRect(playerScreenX, playerScreenY, 6, 6, playerColor);

  //Draw crate if it's moving
  if(crateIsMoving){
    switch(crateMovementDirection){
      case DIR_UP:
        DrawCrate(playerScreenX, playerScreenY - 6);
        //sprite->fillRect(playerScreenX, playerScreenY - 6, 6, 6, crateColor);
        break;
        
      case DIR_DOWN:
        DrawCrate(playerScreenX, playerScreenY + 6);
        //sprite->fillRect(playerScreenX, playerScreenY + 6, 6, 6, crateColor);
        break;
        
      case DIR_LEFT:
        DrawCrate(playerScreenX - 6, playerScreenY);
        //sprite->fillRect(playerScreenX - 6, playerScreenY, 6, 6, crateColor);
        break;
        
      case DIR_RIGHT:
        DrawCrate(playerScreenX + 6, playerScreenY);
        //sprite->fillRect(playerScreenX + 6, playerScreenY, 6, 6, crateColor);
        break;
    }
  }
}

void PrintRewindText(){
  if(!playerInRewind) return;

  sprite->setTextFont(2);
  sprite->setTextSize(1);
  sprite->setTextColor(TFT_GREEN);
  sprite->setTextDatum(TL_DATUM);
  sprite->drawString("REW", 12, 19);
}

int PrintCurrentLevel() {
  sprite->setTextFont(2);
  sprite->setTextSize(1);
  sprite->setTextColor(TFT_WHITE);
  sprite->setTextDatum(TL_DATUM);
  sprite->drawString(levelString, 8, 4);
}


int PrintLevelComplete() {
  sprite->setTextFont(2);
  sprite->setTextSize(1);
  sprite->setTextColor(TFT_WHITE);
  sprite->setTextDatum(TC_DATUM);
  sprite->fillRect(5, 55, 120, 18, TFT_BLACK);
  sprite->drawString("LEVEL COMPLETE", 64, 56);
}

void DrawCrate(int x, int y){
  //sprite->drawIcon(crates_bin, x, y, 6, 6, 1, TFT_MAROON);
  sprite->fillRect(x, y, 6, 6, crateColor);
}

int DrawBoard() {
  cratesLeft = 0;
  x_coordinate = 0 + LEVEL_X_OFFSET;
  y_coordinate = 0 + LEVEL_Y_OFFSET;
  for (int count2 = 0; count2 < 16; count2++) {
    x_coordinate = 6;
    for (int count = 0; count < 19; count++) {

      //Lower layer
      switch(WorkAreaLower[count][count2]){
        default:
          break;

        case TILE_FLOOR:
              sprite->fillRect(x_coordinate, y_coordinate, 6, 6, floorColor);
          break;
          
        case TILE_WALL:
              //sprite->drawIcon(walls_bin, x_coordinate, y_coordinate, 6, 6, 1, TFT_MAROON);
              sprite->fillRect(x_coordinate, y_coordinate, 6, 6, wallColor);
          break;

        case TILE_GOAL:
              //sprite->fillRect(x_coordinate, y_coordinate, 6, 6, goalColor);
            sprite->drawIcon(goals_bin, x_coordinate, y_coordinate, 6, 6, 1, TFT_MAROON);
          break;
      }

      //Upper layer
      switch(WorkAreaUpper[count][count2]){
        default:
          break;

        case TILE_CRATE:
            //Skip drawing crate in movement
            if(crateX == count && crateY == count2){
              if(!crateIsMoving)DrawCrate(x_coordinate, y_coordinate);
            }else{
              DrawCrate(x_coordinate, y_coordinate);
            }

            //Check if the crate sits on a goal, if not mark it as unsolved
            if(WorkAreaLower[count][count2] != TILE_GOAL) cratesLeft++;
          break;

        case TILE_PLAYER:
          playerX = count;
          playerY = count2;

          playerScreenX = playerX*6 + LEVEL_X_OFFSET;
          playerScreenY = playerY*6 + LEVEL_Y_OFFSET;
          
          //Clear data so future reads don't reset the player position
          WorkAreaUpper[count][count2] = TILE_NOTHING;
          break;
      }
      x_coordinate += 6;
    }
    y_coordinate += 6;
  }
}

//Sound effect for level complete screen
void PlayLevelCompleteSound(){
  Piezo.tone(600, 20);
  delay(15);
  Piezo.tone(1000, 30);
  delay(25);
}

//Check if level is solved
void CheckLevelSolved(){
  if(cratesLeft == 0 && !crateIsMoving){
    if(CurrentLevel < LEVEL_COUNT - 1){
      PrintLevelComplete();
          display->commit();
          PlayLevelCompleteSound();
          delay(1000);
      if(selectedItemIndex == 0) {
        //Check if max level is reached
        if(CurrentLevel < LEVEL_COUNT) CurrentLevel++;
        else CurrentScene = SCENE_MENU;
      }
      else {
        if(CurrentExtraLevel < EXTRA_LEVEL_COUNT) CurrentExtraLevel++;
        else CurrentScene = SCENE_MENU;
      }
      DecompressLevel();
    }
  }
}
