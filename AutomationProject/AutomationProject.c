//Header Files
#include <AutomationProject.h>
#include <stdlib.h>
#include <stdbool.h>

//Crystal - Tick Timer - RS232 Configuration
#use delay(crystal=20000000)
#use timer(timer=1,tick=1us,bits=32,NOISR)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

//Setting Pins As Output
#use FIXED_IO( C_outputs=PIN_C5 )
#use FIXED_IO( A_outputs=PIN_A2,PIN_A1,PIN_A0 )
#use fast_io(b)

//Input - Output Pins Definition
#define Input1 PIN_A3
#define Output1 PIN_A0
#define Output2 PIN_A1
#define Output3 PIN_A2
#define Output_Pin   PIN_C5

//Sensor Input Pin Definition
#define Sensor1_Pin  PIN_B4
#define Sensor2_Pin  PIN_B5
#define Sensor3_Pin  PIN_B6

//Encoder Input Pin Definiton
#define Encoder1_Pin1   PIN_B0
#define Encoder1_Pin2   PIN_B1
#define Encoder1_Pin3   PIN_B2
#define Encoder1_Pin4   PIN_B3

#define Encoder2_Pin1   PIN_C0
#define Encoder2_Pin2   PIN_C1
#define Encoder2_Pin3   PIN_C2
#define Encoder2_Pin4   PIN_C3

//Beginning Time Definition
#define beginningTime 12500000


unsigned int32 selectTick, selectCounter;


int encoderVal[5];
float encoderMap;
int encoder1, encoder2;
float highTime, lowTime;
float previous_highTime, previous_lowTime;


int outputState=true;


int sensor1Activity=false;
int sensor2Activity=false;
int sensor3Activity=false;


float period;
int t1,t2,t3;
int closeVal;


float Val;
int multipletion;
float division;


#int_RB
void  Sensor_Interrupt()
{   
   if((input(Sensor1_Pin) == 0) && (sensor1Activity == true))
   {
      t1=0;
   }
   
   if((input(Sensor2_Pin) == 0) && (sensor2Activity == true))
   {
      t2=0;
   }
   
   if((input(Sensor3_Pin) == 0) && (sensor3Activity == true))
   {
      t3=0;
   }
}


unsigned int32 tick_difference(unsigned int32 current,unsigned int32 previous) 
{
   return(current - previous);
}


