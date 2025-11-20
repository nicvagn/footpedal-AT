/* License: This work is distributed under the terms of the GNU General Public License.
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 */


#include <Keyboard.h>


#define DEBOUNCE_DELAY 50

#define BLACK_KEY KEY_LEFT_SHIFT
#define BLACK_PIN 3
#define BLUE_KEY KEY_LEFT_CTRL
#define BLUE_PIN 2
#define PURPLE_KEY KEY_LEFT_GUI
#define PURPLE_PIN 4

// Button structure to hold all state
struct Button {
  uint8_t pin;
  unsigned int key;
  int curRead;
  int lastState;
  unsigned long lastDebounceTime;
  const char* name;
  bool holdKey;
  bool macro;
};
// Define all pedals
Button black = { BLACK_PIN, BLACK_KEY, HIGH, HIGH, 0, "Black/BLACK_PIN", true, false };
Button blue = { BLUE_PIN, BLUE_KEY, HIGH, HIGH, 0, "Blue/BLUE_PIN", true, false };
Button purple = { PURPLE_PIN, PURPLE_KEY, HIGH, HIGH, 0, "Purple/PURPLE_PIN", true, false };
// Define all buttons
Button blueBtn = { 8, 0, HIGH, HIGH, 0, "Blue Macro", false, true };
Button greenBtn = { 9, 0, HIGH, HIGH, 0, "Green Macro", false, true };
Button yellowBtn = { 12, 0, HIGH, HIGH, 0, "Green Macro", true, true };
Button redBtn = { 11, 0, HIGH, HIGH, 0, "Red Macro", true, true };

void setup() {
  // foot pedals
  pinMode(purple.pin, INPUT_PULLUP);
  pinMode(blue.pin, INPUT_PULLUP);
  pinMode(black.pin, INPUT_PULLUP);
  // buttons
  pinMode(redBtn.pin, INPUT_PULLUP);
  pinMode(blueBtn.pin, INPUT_PULLUP);
  pinMode(greenBtn.pin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Serial up");
}

void executeMacro(Button& btn) {
  Serial.println("MACRO TIME!");

  if (btn.pin == 8) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('g');
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release('g');
  } else if (btn.pin == 9) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('e');
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release('e');
  } else if (btn.pin == 12) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('a');
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release('a');
  } else if (btn.pin == 11) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('q');
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release('q');
  }
}

void checkButton(Button& btn) {
  int reading = digitalRead(btn.pin);

  // If the reading changed, reset debounce timer
  if (reading != btn.curRead) {
    btn.lastDebounceTime = millis();
    btn.curRead = reading;
  }

  // If enough time has passed, accept the reading as stable
  if ((millis() - btn.lastDebounceTime) > DEBOUNCE_DELAY) {
    // If the stable state has changed
    if (btn.curRead != btn.lastState) {
      btn.lastState = btn.curRead;

      if (btn.curRead == LOW) {
        Serial.print(btn.name);
        Serial.print(" key: ");
        Serial.print(btn.key);
        Serial.println(" pressed");

        // the button is not momentary
        if (btn.holdKey) {
          if (btn.macro) {
            while (digitalRead(btn.pin) == LOW) {
              executeMacro(btn);
              delay(300);
            }
          } else {
            Keyboard.press(btn.key);
            while (digitalRead(btn.pin) == LOW) { delay(10); }
            Keyboard.release(btn.key);
          }
        } else if (btn.macro) {
          // What macro is executed is based on the pin
          executeMacro(btn);
        }
      } else {
        Serial.print(btn.name);
        Serial.println(" released");
      }
    }
  }
}

void loop() {
  checkButton(purple);
  checkButton(blue);
  checkButton(black);
  checkButton(redBtn);
  checkButton(blueBtn);
  checkButton(greenBtn);
  checkButton(yellowBtn);
}
