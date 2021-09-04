#include <WiFi.h>
#include <HTTPClient.h>
#include "CTBot.h";
CTBot myBot;
const char* ssid = "ASUSX555Q";
const char* pass = "welcomestark";
const char* host = "monitor-udara.promonissanbogor.com";
//-----------------------------------------//
String token = "1995437905:AAErOdyqPisVpTIrQXc5lBYw-wQM-4YESB8";
const int id = 1647096270; // ID USER
//-----------------------------------------//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
//-----------------------------------------//
#define pinSensor 36                           // pin sensor mq7
long RL1 = 1000;                               // 1000 Ohm
long Ro1 = 830;                                // 830 ohm ( SILAHKAN DISESUAIKAN)
int sensorvalue;
int outputmq;
int CO;
double Rs1;
double VRL1;
#define buzzer 13
//-----------------------------------------//
#include <SoftwareSerial.h>
SoftwareSerial co2Serial(32, 33);              // define MH-Z19 RX TX D3 (GPIO0) and D4 (GPIO2)
int ppm_uart;
int outputmhz;
int CO2;
//-----------------------------------------//
int period = 200;
unsigned long time_now = 0;
//-----------------------------------------//
int datakirim = 0;
int waktubuzzer1 = 0, buzzer1, flagbuzzer1 = false;
//-----------------------------------------//
int flagtco1 = false, flagtco2 = false, flagtco3 = false;
int flagtco21 = false, flagtco22 = false, flagtco23 = false;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  //-----------------------------------------//
  co2Serial.begin(9600);
  //-----------------------------------------//
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //-----------------------------------------//
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  if (myBot.testConnection()) {
    Serial.println("Koneksi Bot Telegram Berhasil");
  } else {
    Serial.println("Koneksi Bot Telegram Gagal");
  }
  //-----------------------------------------//
  lcd.setCursor(0, 0);
  lcd.print("   Connecting Wifi  ");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  delay(2000);
  lcd.clear();
  //-----------------------------------------//
  lcd.setCursor(0, 0);
  lcd.print("    Connecting to   ");
  lcd.setCursor(0, 1);
  lcd.print(host);
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  delay(2000);
  lcd.clear();
  //-----------------------------------------//
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("   KUALITAS UDARA   ");
  delay(3000);
  lcd.setCursor(0, 2);
  lcd.print("    SISTEM START    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  delay(3000);
  lcd.clear();
  //-----------------------------------------//
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {
  if (millis() > time_now + period) {
    time_now = millis();
    TBMessage msg;                             //baca pesan masuk dari telegram
awal :
    SensorMq7();
    SensorMhz19b();

    lcd.setCursor(0, 0);
    lcd.print("Co  = ");
    lcd.setCursor(6, 0);
    lcd.print(CO);
    lcd.print(" PPM     ");
    lcd.setCursor(0, 1);
    lcd.print("status : ");
    lcd.setCursor(9, 1);
    if (CO >= 0 && CO <= 74) {
      lcd.print("NORMAL ");
      flagtco2 = false;
      flagtco3 = false;
      if (flagtco1 == false) {
        flagtco1 = true;
        myBot.sendMessage(id, "Kondisi Co : NORMAL | Cek Website : monitor-udara.promonissanbogor.com");
      }
    } else if (CO >= 75 && CO <= 399) {
      lcd.print("WASPADA");
      flagtco1 = false;
      flagtco3 = false;
      if (flagtco2 == false) {
        flagtco2 = true;
        myBot.sendMessage(id, "Kondisi Co : WASPADA ! | Cek Website : monitor-udara.promonissanbogor.com");
      }
    } else {
      lcd.print("BAHAYA ");
      flagtco1 = false;
      flagtco2 = false;
      if (flagtco3 == false) {
        flagtco3 = true;
        myBot.sendMessage(id, "Kondisi Co : BAHAYA ! | Cek Website : monitor-udara.promonissanbogor.com");
      }
    }

    lcd.setCursor(0, 2);
    lcd.print("Co2 = ");
    lcd.setCursor(6, 2);
    lcd.print(CO2);
    lcd.print(" PPM     ");
    lcd.setCursor(0, 3);
    lcd.print("status : ");
    lcd.setCursor(9, 3);
    if (CO2 >= 0 && CO2 <= 1499) {
      lcd.print("NORMAL ");
      flagtco22 = false;
      flagtco23 = false;
      if (flagtco21 == false) {
        flagtco21 = true;
        myBot.sendMessage(id, "Kondisi Co2 : NORMAL | Cek Website : monitor-udara.promonissanbogor.com");
      }
    } else if (CO2 >= 1500 && CO2 <= 7999) {
      lcd.print("WASPADA");
      flagtco21 = false;
      flagtco23 = false;
      if (flagtco22 == false) {
        flagtco22 = true;
        myBot.sendMessage(id, "Kondisi Co2 : WASPADA ! | Cek Website : monitor-udara.promonissanbogor.com");
      }
    } else {
      lcd.print("BAHAYA ");
      flagtco21 = false;
      flagtco22 = false;
      if (flagtco23 == false) {
        flagtco23 = true;
        myBot.sendMessage(id, "Kondisi Co2 : BAHAYA ! | Cek Website : monitor-udara.promonissanbogor.com");
      }
    }

    if (CO >= 400 || CO2 >= 8000) {
      buzzer1 = 1;
    }

    if (buzzer1 == 1 && waktubuzzer1 < 5) {
      if (flagbuzzer1 == false) {
        flagbuzzer1 = true;
        digitalWrite(buzzer, HIGH);
        delay(50);
      } else {
        digitalWrite(buzzer, LOW);
        flagbuzzer1 = false;
        delay(50);
      }
    }
    if (waktubuzzer1 == 5 && buzzer1 == 1) {
      digitalWrite(buzzer, LOW);
      buzzer1 = 0;
      waktubuzzer1 = 0;
      goto awal;
    }
    if (buzzer1 == 1) {
      waktubuzzer1++;
    }

    datakirim++;
    if (datakirim == 5) {
      KirimData();
      datakirim = 0;
    }

  }
}

void KirimData() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection Failed");
    return;
  }
  String Link;
  HTTPClient http;
  Link = "http://" + String(host) + "/tambah_proses.php?co=" + String(CO) + "&co2=" + String(CO2);
  http.begin(Link);
  http.GET();
  String respon = http.getString();
  Serial.println(Link);
  //  Serial.println(respon);
  http.end();
}

