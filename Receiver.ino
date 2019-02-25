#define RECEIVE 7
#define CLOCK 9
#define RISING_EDGE 1
#define FALLING_EDGE 0
#define NO_CHANGE 2

int clockState = LOW;
//int receivedBits[8];
int index = 0;
bool canPrint = true;
char* charString;
void setup() {
  
  pinMode(RECEIVE, INPUT);
  pinMode(CLOCK, INPUT);
  Serial.begin(9600);
  charString = (char*) malloc(sizeof(char)*100);
  charString[99] = 0;

}

void loop() {
  /*
  //int currentClockState = digitalRead(CLOCK);

  if((clockEdge() == RISING_EDGE) & (index < 8)){    
    receivedBits[index++] = digitalRead(RECEIVE);
  }else if((index == 8) & (canPrint == true)){
    Serial.print(bitsToChar(receivedBits));
    index = 0;
  }
  //clockState = currentClockState;
  */

  waitForTransmit();
  receiveBits();
  Serial.println(charString);
}

void receiveBits(){
  int receivedBits[8];
  int bitIndex = 0;
  char c = 0;
  while(1){
    while(bitIndex < 8){
      if(clockEdge() == RISING_EDGE){    
        receivedBits[bitIndex++] = digitalRead(RECEIVE);
      }
    }
    bitIndex = 0;
    c = receivedBits[0]*128 + receivedBits[1]*64 + receivedBits[2]*32+ receivedBits[3]*16+ receivedBits[4]*8+ receivedBits[5]*4+ receivedBits[6]*2+ receivedBits[7]*1;
    charString[index++] = c;
    if (c == 0) return;
  }
  //if(charString[index] - 1) return;
  /*
  if((clockEdge() == RISING_EDGE) & (index < 8)){    
    receivedBits[index++] = digitalRead(RECEIVE);
  }else if((index == 8) & (canPrint == true)){
    c = bitsToChar(receivedBits);
    index = 0;
  }
  */
}

char bitsToChar(int arr[]){
  char c = arr[0]*128 + arr[1]*64 + arr[2]*32+ arr[3]*16+ arr[4]*8+ arr[5]*4+ arr[6]*2+ arr[7]*1;  
  return c;
}

int clockEdge(){
  static int lastClockState = LOW;
  int currentClockState = digitalRead(CLOCK);
  int edgeStatus;

  if((currentClockState != lastClockState) & (lastClockState == LOW)) edgeStatus = RISING_EDGE;
  else if((currentClockState != lastClockState) & (lastClockState == HIGH)) edgeStatus = FALLING_EDGE;
  else edgeStatus = NO_CHANGE;

  lastClockState = currentClockState;
  return edgeStatus;
}

void waitForTransmit(){
  int t = 0;
  while(1){
      int edge = clockEdge();
      if(edge == RISING_EDGE) {t = millis();}
      else if((edge == FALLING_EDGE) & ((millis() - t) > 900)) break;
  }
  

}
