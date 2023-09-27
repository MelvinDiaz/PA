int valorLeido = 0;
int ldr = A0;
void setup() {

  pinMode(ldr, INPUT);
  Serial.begin(115200);
}

void loop() {
  
  valorLeido = analogRead(ldr); 
}
