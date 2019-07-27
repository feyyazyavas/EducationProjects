#include <main.h>
#include <stdlib.h>
#include <stdbool.h>

#use timer(timer=1,tick=1us,bits=32,NOISR)
#use FIXED_IO( C_outputs=PIN_C5 )
#use FIXED_IO( A_outputs=PIN_A2,PIN_A1,PIN_A0 )
#use fast_io(b)


#define Output1 PIN_A0
#define Output2 PIN_A1
#define Output3 PIN_A2

#define Output_Pin   PIN_C5

#define Sensor1_Pin  PIN_B4
#define Sensor2_Pin  PIN_B5
#define Sensor3_Pin  PIN_B6

#define Encoder1_Pin1   PIN_B0
#define Encoder1_Pin2   PIN_B1
#define Encoder1_Pin3   PIN_B2
#define Encoder1_Pin4   PIN_B3

#define Encoder2_Pin1   PIN_C0
#define Encoder2_Pin2   PIN_C1
#define Encoder2_Pin3   PIN_C2
#define Encoder2_Pin4   PIN_C3

#define beginningTime 5000000


unsigned int32 selectTick, selectCounter;
unsigned int32 startTime, previousTime1, previousTime2, previousTime3;

int encoderVal[10];
int highTime, lowTime;

bool programState=true;
bool interruptState=true;
bool interruptEnable=true;
bool condition = true;

bool sensor1Activity=true;
bool sensor2Activity=true;
bool sensor3Activity=true;

bool sensor1_INT=true;
bool sensor2_INT=true;
bool sensor3_INT=true;

bool sensor1State=true;
bool sensor2State=true;
bool sensor3State=true;
bool outputState=true;


#int_RB
void  Sensor_Interrupt()
{   
   if(interruptState == true)
   {  
         if((input(Sensor1_Pin) == 0) && (sensor1_INT == 1) && (sensor1Activity == 1))
         {
            sensor1State = true;
            sensor1_INT = false;
            previousTime1 = get_ticks() - startTime;
            printf("1) gecen sure:%ld\r\n",previousTime1);

         }
         else if((input(Sensor1_Pin) == 1) && (sensor1Activity == 1))
         {
            sensor1_INT = true;
         }
         
         if((input(Sensor2_Pin) == 0) && (sensor2_INT == 1) && (sensor2Activity == 1))
         {
            sensor2State = true;
            sensor2_INT = false;
            previousTime2 = get_ticks() - startTime;
            printf("2) gecen sure:%ld\r\n",previousTime2);
         }
         else if((input(Sensor2_Pin) == 1) && (sensor2Activity == 1))
         {
            sensor2_INT = true;
         }
         
         if((input(Sensor3_Pin) == 0) && (sensor3_INT == 1) && (sensor3Activity == 1))
         {
            sensor3State = true;
            sensor3_INT = false;
            previousTime3 = get_ticks() - startTime;
            printf("3) gecen sure:%ld\r\n",previousTime3);
         }
         else if((input(Sensor3_Pin) == 1) && (sensor3Activity == 1))
         {
            sensor3_INT = true; 
         }
   }
}

unsigned int32 tick_difference(unsigned int32 current,unsigned int32 previous) 
{
   return(current - previous);
}

int encoderCalc_1()
{
   if(input(Encoder1_Pin1) == 1)
   { 
      encoderVal[1] = 1; 
   }
   if(input(Encoder1_Pin2) == 1)
   { 
      encoderVal[2] = 2; 
   }
   if(input(Encoder1_Pin3) == 1)
   { 
      encoderVal[3] = 4; 
   }
   if(input(Encoder1_Pin4) == 1)
   { 
      encoderVal[4] = 8; 
   }
   return(encoderVal[1]+encoderVal[2]+encoderVal[3]+encoderVal[4]);
}

