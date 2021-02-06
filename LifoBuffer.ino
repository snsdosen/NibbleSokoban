//Lifo circular buffer for rewinding feature

//Crate lifo buffer
bool LifoCreate(){
  //Allocate memory for rewind heap
  rewStack = (byte*)malloc(LIFO_SIZE);

  if(rewStack == NULL) return false;
  else return true;
}

//Reset stack
void LifoReset(){
  rewIndex = 0;
  lifoCount = 0;
}

//Push data to stack
void LifoPush(byte value){
#if defined (DEBUG) && defined (LIFO_DEBUG)
  Serial.print("Lifo push: 0x");
  Serial.println(value, HEX);
#endif

  rewStack[rewIndex % LIFO_SIZE] = value;

  rewIndex++;
  if(lifoCount < LIFO_SIZE) lifoCount++;
}

//Pop data from stack
byte LifoPop(){
  byte outData;
  
  if(rewIndex > 0){
    rewIndex--;
    lifoCount--;
    
    outData = rewStack[rewIndex % LIFO_SIZE];

#if defined (DEBUG) && defined (LIFO_DEBUG)
    Serial.print("Lifo pop: 0x");
    Serial.println(outData, HEX);
#endif
  }

  return outData;
}

//Get number of items on the stack
int LifoCount(){
  return lifoCount;
}
