
#include <18f4550.h>     
#device ADC=10  
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#use delay (clock=20000000)
#use i2c(master,sda=pin_b0,scl=pin_b1,slow=100000) //Eeprom haberleþmesi için pinler belirlendi.
#include <2416.c> //24c16 eeprom kütüphanesi.
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1,TIMEOUT=10) //seri haberleþme için rx-tx pinleri belirlendi.

long int bilgi,bilgi2,s1,s2,deger;
char gelen,sicaklik;
int j=1; //sayaç için j tanýmlandý.

void main ( )
{
   setup_adc(adc_clock_div_2);
   setup_adc_ports(ALL_ANALOG);
   
   while(true)
   {
   gelen=getc();
   
   if(gelen=='a')
   {
   set_adc_channel(0);
   delay_ms(20);         
   bilgi=read_adc();
   s1=(bilgi * 0.48828125); //adc'den okunan 0-1023 arasýndaki deðeri 0-5 volt arasýnda bir deðere dönüþtürmek için (5/1024) iþlemi yapýldý, milivolta dönüþtürmek için 1000 ile çarpýldý.
   //Her 10mV'da 1 derece arttýðý için 10'a bölündü. => ((5/1024)*1000)/10 
   
   
   set_adc_channel(1); 
   delay_ms(20);    
   bilgi2=read_adc();
   s2=(bilgi2 * 0.48828125); //Yukarýda yapýlan adc iþlemi burada da yapýldý.
      
   deger=(s1-s2); //-55 ile 150 derece arasýnda ölçüm yapmasý için kurduðumuz devreye göre bu iki deðerin çýkarýlmasý ile ölçüm yapýlýyor.

   write_eeprom(j,deger); //Eepromun j. indisine okunan deðer yazýlýyor.
   delay_ms(100);  
   j=j+1; //j deðeri 1 artýrýlýyor.
   if(j==2000)
   {
   goto git;
   }
   }
   
   if(gelen=='b')
   {
   git:
   for(int k=1; k<j; k++) //Eeproma yazýlan j. satýra kadar okuma yapýlýyor.
   {
   sicaklik=read_eeprom(k);
   delay_ms(20);
   
   printf("%d\r\n",sicaklik);
   delay_ms(500);
   }
   j=1;//Okuma iþlemi bittikten sonra sayaç baþtan baþlatýlýyor.
   }
   }  
}
