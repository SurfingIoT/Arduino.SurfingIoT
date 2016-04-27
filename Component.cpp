#include <Component.h>
#include <Arduino.h>

Component::Component() {}
//very high cost to have each state in a string, this var is for all objects
char readValue[15];
Component::Component(char* name1, int type1, int port1) {
  name=name1;
  type=type1;
  port=port1;
  if(type==DIGITAL)  {
    pinMode(port, OUTPUT);
  }
  state=0;
}
Component::Component(char* name1, int type1, customFunction cf) {
  name=name1;
  type=type1;
  myCustomFunction =cf;

  state=0;
}
Component::Component(char* name1, int type1, customFunction cf, boolean isS) {
  name=name1;
  type=type1;
  myCustomFunction =cf;
  isSensor=isS;
  port=0;
  state=0;
}
char* Component::getValue() {
  itoa(state,readValue, 10);
  return readValue;
}

char* Component::getTypeName() {
   char* typeNames[] = {"DIGITAL", "ANALOG", "PWM", "CUSTOM"};
  return typeNames[type];
}

char* Component::write(char* c1) {
 char* r="\0";
 //debug Serial.println("DEntro do write... ");
 //debug Serial.println(c1);

 if(type==DIGITAL) {
   state = atoi(c1);
   digitalWrite(port, state);
   return r;
 }
 else if(type==PWM) {
   state = atoi(c1);
   analogWrite(port, atoi(c1));
   return r;
 }
 else if(type==CUSTOM) {
   return myCustomFunction(c1); 
 }
}

void Component::emptyReadValue() {
  for(int x=0;x<15;x++) {
     readValue[x]='\0';
  }
}
char* Component::read() {
  emptyReadValue();
  if(type==ANALOG ) {
    //in my country we call this code as "gambiarra" #Gambiarrafeelings
    //mas na verdade o sensor de temperatura analógico causa no ADC do ARduino! 
    state = analogRead(port);
    delay(5);
    state = analogRead(port);
    delay(5);
    state = analogRead(port);
    delay(5);
    return getValue();
  }
  else if(type==DIGITAL || type==PWM) {
    //pinMode(port, INPUT);
    state = digitalRead(port);
    pinMode(port,OUTPUT);
    return getValue();
  } else if(type==SERIAL) {

    int counter=0; 
    while(Serial.available()>0 && counter<15) 
    {
      char c = Serial.read(); 
      delay(5);
      readValue[counter++]=c;
    }
    return readValue;
  } else if(type==CUSTOM) {
    //should call function pointer here..
    char* ccc="\0";
    return myCustomFunction(ccc);

  } 
  else return "\0";
}
