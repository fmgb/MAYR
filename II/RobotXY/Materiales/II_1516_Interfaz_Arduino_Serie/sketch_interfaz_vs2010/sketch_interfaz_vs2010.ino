int ledPin = 13;  
int val = 0;      
int estado = LOW;

void setup() {
  pinMode(ledPin,OUTPUT);   
  Serial.begin(9600);        
}

void loop () {
  val = Serial.read();     

  if (val == 'w') {
    estado = !estado;
    digitalWrite(ledPin,estado);
  }
  if (val == 'r') {
    if (digitalRead(ledPin))
      Serial.print("1");
    else
      Serial.print("0");
  }
}

