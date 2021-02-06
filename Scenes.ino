//This file contains code for scene handling

void RunScene(){
  switch(CurrentScene){
    case SCENE_INTRO:
      CurrentScene++;
      break;

    case SCENE_MENU:
      MainMenu();
      break;

    case SCENE_LOAD:
      DecompressLevel();
      CurrentScene++;
      break;

    case SCENE_GAME:
      MovePlayer();
      DrawScreen();
      CheckLevelSolved();
      break;
  }
}
