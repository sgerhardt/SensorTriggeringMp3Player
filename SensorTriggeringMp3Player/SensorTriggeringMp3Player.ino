#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>


/*
 *This project triggers an MP3 when a short-range ultra-sonic 
 *sensor is triggered. Tested on a VS1053 MP3 shield and 
 *Virtuabotix ultrasonic distance rangefinder.
 */
SdFat sd;
SFEMP3Shield MP3player;

#define trigPin 5
#define echoPin 4

void setup() {

  Serial.begin(9600);
  delay(3000);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //start the shield
  sd.begin(SD_SEL, SPI_HALF_SPEED);
  MP3player.begin();
  MP3player.setVolume(0);

}

void loop() {
  long duration, distance;
  long avgDistance = 0;
  /*
   *Currently receiving an invalid reading every fourth attempt.
   *This loop averages out the readings to compensate for this.
   */
  for(int i=0; i<4; i++){
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    //Convert to centimeters
    distance = (duration/2)/29;
    //Serial.print(distance);
    //Serial.println("cm");
    avgDistance += distance;
    delay(50);
  }
  avgDistance = (avgDistance/4);
  Serial.print(avgDistance);
  Serial.println("cm");
  if(avgDistance < 20 && avgDistance > 0){
    //Tracks need to be named in the following manner on the sd card:
    //track00x.mp3, where x is the track number. Track 1 is being played below.
    MP3player.playTrack(1);
    delay(20000);
  }
}


