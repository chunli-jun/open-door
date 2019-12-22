
#include <U8glib.h>
#define INTERVAL_LCD 20 //定义OLED刷新时间间隔 
unsigned long lcd_time = millis(); //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); //设置OLED型号 
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_osb21)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_04b_03bn)

#define humanHotSensor 4
char buf[10];
char buf1[30];
#include <ESP8266.h>
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
#define EspSerial mySerial
ESP8266 wifi(&EspSerial);
#define UARTSPEED  9600
#endif

#define SSID        F("111")
#define PASSWORD    F("12345678")
#define HOST_NAME   F("api.heclouds.com")
#define HOST_PORT   (80)
#define DEVICEID "562127032" //OneNet上的设备ID
#define PROJECTID "288752" //OneNet上的产品ID
String apiKey="gWYB0GA6bQ9vnbAPqMp0BRVgdBI=";//与你的设备绑定的APIKey

static const byte  GETDATA[]  PROGMEM = {
  "GET  https://api.heclouds.com/devices/562127032/datapoints?datastream_id=reply&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:gWYB0GA6bQ9vnbAPqMp0BRVgdBI=\r\nConnection:close\r\n\r\n"}
;
#define INTERVAL_SENSOR 2000 //定义传感器采样时间间隔 597000
#define INTERVAL_NET 2000 //定义发送时间
#include <Wire.h> //调用库
#define IDLE_TIMEOUT_MS 3000


#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();
#define INTERVAL_OLED 1000
String mCottenData;
String jsonToSend;
char humanHotState; 
char humanHotState_c[7]; //换成char数组传输
unsigned long net_time1 = millis(); //数据上传服务器时间
unsigned long sensor_time = millis(); //传感器采样时间计时器
String postString; //用于存储发送数据的字符串
uint16_t Val;



void setup(void)
{ Wire.begin();
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
  delay(100);
pinMode( humanHotSensor,INPUT);
Serial.println("Initialisation complete.");

  WifiInit(EspSerial, UARTSPEED);

   if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    
  } else {
    
  }

  if (wifi.disableMUX()) {
    
  } else {
  
  }

  Serial.print(F("setup end\r\n"));
}

void loop(void)
{
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.println(F("create tcp ok\r\n"));
    
  }
  if (sensor_time > millis()) sensor_time = millis(); 

if(millis() - sensor_time > INTERVAL_SENSOR) //传感器采样时间间隔 
{ 
getSensorData(); //读串口中的传感器数据
sensor_time = millis();
}

if (net_time1 > millis()) net_time1 = millis();

if (millis() - net_time1 > INTERVAL_NET) //发送数据时间间隔
{ 
updateSensorData(); //将数据上传到服务器的函数
net_time1 = millis();
}
pt();
}
void getSensorData(){ 
humanHotState=digitalRead(humanHotSensor);
Serial.println(humanHotState);
dtostrf(humanHotState, 3, 1, humanHotState_c);
u8g.firstPage();//u8glib规定的写法
 do {
 setFont_L;//选择字体
 u8g.drawStr(0, 10,buf1);//选择位置
 }while( u8g.nextPage() );//u8glib规定的写
 

}


void updateSensorData() {
if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
Serial.print("create tcp ok\r\n");
jsonToSend="{\"humanHotSensor\":";
dtostrf(humanHotState,1,2,buf);
jsonToSend+="\""+String(buf)+"\"";
jsonToSend+="}";
postString="POST /devices/";
postString+=DEVICEID;
postString+="/datapoints?type=3 HTTP/1.1";
postString+="\r\n";
postString+="api-key:";
postString+=apiKey;
postString+="\r\n";
postString+="Host:api.heclouds.com\r\n";
postString+="Connection:close\r\n";
postString+="Content-Length:";
postString+=jsonToSend.length();
postString+="\r\n";
postString+="\r\n";
postString+=jsonToSend;
postString+="\r\n";
postString+="\r\n";
postString+="\r\n";
const char *postArray = postString.c_str(); //将str转化为char数组
Serial.println(postArray);
wifi.send((const uint8_t*)postArray, strlen(postArray)); //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
Serial.println("send success"); 
if (wifi.releaseTCP()) { //释放TCP连接
Serial.print("release tcp ok\r\n");
} 
else {
Serial.print("release tcp err\r\n");
}
postArray = NULL; //清空数组，等待下次传输数据

} else {
Serial.print("create tcp err\r\n");
}
}


  void pt()//接受onenet上的数据并将它显示在串口的
{
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));  
  //char *hello = "GET / HTTP/1.0\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
  //wifi.send((const uint8_t*)hello, strlen(hello));  //直接发送，为什么注释了呢？可以想一下

  wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存
uint8_t buffer[512] = {0};
uint32_t k=0;
uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0) {
   for (uint32_t i = 0; i < len; i++) {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
      {    for(uint32_t j=i+8;j<len;j++)
               {if( (char)buffer[j]=='"'&&(char)buffer[j+1]=='}'&&(char)buffer[j+2]==']'&&(char)buffer[j+3]==',')
                         break;
                else
                         {Serial.print((char)buffer[j]);
                           buf1[k]=((char)buffer[j]);
                              k++;
                           }
               }
       }
      }
}

