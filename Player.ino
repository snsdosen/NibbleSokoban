//This file contains player movement handling

//Crate moving sound
void playCrateSoundEffect(){
  Piezo.tone(500, 10);
}

//Crate on goal moving sound
void playCrateRewSoundEffect(){
  Piezo.tone(300, 10);
}

//Try and move player in the specified direction
void movePlayerUp(){
  if(playerY > 0) {
    if(WorkAreaLower[playerX][playerY - 1] != TILE_WALL && !playerIsMoving) {

      //Check if there is a crate in the way
      if(WorkAreaUpper[playerX][playerY - 1] == TILE_CRATE){
        //Check if it's OK to move crate
        if(WorkAreaUpper[playerX][playerY - 2] == TILE_NOTHING && WorkAreaLower[playerX][playerY - 2] != TILE_WALL){
          //Move crate
          WorkAreaUpper[playerX][playerY - 1] = TILE_NOTHING;
          WorkAreaUpper[playerX][playerY - 2] = TILE_CRATE;
          crateX = playerX;
          crateY = playerY - 2;

          crateIsMoving = true;
          crateMovementDirection = DIR_UP;

          //And player
          playerY--;
          LifoPush(DIR_UP | 0x10);

          playCrateSoundEffect();
        }
      }else{
        playerY--;    //Player is free to move
        LifoPush(DIR_UP);
      }
    }
  }
}


void movePlayerDown(){
  if(playerY < LEVEL_HEIGHT) {
    if(WorkAreaLower[playerX][playerY + 1] != TILE_WALL && !playerIsMoving) {
      
      //Check if there is a crate in the way
      if(WorkAreaUpper[playerX][playerY + 1] == TILE_CRATE){
        //Check if it's OK to move crate
        if(WorkAreaUpper[playerX][playerY + 2] == TILE_NOTHING && WorkAreaLower[playerX][playerY + 2] != TILE_WALL){
          //Move crate
          WorkAreaUpper[playerX][playerY + 1] = TILE_NOTHING;
          WorkAreaUpper[playerX][playerY + 2] = TILE_CRATE;
          crateX = playerX;
          crateY = playerY + 2;
          
          crateIsMoving = true;
          crateMovementDirection = DIR_DOWN;
          
          //And player
          playerY++;
          LifoPush(DIR_DOWN | 0x10);
          
          playCrateSoundEffect();
        }
      }else{
        playerY++;    //Player is free to move
        LifoPush(DIR_DOWN);
      }
    }
  }
}


void movePlayerLeft(){
  if(playerX > 0) {
    if(WorkAreaLower[playerX - 1][playerY] != TILE_WALL && !playerIsMoving) {
      
      //Check if there is a crate in the way
      if(WorkAreaUpper[playerX - 1][playerY] == TILE_CRATE){
        //Check if it's OK to move crate
        if(WorkAreaUpper[playerX - 2][playerY] == TILE_NOTHING && WorkAreaLower[playerX - 2][playerY] != TILE_WALL){
          //Move crate
          WorkAreaUpper[playerX - 1][playerY] = TILE_NOTHING;
          WorkAreaUpper[playerX - 2][playerY] = TILE_CRATE;
          crateX = playerX - 2;
          crateY = playerY;
          
          crateIsMoving = true;
          crateMovementDirection = DIR_LEFT;
          
          //And player
          playerX--;
          LifoPush(DIR_LEFT | 0x10);
          
          playCrateSoundEffect();
        }
      }else{
        playerX--;    //Player is free to move
        LifoPush(DIR_LEFT);
      }
    }
  }
}

void movePlayerRight(){
  if(playerX < LEVEL_WIDTH) {
    if(WorkAreaLower[playerX + 1][playerY] != TILE_WALL && !playerIsMoving) {

      //Check if there is a crate in the way
      if(WorkAreaUpper[playerX + 1][playerY] == TILE_CRATE){
        //Check if it's OK to move crate
        if(WorkAreaUpper[playerX + 2][playerY] == TILE_NOTHING && WorkAreaLower[playerX + 2][playerY] != TILE_WALL){
          //Move crate
          WorkAreaUpper[playerX + 1][playerY] = TILE_NOTHING;
          WorkAreaUpper[playerX + 2][playerY] = TILE_CRATE;
          crateX = playerX + 2;
          crateY = playerY;
          
          crateIsMoving = true;
          crateMovementDirection = DIR_RIGHT;
          
          //And player
          playerX++;
          LifoPush(DIR_RIGHT | 0x10);
          
          playCrateSoundEffect();
        }
      }else{
        playerX++;    //Player is free to move
        LifoPush(DIR_RIGHT);
      }
    }
  }
}

//Rewind player moves
void RewindPlayer(){
  byte popDir = 0;
  bool withCrate = false;
  
  if(bFlag && LifoCount() > 0){
    playerInRewind = true;

    //Do not pop the value unless the player is still
    if(!playerIsMoving){
    popDir = LifoPop();

    //Check if player moved with a crate
    if((popDir & 0x10) > 0) {
      withCrate = true;
      crateIsMoving = true;
      crateMovementDirection = popDir & 0xF;

      //Crate will move to player position
      crateX = playerX;
      crateY = playerY;

      playCrateRewSoundEffect();
    }

    //Get player direction (isolate crate)
    //Move in oposite direction than stored direction since we are rewinding
    switch(popDir & 0xF){
        case DIR_UP:
          if(withCrate){
            WorkAreaUpper[playerX][playerY - 1] = TILE_NOTHING;
            WorkAreaUpper[playerX][playerY] = TILE_CRATE;
          }
          
          playerY++;
          break;
  
        case DIR_DOWN:
          if(withCrate){
            WorkAreaUpper[playerX][playerY + 1] = TILE_NOTHING;
            WorkAreaUpper[playerX][playerY] = TILE_CRATE;
          }
          playerY--;
          break;
  
        case DIR_LEFT:
          if(withCrate){
            WorkAreaUpper[playerX - 1][playerY] = TILE_NOTHING;
            WorkAreaUpper[playerX][playerY] = TILE_CRATE;
          }
          playerX++;
          break;
  
        case DIR_RIGHT:
          if(withCrate){
            WorkAreaUpper[playerX + 1][playerY] = TILE_NOTHING;
            WorkAreaUpper[playerX][playerY] = TILE_CRATE;
          }
          playerX--;
          break;
      }
    }
  }else{
    playerInRewind = false;
  }
}

//All things necessary for player movement
void MovePlayer(){
  RewindPlayer();

  //Ignore movement if player is in rewind state
  if(playerInRewind) return;
  
  if(upFlag) movePlayerUp();
  if(downFlag) movePlayerDown();
  if(leftFlag) movePlayerLeft();
  if(rightFlag) movePlayerRight();
}
