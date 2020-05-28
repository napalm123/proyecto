#include <TinyGPS++.h>
#include <SoftwareSerial.h>  
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
TinyGPSPlus gps; 
SoftwareSerial SIM900(7, 8);
float vel_ant = 0.0;  

void setup(void) {
  Serial.begin(9600); 
  SIM900.begin(9600); 
  lcd.begin(20, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("velocidad:");
} 
void loop() {
  while (Serial.available() > 0) {
   obtener_velocidad(); 
  }
}
void obtener_velocidad(){
if (gps.encode(Serial.read())) {
    float lat,lng;
    if (gps.speed.isValid()) {
      lat = gps.location.lat();
      lng = gps.location.lng();
      Serial.print("Velocidad = ");
      Serial.print(gps.speed.kmph());
      Serial.println(" km/h");
      if(vel_ant > 60 && gps.speed.kmph() == 0)  sos(lat,lng);
      vel_ant = gps.speed.kmph(); 
      ver_display(vel_ant,lat,lng); 
    }else Serial.println("Esperando...");
    } 
}
void sos(float lat, float lng){
    Serial.println("enviando senal de auxilio");
    Serial.print("latitud: ");
    Serial.print(lat);
    Serial.print("  longitud: ");
    Serial.println(lng);
    SIM900.print("AT+CMGF=1\r"); 
    delay(1000);
    SIM900.println("AT + CMGS = \"+573212564383\"");//número que recibirá el msj
    delay(1000);
    SIM900.println("senal de auxilio - ha ocurrido un siniestro de transito");
    SIM900.print("Latitude = ");
    SIM900.print(lat);
    SIM900.print("  Longitude = ");
    SIM900.print(lng);
    delay(1000);
    SIM900.println((char)26); // finaliza el comando AT con ^Z, ASCII 26
    delay(1000);
    SIM900.println();
}
void ver_display(float vel_ant,float lat,float lng){
  lcd.setCursor(10,0);
  lcd.print(vel_ant,2);
  lcd.setCursor(16,0);
  lcd.print("km/h");
  lcd.setCursor(0,1);
  lcd.print("lat:");
  lcd.print(lat,2);
  lcd.setCursor(10,1);
  lcd.print("lng:");
  lcd.print(lng,2);
}
