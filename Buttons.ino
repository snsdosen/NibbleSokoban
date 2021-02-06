//This file contains button handling

//Button release callbacks
void BTN_UP_release(){
  upFlag = false;
}

void BTN_DOWN_release(){
  downFlag = false;
}

void BTN_LEFT_release(){
  leftFlag = false;
}

void BTN_RIGHT_release(){
  rightFlag = false;
}

void BTN_A_release(){
  aFlag = false;
}

void BTN_B_release(){
  bFlag = false;
}

//Button press callbacks
void BTN_UP_press(){
  upFlag = true;
}

void BTN_DOWN_press(){
  downFlag = true;
}

void BTN_LEFT_press(){
  leftFlag = true;
}

void BTN_RIGHT_press(){
  rightFlag = true;
}

void BTN_A_press(){
  aFlag = true;
}

void BTN_B_press(){
  bFlag = true;
}

void BTN_C_held_3000ms(){
  if(CurrentScene != SCENE_GAME) return;
  CurrentScene = SCENE_MENU;
}

void BTN_C_press(){
  if(CurrentScene != SCENE_GAME) return;
  
  //Reload current level
  DecompressLevel();
};

void RegisterButtonCallbacks(){
  //Register input callbacks
  Input::getInstance()->setBtnPressCallback(BTN_UP, BTN_UP_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_UP, BTN_UP_release);
  
  Input::getInstance()->setBtnPressCallback(BTN_DOWN, BTN_DOWN_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, BTN_DOWN_release);
  
  Input::getInstance()->setBtnPressCallback(BTN_LEFT, BTN_LEFT_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_LEFT, BTN_LEFT_release);
  
  Input::getInstance()->setBtnPressCallback(BTN_RIGHT, BTN_RIGHT_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_RIGHT, BTN_RIGHT_release);

  Input::getInstance()->setBtnPressCallback(BTN_A, BTN_A_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_A, BTN_A_release);
  
  Input::getInstance()->setBtnPressCallback(BTN_B, BTN_B_press);
  Input::getInstance()->setBtnReleaseCallback(BTN_B, BTN_B_release);
  
  Input::getInstance()->setBtnPressCallback(BTN_C, BTN_C_press);
  Input::getInstance()->setButtonHeldCallback(BTN_C, 3000, BTN_C_held_3000ms);
};
