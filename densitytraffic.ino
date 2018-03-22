/*
Author: Rajat Bullakkanavar
Project Name: Density based traffic scheduling system and smart street lights
Hardware used: Arduino Mega, IR pairs, LED, Connecting wires

*/

#include<Thread.h>                        
#include<StaticThreadController.h>

//ThreadController con = ThreadController();    
Thread* density_cnt = new Thread();               
Thread signal_con = Thread();


// density controlling ir pair initialisation

int turn = 0;

int pre[] = {0, 0, 0, 0} ;                              //for density function

int obs[] = {2, 3, 4, 5};                       //arduino connections to IR receiver

int densityB[] = {0, 0, 0, 0};                 //setting the initial values of density as zero

//traffic light initializations

int Lane1[] = {6, 7};                       //red,green
int Lane2[] = {8, 9};                       //red,green
int Lane3[] = {10, 11};                      //red,green
int Lane4[] = {12, 13};                      //red,green

//traffic timer initialisations

int timer[] = {0, 0, 0, 0};                   //declaring and initilizing a variable called timer

void reset(int* anyData)
{
  *anyData = 0;
}
int hasObstacle;

int delSec = 1000;
bool lock = false;
void count_density()                  //density count function
{



  hasObstacle = digitalRead(obs[0]); 
  if (hasObstacle == HIGH) 
  {

    densityB[0] = pre[0] + 1;
    pre[0] = densityB[0];

    delay(delSec);
  }



  hasObstacle = digitalRead(obs[1]); 
  if (hasObstacle == HIGH)
  {

    densityB[1] = pre[1] + 1;
    pre[1] = densityB[1];

    delay(delSec);    //when turn isn't 0 it calculates density
  }



  hasObstacle = digitalRead(obs[2]);
  if (hasObstacle == HIGH)
  {

    densityB[2] = pre[2] + 1;
    pre[2] = densityB[2];

    delay(delSec);    //when turn isn't 0 it calculates density
  }


  hasObstacle = digitalRead(obs[3]); 
  if (hasObstacle == HIGH) 
  {

    densityB[3] = pre[3] + 1;
    pre[3] = densityB[3];

    delay(delSec);
  }      //when turn isn't 3 it calculates density


  if (!lock) {


  }
}

void traffic_set()
{
  if (!lock) {
    if (turn < 3) turn++;
    else turn = 0;
  }

  if (turn == 0) {
    lock = true;
    digitalWrite(Lane1[0], LOW);                   
            digitalWrite(Lane1[1], HIGH);
    digitalWrite(Lane2[0], HIGH);
    digitalWrite(Lane2[1], LOW);
    digitalWrite(Lane3[0], HIGH);
    digitalWrite(Lane3[1], LOW);
    digitalWrite(Lane4[0], HIGH);
    digitalWrite(Lane4[1], LOW);
    Serial.print("density 0=");
    Serial.println(densityB[0]);
    timer[0] = 3000 * densityB[0];
    Serial.print("timer 1 reads = ");
    Serial.println(timer[0]);
    delay(timer[0]);

    densityB[0] = 0;
    pre[0] = 0;
    Serial.print("density after reset 0=");
    Serial.println(densityB[0]);
    timer[0] = 0;
    lock = false;
  }

  if (turn == 1) {
    lock = true;
    digitalWrite(Lane1[0], HIGH);
    digitalWrite(Lane1[1], LOW);
    digitalWrite(Lane2[0], LOW);
    digitalWrite(Lane2[1], HIGH);
    digitalWrite(Lane3[0], HIGH);
    digitalWrite(Lane3[1], LOW);
    digitalWrite(Lane4[0], HIGH);
    digitalWrite(Lane4[1], LOW);
    Serial.print("density 1=");
    Serial.println(densityB[1]);
    timer[1] = 3000 * densityB[1];
    Serial.print("timer 2 reads = ");
    Serial.println(timer[1]);
    delay(timer[1]);
    pre[1] = 0;
    densityB[1] = 0;
    Serial.print("density after reset 1=");
    Serial.println(densityB[1]);
    timer[1] = 0;
    lock = false;
  }

  if (turn == 2) {
    lock = true;
    digitalWrite(Lane1[0], HIGH);
    digitalWrite(Lane1[1], LOW);
    digitalWrite(Lane2[0], HIGH);
    digitalWrite(Lane2[1], LOW);
    digitalWrite(Lane3[0], LOW);
    digitalWrite(Lane3[1], HIGH);
    digitalWrite(Lane4[0], HIGH);
    digitalWrite(Lane4[1], LOW);

    Serial.print("density 2=");
    Serial.println(densityB[2]);
    timer[2] = 3000 * densityB[2];
    Serial.print("timer 3 reads = ");
    Serial.println(timer[2]);
    delay(timer[2]);
    pre[2] = 0;
    densityB[2] = 0;

    Serial.print("density after reset 2=");
    Serial.println(densityB[2]);
    timer[2] = 0;
    lock = false;
  }

  if (turn == 3) {
    lock = true;
    digitalWrite(Lane1[0], HIGH);
    digitalWrite(Lane1[1], LOW);
    digitalWrite(Lane2[0], HIGH);
    digitalWrite(Lane2[1], LOW);
    digitalWrite(Lane3[0], HIGH);
    digitalWrite(Lane3[1], LOW);
    digitalWrite(Lane4[0], LOW);
    digitalWrite(Lane4[1], HIGH);

    Serial.print("density 3=");
    Serial.println(densityB[3]);


    timer[3] = 3000 * densityB[3];
    Serial.print("timer 4 reads = ");
    Serial.println(timer[3]);
    delay(timer[3]);
    pre[3] = 0;
    densityB[3] = 0;

    Serial.print("density after reset 3=");
    Serial.println(densityB[3]);

    timer[3] = 0;
    lock = false;
  }


}

Thread* theThread = new Thread(count_density);

StaticThreadController<3> controll (density_cnt, &signal_con, theThread);

void setup()
{
  //delaring all the IRs to be input
  for (int i = 0; i <= 4; i++)
  {
    pinMode(obs[i], INPUT);
  }

  //initializing all the IRs to be low
  for (int i = 0; i <= 4; i++)
  {
    digitalWrite(obs[i], LOW);
  }

  Serial.begin(9600);

  //traffic light initialisation


  for (int i = 0; i < 2; i++)
  {
    pinMode(Lane1[i], OUTPUT);
    pinMode(Lane2[i], OUTPUT);
    pinMode(Lane3[i], OUTPUT);
    pinMode(Lane4[i], OUTPUT);
  }

  digitalWrite(Lane1[0], HIGH);
  digitalWrite(Lane2[0], HIGH);
  digitalWrite(Lane3[0], HIGH);
  digitalWrite(Lane4[0], HIGH);
  digitalWrite(Lane1[1], LOW);
  digitalWrite(Lane2[1], LOW);
  digitalWrite(Lane3[1], LOW);
  digitalWrite(Lane4[1], LOW);


  density_cnt->onRun(count_density);               
  density_cnt->setInterval(500);                   

  signal_con.onRun(traffic_set);                  
  signal_con.setInterval(500);                    
  controll[3].setInterval(150);
}
void loop()
{


  controll.run();                                     //asking controller to run


}

