#include <BLDC-Kod.h>

#define x1 PIN_D0
#define x0 PIN_D1
#define y1 PIN_D2
#define y0 PIN_D3
#define z1 PIN_D4
#define z0 PIN_D5
#use timer(timer=1,tick=1us,bits=32,NOISR)

int16 bilgi,delay=4000;
int durum=1;
signed int16 emfx=0;
signed int16 emfy;
signed int16 emfz;
signed int16 ort_deger;
signed int16 delta_x=0;
signed int16 Lastdelta=-1;
unsigned int32 current_tick, previous_tick;

unsigned int32 tick_difference(unsigned int32 current,unsigned int32 previous) 
{
 return(current - previous);
}

void main()
{
   setup_adc(adc_clock_div_16);
   setup_adc_ports(AN0_TO_AN3);

   while(TRUE)
   {        
      set_adc_channel(1);
      delay_us(10);
      emfx=read_adc();
      
      set_adc_channel(2);
      delay_us(10);
      emfy=read_adc();
      
      set_adc_channel(3);
      delay_us(10);
      emfz=read_adc();
      ort_deger=(emfx+emfy+emfz)/3;
      
      current_tick = get_ticks();
      
      if(tick_difference(current_tick , previous_tick) >= delay) 
      {
         previous_tick += delay;

         switch(durum)
         {
            case 1:
            //Z-Y
            output_low(x1);
            output_low(x0);
            output_low(y1);
            output_low(z0);
            output_high(y0);
            output_high(z1);
            
            delta_x=(emfx - ort_deger);
            
            break;
            
            case 2:    
            //X-Y
            output_low(x0);
            output_low(y1);
            output_low(z1);
            output_low(z0);
            output_high(x1);
            output_high(y0);
                    
            delta_x=(emfz - ort_deger);
            
            break;
            
            case 3:         
             //X-Z
            output_low(x0);
            output_low(y1);
            output_low(y0);      
            output_low(z1);
            output_high(z0);
            output_high(x1);
            
            delta_x=(emfy - ort_deger);
            
            break;
            
            case 4:
            //Y-Z
            output_low(x1);
            output_low(x0);     
            output_low(y0);
            output_low(z1);
            output_high(y1);
            output_high(z0);
            
            delta_x=(emfx - ort_deger);
            break;
            
            case 5:
            //Y-X
            output_low(x1);
            output_low(y0);
            output_low(z1);
            output_low(z0);   
            output_high(x0);
            output_high(y1);

            delta_x=(emfz - ort_deger);
            
            break;
            
            case 6:
            //Z-X
            output_low(x1);
            output_low(y1);
            output_low(y0);    
            output_low(z0);
            output_high(z1);
            output_high(x0);
            
            delta_x=(emfy - ort_deger);
            
            break;
         }
      
        if(Lastdelta<=0)
         {  
            if(delta_x>0)
            {
               Lastdelta=delta_x;
               durum=durum+1;
               if(durum>6)
               {
                  durum=1;
               }
            }
         }
         if(Lastdelta>0)
         {  
            if(delta_x<=0)
            {
               Lastdelta=delta_x;
               durum=durum+1;
               if(durum>6)
               {
                  durum=1;
               }
            }
         }
      } 
      
      set_adc_channel(0);
      delay_us(10);
      bilgi=read_adc();
      delay=bilgi*0.98;  
   } 
}
