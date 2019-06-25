
#include <18f4550.h>     
#device ADC=10  
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#use delay (clock=20000000)
#use i2c(master,sda=pin_b0,scl=pin_b1,slow=100000) //Eeprom haberle�mesi i�in pinler belirlendi.
#include <2416.c> //24c16 eeprom k�t�phanesi.
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1,TIMEOUT=10) //seri haberle�me i�in rx-tx pinleri belirlendi.

long int bilgi,bilgi2,s1,s2,deger;
char gelen,sicaklik;
int j=1; //saya� i�in j tan�mland�.

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
   s1=(bilgi * 0.48828125); //adc'den okunan 0-1023 aras�ndaki de�eri 0-5 volt aras�nda bir de�ere d�n��t�rmek i�in (5/1024) i�lemi yap�ld�, milivolta d�n��t�rmek i�in 1000 ile �arp�ld�.
   //Her 10mV'da 1 derece artt��� i�in 10'a b�l�nd�. => ((5/1024)*1000)/10 
   
   
   set_adc_channel(1); 
   delay_ms(20);    
   bilgi2=read_adc();
   s2=(bilgi2 * 0.48828125); //Yukar�da yap�lan adc i�lemi burada da yap�ld�.
      
   deger=(s1-s2); //-55 ile 150 derece aras�nda �l��m yapmas� i�in kurdu�umuz devreye g�re bu iki de�erin ��kar�lmas� ile �l��m yap�l�yor.

   write_eeprom(j,deger); //Eepromun j. indisine okunan de�er yaz�l�yor.
   delay_ms(100);  
   j=j+1; //j de�eri 1 art�r�l�yor.
   if(j==2000)
   {
   goto git;
   }
   }
   
   if(gelen=='b')
   {
   git:
   for(int k=1; k<j; k++) //Eeproma yaz�lan j. sat�ra kadar okuma yap�l�yor.
   {
   sicaklik=read_eeprom(k);
   delay_ms(20);
   
   printf("%d\r\n",sicaklik);
   delay_ms(500);
   }
   j=1;//Okuma i�lemi bittikten sonra saya� ba�tan ba�lat�l�yor.
   }
   }  
}
