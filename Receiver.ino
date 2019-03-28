#define RECEIVE 7
#define CLOCK 2

/*
  FOR INITIAL SYNCHRONIZATION YOU MAY HAVE TO PRESS THE RESET BUTTON OF EACH
  DEVICE AT THE SAME TIME.
  
  IF RECEIVER SIDE SKIPS BITS, CHARS OR EVEN STRINGS IT MUST BE RESET OR ANY
  INCOMING BITS WILL BE INTERPRETED WRONG.

  THE CLOCK IS ASSIGNED TO PIN D2 (BECAUSE IT HAS INTERRUPTS). FROM MY TESTS
  THE ARDUINO UNO AND NANO PINS BEHAVE RANDOMLY (PROBABLY FLOATING) AND CAUSES
  UNWANTED INTERRUPTS AT THE RECEIVER SIDE WHEN RESET. (FROM HALF TO A SECOND)
  TO PREVENT THAT FROM HAPPENING THE CLOCK PIN SHOULD BE PULLED DOWN TO GND
  WITH A RESISTOR. (FROM MY EXPERIENCES THE PULLDOWN RESISTOR CAN BE ANYTHING
  FROM 1K TO 100K BUT I RECOMMEND HIGHEST TO PREVENT TOO MUCH CURRENT GOING 
  WASTE. 100K DID THE JOB WELL)

  ARDUINO T D2 |-----
                    |
                    |   100K
                    -----ww----| GND (COMMON)
                    |
                    |
  ARDUINO R D2 |-----

  ARDUINO T D8 ------------ D7 ARDUINO R

  ALSO YOU HAVE TO HAVE A COMMON GROUND BETWEEN TRANSMITTER AND THE RECEIVER
  SIDE.

  WHEN TWO ARDUINOS WORKING WITHOUT ERROR. RESETTING THE TRANSMITTER ARDUINO
  CAUSES THE RECEIVER SERIAL PORT TO DISPLAY:
  
    HELLO
    WORLD!
    THIS IS ARDUINO UNO

  IF NOT, MOST PROBABLY A BIT IS SKIPPED. RESET THE RECEIVER ARDUINO AND TRY 
  AGAIN. IF THE ERROR PERSIST, YOU CAN TRY INCREASING THE TIME INTERVAL T. 
  BUT 10 MICROSECONDS WORKED FINE WITH ME.

  WITH T = 10 MICROSECONDS A BYTE WILL BE TRANSMITTED IN 160 MICROSECONDS.
  ALSO WHEN THE STRING TRANSMITTED A 5 MILLISECONS DELAY GIVEN IS IN ORDER TO
  GIVE TIME TO RECEIVER SIDE FOR ARRANGING RECEIVED CHARS TO A STRING.

  LASTLY, I DID NOT USE A BUFFER ON THE RECEIVER AND READ THE STRING OR A BYTE
  FROM BUFFER WHEN ANY AVAILABLE AS I SHOULD. I DIDNT GET INTO THAT BECAUSE 
  THIS IS JUST A PROOF OF CONCEPT.

  March 2019
  nurullah.emsen@gmail.com
  
*/

volatile int receivedBits[8];
volatile int index = 0;

void setup() {
  pinMode(RECEIVE, INPUT);
  pinMode(CLOCK, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  startReceiving();
}

/*
 * Interrupt function.
 * When a rising edge is detected. It reads a bit from RECEIVE pin and writes
 * it into a array called receivedBits.
 */
void readBit(){
  receivedBits[index++] = digitalRead(RECEIVE);
}

/*
 * Main receiver function. I didn't use a buffer and read bytes or strings from
 * it. It starts the interrupt process and looks for last incoming char.
 * In that while loop, if the variable index reaches 8, the 8 bits from global
 * array receivedBits converted into a char. And that char is added at the end of
 * the string. If the last incoming char is NULL (00000000) it detaches interrupt,
 * prints the completed string to Serial Port Dİsplay and frees the string memory.
 * I did not try to write functions that returns bytes or strings. This is just
 * a proof of concept.
  */
void startReceiving(){
  char* charString = (char*) malloc(100*sizeof(char));
  attachInterrupt(digitalPinToInterrupt(CLOCK), readBit, RISING);
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
      index = 0;
    }
  }
  detachInterrupt(digitalPinToInterrupt(CLOCK));
  Serial.println(charString);
  free(charString);
}
