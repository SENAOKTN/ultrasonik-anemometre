#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int nem, sicaklik, basinc;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET 44
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int rainPin = 3;
int rainStatus = 0;

#include <MPU6050.h>
MPU6050 mpu;

void setup() {
  pinMode(rainPin, INPUT);
  Serial.begin(9600);
  while (!Serial);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Old Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  if(!bmp.begin()){
    while(1);
  }
  
  
}

void loop() {

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);

  float nem_f = dht.readHumidity();
  float sicaklik_f = dht.readTemperature();

  if (isnan(nem) || isnan(sicaklik)) {
    Serial.println("Sensörden veri okunamıyor.");
    return;
  }

  nem = nem_f;
  sicaklik = sicaklik_f;

  display.setCursor(0,0);             
  display.print(sicaklik);
  display.println(" Derece");
  display.print("Nem:%");
  display.println(nem);
  display.display();


  rainStatus = digitalRead(rainPin);

  if (rainStatus == LOW) {
    display.println("Yagmur var");
    display.display();
    
  } else {
    display.println("Yagmur yok");
    display.display();
  }

  sensors_event_t event;
  bmp.getEvent(&event);

  if (event.pressure) {
    // Basınç ölçümü

    basinc = event.pressure;
    int yukseklik = SENSORS_PRESSURE_SEALEVELHPA;
    
    display.print("Basinc: ");
    display.print(basinc);
    display.println(" hPa ");
    display.print("Yukseklik:");
    display.print(yukseklik);
    //display.print(bmp.pressureToAltitude(yukseklik, event.pressure));
    display.println(" metre");
    display.display();

    
  } else {
    Serial.println("Sensörden veri alınamıyor.");
  }


  delay(1000);

  
  
 
  
}
