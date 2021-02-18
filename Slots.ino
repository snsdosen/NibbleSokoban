//This code contains slot handling routines

//Load settings from EEPROM
void loadSettings(){
  byte readByte = 0;
  for(int i = 0; i < 6; i++){
    readByte = EEPROM.read(i);
    if(readByte == 0xFF) savedSlots[i] = 0;
    else savedSlots[i] = readByte;

    Serial.println(savedSlots[i], HEX);
  }
}

//Save settings to EEPROM
void saveSettings(){
  //Save settings from current player
  EEPROM.write(slotsSelector * 2, CurrentLevel);
  EEPROM.write((slotsSelector * 2) + 1, CurrentExtraLevel);
  
  EEPROM.commit();
}

void playSlotsSfx(){
  Piezo.tone(500, 10);
}

void ssMoveSelectorUp(){
  if(slotsSelector > 0) {
    playSlotsSfx();
    slotsSelector--;
  }
  dPadTimeout = SD_PAD_TIMEOUT;
}

void ssMoveSelectorDown(){
  if(slotsSelector < 2){
    playSlotsSfx();
    slotsSelector++;
  }
  dPadTimeout = SD_PAD_TIMEOUT;
}

//
void ssConfirmSelection(){
  aTimeout = true;

  //Load active player data from loaded data
  CurrentLevel = savedSlots[slotsSelector * 2];
  CurrentExtraLevel = savedSlots[(slotsSelector * 2) + 1];
  
  CurrentScene = SCENE_MENU;
}

void SlotSelection(){
  sprite->clear(TFT_BLACK);

  sprite->setTextFont(1);
  sprite->setTextSize(2);
  sprite->setTextColor(TFT_WHITE);
  sprite->setTextDatum(CC_DATUM);

  //Draw selector of selected slot
  sprite->fillRect(20, 33 + slotsSelector * 20, 88, 20, TFT_RED);

  sprite->drawString("Save 1", 64, 44);
  sprite->drawString("Save 2", 64, 64);
  sprite->drawString("Save 3", 64, 84);

  //Read input
  if(upFlag && dPadTimeout == 0) ssMoveSelectorUp();
  if(downFlag && dPadTimeout == 0) ssMoveSelectorDown();

  //Check if choice is confirmed
  if(aFlag && !aTimeout) ssConfirmSelection();
  //if(!aFlag) aTimeout = false;

  //Decrement timeout
  if(dPadTimeout > 0) dPadTimeout--;
}
