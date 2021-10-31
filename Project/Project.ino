// C++ code
//
const int trigger1 = 2; //Trigger pin of 1st Sensor
const int echo1 = 8; //Echo pin of 1st Sesnor
const int trigger2 = 4; //Trigger pin of 2nd Sensor
const int echo2 = 7;//Echo pin of 2nd Sensor

long time_taken;
int dist,distL,distR;

void setup() {
  Serial.begin(9600); 

  pinMode(trigger1, OUTPUT); 
  pinMode(echo1, INPUT); 
  pinMode(trigger2, OUTPUT); 
  pinMode(echo2, INPUT); 
}

/*
Trigger = Generate a pulse
Echo = Recieve a pulse
Time taken between generating a pulse and recieving the pulse
gives the distance
Formula --> distance_in_cm = (time_taken / 2) / 29
Working of ultrasonic sensor
Trigger generates the wave and sends it in open space.
The wave hits an obstacle and is reflected back.
The echo pin recieves the reflected wave and the time taken is measured.
*/

/*###Function to calculate distance###*/
void calculate_distance(int trigger, int echo)
{
  // Generate wave
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Recieve wave
  /*
    The pulseIn() starts the timer when echo pin goes from low 
    to high and stops the timer when it goes back to low.
  */
  time_taken = pulseIn(echo, HIGH);
  dist= time_taken*0.01724; // Formula to calculate distance from time taken
  if (dist>50)
  dist = 50;
}

void loop() {
  calculate_distance(trigger1,echo1);
  distL =dist; //get distance of left sensor

  calculate_distance(trigger2,echo2);
  distR =dist; //get distance of right sensor

  
    Serial.println(distL);
    Serial.println(distR);
  

  //Pause Modes -Hold
  // Both hands should simultaneously be at a distance between 40 - 50 cm
  if ((distL >40 && distR>40) && (distL <50 && distR<50)) //Detect both hands
  {
    Serial.println("Play/Pause");
  }

  calculate_distance(trigger1,echo1);
  distL =dist;

  calculate_distance(trigger2,echo2);
  distR =dist;

  //Control Modes
  //Lock Left - Control Mode
  if (distL>=13 && distL<=17)
  {
    delay(100); //Hand Hold Time
    calculate_distance(trigger1,echo1);
    distL =dist;
    if (distL>=13 && distL<=17)
    {
      Serial.println("Left Locked"); // Activating the left sensor only
      while(distL<=40)
      {
        calculate_distance(trigger1,echo1);
        distL =dist;
        if (distL<10) //Hand pushed in 
        {
          Serial.println ("Volume Up");
        }
        if (distL>20) //Hand pulled out
        {
          Serial.println ("Volume Down");
        }
      }
    }
  }

  //Lock Right - Control Mode
  if (distR>=13 && distR<=17)
  {
    delay(100); //Hand Hold Time
    calculate_distance(trigger2,echo2);
    distR =dist;
    if (distR>=13 && distR<=17)
    {
      Serial.println("Right Locked"); // Activating the right sensor only
      while(distR<=40)
      {
        calculate_distance(trigger2,echo2);
        distR =dist;
        if (distR<10) //Right hand pushed in
        {
          Serial.println ("Rewind");
        }
        if (distR>20) //Right hand pulled out
        {
          Serial.println ("Forward");
        }
    }
  }
  }
}
