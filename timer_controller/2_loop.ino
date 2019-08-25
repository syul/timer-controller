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


  lcd.begin(16, 2);
  lcd.createChar(0, clockChar);
  lcd.createChar(1, memChar);
  lcd.createChar(2, arrowChar);
  lcd.createChar(3, cursorChar);
  
  set_time(); 
  
  current_entry = new c_screen;
  
  ai.onB1Click(&onButton1Click);
  ai.onB2Click(&onButton2Click);
  ai.onB3Click(&onButton3Click);
  ai.onB4Click(&onButton4Click);
};

void loop() {
  current_entry->output();
  ai.readInput(A3);
};
