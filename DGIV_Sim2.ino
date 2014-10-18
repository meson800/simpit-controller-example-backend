/*
DGIV Simulator Arduino Code
By Christopher Johnstone
*/
#define readPin 10
#define startMux 2
//Channels for the multiplexers
int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };
//changed to bool instad of int  
bool switches [16][16];
byte sLoop [225] = {0,1,2,3,4,5,6,7,16,17,18,19,20,21,22,23,32,33,34,35,36,37,38,39,48,49,50,51,52,53,54,55,64,65,66,67,68,69,70,71,8,9,10,11,12,13,14,15,24,25,26,27,28,29,30,31,40,41,42,43,44,45,46,47,56,57,58,59,72,73,74,75,88,89,90,91,96,97,98,99,100,112,113,114,115,116,128,129,130,131,132,144,145,146,147,148,149,160,161,162,163,164,165,176,177,178,179,180,181,192,193,194,195,196,197};
//byte sLoop[255] = {115};

int sLoopLength;

void setup()
{
  //sLoopLength = sizeof(sLoop) / sizeof(short);
  sLoopLength = 115;
  Serial.begin(9600);
  for (int i = startMux; i < startMux + 8; i++)
  {
    pinMode(i,OUTPUT);
  }
  pinMode(readPin,INPUT);
  
  
}

void loop()
{
  //check for a reset or a flush
  if(Serial.available() > 0)
  {
    int inByte = Serial.read();
    /*if (inByte == 'r')
    {
      sLoopLength = Serial.read();
      while (Serial.available() < sLoopLength) {}
      //now that we have all of the bytes
      for (int i = 0; i < sLoopLength; i++)
        sLoop[i] = Serial.read();
    }
    */
    //flush bytes if we reset or if we just want the flush- it doesn't matter
    if (inByte == 'f' || inByte == 'r')
    {
      for(int i = 0; i < sLoopLength; i++)
       {
         int x = sLoop[i] - ((sLoop[i] / 16) * 16);
         int y = (sLoop[i] / 16);
         switches[x][y] = readSwitch(x,y);
         writeState(sLoop[i],switches[x][y]);
       }
       Serial.println();
    }
  }
  
  //loop through all of the switches
  for (short i = 0; i < sLoopLength; i++)
  {
   int x = sLoop[i] - ((sLoop[i] / 16) * 16);
   int y = (sLoop[i] / 16);

    
      int tempState = readSwitch(x,y);
      if (tempState != switches[x][y])
      {
        delay(50);
        if (tempState == readSwitch(x,y))
        {
        switches[x][y] = tempState;

        //split current switch into digits
        int numberToSplit = (x+(y*16));
        writeState(numberToSplit,tempState);
      }
      }
 }
}


int readSwitch(int x, int y)
{ 

  
  //loop over mux's control pins and set correct value
  for (int i = 0; i < 4; i++)
  {
    //write to ouput mux
  digitalWrite(startMux + i, muxChannel[x][i]);
  //write to input mux
  digitalWrite(startMux + i + 4, muxChannel[y][i]);
  }
  return digitalRead(readPin);
  
}

void writeState(int num, int state)
{
    int digits [3];
    for (int n = 0; n<3; n++)
    {
       digits[2-n] = num % 10;
       num /= 10;
    }
    //form is .  1st digit  2nd digit  3 digit  ,  state  /
    Serial.print('.');
    //print digits
    Serial.print(digits[0]);
    Serial.print(digits[1]);
    Serial.print(digits[2]);
    //Print seperator
    Serial.print(',');
    //Print state
    Serial.print(state);
    //Print ending token
    Serial.print('/');
}
