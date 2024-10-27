//ver2  可以用來 只print出加速度值   若改成c的 可以手動創造data.txt
// 2024.10.27 這個程式用來收集訓練資料，收集是用print的,
#include <Wire.h>
#include <Adafruit_Sensor.h>    // Adafruit  sensor library
#include <Adafruit_ADXL345_U.h>  // ADXL345 library
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();   // ADXL345 Object
//=========================== i2c adxl345 read

const byte INT_PIN = 15;
volatile int flag = 0;
volatile int pushed = 0;
volatile int more_than_once = 0;
volatile int a = 0;     //time a
volatile int b = 0;     //time b
//time a and b for button debounce用
//====
#define max_data_len  400
#define new_length  50

volatile int count = 0;
//==
volatile int pushed_times = 0;
volatile int k=0;
void irq(){
  pushed = 1;
  if(pushed == 1 && more_than_once == 0){   //真正的按 開始或結束
      flag = !flag;
      a = millis();
      more_than_once =1;
      pushed_times++;
      k=0;
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode ( INT_PIN , INPUT_PULLUP );
  attachInterrupt ( digitalPinToInterrupt ( INT_PIN ), irq , FALLING );

  if(!accel.begin())   // if ASXL345 sensor not found
  {
    Serial.println("ADXL345 not detected");
    while(1);
  }
}

void loop() {
  b = millis();
  if (b - a >= 350){
    pushed = 0;
    more_than_once = 0;
  }
  if(flag == 0)
    count=0;
  if(flag == 1 ){    //此部分都為reading
    if(count < max_data_len ){
      sensors_event_t event;
      accel.getEvent(&event);

      Serial.print(event.acceleration.x);
      Serial.print(" ");
      Serial.print(event.acceleration.y);
      Serial.print(" ");
      Serial.print(event.acceleration.z);//m/s^2
      Serial.print("\n");
      delay(10);//最快一秒抓100次  
      count++;
    }
  }
  if(pushed_times%2 == 0){
    if (k == 0){  //只是為了只顯示一次 (此為k的存在目的)
      Serial.print("====================");
      Serial.println(pushed_times/2);
      k=1;
    }
  }
}
