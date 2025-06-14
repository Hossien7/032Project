#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi AP تنظیمات
#define APSSID "GasSystem"
#define APPASSWORD "12345678"

// پین‌ها
#define MQ2_PIN A0
#define RELAY1_PIN 13  // D7
#define RELAY2_PIN 12  // D6
#define BUZZER_PIN 14  // GPIO14 = D5


// آستانه تشخیص گاز
#define GAS_THRESHOLD 600

ESP8266WebServer server(80);

// وضعیت رله‌ها
bool relay1State = false;
bool relay2State = false;

// مقدار خوانده شده از MQ2
int gasLevel = 0;

// روشن/خاموش کردن رله‌ها
void updateRelays() {
  digitalWrite(RELAY1_PIN, relay1State ? HIGH : LOW);
  digitalWrite(RELAY2_PIN, relay2State ? HIGH : LOW);
}

// صفحه وب HTML
String webPage() {
  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<meta http-equiv='refresh' content='5'>";
  page += "<title>Gas Detection</title>";
  page += "<style>";
  page += "body { font-family: 'Segoe UI', sans-serif; background: #f0f0f0; margin: 0; padding: 0; text-align: center; }";
  page += "h1 { color: #333; margin-top: 20px; }";
  page += ".status { font-size: 18px; margin: 20px auto; padding: 15px; border-radius: 10px; width: 80%; max-width: 400px; }";
  page += ".safe { background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb; }";
  page += ".alert { background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }";
  page += ".relay-card { background: #fff; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); padding: 20px; margin: 20px auto; width: 80%; max-width: 400px; }";
  page += "button { padding: 12px 25px; font-size: 16px; border: none; border-radius: 5px; cursor: pointer; margin-top: 10px; }";
  page += "button.on { background-color: #28a745; color: white; }";
  page += "button.off { background-color: #dc3545; color: white; }";
  page += "footer { margin-top: 30px; color: #888; font-size: 14px; }";
  page += "</style></head><body>";

  page += "<h1>Gas Detection System</h1>";
  page += "<div class='status " + String(gasLevel > GAS_THRESHOLD ? "alert" : "safe") + "'>";
  page += "<strong>Gas Level:</strong> " + String(gasLevel) + "<br>";

  if (gasLevel > GAS_THRESHOLD) {
    page += "⚠️ <strong>Gas Detected! Relays turned OFF</strong>";
  } else {
    page += "✅ <strong>Safe</strong>";
  }

  page += "</div>";

  // Relay 1
  page += "<div class='relay-card'>";
  page += "<h3>Relay 1: " + String(relay1State ? "ON" : "OFF") + "</h3>";
  page += "<form action='/toggle1' method='POST'>";
  page += "<button class='" + String(relay1State ? "off" : "on") + "'>" + String(relay1State ? "Turn OFF" : "Turn ON") + "</button>";
  page += "</form></div>";

  // Relay 2
  page += "<div class='relay-card'>";
  page += "<h3>Relay 2: " + String(relay2State ? "ON" : "OFF") + "</h3>";
  page += "<form action='/toggle2' method='POST'>";
  page += "<button class='" + String(relay2State ? "off" : "on") + "'>" + String(relay2State ? "Turn OFF" : "Turn ON") + "</button>";
  page += "</form></div>";

  page += "<footer>Created by AzarAfarin &copy; 2024</footer>";
  page += "</body></html>";
  return page;
}


void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // بیزر خاموش باشد در ابتدا

  // تنظیم پین‌ها
  pinMode(MQ2_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // رله‌ها خاموش شوند
  relay1State = false;
  relay2State = false;
  updateRelays();

  // راه‌اندازی WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(APSSID, APPASSWORD);
  Serial.println("Access Point Started: " + WiFi.softAPIP().toString());

  // تعریف مسیرها
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", webPage());
  });

  server.on("/toggle1", HTTP_POST, []() {
    relay1State = !relay1State;
    updateRelays();
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });

  server.on("/toggle2", HTTP_POST, []() {
    relay2State = !relay2State;
    updateRelays();
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // خواندن مقدار MQ2
  gasLevel = analogRead(MQ2_PIN);
  Serial.println("Gas: " + String(gasLevel));

  // اگر گاز زیاد بود، رله‌ها را خاموش کن
  if (gasLevel > GAS_THRESHOLD) {
    if (relay1State || relay2State) {
      relay1State = false;
      relay2State = false;
      updateRelays();
      Serial.println("⚠️ Gas Detected: Relays turned OFF");
    }
    digitalWrite(BUZZER_PIN, HIGH);  // روشن کردن بیزر
  } else {
  digitalWrite(BUZZER_PIN, LOW);   // خاموش کردن بیزر
}

  delay(1000);  // برای پایدار ماندن قرائت‌ها
}
