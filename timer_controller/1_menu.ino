/*
 * MAIN MENU SET TIME ENTRY CLASS CONTROLLER
 *
 */
template <class T>
class c_set_time_menu: public c_entry {
  public:
    Delegate<T>* delegate;
    byte entry;
    byte current_hour;
    byte current_minute;
    c_set_time_menu(c_entry*, char*, Delegate<T>* delegate);
    void init(byte, byte);
    void output();
    void onExit();
    void onEnter();
    void onUp();
    void onDown();
};

template <class T>
c_set_time_menu<T>::c_set_time_menu(c_entry* parent, char* display_name, Delegate<T>* delegate)
  :c_entry(parent, display_name) {
  this->entry = 0;
  this->delegate = delegate;
};

template <class T>
void c_set_time_menu<T>::init(byte hour, byte minute) {
  this->current_hour = hour;
  this->current_minute = minute;
}

template <class T>
void c_set_time_menu<T>::output() {
  this->output_time(4, 0, this->current_hour, this->current_minute);
  switch(this->entry) {
    case 1:
      lcd.setCursor(5,1);
      lcd.write(3);
      lcd.write(3);
      lcd.setCursor(8,1);
      lcd.write(" ");
      lcd.write(" ");
      break;
    case 2:
      lcd.setCursor(5,1);
      lcd.write(" ");
      lcd.write(" ");
      lcd.setCursor(8,1);
      lcd.write(3);
      lcd.write(3);
      break;
    default:
      lcd.setCursor(5,1);
      lcd.write(" ");
      lcd.write(" ");
      lcd.setCursor(8,1);
      lcd.write(" ");
      lcd.write(" ");
      break;
  }
};

template <class T>
void c_set_time_menu<T>::onExit() {
  tmElements_t tm;
  tm.Hour = this->current_hour;
  tm.Minute = this->current_minute;
  this->delegate->trigger(tm);
  lcd.clear();
  this->entry = 0;
  current_entry = this->parent;
};

template <class T>
void c_set_time_menu<T>::onEnter() {
  if(this->entry == 2) {
    this->entry = 0;
  } else {
    this->entry ++;
  }
};

template <class T>
void c_set_time_menu<T>::onUp() {
  if(this->entry == 1) {
    this->current_hour ++;
  }
  if(this->entry == 2) {
    this->current_minute ++;
  }
  if(this->current_hour == 24) {
    this->current_hour = 0;
  }
  if(this->current_minute == 60) {
    this->current_minute = 0;
  }
};

template <class T>
void c_set_time_menu<T>::onDown() {
 if(this->entry == 1) {
    this->current_hour --;
  }
  if(this->entry == 2) {
    this->current_minute --;
  }
  if(this->current_hour == 255) {
    this->current_hour = 23;
  }
  if(this->current_minute == 255) {
    this->current_minute = 59;
  }
};
/*
 * MAIN MENU SET DATE ENTRY CLASS CONTROLLER
 *
 */
