class debouncer {

  private:
    const unsigned long debounce_latency;
    int prev_signal {LOW};
    unsigned long prev_signal_time{0};

  public:
    debouncer(unsigned long latency = 100): debounce_latency {latency} {}
    
    int debounce(int current_signal) {

      Serial.println(current_signal);

      if((prev_signal == HIGH && current_signal == HIGH) || prev_signal == LOW && current_signal == LOW)
        return current_signal;

      if((millis() - prev_signal_time) > debounce_latency) {
        prev_signal_time = millis();

        if(current_signal == HIGH) prev_signal = HIGH;
        else if(current_signal == LOW) prev_signal = LOW;
        
        return current_signal;
      }
            
      return prev_signal;
    }
  
};





class controller {

  #define CLOSE false
  #define OPEN true
  #define READABLE true
  #define UNREADABLE false
  
  public:
    controller(int in_pin, int out_pin, bool signal_debounce = true): 
      in_pin {in_pin}, out_pin{out_pin}, signal_debounce {signal_debounce} {}

    void update() {
        int val = digitalRead(in_pin);
        if(signal_debounce) val = bounce_controller.debounce(val);
        
        if(val == HIGH) {
          if(readable) {
            readable = UNREADABLE;
            toggle_state();
            out();
          }
        } else {
          readable = READABLE;
        }
    }

  private:
    int in_pin, out_pin;
    bool state {CLOSE};
    bool readable {READABLE};
    bool signal_debounce;
    debouncer bounce_controller {};

  private:
    void toggle_state() {
          state = !state;
    }
    void out() {
      if(state == CLOSE) {
        digitalWrite(out_pin, LOW);
      }
      else { // state == OPEN
        digitalWrite(out_pin, HIGH);
      }
    }

};













void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(12, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}


controller ctl1 {2, LED_BUILTIN}; // test led with debouncing
controller ctl2 {2, 12, false}; // test led without debouncing

void loop() {

  ctl1.update(); // with debouncing
  ctl2.update(); // without debouncing

}
