/*
 * MAIN MENU SET TIME ENTRY CLASS CONTROLLER
 *
 */
class c_set_time_menu: public c_entry {
  public:
    byte entry;
    byte current_hour;
    byte current_minute;
    c_set_time_menu(c_entry*, char*);
    void init(byte, byte);
    void output();
    void onExit();
    void onEnter();
    void onUp();
    void onDown();
};

c_set_time_menu::c_set_time_menu(c_entry* parent, char* display_name)
  :c_entry(parent, display_name) {
  this->entry = 0;
};

void c_set_time_menu::init(byte hour, byte minute) {
  this->current_hour = hour;
  this->current_minute = minute;
}

void c_set_time_menu::output() {
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

void c_set_time_menu::onExit() {
  tmElements_t tm;
  RTC.read(tm);
  tm.Hour = this->current_hour;
  tm.Minute = this->current_minute;
  RTC.write(tm);
  lcd.clear();
  this->entry = 0;
  current_entry = this->parent;
};

void c_set_time_menu::onEnter() {
  if(this->entry == 2) {
    this->entry = 0;
  } else {
    this->entry ++;
  }
};

void c_set_time_menu::onUp() {
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

void c_set_time_menu::onDown() {
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
};

class c_set_channel_a_timer: public c_entry {
  public:
    c_set_channel_a_timer(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL A MENU CLASS CONTROLLER
 * - Set mode
 * - Set time
 *
 */
class c_set_channel_a: public c_select_entry {
  public:
    c_set_channel_a(c_entry* parent, char* display_name)
    : c_select_entry(parent, display_name) {
      this->children[0] = new c_set_channel_a_mode(this, "Set mode");
      this->children[1] = new c_set_channel_a_timer(this, "Set time");
      this->children_count = 2;
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
};

c_set_channel_b_mode::c_set_channel_b_mode(c_entry* parent, char* display_name): c_select_entry(parent, display_name) {
  this->children[0] = new c_set_channel_b_mode_timer(this, "Timer");
  this->children[1] = new c_set_channel_b_mode_on(this, "Always on");
  this->children[2] = new c_set_channel_b_mode_off(this, "Always off");
  this->children_count = 3;
};
/*
 * CHANNELB SET TIMER MENU ENTRY CLASS CONTROLLER
 *
 */
class c_set_channel_b_timer: public c_entry {
  public:
    c_set_channel_b_timer(c_entry* parent, char* display_name): c_entry(parent, display_name) {};
};
/*
 * CHANNEL B MENU CLASS CONTROLLER
 * - Set mode
 * - Set time
 *
 */
class c_set_channel_b: public c_select_entry {
  public:
    c_set_channel_b(c_entry*, char*);
};

c_set_channel_b::c_set_channel_b(c_entry* parent, char* display_name): c_select_entry(parent, display_name) {
  this->children[0] = new c_set_channel_b_mode(this, "Set mode");
  this->children[1] = new c_set_channel_b_timer(this, "Set time");
  this->children_count = 2;
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
};

c_main_menu::c_main_menu(c_entry* parent): c_select_entry(parent) {
  this->children[0] = new c_set_time_menu(this, "Set time");
  this->children[1] = new c_set_date_menu(this, "Set date");
  this->children[2] = new c_set_channel_a(this, "Set channel A");
  this->children[3] = new c_set_channel_b(this, "Set channel B");
  this->children_count = 4;
};

void c_main_menu::onEnter() {
  switch(this->cursor_pos) {
    case 0:
      tmElements_t tm;
      c_set_time_menu* entry = static_cast<c_set_time_menu*>(this->children[0]);
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
