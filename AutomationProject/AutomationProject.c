//Header Files
#include <AutomationProject.h>
#include <stdlib.h>
#include <stdbool.h>

//Crystal - Tick Timer - RS232 Configuration
#use delay(crystal=20000000)
#use timer(timer=1,tick=1us,bits=32,NOISR)
//!#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

//Setting Pins As Output
#use FIXED_IO( C_outputs=PIN_C3 )
#use FIXED_IO( A_outputs=PIN_A5,PIN_A2,PIN_A1,PIN_A0 )
#use fast_io(b)

//Input - Output Pins Definition
#define Input1 PIN_A3
#define Output1 PIN_A2
#define Output2 PIN_A1
#define Output3 PIN_A0
#define Output_Pin   PIN_C3
#define Led PIN_A5

//Sensor Input Pin Definition
#define Sensor1_Pin  PIN_B6
#define Sensor2_Pin  PIN_B5
#define Sensor3_Pin  PIN_B4

//Encoder Input Pin Definiton
#define Encoder1_Pin1   PIN_B3
#define Encoder1_Pin2   PIN_B2
#define Encoder1_Pin3   PIN_B1
#define Encoder1_Pin4   PIN_B0

#define Encoder2_Pin1   PIN_C7
#define Encoder2_Pin2   PIN_C6
#define Encoder2_Pin3   PIN_C5
#define Encoder2_Pin4   PIN_C4



unsigned int32 selectTick, selectCounter;
unsigned int32 selectTick_Led, selectCounter_Led;
int beginningTime;


int encoderVal[5];
float encoderMap;
int encoder1, encoder2;
float highTime, lowTime;
float previous_highTime, previous_lowTime;
int previous_beginningTime;


int outputState=true;


int sensor1Activity=false;
int sensor2Activity=false;
int sensor3Activity=false;


int t1,t2,t3;
int closeVal;
int counter=0;
int pin1=false;


#int_RB
void  Sensor_Interrupt()
{   
   if((input(Sensor1_Pin) == 0))
   {
      t1=0;
      output_high(Output1);
      sensor1Activity = true;
   }
   
   if((input(Sensor2_Pin) == 0))
   {
      t2=0;
      output_high(Output2);
      sensor2Activity = true;
   }
   
   if((input(Sensor3_Pin) == 0))
   {
      t3=0;
      output_high(Output3);
      sensor3Activity = true;
   }
}

void Led_Control()
{
   if(get_ticks() - selectTick_Led >= 125000)
   {
      selectTick_Led = get_ticks();
      selectCounter_Led++;
   }
   
   if(selectCounter_Led > 0 && selectCounter_Led <= 4)
   {          
      output_high(Led);     
   }
   else if(selectCounter_Led > 4 && selectCounter_Led <= 8)
   {  
      output_low(Led);
   }
   
   if(selectCounter_Led >= 8)
   {
      selectCounter_Led = 0;
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
//!   printf("encoder1:%d\r\n",encoder1);
   
   encoder2 = encoderGetValue(Encoder2_Pin1, Encoder2_Pin2, Encoder2_Pin3, Encoder2_Pin4);
//!   printf("encoder2:%d\r\n",encoder2);
   
   if(encoder1 <= 6)
   {
      switch(encoder1)
      {
         case 0:
         {
            encoderMap = mapf(encoder2,0,15,32,16);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 1:
         {
            encoderMap = mapf(encoder2,0,15,24,12);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 2:
         {
            encoderMap = mapf(encoder2,0,15,16,8);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 3:
         {
            encoderMap = mapf(encoder2,0,15,12,6);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 4:
         {
            encoderMap = mapf(encoder2,0,15,8,4);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 5:
         {
            encoderMap = mapf(encoder2,0,15,6,3);
            encoderMap = encoderMap*10;
            break;
         }
         
         case 6:
         {
            encoderMap = mapf(encoder2,0,15,4,2);
            encoderMap = encoderMap*10;
            break;
         } 
      }
   }
   else
   {
      encoderMap = 20;
      beginningTime = 4;
   }
   return(encoderMap);
}


void ClosingLoopCalc()
{
   if(encoder1 < 3)
   {
      closeVal = 2;
      beginningTime = 2;
      previous_beginningTime = beginningTime;
   }
   else
   {
      closeVal = 3;
      beginningTime = 3;
      previous_beginningTime = beginningTime;
   }
}


void OutputControl(int16 _highTime, int16 _lowTime)
{
   Led_Control();
   
   if(get_ticks() - selectTick >= 125000)
   {
      selectTick = get_ticks();
      selectCounter++;
   }
   
   if(selectCounter > 0 && selectCounter <= _highTime)
   {          
      if(outputState == true)
      {  
         counter++;
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
         outputState = false;
         output_high(Output_Pin);   
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


void Start()
{
   start:
   output_high(Output1);
   output_high(Output2);
   output_high(Output3);
   
   do
   {
      OutputControl(highTime, lowTime);
      
      if(input(Input1) == 0 && pin1==true)
      {
//!         printf("RESET:\r\n");
         reset_cpu();
         
      }
      
      if(input(Input1) == 1 && pin1==false)
      {
         pin1 = true;
         lowTime=10;
         counter = 0;
         beginningTime = 4;
         selectCounter = 0;
         selectTick = 0;
                 
         goto start;
      }
   }
   while(counter < beginningTime);
   

   if(sensor1Activity == true)
   {
      output_high(Output1);
   }
   else
   {
      output_low(Output1);
   }
   
   if(sensor2Activity == true)
   {
      output_high(Output2);
   }
   else
   {
      output_low(Output2);
   }

   if(sensor3Activity == true)
   {
      output_high(Output3);
   }
   else
   {
      output_low(Output3);
   }

   selectCounter = 0;
   selectTick = 0;
}


void ClosingControl()
{
   if(t1 > closeVal)
   {
      output_low(Output1);
   }
   
   if(t2 > closeVal)
   {
      output_low(Output2);
   }
   
   if(t3 > closeVal)
   {
      output_low(Output3);
   }
}


void main()
{  
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RB);
   
   output_low(Output_Pin);
   output_high(Led);
   
   highTime = 10;
//!   printf("highTime:%f\r\n",highTime);
   
   lowTime = encoderCalc();
//!   printf("lowTime:%f\r\n",lowTime);
   
   previous_highTime = highTime;
   previous_lowTime = lowTime;
      
   ClosingLoopCalc();
   
   if(input(Input1) == 1 && pin1 == false)
   {
      pin1 = true;
      lowTime=10;
      counter = 0;
      beginningTime = 4;
   }
   
   Start();
   
   while(TRUE)
   {  
      OutputControl(highTime, lowTime);
      ClosingControl();
      
      if(input(Input1) == 0 && pin1==true)
      {
         reset_cpu();
      }
      
      if(input(Input1) == 1 && pin1==false)
      {
         pin1 = true;
         lowTime=10;
         counter = 0;
         beginningTime = 4;
         selectCounter = 0;
         selectTick = 0;
         Start();
      }
   }
}
