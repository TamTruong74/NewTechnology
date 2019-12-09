
 

#include <Servo.h>
#include "DHT.h"
#include <LiquidCrystal.h>

#define servoPin 2
#define rainSensorPin A2
#define DHTPIN 12    // Chân DATA nối với PD0
#define LDR_PIN A1    // Chân PE3 nối với chân OUT cảm biến as
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define led1 13
#define led2 A3 
int humDHT;
int tempDHT;
int lumen;
String rain;

DHT dht(DHTPIN, DHTTYPE);
Servo myPointer;
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

void setup() {
  // Khởi tạo cổng serial baud 115200
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  lcd.print("Xin chao");
  
  myPointer.attach(servoPin);
  pinMode(rainSensorPin, INPUT);  
  pinMode(led1, OUTPUT);
//  pinMode(led2, OUTPUT);
 
}

void loop() {
  delay(1000);
  readSensors();
  printData();
  showDataLCD();
  systemRain();
  
}
void showLed(int as, int nd)
{
  if (as<50)
    digitalWrite(led1, HIGH);
  else
    digitalWrite(led1, LOW);
  if (nd<10)
    digitalWrite(led2, HIGH);
  else
    digitalWrite(led2, LOW);
}

int getLumen(int anaPin)
{
  int anaValue = 0;
  for (int i = 0; i < 10; i++) // Đọc giá trị cảm biến 10 lần và lấy giá trị trung bình
  {
    anaValue += analogRead(anaPin);
    delay(50);
  }

  anaValue = anaValue / 10;
  anaValue = map(anaValue, 1023, 0, 0, 100); //Tối:0  ==> Sáng 100%
  return anaValue;
}

void systemRain(void)
{
  int value = digitalRead(rainSensorPin);//Đọc tín hiệu cảm biến mưa

  if ( value == HIGH )
  {
    Serial.println("khong mua");
    myPointer.write(90);  // Servo ở vị trí 90 độ.
    rain="KhMongua";
  }
  else
  {
    Serial.println("mua mat tieu luon");
    delay(100);        // Dừng 1s trước khi mở cửa.
    myPointer.write(0);   // Servo ở vị trí 0 độ.
    rain="KhongMua";

  }
  
}
void readSensors(void)
{
  tempDHT = dht.readTemperature();   //Đọc nhiệt độ DHT22
  humDHT = dht.readHumidity();       //Đọc độ ẩm DHT22
  lumen = getLumen(LDR_PIN);         //Đọc ánh sáng
  showLed(lumen, tempDHT);

}
void showDataLCD(void)
{
  lcd.clear();            //Xóa màn hình
  lcd.setCursor(0, 1);
  lcd.print(" DO.AM% = ");
  lcd.print(humDHT);
  lcd.println("  % " );

  lcd.setCursor(1, 0);
  lcd.print(" NH.DO = ");
  lcd.print(tempDHT);
  lcd.println(" *C ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 1);

  lcd.setCursor(1, 0);
  lcd.print("A.SANG% = ");
  lcd.print(lumen);
  lcd.println(" %  ");
  lcd.setCursor(1, 1);
  lcd.print(rain);
  lcd.println(" %  ");
}
void printData(void)
{
  // IN thông tin ra màn hình
  Serial.print("Do am: ");
  Serial.print(humDHT);
  Serial.print(" %\t");
  Serial.print("Nhiet do: ");
  Serial.print(tempDHT);
  Serial.print(" *C\t");
  Serial.print("Anh sang: ");
  Serial.print(lumen);
  Serial.print(" %\t");

}
