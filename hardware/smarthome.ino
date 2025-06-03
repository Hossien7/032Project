#include <Arduino.h>
const int BUZZER_PIN = D1;

/////////////////////////////////////////////////////////{ var }/////////////////////////////////////////////////////////
//این ها فرکانس های تولید صدا هستند
const int TONE_HIGH = 1000;  // فرکانس بالا - 1000 هرتز
const int TONE_LOW = 500;    // فرکانس پایین - 500 هرتز
const int TONE_ALARM = 800;    // فرکانس آژیر خطر - 800 هرتز


/////////////////////////////////////////////////////////{ setup_func }/////////////////////////////////////////////////////////

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);  // تنظیم پین buzzer به عنوان خروجی

}


// تابع تولید صدای آژیر پلیس
void policeSiren() {
  // صدای بالا
  for(int i = TONE_LOW; i < TONE_HIGH; i++) {
    tone(BUZZER_PIN, i);
    delay(1);
  }
  // صدای پایین
  for(int i = TONE_HIGH; i > TONE_LOW; i--) {
    tone(BUZZER_PIN, i);
    delay(1);
  }
}


// تابع تولید صدای آژیر خطر
void dangerAlarm(boolean status) {
    if (status == 0) {
        return 0;
    }
    else if(status == 1) {
        // الگوی اول - سه بیپ کوتاه
        for(int i = 0; i < 3; i++) {
            tone(BUZZER_PIN, TONE_ALARM);
            delay(200);
            noTone(BUZZER_PIN);
            delay(200);
        }
    
    // مکث بین الگوها
    delay(1000);
    
    // الگوی دوم - یک بیپ طولانی
    tone(BUZZER_PIN, TONE_ALARM);
    delay(1500);
    noTone(BUZZER_PIN);
    delay(500);
    }
}


void loop() {
    for(int i = 0; i < 3; i++) {    // آژیر برای سه بار
        dangerAlarm();
    }
    noTone(BUZZER_PIN);


}