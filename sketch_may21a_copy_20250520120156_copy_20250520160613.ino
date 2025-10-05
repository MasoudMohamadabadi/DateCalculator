#include <Keypad.h>
#include <LiquidCrystal.h>

// تنظیمات LCD
LiquidCrystal lcd(12, 7, 6, 5, 4, 3);

// تنظیمات کیپد
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[COLS] = {A2,A1,A0,A3};
byte colPins[ROWS] = {8, 9, 10, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// متغیرهای برنامه
const int resetButton = 2;
String inputDate = "";
int day, month, year;
bool dateEntered = false;
unsigned long daysPassed = 0;
bool refreshDisplay = true;

// تاریخ امروز (به صورت ثابت برای مثال)
const int currentYear = 1404;
const int currentMonth = 4;
const int currentDay = 31;

void setup() {
  // راه اندازی LCD
  lcd.begin(16, 2);
  
  // راه اندازی میکروسوئچ
  pinMode(resetButton, INPUT_PULLUP);
  
  // نمایش راهنما
  lcd.print("Enter Date:");
  lcd.setCursor(0, 1);
  lcd.print("YYYY/MM/DD");
}

void loop() {
  // بررسی ریست
  if (digitalRead(resetButton) == LOW) {
    resetSystem();
    delay(200); // Debounce
  }
  
  if (!dateEntered) {
    getDateInput();
  } else {
    if (refreshDisplay) {
      calculateDaysPassed();
      displayResults();
      refreshDisplay = false;
    }
  }
}

void getDateInput() {
  char key = keypad.getKey();
  
  if (key) {
    refreshDisplay = true;
    
    if (key == '#') { // تایید تاریخ
      if (parseDate()) {
        dateEntered = true;
      } else {
        lcd.clear();
        lcd.print("Invalid Date!");
        delay(2000);
        resetInput();
      }
    } else if (key == '*') { // پاک کردن
      resetInput();
    } else if (isdigit(key) && inputDate.length() < 8) { // فقط اعداد
      inputDate += key;
      updateInputDisplay();
    }
  }
}

bool parseDate() {
  if (inputDate.length() != 8) return false;
  
  year = inputDate.substring(0, 4).toInt();
  month = inputDate.substring(4, 6).toInt();
  day = inputDate.substring(6, 8).toInt();
  
  // اعتبارسنجی تاریخ
  if (year < 1300 || year > 1500 ||  month < 1 || month > 12 || day < 1 || day > 31) 
    return false;
  
  // اعتبارسنجی روزهای ماه
  if ((month > 6 && day > 30) || (month == 12 && day > 29)) 
    return false;
  
  return true;
}

void calculateDaysPassed() {
  // محاسبه ساده اختلاف روزها
  daysPassed = (currentYear - year) * 365 + (currentMonth - month) * 30 + (currentDay - day);
}

void displayResults() {
  // نمایش تاریخ روی LCD (بدون پاک کردن غیرضروری)
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  lcd.print(year);
  lcd.print("/");
  if(month < 10) lcd.print("0");
  lcd.print(month);
  lcd.print("/");
  if(day < 10) lcd.print("0");
  lcd.print(day);
  lcd.print("    "); // پاک کردن باقیمانده متن
  
  lcd.setCursor(0, 1);
  
  // بررسی آیا تاریخ امروز است
  if (daysPassed == 0) {
    lcd.print("Today!          ");
  } else {
    lcd.print("Days passed: ");
    lcd.print(daysPassed);
    lcd.print(" ");
  }
}

void updateInputDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                "); // پاک کردن خط دوم
  lcd.setCursor(0, 1);
  
  // نمایش فرمت شده ورودی
  if (inputDate.length() > 0) {
    lcd.print(inputDate.substring(0, 4));
    if (inputDate.length() > 4) {
      lcd.print("/");
      lcd.print(inputDate.substring(4, 6));
      if (inputDate.length() > 6) {
        lcd.print("/");
        lcd.print(inputDate.substring(6));
      }
    }
  }
}

void resetInput() {
  inputDate = "";
  lcd.setCursor(0, 1);
  lcd.print("YYYY/MM/DD");
}

void resetSystem() {
  inputDate = "";
  dateEntered = false;
  refreshDisplay = true;
  lcd.clear();
  lcd.print("Enter Date:");
  lcd.setCursor(0, 1);
  lcd.print("YYYY/MM/DD");
}