//Main menu handling

void playMenuSfx(){
  Piezo.tone(500, 10);
}

void ConfirmSelection(){
  CurrentScene = SCENE_LOAD;
}

void MoveSelectorUp(){
  if(selectedItemIndex == 0) return;
  playMenuSfx();
  selectedItemIndex = 0;
}

void MoveSelectorDown(){
  if(selectedItemIndex == 1) return;
  playMenuSfx();
  selectedItemIndex = 1;
}

void IncrementLevelSelector(){
  if(selectedItemIndex == 0){
    if(CurrentLevel < LEVEL_COUNT - 1) {
      CurrentLevel++;
      playMenuSfx();
    }
  }else{
    if(CurrentExtraLevel < EXTRA_LEVEL_COUNT - 1) {
      CurrentExtraLevel++;
      playMenuSfx();
    }
  }

  dPadTimeout = D_PAD_TIMEOUT;
}

void DecrementLevelSelector(){
    if(selectedItemIndex == 0){
    if(CurrentLevel > 0) {
      CurrentLevel--;
      playMenuSfx();
    }
  }else{
    if(CurrentExtraLevel > 0) {
      CurrentExtraLevel--;
      playMenuSfx();
    }
  }
  
  dPadTimeout = D_PAD_TIMEOUT;
}

void MainMenu(){
  sprite->clear(TFT_BLACK);

  sprite->drawIcon(logo_img, 24, 28, 40, 17, 2, TFT_WHITE);

  //Print software version
  sprite->setTextFont(1);
  sprite->setTextSize(1);
  sprite->setTextColor(TFT_WHITE);
  sprite->setTextDatum(TR_DATUM);
  sprite->drawString(GAME_VERSION, 100, 50);
  
  sprite->setTextFont(2);
  sprite->setTextSize(1);
  sprite->setTextColor(TFT_WHITE);
  sprite->setTextDatum(TC_DATUM);
  sprite->drawString("SOKOBAN", 64, 6);
  
  //Draw selector before drawing text
  sprite->fillRect(20, 68 + (18 * selectedItemIndex), 88, 18, TFT_RED);

  sprintf (BCD, "%02d", CurrentLevel + 1);
  menuItemOriginal[11] = BCD[0];
  menuItemOriginal[12] = BCD[1];
    
  sprite->drawString(menuItemOriginal, 64, 68);

  sprintf (BCD, "%02d", CurrentExtraLevel + 1);
  menuItemExtra[8] = BCD[0];
  menuItemExtra[9] = BCD[1];
  
  sprite->drawString(menuItemExtra, 64, 88);

  sprite->drawString("Sinisa & Darko D.", 64, 110);

  //Read input
  if(upFlag) MoveSelectorUp();
  if(downFlag) MoveSelectorDown();

  if(leftFlag && dPadTimeout == 0) DecrementLevelSelector();
  if(rightFlag && dPadTimeout == 0) IncrementLevelSelector();

  if(aFlag) ConfirmSelection();

  //Decrement timeout
  if(dPadTimeout > 0) dPadTimeout--;
}
