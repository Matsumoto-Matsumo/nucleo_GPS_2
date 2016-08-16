#include "mbed.h"

DigitalOut myled(LED1);
Serial gps(D1, D0);        // tx, rx
Serial pc(USBTX, USBRX);    // tx, rx
 
void pc_rx() {

      unsigned char c;
      int i,rlock;
      char gps_data[256],gps2_data[256];
      char ns,ew;
      float time,hokui,tokei;
      float g_hokui,g_tokei;
      float d_hokui,m_hokui,d_tokei,m_tokei;
      
      gps.baud(9600);
      pc.printf("*** GPS GT-720F ***");    

    while (1) {
      i=0;
      while(gps.getc()!='$'){
      }
      
      while((gps_data[i]=gps.getc()) != '\r'){
        i++;
        if(i==256){
           pc.printf("*** Div Error! ***\n");
           i=255;
           break;
         }
      }
      gps_data[i]='\0';
      
      if( sscanf(gps_data, "GPGGA,%f,%f,%c,%f,%c,%d",&time,&hokui,&ns,&tokei,&ew,&rlock) >= 1){
        if(rlock==1){
          pc.printf("Status:Lock(%d)\n\r",rlock);  
         //logitude
          d_tokei= int(tokei/100);
          m_tokei= (tokei-d_tokei*100)/60;
          g_tokei= d_tokei+m_tokei; 
          pc.printf("Log:%4.5f,",g_tokei);
         //Latitude
          d_hokui=int(hokui/100);
          m_hokui=(hokui-d_hokui*100)/60;
          g_hokui=d_hokui+m_hokui;
          pc.printf("Lat:%4.5f\n\r",g_hokui);
          
        }
        else{
          pc.printf("\n\rStatus:unLock(%d)\n\r",rlock);
          for(i=0;i<40;i++){
            pc.printf("%c",gps_data[i]);
          }
        }
      }//if
    }//while
}

int main(){
    gps.attach(pc_rx,Serial::RxIrq);
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}