void SensorMhz19b() {
  ppm_uart = readCO2UART();
}

int readCO2UART() {
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char response[9];
  co2Serial.write(cmd, 9); //request PPM CO2
  // clear the buffer
  memset(response, 0, 9);
  int i = 0;
  while (co2Serial.available() == 0) {
    i++;
  }
  if (co2Serial.available() > 0) {
    co2Serial.readBytes(response, 9);
  }
  // print out the response in hexa
  for (int i = 0; i < 9; i++) {
    //    Serial.print(String(response[i], HEX));
    //    Serial.print("   ");
  }
  //  Serial.println("");
  // checksum
  byte check = getCheckSum(response);
  if (response[8] != check) {
    //    Serial.println("Checksum not OK!");
    //    Serial.print("Received: ");
    //    Serial.println(response[8]);
    //    Serial.print("Should be: ");
    //    Serial.println(check);
  }
  // ppm
  const int batasmhz = 50000;
  outputmhz = 256 * (int)response[2] + response[3];
  CO2 = min(outputmhz, batasmhz);
  Serial.print("CO2 : ");
  Serial.print(CO2);
  Serial.println(" ppm");
  // temp
  //  byte temp = response[4] - 40;
  //  Serial.print("Sensor Temperature: ");
  //  Serial.println(temp);
  // status
  byte status = response[5];
  //  Serial.print("Status: ");
  //  Serial.println(status);
  if (status == 0x40) {
    //    Serial.println("Status OK");
  }
  return ppm_uart;
}
byte getCheckSum(char *packet) {
  byte i;
  unsigned char checksum = 0;
  for (i = 1; i < 8; i++) {
    checksum += packet[i];
  }
  checksum = 0xff - checksum;
  checksum += 1;
  return checksum;
}

void SensorMq7() {
  const int batasmq7 = 6400;
  sensorvalue = analogRead(pinSensor);
  VRL1 = sensorvalue * 3.3 / 4095;
  Rs1 = ( 3.3 * RL1 / VRL1 ) - RL1;
  outputmq = 100 * pow(Rs1 / Ro1, -1.53);
  if (outputmq >= 0 && outputmq <= 6400) {
    CO = min(outputmq, batasmq7);
    Serial.print("CO : ");
    Serial.print(CO);
    Serial.println(" ppm");
  } else {
    CO = 6400;
    Serial.print("CO : ");
    Serial.print(CO);
    Serial.println(" ppm");
  }
}