double mapf(double val, double in_min, double in_max, double out_min, double out_max) 
{
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int encoderGetValue(int8 pin1, int8 pin2, int8 pin3, int8 pin4)
{
   if(input(pin1))
   { 
      encoderVal[1] = 1; 
   }
   else if(!input(pin1))
   {
      encoderVal[1] = 0;
   }
   
   if(input(pin2))
   { 
      encoderVal[2] = 2; 
   }
   else if(!input(pin2))
   {
      encoderVal[2] = 0;
   }
   
   if(input(pin3))
   { 
      encoderVal[3] = 4; 
   }
   else if(!input(pin3))
   {
      encoderVal[3] = 0;
   }
   
   if(input(pin4))
   { 
      encoderVal[4] = 8; 
   }
   else if(!input(pin4))
   {
      encoderVal[4] = 0;
   }
   return(encoderVal[1] + encoderVal[2] + encoderVal[3] + encoderVal[4]);
}


float encoderCalc()
{
   encoder1 = encoderGetValue(Encoder1_Pin1, Encoder1_Pin2, Encoder1_Pin3, Encoder1_Pin4);
   printf("encoder1:%d\r\n",encoder1);
   
   encoder2 = encoderGetValue(Encoder2_Pin1, Encoder2_Pin2, Encoder2_Pin3, Encoder2_Pin4);
   printf("encoder2:%d\r\n",encoder2);
   
   if(encoder1 <= 6)
   {
      switch(encoder1)
      {
         case 0:
         {
            encoderMap = 20;
            break;
         }
         
         case 1:
         {
            encoderMap = mapf(encoder2,0,15,2,4);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 2:
         {
            encoderMap = mapf(encoder2,0,15,4,6);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 3:
         {
            encoderMap = mapf(encoder2,0,15,6,8);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 4:
         {
            encoderMap = mapf(encoder2,0,15,8,10);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 5:
         {
            encoderMap = mapf(encoder2,0,15,10,13);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 6:
         {
            encoderMap = mapf(encoder2,0,15,13,17);
            encoderMap = encoderMap*10;
            break;
         } 
      }
   }
   else
   {
      encoderMap = 20;
   }
   return(encoderMap);
}


void ClosingLoopCalc()
{
   period = (float)highTime*2;
   period = (float)period/10;
   
   if(period > 10)
   {
      closeVal = 1;
      printf("kapanma:%d\r\n",CloseVal);
   }
   else if(period <= 10)
   {
      period = (float)period;
      Val = (10/period);
      multipletion = Val*10;
      multipletion = multipletion%10;
      
      if(multipletion >= 5)
      {
         division = (float)multipletion/10;
         Val = Val + division;
         CloseVal=Val;
         printf("kapanma:%d\r\n",CloseVal);
      }
      else if(multipletion < 5)
      {
         division = (float)multipletion/10;
         Val = Val - division;
         CloseVal=Val;
         printf("kapanma:%d\r\n",CloseVal);
      }     
   }
}


void Pin2Control()
{
   if(input(Input1) == 1)
   {
      highTime = 20;
      lowTime = 20;
      t1 = 0;
      t2 = 0;
      t3 = 0;
   }
   else
   {
      highTime = previous_highTime;
      lowTime = previous_lowTime;
   }
}

void OutputControl(int16 _highTime, int16 _lowTime)
{
   if(get_ticks() - selectTick >= 125000)
   {
      selectTick = get_ticks();
      selectCounter++;
   }
   
   if(selectCounter > 0 && selectCounter <= _highTime)
   {          
      if(outputState == true)
      {  
         if(sensor1Activity == true)
         {
            t1++;
         }
         
         if(sensor2Activity == true)
         {
            t2++;
         }
         
         if(sensor3Activity == true)
         {
            t3++;
         }
         
         output_high(Output_Pin);
         outputState = false;
      }    
   }
   else if(selectCounter > _highTime && selectCounter <= (_highTime + _lowTime))
   {  
      output_low(Output_Pin);
      outputState = true;
   }
   
   if(selectCounter >= (_highTime + _lowTime))
   {
      selectCounter = 0;
   }  
}


void SensorActivityControl()
{
   if(input(Sensor1_Pin) == 0)
   {
      Sensor1Activity = true;
   }
   
   if(input(Sensor2_Pin) == 0)
   {
      Sensor2Activity = true;
   }
   
   if(input(Sensor3_Pin) == 0)
   {
      Sensor3Activity = true;
   }
}


void Start()
{
   output_high(Output1);
   output_high(Output2);
   output_high(Output3);
   
   do
   {
      Pin2Control();
      OutputControl(highTime, lowTime);
      SensorActivityControl();
   }
   while(get_ticks() < beginningTime);
   
   output_low(Output_Pin);
   output_low(Output1);
   output_low(Output2);
   output_low(Output3);

   if(sensor1Activity == true)
   {
      output_high(Output1);
   }
   
   if(sensor2Activity == true)
   {
      output_high(Output2);
   }

   if(sensor3Activity == true)
   {
      output_high(Output3);
   }

   selectCounter = 0;
   selectTick = 0;
}


void ClosingControl()
{
   if(t1 > closeVal)
   {
      sensor1Activity = false;
      output_low(Output1);
   }
   
   if(t2 > closeVal)
   {
      sensor2Activity = false;
      output_low(Output2);
   }
   
   if(t3 > closeVal)
   {
      sensor3Activity = false;
      output_low(Output3);
   }
}


void main()
{  
   disable_interrupts(GLOBAL);
   disable_interrupts(INT_RB);
     
   output_low(Output1);
   output_low(Output2);
   output_low(Output3);
   output_low(Output_Pin);
   
   highTime = encoderCalc();
   printf("highTime:%f\r\n",highTime);
   
   lowTime = highTime;
   printf("lowTime:%f\r\n",lowTime);
   
   previous_highTime = highTime;
   previous_lowTime = lowTime;
      
   ClosingLoopCalc();
   
   Start();
   
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RB);
   
   while(TRUE)
   {  
      OutputControl(highTime, lowTime);
      ClosingControl();
      Pin2Control();
   }
}
