// =====================================================
// Smart Baby Monitor FINAL with Blynk + Web Server
// ESP8266 + DHT11 + LCD + Fan + LEDs + Buzzer
// =====================================================

// ---------------- Blynk ----------------
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN YOUR_AUTH_TOKEN"

// ---------------- Libraries ----------------
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ---------------- WiFi ----------------
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27,16,2);

// ---------------- DHT11 ----------------
#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

// ---------------- Pins ----------------
#define GREEN_LED D5
#define RED_LED D6
#define BUZZER D7
#define FAN_PIN D0

// ---------------- Settings ----------------
#define FAN_TEMP 30

#define BUZZER_ON LOW
#define BUZZER_OFF HIGH

// ---------------- Variables ----------------
float temperature = 0;
float humidity = 0;

bool dangerState = false;
bool fanState = false;


// ---------------- Notification Control ----------------
bool lastDangerState = false;

unsigned long lastNotificationTime = 0;


// ---------------- Buzzer Pattern Control ----------------
unsigned long buzzerTimer = 0;

bool buzzerState = false;

int beepCount = 0;

#define BEEP_TIME 150
#define PAUSE_TIME 800


// ---------------- Timers ----------------
unsigned long dhtTimer = 0;

unsigned long lcdTimer = 0;


// ---------------- Web Server ----------------
ESP8266WebServer server(80);


// ---------------- Blynk Virtual Pins ----------------
// V0 = Temperature
// V1 = Fan Status
// V2 = Safety Status
// V3 = Unsafe Control

// ---------------- Sensor Data ----------------

void sendSensorData()
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature))
  {
    return;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.virtualWrite(V0,temperature);
  }


  // Automatic fan control
  if (temperature >= FAN_TEMP)
  {
    fanState = true;
    digitalWrite(FAN_PIN,HIGH);
  }
  else
  {
    fanState = false;
    digitalWrite(FAN_PIN,LOW);
  }


  if (WiFi.status() == WL_CONNECTED)
  {
    if (fanState)
    {
      Blynk.virtualWrite(V1,"ON");
    }
    else
    {
      Blynk.virtualWrite(V1,"OFF");
    }
  }


  if (WiFi.status() == WL_CONNECTED)
  {
    if (dangerState)
    {
      Blynk.virtualWrite(V2,"UNSAFE");
    }
    else
    {
      Blynk.virtualWrite(V2,"SAFE");
    }
  }
}


// ---------------- Blynk Unsafe Control ----------------

BLYNK_WRITE(V3)
{
  int value = param.asInt();

  if (value == 1)
  {
    dangerState = true;
  }
  else
  {
    dangerState = false;
    lastDangerState = false;
  }
}


// ---------------- Blynk Notification ----------------

void checkUnsafeNotification()
{
  if (dangerState == true && lastDangerState == false)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Blynk.logEvent(
        "unsafe_alert",
        "WARNING: Baby monitor detected an unsafe condition!"
      );

      Serial.println("Notification Sent");
    }

    lastDangerState = true;
  }

  if (dangerState == false)
  {
    lastDangerState = false;
  }
}

// ---------------- HTTP Control ----------------

void handleDanger()
{
  dangerState = true;
  lastDangerState = false;

  server.send(200,"text/plain","UNSAFE");
}

void handleSafe()
{
dangerState = false;

lastDangerState = false;

server.send(200,"text/plain","SAFE");
}


// ---------------- HTTP Web Page ----------------

void handleRoot()
{
  String page = "";

  page += "Smart Baby Monitor";

  if (dangerState)
  {
    page += "Status: UNSAFE";
  }
  else
  {
    page += "Status: SAFE";
  }


  page += "Temperature: ";
  page += temperature;
  page += " C";


  page += "Fan: ";

  if (fanState)
  {
    page += "ON";
  }
  else
  {
    page += "OFF";
  }


  server.send(200,"text/html",page);
}

// ---------------- Buzzer Pattern ----------------

