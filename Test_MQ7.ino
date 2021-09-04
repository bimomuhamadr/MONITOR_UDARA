#define pinSensor 36 // mendefinisikan bahwa pin yang digunakan 
// untuk membaca sensor adalah pin A0

void setup()
{
  Serial.begin(9600);
}

long RL = 1000; // 1000 Ohm
long Ro = 830; // 830 ohm ( SILAHKAN DISESUAIKAN)

void loop()
{
  int sensorvalue = analogRead(pinSensor); // membaca nilai ADC dari sensor
  Serial.println(sensorvalue);
  double VRL = sensorvalue * 3.3 / 4095; // mengubah nilai ADC ( 0 - 1023 ) menjadi nilai voltase ( 0 - 5.00 volt )
  Serial.print("VRL : ");
  Serial.print(VRL);
  Serial.println(" volt");

  double Rs = ( 3.3 * RL / VRL ) - RL;
  Serial.print("Rs : ");
  Serial.print(Rs);
  Serial.println(" Ohm");

  double ppm = 100 * pow(Rs / Ro, -1.53); // ppm = 100 * ((rs/ro)^-1.53);
  Serial.print("CO : ");
  Serial.print(ppm);
  Serial.println(" ppm");

  Serial.println();
  delay(500);
}
