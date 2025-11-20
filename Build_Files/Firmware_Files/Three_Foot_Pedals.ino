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
#define BLACK_KEY KEY_HOME
#define BLUE_KEY KEY_END
#define PURPLE_KEY KEY_ESC

// Button structure to hold all state
struct Button {
  uint16_t pin;
  uint16_t key;
  int curRead;
  int lastState;
  unsigned long lastDebounceTime;
  const char* name;
  bool holdKey;  // true for modifier keys, false for macros
};

// Define all buttons
Button black = {3, BLACK_KEY, HIGH, HIGH, 0, "Black", true};
Button blue = {1, BLUE_KEY, HIGH, HIGH, 0, "Blue", true};
Button purple = {2, PURPLE_KEY, HIGH, HIGH, 0, "Purple", true};

Button macro5 = {4, 0, HIGH, HIGH, 0, "Macro/Pin-3", false};
Button macro6 = {4, 0, HIGH, HIGH, 0, "Macro/Pin-4", false};
Button macro7 = {5, 0, HIGH, HIGH, 0, "Macro/Pin-5", false};
Button macro8 = {6, 0, HIGH, HIGH, 0, "Macro/Pin-6", false};

void setup() {
  pinMode(purple.pin, INPUT_PULLUP);
  pinMode(blue.pin, INPUT_PULLUP);
  pinMode(black.pin, INPUT_PULLUP);
  //pinMode(macro5.pin, INPUT_PULLUP);
  //pinMode(macro6.pin, INPUT_PULLUP);
  //pinMode(macro7.pin, INPUT_PULLUP);
  //pinMode(macro8.pin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Serial up");
}

void executeMacro() {
  Serial.println("MACRO TIME!");
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

        if (btn.holdKey) {
          Keyboard.press(btn.key);
          while (digitalRead(btn.pin) == LOW) { delay(10); }
          Keyboard.release(btn.key);
        } else {
          // TODO make function part of btn def
          executeMacro();
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
  checkButton(macro5);
  checkButton(macro6);
  checkButton(macro7);
  checkButton(macro8);
}
