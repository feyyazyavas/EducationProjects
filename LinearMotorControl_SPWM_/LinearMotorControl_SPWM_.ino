
const int EN1 = 2;
const int EN2 = 4;
const int EN3 = 7;
 
const int IN1 = 9;
const int IN2 = 10;
const int IN3 = 11;

const int Hall1 = 12;
const int Hall2 = 13;
const int DIG = 8;

int Phase_X;
int Phase_Y;
int Phase_Z;
int sinArrayLength;
int PhaseDiff;
int rotation=1;
int event=1 ; // event=true --> forward , event=false --> backward
int seal1=1;
int seal2=1;
int seal3=1;
int seal4=1;
int val1,val2;
int dig_read;
int i=0,j=0,k=0,x=1;
char data;

// SPWM
//const int pwmSin[] = {127, 138, 149, 160, 170, 181, 191, 200, 209, 217, 224, 231, 237, 242, 246, 250, 252, 254, 254, 254, 252, 250, 246, 242, 237, 231, 224, 217, 209, 200, 191, 181, 170, 160, 149, 138, 127, 116, 105, 94, 84, 73, 64, 54, 45, 37, 30, 23, 17, 12, 8, 4, 2, 0, 0, 0, 2, 4, 8, 12, 17, 23, 30, 37, 45, 54, 64, 73, 84, 94, 105, 116 };

//const int pwmSin[]={127,129,131,134,136,138,140,143,145,147,149,151,154,156,158,160,162,164,166,169,171,173,175,177,179,181,183,185,187,189,191,193,195,196,198,200,202,204,205,207,209,211,212,214,216,217,219,220,222,223,225,226,227,229,230,231,233,234,235,236,237,239,240,241,242,243,243,244,245,246,247,248,248,249,250,250,251,251,252,252,253,253,253,254,254,254,254,254,254,254,255,254,254,254,254,254,254,254,253,253,253,252,252,251,251,250,250,249,248,248,247,246,245,244,243,243,242,241,240,239,237,236,235,234,233,231,230,229,227,226,225,223,222,220,219,217,216,214,212,211,209,207,205,204,202,200,198,196,195,193,191,189,187,185,183,181,179,177,175,173,171,169,166,164,162,160,158,156,154,151,149,147,145,143,140,138,136,134,131,129,127,125,123,120,118,116,114,111,109,107,105,103,100,98,96,94,92,90,88,85,83,81,79,77,75,73,71,69,67,65,63,61,59,58,56,54,52,50,49,47,45,43,42,40,38,37,35,34,32,31,29,28,27,25,24,23,21,20,19,18,17,15,14,13,12,11,11,10,9,8,7,6,6,5,4,4,3,3,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,4,5,6,6,7,8,9,10,11,11,12,13,14,15,17,18,19,20,21,23,24,25,27,28,29,31,32,34,35,37,38,40,42,43,45,47,49,50,52,54,56,58,59,61,63,65,67,69,71,73,75,77,79,81,83,85,88,90,92,94,96,98,100,103,105,107,109,111,114,116,118,120,123,125,127};

// SVPWM

//const int pwmSin[]={128,131,135,138,141,145,148,151,155,158,161,164,168,171,174,177,181,184,187,190,193,197,200,203,206,209,212,215,218,221,224,225,225,226,227,228,229,230,230,231,232,232,233,234,234,235,235,236,236,236,237,237,237,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,237,237,237,236,236,236,235,235,234,234,233,232,232,231,230,230,229,228,227,226,225,225,224,225,225,226,227,228,229,230,230,231,232,232,233,234,234,235,235,236,236,236,237,237,237,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,237,237,237,236,236,236,235,235,234,234,233,232,232,231,230,230,229,228,227,226,225,225,224,221,218,215,212,209,206,203,200,197,193,190,187,184,181,177,174,171,168,164,161,158,155,151,148,145,141,138,135,131,128,125,121,118,115,111,108,105,101,98,95,92,88,85,82,79,75,72,69,66,63,59,56,53,50,47,44,41,38,35,32,31,31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,19,19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,20,20,20,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,31,31,30,29,28,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,19,19,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,20,20,20,21,21,22,22,23,24,24,25,26,26,27,28,29,30,31,31,32,35,38,41,44,47,50,53,56,59,63,66,69,72,75,79,82,85,88,92,95,98,101,105,108,111,115,118,121,125};


const int pwmSin[] = {128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124};

void setup() 
{
  PwmFrequencySetting(IN1);
  PwmFrequencySetting(IN2);
  PwmFrequencySetting(IN3);
 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  
  pinMode(EN1, OUTPUT); 
  pinMode(EN2, OUTPUT); 
  pinMode(EN3, OUTPUT);

  pinMode(Hall1, INPUT);
  pinMode(Hall2, INPUT);
 
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(EN3, HIGH);
  
  Serial.begin(9600);
 
  sinArrayLength = sizeof(pwmSin)/sizeof(int); 
  PhaseDiff = sinArrayLength/3;
  Phase_X = 0;
  Phase_Y = Phase_X + PhaseDiff;
  Phase_Z = Phase_Y + PhaseDiff;
  sinArrayLength--;
  StartingPosition();
}

