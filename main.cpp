/*
説明
Nucleo-F303K8とGPSmモジュールを使ったサンプルプログラム

参考
http://www.hiramine.com/physicalcomputing/general/gps_nmeaformat.html


以下ピン配置
Nucleo  GPSモジュール
GND-----GND-----------0V
５ｖ------VIN
D0------TX
D1------RX
*/
#include "mbed.h"

DigitalOut myled(LED1);
Serial gps(D1, D0);       // tx, rx
Serial pc(PA_2, PA_3);    // tx, rx
int i,rlock,mode;
char gps_data[256],gps2_data[256];
char ns,ew;
float w_time,hokui,tokei;
float g_hokui,g_tokei;
float d_hokui,m_hokui,d_tokei,m_tokei;

void getGPS() {
  unsigned char c = gps.getc();
  if( c=='$' || i == 256){
    mode = 0;
    i = 0;
  }
  if(mode==0){
    if((gps_data[i]=c) != '\r'){
      i++;
    }else{
      gps_data[i]='\0';
      // pc.printf("%s\r\n",gps_data);

      if( sscanf(gps_data, "$GPGGA,%f,%f,%c,%f,%c,%d",&w_time,&hokui,&ns,&tokei,&ew,&rlock) >= 1){
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
          pc.printf("%s",gps_data);
        }
      }//if
    }
  }
}

int main(){
  pc.printf("*** GPS GT-720F ***");
  gps.baud(9600);
  pc.baud(115200);
  gps.attach(getGPS,Serial::RxIrq);
  while(1) {
    myled = 1;
    wait(0.2);
    myled = 0;
    wait(0.2);
  }
}
