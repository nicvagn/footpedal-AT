// =============== Teensy Keyboard Button Pad ================= //
// This version uses the *native Teensy HID keyboard* API.
// For Teensy 2.0 (ATmega32u4). No Arduino Keyboard.h needed.
// ============================================================ //
#define DEBOUNCE_DELAY 50

#define BLUE_KEY MODIFIERKEY_ALT
#define BLACK_KEY KEY_HOME
#define PURPLE_KEY KEY_ENTER

// ------------------------------------------------------------
// Button struct
// ------------------------------------------------------------
struct Button {
  uint8_t pin;                   // GPIO pin
  uint16_t key;                  // HID keycode (16-bit)
  bool holdKey;                  // hold until release?
  int stableState;               // last accepted state
  int rawState;                  // last raw read
  unsigned long lastChangeTime;  // for debounce
  const char* name;              // debug print
};

// ------------------------------------------------------------
// Button definitions
// ------------------------------------------------------------
Button blue = { 1, BLUE_KEY, true, HIGH, HIGH, 0, "Blue" };
Button purple = { 4, PURPLE_KEY, true, HIGH, HIGH, 0, "Purple" };
Button black = {2, BLACK_KEY, true, HIGH, HIGH, 0, "Black" };

// ------------------------------------------------------------
// Teensy key helpers
// ------------------------------------------------------------
void pressKey(uint16_t code) {
  if (isModifier(code)) {
    Keyboard.set_modifier(code);
    Keyboard.send_now();
  } else {
    Keyboard.set_key1(code & 0xFF);  // HID usage ID
    Keyboard.send_now();
  }
}

bool isModifier(uint16_t kc) {
  return (kc >= MODIFIERKEY_CTRL && kc <= MODIFIERKEY_RIGHT_GUI);
}

void releaseKey() {
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

void pressModifier(uint8_t mod) {
  Keyboard.set_modifier(mod);  // send modifier
  Keyboard.send_now();
}

void releaseModifier() {
  Keyboard.set_modifier(0);  // clear all modifiers
  Keyboard.send_now();
}


void executeMacro() {
  Serial.println("Running macro!");
  uint8_t hid = KEY_HOME & 0xFF;
  // Example macro: CTRL + ALT + HOME
  Keyboard.set_modifier(MODIFIERKEY_CTRL | MODIFIERKEY_ALT);
  Keyboard.set_key1(hid);
  Keyboard.send_now();

  delay(50);

  // Release
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
}

// ------------------------------------------------------------
// Check button with debounce + HID action
// ------------------------------------------------------------
void checkButton(Button& btn) {
  int reading = digitalRead(btn.pin);

  // If raw reading changed, reset debounce timer
  if (reading != btn.rawState) {
    btn.rawState = reading;
    btn.lastChangeTime = millis();
  }

  // If stable for long enough, accept new state
  if (millis() - btn.lastChangeTime > DEBOUNCE_DELAY) {
    if (reading != btn.stableState) {
      btn.stableState = reading;
      Serial.print(btn.name);
      Serial.println(" changed.");

      if (reading == LOW) {
        // Button pressed
        Serial.print(btn.name);
        Serial.print(btn.pin);
        Serial.println(" pressed");

        if (btn.holdKey) {

          if (isModifier(btn.key)) {
            pressModifier(btn.key);
            while (digitalRead(btn.pin) == LOW) delay(5);
            releaseModifier();
          } else {
            pressKey(btn.key);
            while (digitalRead(btn.pin) == LOW) delay(5);
            releaseKey();
          }
        }
      }

    //} else {
      //executeMacro();
    }

  } else {
    // button released
    //Serial.print(btn.name );
    //Serial.print(btn.pin);
    //Serial.println(" released");
  }
}


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Teensy keyboard online.");

  pinMode(blue.pin, INPUT_PULLUP);
  pinMode(purple.pin, INPUT_PULLUP);
  pinMode(black.pin, INPUT_PULLUP);
}

// ------------------------------------------------------------
// Main loop
// ------------------------------------------------------------
void loop() {
  checkButton(blue);
  checkButton(purple);
  checkButton(black);
}
