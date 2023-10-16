int led = 2;
int waitTime = 1000;
int actualTime = 0;
int previous = 0;
bool flag = false;

void setup() {

  pinMode(led, OUTPUT);
  Serial.begin(115200);
}

void loop() {

  actualTime = millis();
  Serial.println(millis());

  if ((actualTime - previous) >= waitTime) {
    previous = actualTime;
    flag = !flag;
    digitalWrite(led, flag);
  } 
  
}
