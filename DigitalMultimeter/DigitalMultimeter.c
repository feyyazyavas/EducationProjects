#include <DigitalMultimeter.h>
#include <74595.c>
#use fast_io(B)
#define NUMBER_OF_74595 3
 
int rakamlar[11]={63,6,91,79,102,109,125,7,127,111};
long int bilgi,bilgi1;
float volt,amper;
int onlar,birler,virgul,virgulonce,onlar1,birler1,virgul1,virgulonce1;
int data[3];

void main()
{
   setup_adc_ports(sAN0|sAN1);
   setup_adc(ADC_CLOCK_DIV_32);

   while(TRUE)
   {
      set_adc_channel(0);
      delay_us(20);
      bilgi=read_adc();
      volt=bilgi*0.048;

      set_adc_channel(1);
      delay_us(20);
      bilgi1=read_adc();
      amper=bilgi1*0.0048;     
      
      virgul=volt*10;
      virgul=virgul%10;     
      virgulonce=volt-(virgul/10);     
      birler=virgulonce%10;
      onlar=(virgulonce-birler)/10;
      
      virgul1=amper*10;
      virgul1=virgul1%10;     
      virgulonce1=amper-(virgul1/10);     
      birler1=virgulonce1%10;
      onlar1=(virgulonce1-birler1)/10;
      
      Data[0]=rakamlar[onlar];
      Data[1]=rakamlar[birler];
      Data[2]=rakamlar[virgul];
      for(int i=0; i<3; i++)
      {
      write_expanded_outputs(&Data[i]);
      }
      delay_ms(500);
      
      Data[0]=rakamlar[onlar1];
      Data[1]=rakamlar[birler1];
      Data[2]=rakamlar[virgul1];
      for(int j=0; j<3; j++)
      {
      write_expanded_outputs(&Data[j]);
      }
      delay_ms(500);
   }
}
