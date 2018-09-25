#include "touch.h"
#include "Arduino.h"

int XC4630_touchrawx(){           //raw analog value
  int x;
  pinMode(8,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(8,LOW);            //doesn't matter if this changes
  digitalWrite(A2,HIGH);          //this is normally high between screen commands
  pinMode(A3,INPUT_PULLUP);       //this is normally high between screen commands
  pinMode(9,INPUT_PULLUP);        //doesn't matter if this changes
  analogRead(A3);                 //discard first result after pinMode change
  delayMicroseconds(30);
  x=analogRead(A3);
  pinMode(A3,OUTPUT);
  digitalWrite(A3,HIGH);          //restore output state from above
  pinMode(9,OUTPUT);
  return(x);  
}

int XC4630_touchrawy(){           //raw analog value
  int y;
  pinMode(9,OUTPUT);
  pinMode(A3,OUTPUT);
  digitalWrite(9,LOW);            //doesn't matter if this changes
  digitalWrite(A3,HIGH);          //this is normally high between screen commands
  pinMode(A2,INPUT_PULLUP);       //this is normally high between screen commands
  pinMode(8,INPUT_PULLUP);        //doesn't matter if this changes
  analogRead(A2);                 //discard first result after pinMode change
  delayMicroseconds(30);
  y=analogRead(A2);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,HIGH);          //restore output state from above
  pinMode(8,OUTPUT);
  return(y);  
}

int XC4630_touchx(){
  int xc = 0; //map(XC4630_touchrawx(),XC4630_TOUCHX0,XC4630_TOUCHX1,0,240-1);
  if(xc>240-1){xc=-1;}         //off screen
  return xc;
}

int XC4630_touchy(){
  int yc = 0; //map(XC4630_touchrawy(),XC4630_TOUCHY0,XC4630_TOUCHY1,0,320-1);
  if(yc>320-1){yc=-1;}         //off screen
  return yc;
}

int touch(int x1,int y1,int x2,int y2){    //touch occurring in box?
  int x,y;
  x=XC4630_touchx();
  if(x<x1){return 0;}
  if(x>x2){return 0;}
  y=XC4630_touchy();
  if(y<y1){return 0;}
  if(y>y2){return 0;}
  return 1;
}
