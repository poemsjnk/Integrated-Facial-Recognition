const int buttonPin = 13;
uint btnCount = 0;
bool onceFlag = false;
unsigned long myTime = 0;
const unsigned long interval = 5000;
UART mySerial(11, NC, NC, NC);
String inputString = "";
bool stringComplete = false;
bool stateMachines = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(LEDR, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(LEDR, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin), button_interrupt, FALLING);
  onceFlag = false;
  mySerial.begin(9600);
  digitalWrite(LEDR,HIGH);
  inputString.reserve(20);
  Serial.begin(9600);
  while(!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }

  myservo.attach(12);
  mySerial.begin(9600);
}

void loop() {
  if (onceFlag) {
    digitalWrite(LEDR, LOW);
    if(millis() - myTime > 5000)
    {   digitalWrite(LEDR, HIGH);
        onceFlag = false;
    }
  }
  else
  {   myTime = millis();    }

  if (APDS.proximityAvailable()) {  
    int proximity = APDS.readProximity();


    Serial.println(proximity);

    if (proximity < 10)
    {
        mySerial.println("start");
        myTime = millis();
        stringComplete = false;
        while(1){
            if(mySerial.available()){
                while(mySerial.available()){
                    char inChar = (char)mySerial.read();
                    inputString += inChar;
                    if(inChar == '\n') {
                        stringComplete = true;
                    }
                    delay(50);
                
                    Serial.println(inputString);
                    if(stringComplete){
                          digitalWrite(LEDB,LOW);
                          delay(3000);
                          digitalWrite(LEDB, HIGH);
                          Serial.println("response received");
                    }
              }
              if(millis() - myTime > 10000) {
                  Serial.println("timeout mission fail");
                  break;
              }
              stateMachines = false;
              digitalWrite(LEDR, HIGH);
              Serial.println("mission done");
            }
        }
    }
  }
}
