void onButton1Click() {
  current_entry->onUp();
};

void onButton2Click() {
  current_entry->onDown();
};

void onButton3Click() {
  current_entry->onEnter();
};

void onButton4Click() {
  current_entry->onExit();
};

void setup() {
  
  Serial.begin(9600);

  byte clockChar[] = {
    B01110,
    B10101,
    B10111,
    B10001,
    B01110,
    B00000,
    B00000,
    B00000
  };

  byte memChar[] = {
    B01110,
    B11111,
    B01110,
    B11111,
    B01110,
    B00000,
    B00000,
    B00000
  };

  byte arrowChar[] = {
    B00000,
    B00100,
    B00010,
    B11111,
    B11111,
    B00010,
    B00100,
    B00000
  };

  byte cursorChar[] = {
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };

  byte timerOnChar[] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,
    B00000,
    B00000,
    B00000
  };

  byte timerOffChar[] = {
    B01110,
    B11001,
    B10101,
    B10011,
    B01110,
    B00000,
    B00000,
    B00000
  };


  lcd.begin(16, 2);
  lcd.createChar(0, clockChar);
  lcd.createChar(1, memChar);
  lcd.createChar(2, arrowChar);
  lcd.createChar(3, cursorChar);
  lcd.createChar(4, timerOnChar);
  lcd.createChar(5, timerOffChar);
  
  set_time(); 
  
  current_entry = new c_screen;
  
  ai.onB1Click(&onButton1Click);
  ai.onB2Click(&onButton2Click);
  ai.onB3Click(&onButton3Click);
  ai.onB4Click(&onButton4Click);

  pinMode(CHANNEL_B_LED, OUTPUT);
  pinMode(CHANNEL_A_LED, OUTPUT);

  pinMode(CHANNEL_B, OUTPUT);
  pinMode(CHANNEL_A , OUTPUT);
};

void loop() {

  RTC.read(current_time);
  
  current_entry->output();
  ai.readInput(A3);
  
  const byte channel_a_status = EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_MODE);
  const byte channel_b_status = EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_MODE);

  if(channel_a_status == CHANNEL_ON) {
    digitalWrite(CHANNEL_A_LED, HIGH);
    digitalWrite(CHANNEL_A, LOW);
  }
  if(channel_a_status == CHANNEL_OFF) {
    digitalWrite(CHANNEL_A_LED, LOW);
    digitalWrite(CHANNEL_A, HIGH);
  }

  if(channel_a_status == CHANNEL_TIMER) {
    tmElements_t tm_a_on;
    tmElements_t tm_a_off;
    
    tm_a_on.Hour = EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_ON);
    tm_a_on.Minute = EEPROM.read(MEMORY_CHANNEL_A_ADDR_MINUTE_ON);
    tm_a_off.Hour = EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_OFF);
    tm_a_off.Minute = EEPROM.read(MEMORY_CHANNEL_A_ADDR_MINUTE_OFF);

    if(tm_a_on.Hour <= current_time.Hour && tm_a_off.Hour > current_time.Hour) {
      digitalWrite(CHANNEL_A_LED, HIGH);
      digitalWrite(CHANNEL_A, LOW);
    } else if (tm_a_on.Minute <= current_time.Minute && tm_a_off.Minute > current_time.Minute) {
      digitalWrite(CHANNEL_A_LED, HIGH);
      digitalWrite(CHANNEL_A, LOW);
    } else {
      digitalWrite(CHANNEL_A_LED, LOW);
      digitalWrite(CHANNEL_A, HIGH);
    }
  }

  if(channel_b_status == CHANNEL_ON) {
    digitalWrite(CHANNEL_B_LED, HIGH);
    digitalWrite(CHANNEL_B, LOW);
  }
  if(channel_b_status == CHANNEL_OFF) {
    digitalWrite(CHANNEL_B_LED, LOW);
    digitalWrite(CHANNEL_B, HIGH);
  }

  if(channel_b_status == CHANNEL_TIMER) {
    tmElements_t tm_b_on;
    tmElements_t tm_b_off;
    
    tm_b_on.Hour = EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_ON);
    tm_b_on.Minute = EEPROM.read(MEMORY_CHANNEL_B_ADDR_MINUTE_ON);
    tm_b_off.Hour = EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_OFF);
    tm_b_off.Minute = EEPROM.read(MEMORY_CHANNEL_B_ADDR_MINUTE_OFF);
    
    if(tm_b_on.Hour <= current_time.Hour && tm_b_off.Hour > current_time.Hour) {
      digitalWrite(CHANNEL_B_LED, HIGH);
      digitalWrite(CHANNEL_B, LOW);
    } else if (tm_b_on.Minute <= current_time.Minute && tm_b_off.Minute > current_time.Minute) {
      digitalWrite(CHANNEL_B_LED, HIGH);
      digitalWrite(CHANNEL_B, LOW);
    } else {
      digitalWrite(CHANNEL_B_LED, LOW);
      digitalWrite(CHANNEL_B, HIGH);
    }
  }
  
};