void loop()
{
  dig_read=digitalRead(DIG);
  if(dig_read==0)
  {
    if(seal3==1)
    {
      seal3=0;
      Serial.println(1);
    }
  }
  else seal3=1;
  
  if(dig_read==1)
  {
    if(seal4==1)
    {
      seal4=0;
      Serial.println(0);
    }
  }
  else seal4=1;
  
  if(Serial.available()>0)
  {
    data=Serial.read();
    
    if(data=='w')
    {
      if(k==1 || x==1)
      {
        backward();
        x=0;
      }
    }
  }

  val1=digitalRead(Hall1);
  if(val1==0)
  {
    j=0;
    if(seal1==1)
    {
      i++;
      seal1=0; 
      if(i==1)
      {
        k=0;
        forward(); 
      }
    }  
  }
  else seal1=1;
  
  val2=digitalRead(Hall2);
  if(val2==0)
  {
    i=0;
    if(seal2==1)
    {  
      j++;
      seal2=0;   
      if(j==1)
      {
        k++;
        stop();
      } 
    }  
  }
  else seal2=1;
  
  analogWrite(IN1, pwmSin[Phase_X]);
  analogWrite(IN2, pwmSin[Phase_Y]);
  analogWrite(IN3, pwmSin[Phase_Z]);
  
  if (event==1) rotation = 1; 
  else if(event==0) rotation = -1;
  else rotation = 0;
  
  Phase_X = Phase_X + rotation;
  Phase_Y = Phase_Y + rotation;
  Phase_Z = Phase_Z + rotation;
  
  if(Phase_X > sinArrayLength)
  Phase_X = 0; 
  else if(Phase_X < 0)
  Phase_X = sinArrayLength;
 
  if(Phase_Y > sinArrayLength)
  Phase_Y = 0;
  else if(Phase_Y < 0)
  Phase_Y = sinArrayLength;
 
  if(Phase_Z > sinArrayLength)
  Phase_Z = 0; 
  else if(Phase_Z < 0)
  Phase_Z = sinArrayLength;
  
  delayMicroseconds(150);
}

void forward()
{
  event=1;
  if(rotation==-1)
  {
    Phase_X = 0;
    Phase_Y = Phase_X + PhaseDiff;
    Phase_Z = Phase_Y + PhaseDiff;  
  }  
}

void backward()
{
  event=0;
  if(rotation==1)
  {
    Phase_X = 0;
    Phase_Y = Phase_X + PhaseDiff;
    Phase_Z = Phase_Y + PhaseDiff;
  }  
}

void stop()
{
  event=2;
  if(rotation==1 || rotation==-1)
  {
    Phase_X = 0;
    Phase_Y = Phase_X + PhaseDiff;
    Phase_Z = Phase_Y + PhaseDiff;  
  }  
}

void PwmFrequencySetting(int pin) 
{
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) 
  {
    if(pin == 5 || pin == 6) 
    {
      TCCR0B = TCCR0B & 0b11111000 | 0x01;
    } 
    else 
    {
      TCCR1B = TCCR1B & 0b11111000 | 0x01;      //31 KHz
      //TCCR1B = TCCR1B & B11111000 | B00000010;  //4 KHz
      //TCCR1B = TCCR1B & B11111000 | B00000011;  //500 Hz
      //TCCR1B = TCCR1B & B11111000 | B00000100;  //122 Hz
      //TCCR1B = TCCR1B & B11111000 | B00000101;  //30 Hz
    }
  }
  else if(pin == 3 || pin == 11) 
  {
    TCCR2B = TCCR2B & 0b11111000 | 0x01;     //31 KHz
    //TCCR2B = TCCR2B & B11111000 | B00000010; //4 KHz
    //TCCR2B = TCCR2B & B11111000 | B00000100; //500 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000110; //122 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000111; //30 Hz
  }
}

void StartingPosition()
{
  int initialVal1=digitalRead(Hall1);
  int initialVal2=digitalRead(Hall2);

  if(initialVal1==0 || initialVal2==0)
  {
    event=1;
  }
  
  if (event==1) 
  rotation = 1;   
  else if(event==0) 
  rotation = -1;  
  else
  rotation = 0;

  do
  {
    analogWrite(IN1, pwmSin[Phase_X]);
    analogWrite(IN2, pwmSin[Phase_Y]);
    analogWrite(IN3, pwmSin[Phase_Z]);
    
    Phase_X = Phase_X + rotation;
    Phase_Y = Phase_Y + rotation;
    Phase_Z = Phase_Z + rotation;
 
    if(Phase_X > sinArrayLength)
    Phase_X = 0;    
    else if(Phase_X < 0)
    Phase_X = sinArrayLength;
 
    if(Phase_Y > sinArrayLength)
    Phase_Y = 0;   
    else if(Phase_Y < 0)
    Phase_Y = sinArrayLength;
 
    if(Phase_Z > sinArrayLength)
    Phase_Z = 0;    
    else if(Phase_Z < 0)
    Phase_Z = sinArrayLength;
    
    delayMicroseconds(300);
  }
  while(digitalRead(Hall2)==1);
  stop();
}
