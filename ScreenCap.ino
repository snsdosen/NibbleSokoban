//This file contains routines for screen capture
//DEBUG must be defined. Use a provided python script to capture screen
#ifdef DEBUG

  //Capture line on screen in raw format and send it to serial
  void CaptureScreen(byte line){
    if(line > 128) return;    //Do not send data outside of the screen area
    uint16_t pixelData = 0;

    //Go through all 128 pixels in line
    for(int i = 0; i < 128; i++){
      pixelData = sprite->readPixel(i, line);
      Serial.write(pixelData >> 8);
      Serial.write(pixelData & 0xFF);
    }
  }
  
  //Discard all data from the input buffer
  void SerialFlush(){
    while(Serial.available() > 0) Serial.read();
  }

  void ScreenCapHandler(){
    if(Serial.available() > 1){
      if(Serial.read() == 's') CaptureScreen(Serial.read());
      SerialFlush();
    }
  }

#endif
