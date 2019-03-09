#define RECEIVE 7
#define CLOCK 2

//PRESS THE RESET BUTTON AT THE SAME TIME

//char bitsToChar(int arr[]);
volatile int receivedBits[8];
volatile int index = 0;
volatile bool first = true;


void setup() {
  pinMode(RECEIVE, INPUT);
  pinMode(CLOCK, OUTPUT);
  Serial.begin(9600);
  /*
  bool arr[8] = {0,1,0,1,0,1,0,1};
  char c = 0;
  for(int i = 0; i < 8; i++){
    c = c << 1;
    c = c | arr[i];
  
  }
  Serial.println(c);
  */
  delay(1000);
  //if(isReady() == HIGH) startReceiving();
  //startReceiving();
  //startReceiving();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(isReady() == HIGH) startReceiving();
}

void readBit(){
  if(first && index == 0) {
    first = false;
    return;
  }
  receivedBits[index++] = digitalRead(RECEIVE);
}

void startReceiving(){
  delay(10);
  //Serial.println("StartReceiving()");
  
  char* charString = (char*) malloc(100*sizeof(char));
  attachInterrupt(digitalPinToInterrupt(CLOCK), readBit, RISING);
  //index--;
  int stringIndex = 0;
  char c = 1;
  while(c){
    if(index == 8){
      c = 0;
      for(int i = 0; i < 8; i++){
        c = c << 1;
        c = c | receivedBits[i];
  
      }
      charString[stringIndex++] = c; 
      //detachInterrupt(digitalPinToInterrupt(CLOCK));
      //canPrint = false;
      index = 0;
    }
  }
  detachInterrupt(digitalPinToInterrupt(CLOCK));
  Serial.println(charString);
  free(charString);
  first = true;
  
  pinMode(CLOCK, OUTPUT);
}

int isReady(){
  pinMode(CLOCK, INPUT);
  return digitalRead(CLOCK);
}
