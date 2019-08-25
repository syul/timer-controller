// setup LCD Screen
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// store current time
tmElements_t current_time;

// monthes array
const char *month_names[12] = {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

// get time fron DS1307 module
bool get_time(const char *str) {
  int Hour, Min, Sec;
  if(sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) {
    return false;
  }
  current_time.Hour = Hour;
  current_time.Minute = Min;
  current_time.Second = Sec;
  return true;
};

// get date from DS1307 module
bool get_date(const char *str) {
  char Month[12];
  int Day, Year;
  uint8_t month_index;
  if(sscanf(str, "$s %d %d", Month, &Day, &Year) != 3) {
    return false;
  }
  for(month_index = 0; month_index < 12; month_index ++) {
    if(strcmp(Month, month_names[month_index]) == 0) {
      break;
    }
  }
  if (month_index >= 12) {
    return false;
  }
  current_time.Day = Day;
  current_time.Month = month_index + 1;
  current_time.Year = CalendarYrToTm(Year);
  return true;
};

// set time into DS1307 module
void set_time() {
  if(!RTC.read(current_time) && RTC.chipPresent() && get_date(__DATE__) && get_time(__TIME__)) {
    RTC.write(current_time);
  }
};

class AnalogInput 
{
  public:
    AnalogInput(int (*analogReadFn)(uint8_t));
    AnalogInput(int (*analogReadFn)(uint8_t), unsigned int, unsigned int, unsigned int, unsigned int);
    void readInput(uint8_t);
    void dispatch(AI_BUTTONS);
    void onB1Click(void (*f)());
    void onB2Click(void (*f)());
    void onB3Click(void (*f)());
    void onB4Click(void (*f)());
  private:
    int aiB1Code;
    int aiB2Code;
    int aiB3Code;
    int aiB4Code;
    int currentButton;
    unsigned long currentTime;
    void init(int (*analogReadFn)(uint8_t));
    void (*onB1ClickCB)();
    void (*onB2ClickCB)();
    void (*onB3ClickCB)();
    void (*onB4ClickCB)();
    int (*analogReadFn)(uint8_t);
};

AnalogInput::AnalogInput(int (*analogReadFn)(uint8_t))
{
  this -> aiB1Code = AI_B1_CODE;
  this -> aiB2Code = AI_B2_CODE;
  this -> aiB3Code = AI_B3_CODE;
  this -> aiB4Code = AI_B4_CODE;
  
  init(analogReadFn);
};

AnalogInput::AnalogInput(int (*analogReadFn)(uint8_t), unsigned int b1, int unsigned b2, int unsigned b3, int unsigned b4)
{
  this -> aiB1Code = b1;
  this -> aiB2Code = b2;
  this -> aiB3Code = b3;
  this -> aiB4Code = b4;

  init(analogReadFn);
};

void AnalogInput::init(int (*analogReadFn)(uint8_t))
{
  this -> currentTime = millis();
  this -> analogReadFn = analogReadFn;
}

void AnalogInput::onB1Click(void (*f)())
{
  this->onB1ClickCB = f;
};

void AnalogInput::onB2Click(void (*f)())
{
  this->onB2ClickCB = f;
};

void AnalogInput::onB3Click(void (*f)())
{
  this->onB3ClickCB = f;
};

void AnalogInput::onB4Click(void (*f)())
{
  this->onB4ClickCB = f;
};

void AnalogInput::dispatch(AI_BUTTONS button)
{

  if(millis() - this -> currentTime < AI_DELAY)
  {
    return;
  }

  this -> currentTime = millis();
  
  switch(button)
  {
    case AI_B1:
      this -> onB1ClickCB();
      break;
    case AI_B2:
      this -> onB2ClickCB();
      break;
    case AI_B3:
      this -> onB3ClickCB();
      break;
    case AI_B4:
      this -> onB4ClickCB();
      break;
  }
};

void AnalogInput::readInput(uint8_t pin)
{
  int code = this -> analogReadFn(pin);

  if(this->currentTime > millis())
  {
    this->currentTime = millis();
  }
  
  if(code >= this -> aiB4Code - AI_ACCURACY && code <= this -> aiB4Code + AI_ACCURACY)
  {
    dispatch(AI_B4);
  }
  else if (code >= this -> aiB3Code - AI_ACCURACY && code <= this -> aiB3Code + AI_ACCURACY)
  {
    dispatch(AI_B3);
  }
  else if (code >= this -> aiB2Code - AI_ACCURACY && code <= this -> aiB2Code + AI_ACCURACY)
  {
    dispatch(AI_B2);
  }
  else if (code >= this -> aiB1Code - AI_ACCURACY && code <= this -> aiB1Code + AI_ACCURACY)
  {
    dispatch(AI_B1);
  }
};
/*
 * MENU ENTRY CLASS CONTROLLER
 */
class c_entry {
  public:
    c_entry() {
      this->parent = NULL;
      this->display_name = "";
    }
    
    c_entry(c_entry* parent) {
      this->parent = parent;
      this->display_name = "";
    }
    
    c_entry(c_entry* parent, char* display_name) {
      this->parent = parent;
      this->display_name = display_name;
    }
    
    c_entry* parent;  
    char* display_name;
    
    virtual void onEnter() {};
    
    virtual void onExit() {};
    
    virtual void onUp() {};
    
    virtual void onDown() {};
    
    virtual void output() {};

    void output_time_value(byte col, byte row, byte value, bool print_delimiter) {
      lcd.setCursor(col, row);
      char* output = new char(2);
      if(value > 9) {
        sprintf(output, "%d", value);
      } else {
        sprintf(output, "0%d", value);
      }
      lcd.print(output);
      if(print_delimiter) {
        lcd.print(":");
      }
      delete output;
    };

    void output_time(byte col, byte row, byte hour, byte minute) {
      lcd.setCursor(col, row);
      lcd.write(byte(0));
      this->output_time_value(col + 1, row, hour, true);
      this->output_time_value(col + 4, row, minute, false);
    };
};
// current menu entry
c_entry* current_entry;
/*
 * MENU ENTRY THAT CAN SELECT CHILDREN ITEMS CLASS CONTROLLER
 */
class c_select_entry: public c_entry {
  public:
    c_select_entry(): c_entry() {
      this->cursor_pos = 0;
      this->scroll_pos = 0;
    }
    
    c_select_entry(c_entry* parent): c_entry(parent) {
      this->cursor_pos = 0;
      this->scroll_pos = 0;
    }
    
    c_select_entry(c_entry* parent, char* display_name): c_entry(parent, display_name) {
      this->cursor_pos = 0;
      this->scroll_pos = 0;
    }
    
    c_entry* children[16];
    
    byte children_count;
    byte cursor_pos;
    byte scroll_pos;
    
    virtual void onEnter() {
      lcd.clear();
      RTC.read(current_time);
      current_entry = this->children[this->cursor_pos];
    };
    
    virtual void onExit() {
      lcd.clear();
      current_entry = this->parent;
    };
    
    virtual void onUp() {
      lcd.clear();
      if(this->cursor_pos == 0) {
        return;
      }
      this->cursor_pos --;
      this->scroll_pos = cursor_pos / 2;
    };
    
    virtual void onDown() {
      lcd.clear();
      if(this->cursor_pos == this->children_count - 1) {
        return;
      }
      this->cursor_pos ++;
      this->scroll_pos = cursor_pos / 2;
    };
    
    virtual void output() {
      // output menu items
      for(byte i = this->scroll_pos * 2, row = 0; i < this->scroll_pos * 2 + 2; i ++, row ++) {
        if(i == this->children_count) {
          break;
        }
        lcd.setCursor(1, row);
        lcd.print(this->children[i]->display_name);  
      }
      // output cursor position
      lcd.setCursor(0, this->cursor_pos - this->scroll_pos * 2);
      lcd.print(">");    
    };
};

// analog input handler
AnalogInput ai(analogRead);
