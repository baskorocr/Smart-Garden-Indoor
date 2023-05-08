//pin 5 = DHT11
//Pin A0 = SoilMosture
//pin1-2 = Relay 1-2
//pin6-8 = RGB led

#define BLYNK_PRINT Serial 
#include <Blynk.h> //untuk library blynk
#include <ESP8266WiFi.h> //untuk library ESP8266WiFi
#include <WiFiClient.h> //library dari wifi client untuk mengirim data ke thingspeak
#include <BlynkSimpleEsp8266.h> //library blynk 
#include <SimpleTimer.h> 
#include "DHT.h" //library sensor DHT
#include <ThingSpeak.h> //untuk library thingspeak
#define DHTTYPE DHT11 //tipe dari DHT
#define dhtPin 14 //pin dari DHT

DHT dht (dhtPin,DHTTYPE); //Deklarasi objek dht menggunakan class DHT pada library

WidgetLCD lcd(V3); //inisialisasi widget LCD

int sensorValue = 0; //nilai dari sensor value
int relay1 = D1; //deklarasi relay 1 di D1
int relay2 = D2; //deklarasi relay 2 di D2
int sensorPin = A0; //deklarasi sensor pin di A0
float t; //deklarasi untuk menampung nilai temperatur (suhu) bilangan cacah
float h; //deklarasi untuk menampung nilai humidity (kelembaban) bilangan cacah 


char auth[] = "MBun8kK5YjBWWwY16msIjP6klH16xzrs"; //kode auth ke blynk


char ssid[] = "CPH1701"; // ID untuk masuk ke blynk
char pass[] = "asu123ok"; // pass untuk masuk ke blynk

char* tulis ="91NQM8IAFW67ICD2"; // API dari thingspeak
const long ChannelId  =1275021; // nomor channel dari thingspeak

const char* server = "api.thingspeak.com"; // sebagai akuisisi data

WiFiClient client; // untuk mengirim data ke thingspeak
SimpleTimer timer;


void setup()
{
  // Debug console
  Serial.begin(9600); // untuk menentukan kecepatan pengiriman dan penerimaan 
  dht.begin(); // pembacaan module sensor
  Blynk.begin(auth, ssid, pass, "blynk.jpnet.co.th", 8080); // 
  pinMode(sensorPin, INPUT); // untuk mengkonfigurasi pin sensor pin menjadi input
  pinMode(relay1, OUTPUT); // untuk mengkonfigurasi pin relay 1 menjadi output
  pinMode(relay2,OUTPUT); // untuk mengkonfigurasi pin relay 2 menjadi output

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass); //menghubungkan ke WiFi
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
}

void sensorSuhu() 
{
  h = dht.readHumidity(); // deklarasi h untuk membaca dht humidity
  t = dht.readTemperature(); // deklarasi t untuk membaca dht temperature
  lcd.print(0,0,"humi= "); 
  lcd.print(7,0,h);
  lcd.print(12,0,"%");
  lcd.print(0,1,"temp=");
  lcd.print(7,1,t);
  lcd.print(11,1,"°C");
}

void loop()
{
  timer.setInterval(2000, sensorSuhu); 
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  Serial.println(t);
  //sensor value belum dicek
  if(sensorValue >900) //value max dan minim blm diketahui
  {
    digitalWrite(relay2, LOW);
    delay(1500); //waktu penyimpanan belum diketahui, tergantung penempatan sensor dengan air
  }
  else if(sensorValue<900)
  {
    digitalWrite(relay2, HIGH);
    delay(5000);
  }

 ThingSpeak.begin(client); //melakukan inisialisasi ke ThingSpeak
////data akan tampilkan pada thingspeak sesuai dengan akun yang dihubungkan
 ThingSpeak.writeField(ChannelId,1,sensorValue,tulis);
 ThingSpeak.writeField(ChannelId,2,t,tulis);
 ThingSpeak.writeField(ChannelId,3,h,tulis);
 delay(1000);
  
  Blynk.run(); //menjalankan Blynk
  timer.run();

}
