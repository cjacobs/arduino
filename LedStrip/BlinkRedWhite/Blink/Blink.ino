
#define RED_LED 11
#define WHITE_LED 12

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
}

void loop() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);
  delay(500);

  digitalWrite(RED_LED, LOW);
  digitalWrite(WHITE_LED, HIGH);    
  delay(500);
}
