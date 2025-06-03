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



// jingle bells...
// تعریف نت‌های پایه (اکتاو 4)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// رشته کد شده برای Jingle Bells
// فرمت: [نت][اکتاو][مدت زمان]
// مثال: E41 یعنی نت E در اکتاو 4 با مدت زمان 1 (چهارم نت)
const char* JINGLE_BELLS = "E41E41E42E41E41E42E41G41C41D41E44F41F41F41F41F41E41E41E41E41E41D41D41E41D42G42";

// تابع تبدیل کد نت به فرکانس
int getNoteFrequency(char note, char octave) {
    int baseFreq;
    switch(note) {
        case 'C': baseFreq = NOTE_C4; break;
        case 'D': baseFreq = NOTE_D4; break;
        case 'E': baseFreq = NOTE_E4; break;
        case 'F': baseFreq = NOTE_F4; break;
        case 'G': baseFreq = NOTE_G4; break;
        case 'A': baseFreq = NOTE_A4; break;
        case 'B': baseFreq = NOTE_B4; break;
        default: return 0;
    }
    // تنظیم اکتاو
    int octaveDiff = (octave - '4');
    if (octaveDiff > 0) {
        for (int i = 0; i < octaveDiff; i++) baseFreq *= 2;
    } else if (octaveDiff < 0) {
        for (int i = 0; i > octaveDiff; i--) baseFreq /= 2;
    }
    
    return baseFreq;
}


void playJingleBells() {
    const char* ptr = JINGLE_BELLS;
    
    while (*ptr) {  // تا زمانی که به انتهای رشته نرسیده‌ایم
        // خواندن اطلاعات نت از رشته
        char note = *ptr++;      // حرف نت
        char octave = *ptr++;    // شماره اکتاو
        char duration = *ptr++;  // مدت زمان
        
        // محاسبه فرکانس نت
        int frequency = getNoteFrequency(note, octave);
        
        // محاسبه مدت زمان نت
        int noteDuration = 1000 / (duration - '0');
        
        // نواختن نت
        tone(BUZZER_PIN, frequency, noteDuration);
        
        // مکث بین نت‌ها
        delay(noteDuration * 1.3);
        noTone(BUZZER_PIN);
    }
    
    delay(1000);  // مکث در پایان آهنگ
}



void loop() {
    for(int i = 0; i < 3; i++) {    // آژیر برای سه بار
        dangerAlarm();
    }
    noTone(BUZZER_PIN);


}