int encoderCalc_2()
{
   if(input(Encoder2_Pin1) == 1)
   { 
      encoderVal[5] = 1; 
   }
   if(input(Encoder2_Pin2) == 1)
   { 
      encoderVal[6] = 2; 
   }
   if(input(Encoder2_Pin3) == 1)
   { 
      encoderVal[7] = 4; 
   }
   if(input(Encoder2_Pin4) == 1)
   { 
      encoderVal[8] = 8; 
   }
   return(encoderVal[5]+encoderVal[6]+encoderVal[7]+encoderVal[8]);
}

void Start()
{
   do
   {
      output_high(Output_Pin);
      if(input(Sensor1_Pin) == 0)
      {
         sensor1Activity = true;
         printf("sensor1 aktif\r\n");
         output_high(Output1);
      }
   
      if(input(Sensor2_Pin) == 0)
      {
         sensor2Activity = true;
         printf("sensor2 aktif\r\n");
         output_high(Output2);
      }
   
      if(input(Sensor3_Pin) == 0)
      {
         sensor3Activity = true;
         printf("sensor3 aktif\r\n");
         output_high(Output3);
      }
   }
   while(get_ticks() < beginningTime);
}

void OutputControl()
{
   if(programState == true)
      {
         if(get_ticks() - selectTick >= 1250000)
         {
            selectTick = get_ticks();
            selectCounter++;
         }
         
         if(selectCounter > 0 && selectCounter <= highTime)
         {  
            output_high(Output_Pin);
            if(condition == true)
            {
               if((sensor1State && sensor2State && sensor3State && outputState) == true)
               {  
                  startTime = get_ticks();
                  printf("1)zaman kaydedildi\r\n");
                  sensor1State = false;
                  sensor2State = false;
                  sensor3State = false;
                  outputState = false;
                  condition = false;
                  
                  //interrupt bir kere aktif et.
                  if(interruptEnable == true)
                  {
                     enable_interrupts(GLOBAL);
                     enable_interrupts(INT_RB);
                     printf("interrupt enabled\r\n"); 
                     interruptState=true;     
                     interruptEnable = false;
                  }  
               }    
            }
            else if(condition == false)  
            {
               if((!(((sensor1State && !sensor1Activity) || (sensor1Activity && !sensor1State)) && ((sensor2State && !sensor2Activity) || (sensor2Activity && !sensor2State)) && ((sensor3State && !sensor3Activity) || (sensor3Activity && !sensor3State))) && outputState) == true)
               {  
                  startTime = get_ticks();
                  printf("2)zaman kaydedildi\r\n");
                  sensor1State = false;
                  sensor2State = false;
                  sensor3State = false;
                  outputState = false;
               }
            }
         }
         else if(selectCounter > highTime && selectCounter <= (highTime + lowTime))
         {  
            output_low(Output_Pin);
            outputState = true;
         }
         
         if(selectCounter >= (highTime + lowTime))
         {
            selectCounter = 0;
         }
         
         if(get_ticks() - startTime > 20000000)
         {
            interruptState = false;
            programState = false;
            printf("error-program durdu.\r\n");
            disable_interrupts(GLOBAL);
            disable_interrupts(INT_RB);
            output_low(Output_Pin);
            output_low(Output1);
            output_low(Output2);
            output_low(Output3);
         }
         
//!         if(previousTime > 5000000)
//!         {
//!            printf("error-program durdu.\r\n");
//!            output_low(Output_Pin);
//!            programState = 0;
//!         }
      }
}

void closingControl()
{
 
}

void main()
{  
   output_low(Output_Pin);
   output_low(Output1);
   output_low(Output2);
   output_low(Output3);
   
   highTime = encoderCalc_1();
   lowTime = encoderCalc_2();
   
   disable_interrupts(GLOBAL);
   disable_interrupts(INT_RB);
   
   Start();
   
   while(TRUE)
   {  
      OutputControl();
      ClosingControl();
   }
}
