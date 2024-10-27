#include <Wire.h>
#include <Adafruit_Sensor.h>    // Adafruit  sensor library
#include <Adafruit_ADXL345_U.h>  // ADXL345 library
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();   // ADXL345 Object
//=========================== i2c adxl345 read

const byte INT_PIN = 15;
volatile int flag = 0;
volatile int pushed = 0;
volatile int more_than_once = 0;
volatile int time_a = 0;     //time a
volatile int time_b = 0;     //time b
//====
#define max_data_len  400
#define new_length  50

float raw_x[max_data_len]={0.0},raw_y[max_data_len]={0.0},raw_z[max_data_len]={0.0};
float test_data[3 * new_length] = {0.0};

volatile int count = 0;
//==
volatile int pushed_times = 0;
volatile int k=0;
//===predict
#define n_features new_length*3
#define samples 1  //testdata數 之後可把sample刪了
float w[n_features] = {-0.04071,-0.04200,-0.04493,-0.05621,-0.03840,0.00393,0.05789,0.11229,0.11695,0.08287,0.09192,0.13973,0.18635,0.15229,0.22943,0.22002,0.12750,0.06889,-0.04005,-0.04290,0.07632,-0.08349,-0.05230,-0.14244,-0.08455,-0.03976,-0.04218,-0.13398,-0.09938,-0.08373,-0.05045,-0.05900,-0.04537,-0.07166,-0.08745,-0.04722,-0.12276,-0.04281,-0.11360,-0.03163,-0.08465,-0.04314,-0.05391,-0.02303,-0.01337,-0.02035,0.04985,-0.00460,0.02682,0.17297,-0.22512,-0.25566,-0.28123,-0.30236,-0.26338,-0.19732,-0.10667,-0.04840,0.02692,0.07824,0.10598,0.15591,0.21595,0.28179,0.35377,0.45026,0.57385,0.62322,0.51465,0.38892,0.10002,-0.19152,-0.30376,-0.34033,-0.28591,-0.30171,-0.22199,-0.14105,-0.13509,-0.17856,-0.21410,-0.17416,-0.13239,-0.08194,-0.04729,-0.03404,-0.03191,-0.03904,-0.05191,-0.05005,-0.04780,-0.03756,-0.04795,-0.05517,-0.05720,-0.06757,-0.08623,-0.12067,-0.13437,-0.10848,0.18192,0.19504,0.21579,0.24541,0.26080,0.26282,0.24715,0.20117,0.14038,0.08674,0.00463,-0.08073,-0.16542,-0.22770,-0.24038,-0.24699,-0.11760,0.09490,0.13763,0.21740,0.21318,0.19692,0.16656,0.11641,0.06760,0.01036,-0.00405,-0.01767,-0.04730,-0.08559,-0.08884,-0.10627,-0.10222,-0.10661,-0.11334,-0.12337,-0.10801,-0.10774,-0.10715,-0.09384,-0.08710,-0.07451,-0.05859,-0.04316,-0.03008,-0.02006,-0.02769,-0.03654,-0.02170,0.07682};


float b=0.27362;
float tmp[n_features];  //to fetch test data

float sigmoid(float x){   // sigmoid activation is coded
  double y = 1/(1+exp(-x));
  return y;
}
//code for n-input neuron with sigmoid activation
float neuron(float W[],float B, float X[],int nn){ //n is feature size
  float z,y = 0.0;
  for(int i=0; i < nn; i++)
  {
     y += W[i]*X[i];
  }
  y += B;
  z = sigmoid(y);
  return z;
}

void irq(){
  pushed = 1;
  if(pushed == 1 && more_than_once == 0){   //真正的按 開始或結束
      flag = !flag;
      time_a = millis();
      more_than_once =1;
      pushed_times++;
      k=0;
  }
}

void normalize(float* data, int length) // 將數列正規化到0到1之間
{
    float min = data[0];
    float max = data[0];
    int i;
    // 找出數列中的最小值和最大值
    for (int i = 1; i < length; i++) {
        if (data[i] < min) {
            min = data[i];
        }
        if (data[i] > max) {
            max = data[i];
        }
    }

    for (int i = 0; i < length; i++) {
        data[i] = (data[i] - min) / (max - min);
    }
}

void scale_data(float (*data), int length) {
    int i;
    float factor = (float)(length - 1) / (new_length - 1);
    float *new_data;
    float max, min, mean;
    if((new_data=(float*)malloc(new_length * sizeof(float)))==NULL)
        printf("malloc error\n");
    for (i = 0; i < new_length; i++) {
        float index = i * factor;
        int lower_index = (int)index;
        int upper_index = lower_index + 1;
        float weight = index - lower_index;
        if (upper_index >= length) {
            new_data[i] = (data)[lower_index];
        } else {
            new_data[i] = (1 - weight) * (data)[lower_index] + weight * (data)[upper_index];
        }
    }
    // data正規化
    normalize(new_data,new_length);
    memcpy(data, new_data, new_length * sizeof(float));
    free(new_data);
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
  while(1){
    time_b = millis();
    if (time_b - time_a >= 350){
      pushed = 0;
      more_than_once = 0;
    }
    //if(flag == 0)
    //  count=0;
    if(flag == 1 ){    //此部分都為reading
      if(count < max_data_len ){
        sensors_event_t event;
        accel.getEvent(&event);
        raw_x[count]=event.acceleration.x;
        Serial.print(raw_x[count]);
        Serial.print(" ");
        raw_y[count]=event.acceleration.y;
        Serial.print(raw_y[count]);
        Serial.print(" ");
        raw_z[count]=event.acceleration.z;
        Serial.print(raw_z[count]);//m/s^2
        Serial.print("\n");
        delay(10);//最快一秒抓100次  
        count++;
      }
    }
    if(pushed_times!=0 && pushed_times%2 == 0){
      if (k == 0){  //只是為了只顯示一次 (此為k的存在目的)
        Serial.print("========== round ");
        Serial.print(pushed_times/2);
        Serial.println(" ==========");
        k=1;
        break;  //跳出catch data模式
      }
    }
  }
  //scale and predicte
  scale_data(raw_x, count);                                             
  scale_data(raw_y, count);
  scale_data(raw_z, count);   //至此raw_x y z已經是處理過的了
  for (int j = 0; j < new_length; j++) 
  {
    test_data[j] = raw_x[j];
    test_data[j + new_length] = raw_y[j];
    test_data[j + new_length*2] = raw_z[j];
  }
  /*
  for(int k=0;k < new_length*3 ;k++){ //scale後的資料 看看是否有正確scale
    Serial.print(k);
    Serial.print(":");
    Serial.print(test_data[k]);
    Serial.print(" ");
  }
  */
  //****** Prediction using evaluated model **//
  Serial.println(F("\nPredicted values: "));
  float y[samples];
  int j=0;
  for(int i=0; i< samples; i++) 
  {
    j = n_features*i;
    for(int k=0;k<n_features;k++){
    tmp[k]=test_data[j+k];
    }
    y[i] = neuron(w,b,tmp,n_features);
    if(y[i]>0.5){
      y[i]=1;
      Serial.print(y[i],2);
      Serial.print(F("\t"));
      Serial.println("Defence !!!");
    }
    else{
      y[i]=0;
      Serial.print(y[i],2);
      Serial.print(F("\t"));
      Serial.println("Attack !!!");
    }
  }
  count=0;//整個過程結束後再把count設為0
}
