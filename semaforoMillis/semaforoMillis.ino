int trafficLight[3];
trafficLight[0] = 4;
trafficLight[1] = 0;
trafficLight[2] = 4;
int waitTime = 1000;
int actualTime = 0;
int previous = 0;
bool flag = false;



void setup() {

  pinMode(trafficLight[0], OUTPUT);
  pinMode(trafficLight[1], OUTPUT);
  pinMode(trafficLight[2], OUTPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println(millis());
  digitalWrite(trafficLight[0], HIGH);

  if(millis() > actualTime + waitTime){
    digitalWrite(trafficLight[0], LOW);
    digitalWrite(trafficLight[1], HIGH);
    actualTime += waitTime; 
  }
}