class c_set_date_menu: public c_entry {
  public:  
    c_set_date_menu(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL A MODE TIMER MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_a_mode_timer: public c_entry {
  public:
    c_set_channel_a_mode_timer(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL A MODE ON MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_a_mode_on: public c_entry {
  public:
    c_set_channel_a_mode_on(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL A MODE OFF MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_a_mode_off: public c_entry {
  public:
    c_set_channel_a_mode_off(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL A MODE MENU CLASS CONTROLLER
 * - Timer
 * - Always on
 * - Always off
 */
class c_set_channel_a_mode: public c_select_entry {
  public:
    c_set_channel_a_mode(c_entry* parent, char*  display_name)
    : c_select_entry(parent, display_name) {
      this->children[0] = new c_set_channel_a_mode_timer(this, "Timer");
      this->children[1] = new c_set_channel_a_mode_on(this, "Always on");
      this->children[2] = new c_set_channel_a_mode_off(this, "Always off");
      this->children_count = 3;
    };
    void onEnter() {
      if(this->cursor_pos == 0) {
        EEPROM.write(MEMORY_CHANNEL_A_ADDR_HOUR_MODE, CHANNEL_MODE_TIMER);
      }

      if(this->cursor_pos == 1) {
        EEPROM.write(MEMORY_CHANNEL_A_ADDR_HOUR_MODE, CHANNEL_MODE_ON);
      }

      if(this->cursor_pos == 2) {
        EEPROM.write(MEMORY_CHANNEL_A_ADDR_HOUR_MODE, CHANNEL_MODE_OFF);
      }
      
      c_select_entry::onExit();
    };
    void onExit() {
      this->onEnter();
    };
    void init(byte pos) {
      lcd.clear();
      this->cursor_pos = pos;
      this->scroll_pos = cursor_pos / 2;
    };
};
/*
 * CHANNEL A MENU CLASS CONTROLLER
 * - Set mode
 * - Set time on
 * - Set time off
 *
 */
class c_set_channel_a: public c_select_entry {
  public:
    c_set_channel_a(c_entry* parent, char* display_name)
    : c_select_entry(parent, display_name) {
      this->delegate_on = new Delegate<c_set_channel_a>(this, &this->on_set_time_on);
      this->delegate_off = new Delegate<c_set_channel_a>(this, &this->on_set_time_off);
      this->children[0] = new c_set_channel_a_mode(this, "Set mode");
      this->children[1] = new c_set_time_menu<c_set_channel_a>(this, "Set time on", this->delegate_on);
      this->children[2] = new c_set_time_menu<c_set_channel_a>(this, "Set time off", this->delegate_off);
      this->children_count = 3;
    };
  private:
    Delegate<c_set_channel_a>* delegate_on;
    Delegate<c_set_channel_a>* delegate_off;
    void on_set_time_on(tmElements_t tm) {
      EEPROM.write(MEMORY_CHANNEL_A_ADDR_HOUR_ON, tm.Hour);
      EEPROM.write(MEMORY_CHANNEL_A_ADDR_MINUTE_ON, tm.Minute);
    };
    void on_set_time_off(tmElements_t tm) {
      EEPROM.write(MEMORY_CHANNEL_A_ADDR_HOUR_OFF, tm.Hour);
      EEPROM.write(MEMORY_CHANNEL_A_ADDR_MINUTE_OFF, tm.Minute);
    };
    void onEnter() {
      tmElements_t tm;

      if(this->cursor_pos == 0) {
        c_set_channel_a_mode* entry_mode = static_cast<c_set_channel_a_mode*>(this->children[0]);
        entry_mode->init(EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_MODE));
      }
      
      if(this->cursor_pos == 1) {
        c_set_time_menu<c_set_channel_a>* entry_on = static_cast<c_set_time_menu<c_set_channel_a>*>(this->children[1]);
        tm.Hour = EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_ON);
        tm.Minute = EEPROM.read(MEMORY_CHANNEL_A_ADDR_MINUTE_ON);
        entry_on->init(tm.Hour == 255 ? 0 : tm.Hour, tm.Minute == 255 ? 0 : tm.Minute);
      }

      if(this->cursor_pos == 2) {
        c_set_time_menu<c_set_channel_a>* entry_off = static_cast<c_set_time_menu<c_set_channel_a>*>(this->children[2]);
        tm.Hour = EEPROM.read(MEMORY_CHANNEL_A_ADDR_HOUR_OFF);
        tm.Minute = EEPROM.read(MEMORY_CHANNEL_A_ADDR_MINUTE_OFF);
        entry_off->init(tm.Hour == 255 ? 0 : tm.Hour, tm.Minute == 255 ? 0 : tm.Minute);
      }
      
      c_select_entry::onEnter();
    };
    
};
/*
 * CHANNEL B MODE TIMER MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_b_mode_timer: public c_entry {
  public:
    c_set_channel_b_mode_timer(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL B MODE ON MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_b_mode_on: public c_entry {
  public:
    c_set_channel_b_mode_on(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL B MODE OFF MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_b_mode_off: public c_entry {
  public:
    c_set_channel_b_mode_off(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL B MODE MENU CLASS CONTROLLER
 * - Timer
 * - Always on
 * - Always off
 */
class c_set_channel_b_mode: public c_select_entry {
  public:
    c_set_channel_b_mode(c_entry* parent, char* display_name);
    void init(byte);
    void onEnter();
    void onExit();
};

c_set_channel_b_mode::c_set_channel_b_mode(c_entry* parent, char* display_name): c_select_entry(parent, display_name) {
  this->children[0] = new c_set_channel_b_mode_timer(this, "Timer");
  this->children[1] = new c_set_channel_b_mode_on(this, "Always on");
  this->children[2] = new c_set_channel_b_mode_off(this, "Always off");
  this->children_count = 3;
}

void c_set_channel_b_mode::onEnter() {
  if(this->cursor_pos == 0) {
    EEPROM.write(MEMORY_CHANNEL_B_ADDR_HOUR_MODE, CHANNEL_MODE_TIMER);
  }

  if(this->cursor_pos == 1) {
    EEPROM.write(MEMORY_CHANNEL_B_ADDR_HOUR_MODE, CHANNEL_MODE_ON);
  }

  if(this->cursor_pos == 2) {
    EEPROM.write(MEMORY_CHANNEL_B_ADDR_HOUR_MODE, CHANNEL_MODE_OFF);
  }
  
  c_select_entry::onExit();
};

void c_set_channel_b_mode::onExit() {
  this->onEnter();
};

void c_set_channel_b_mode::init(byte pos) {
  lcd.clear();
  this->cursor_pos = pos;
  this->scroll_pos = cursor_pos / 2;
};

/*
 * CHANNEL B MENU CLASS CONTROLLER
 * - Set mode
 * - Set time on
 * - Set time off
 *
 */
class c_set_channel_b: public c_select_entry {
  public:
    c_set_channel_b(c_entry* parent, char* display_name)
      : c_select_entry(parent, display_name) {
      this->delegate_on = new Delegate<c_set_channel_b>(this, &this->on_set_time_on);
      this->delegate_off = new Delegate<c_set_channel_b>(this, &this->on_set_time_off);
      this->children[0] = new c_set_channel_b_mode(this, "Set mode");
      this->children[1] = new c_set_time_menu<c_set_channel_b>(this, "Set time on", this->delegate_on);
      this->children[2] = new c_set_time_menu<c_set_channel_b>(this, "Set time off", this->delegate_off);
      this->children_count = 3;
    };
  private:
    Delegate<c_set_channel_b>* delegate_on;
    Delegate<c_set_channel_b>* delegate_off;
    void on_set_time_on(tmElements_t tm) {
      EEPROM.write(MEMORY_CHANNEL_B_ADDR_HOUR_ON, tm.Hour);
      EEPROM.write(MEMORY_CHANNEL_B_ADDR_MINUTE_ON, tm.Minute);
    }
    void on_set_time_off(tmElements_t tm) {
      EEPROM.write(MEMORY_CHANNEL_B_ADDR_HOUR_OFF, tm.Hour);
      EEPROM.write(MEMORY_CHANNEL_B_ADDR_MINUTE_OFF, tm.Minute);
    };
    void onEnter() {
      tmElements_t tm;

      if(this->cursor_pos == 0) {
        c_set_channel_b_mode* entry_mode = static_cast<c_set_channel_b_mode*>(this->children[0]);
        entry_mode->init(EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_MODE));
      }

      if(this->cursor_pos == 1) {
        c_set_time_menu<c_set_channel_b>* entry_on = static_cast<c_set_time_menu<c_set_channel_b>*>(this->children[1]);
        tm.Hour = EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_ON);
        tm.Minute = EEPROM.read(MEMORY_CHANNEL_B_ADDR_MINUTE_ON);
        entry_on->init(tm.Hour == 255 ? 0 : tm.Hour, tm.Minute == 255 ? 0 : tm.Minute);
      }

      if(this->cursor_pos == 2) {
        c_set_time_menu<c_set_channel_b>* entry_off = static_cast<c_set_time_menu<c_set_channel_b>*>(this->children[2]);
        tm.Hour = EEPROM.read(MEMORY_CHANNEL_B_ADDR_HOUR_OFF);
        tm.Minute = EEPROM.read(MEMORY_CHANNEL_B_ADDR_MINUTE_OFF);
        entry_off->init(tm.Hour == 255 ? 0 : tm.Hour, tm.Minute == 255 ? 0 : tm.Minute);
      }

      c_select_entry::onEnter();
    };
};
/*
 * MAIN MENU CLASS CONTROLLER
 * - Set time
 * - Set date
 * - Set channel A
 * - Set channel B
 */
class c_main_menu: public c_select_entry {
  public:
    c_main_menu(c_entry*);
    void onEnter();
    void onSetTime(tmElements_t);
  private:
    Delegate<c_main_menu>* delegate;
};

c_main_menu::c_main_menu(c_entry* parent): c_select_entry(parent) {
  this->delegate = new Delegate<c_main_menu>(this, &this->onSetTime);
  this->children[0] = new c_set_time_menu<c_main_menu>(this, "Set time", this->delegate);
  this->children[1] = new c_set_date_menu(this, "Set date");
  this->children[2] = new c_set_channel_a(this, "Set channel A");
  this->children[3] = new c_set_channel_b(this, "Set channel B");
  this->children_count = 4;
};

void c_main_menu::onSetTime(tmElements_t tm) {
  tmElements_t newTm;
  RTC.read(newTm);
  newTm.Hour = tm.Hour;
  newTm.Minute = tm.Minute;
  RTC.write(newTm);
};

void c_main_menu::onEnter() {
  switch(this->cursor_pos) {
    case 0:
      tmElements_t tm;
      c_set_time_menu<c_main_menu>* entry = static_cast<c_set_time_menu<c_main_menu>*>(this->children[0]);
      RTC.read(tm);
      entry->init(tm.Hour, tm.Minute);
      break;
    default:
      break;
  };
  c_select_entry::onEnter();
};
/*
 * START SCREEN CLASS CONTROLLER
 * - current time
 */
class c_screen: public c_entry {
  private:
    unsigned long previous_tick = 0;
    unsigned long current_tick = 0;
  public:
    c_entry* children[1];
    c_screen() {
      this->children[0] = new c_main_menu(this);
    };
    
    void print_free_memory(byte col, byte row) {
      int value = freeMemory();
      char* output = new char(2);
      if(value < 10) {
        sprintf(output, "000%d", value);
      } else if (value < 100) {
        sprintf(output, "00%d", value);
      } else if (value < 1000) {
        sprintf(output, "0%d", value);
      } else {
        sprintf(output, "%d", value);
      }
      lcd.setCursor(col, row);
      lcd.write(byte(1));
      lcd.print(output);
      lcd.print("B");
      delete output; 
    };
    /*
     * Main function which is called every one second and updates LCD display data.
     */
    void tick() {
      // init time structure
      tmElements_t tm;
      // fill current time structure with data retrieved from RTC module
      RTC.read(tm);
      // output channels state
      lcd.setCursor(0, 0);
      lcd.print("A");
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.print("B");
      lcd.write(byte(2));
      // output free memory
      this->print_free_memory(10, 1);
      // output time
      this->output_time(10, 0, tm.Hour, tm.Minute);
    };
    /*
     * As function is called in main controller loop, code, inside this function, runs on 
     * every iteration of controller cycle. In our case it will be called with frequency
     * of around 16Mhz which is too often. To avoid controller overload main function,
     * which is responsible for interaction with LCD and timer chip, will be run every
     * one second, which is enough to output correct data on screen.
     */
    void output() {
      // set initial variable to observ how much time has passed from time when
      // controller has started.
      if(!this->previous_tick) {
        this->previous_tick = millis();
      };
      // get current value of internal controller timer
      this->current_tick = millis();
      // check the length of the period between last tick and current controller
      // timer valust. It this period is more than one second, then run main tick
      // function and update tick variables
      if(this->current_tick - this->previous_tick > 1000) {
        this->previous_tick = millis();
        this->tick();
      } else if(this->previous_tick > this->current_tick) {
        this->previous_tick = 0;
      }; 
    };

    void onEnter() {
      lcd.clear();
      current_entry = this->children[0];
    };

    void onExit() {
      return;
    }
};