void updateBuzzer()
{
  if (!dangerState)
  {
    digitalWrite(BUZZER,BUZZER_OFF);

    buzzerState = false;
    beepCount = 0;

    return;
  }


  unsigned long currentMillis = millis();


  if (buzzerState)
  {
    if (currentMillis - buzzerTimer >= BEEP_TIME)
    {
      buzzerTimer = currentMillis;

      digitalWrite(BUZZER,BUZZER_OFF);

      buzzerState = false;

      beepCount++;


      if (beepCount >= 2)
     {
     delay(PAUSE_TIME);

     beepCount = 0;
     }
    }
  }
  else
  {
    if (currentMillis - buzzerTimer >= BEEP_TIME)
    {
      buzzerTimer = currentMillis;

      digitalWrite(BUZZER,BUZZER_ON);

      buzzerState = true;
    }
  }
}


// ---------------- Setup ----------------

void setup()
{
  Serial.begin(115200);


  // Pin setup
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(FAN_PIN,OUTPUT);


  // Default state
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(RED_LED,LOW);
  digitalWrite(BUZZER,BUZZER_OFF);
  digitalWrite(FAN_PIN,LOW);



  // LCD setup
  Wire.begin(D2,D1);

  lcd.init();
  lcd.backlight();


  lcd.setCursor(0,0);
  lcd.print("Baby Monitor");


  lcd.setCursor(0,1);
  lcd.print("Starting...");



  // DHT start
  dht.begin();



  // WiFi
  WiFi.begin(ssid,pass);


  unsigned long wifiStart = millis();


  while(WiFi.status()!=WL_CONNECTED && millis()-wifiStart < 10000)
  {
    delay(500);

    Serial.print(".");
  }



  if(WiFi.status()==WL_CONNECTED)
  {
    Serial.println();

    Serial.println("WiFi Connected");


    Serial.print("http://");

    Serial.println(WiFi.localIP());



    Blynk.config(BLYNK_AUTH_TOKEN);


    if(Blynk.connect())
    {
      Serial.println("Blynk Connected");
    }
    else
    {
      Serial.println("Blynk Failed");
    }



    server.on("/",handleRoot);

    server.on("/danger",handleDanger);

    server.on("/safe",handleSafe);



    server.begin();


    Serial.println("HTTP Server Started");
  }
  else
  {
    Serial.println();

    Serial.println("Offline Mode");
  }


  delay(2000);


  lcd.clear();
}

// ---------------- Main Loop ----------------

void loop()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();

    server.handleClient();
  }


  checkUnsafeNotification();



  // Read DHT11 every 2 seconds
  if(millis() - dhtTimer >= 2000)
  {
    dhtTimer = millis();

    sendSensorData();
  }



  // Update LCD every 1 second
  if(millis() - lcdTimer >= 1000)
  {
    lcdTimer = millis();

    updateLCD();
  }



  // Safety LED control
  if(dangerState)
  {
    digitalWrite(GREEN_LED,LOW);

    digitalWrite(RED_LED,HIGH);
  }
  else
  {
    digitalWrite(GREEN_LED,HIGH);

    digitalWrite(RED_LED,LOW);
  }



  // Buzzer pattern
  updateBuzzer();
}



// ---------------- LCD Update Function ----------------

void updateLCD()
{
  lcd.clear();


  if(dangerState)
  {
    lcd.setCursor(0,0);

    lcd.print("UNSAFE!");


    lcd.setCursor(0,1);

    lcd.print("T:");

    lcd.print(temperature);

    lcd.print("C FAN:");



    if(fanState)
    {
      lcd.print("ON");
    }
    else
    {
      lcd.print("OFF");
    }
  }
  else
  {
    lcd.setCursor(0,0);

    lcd.print("SAFE");


    lcd.setCursor(0,1);

    lcd.print("T:");

    lcd.print(temperature);

    lcd.print("C FAN:");



    if(fanState)
    {
      lcd.print("ON");
    }
    else
    {
      lcd.print("OFF");
    }
  }
}
