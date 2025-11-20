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

// Pin definitions
#define PURPLE_PIN 7
#define WHITE_PIN 6
#define RED_PIN 5
#define DEBOUNCE_DELAY 50
#define KEY_PRESSED LOW  // Assuming button pulls pin LOW when pressed

// State variables for left button
int curLeftRead = HIGH;
int lastLeftState = HIGH;
unsigned long lastLeftDebounceTime = 0;

// State variables for right button
int curRightRead = HIGH;
int lastRightState = HIGH;
unsigned long lastRightDebounceTime = 0;

void setup() {
  pinMode(PURPLE_PIN, INPUT_PULLUP);   // Enable internal pull-up resistor
  pinMode(RED_PIN, INPUT_PULLUP);   // Enable internal pull-up resistor
  pinMode(WHITE_PIN, INPUT_PULLUP);   // Enable internal pull-up resistor
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  doLeft();
  doRight();
  // Your other code here
}

void doPurple() {
  int purpleRead = digitalRead(PURPLE_PIN);

  // If the reading changed (due to noise or actual press)
  if (leftRead != curLeftRead) {
    lastLeftDebounceTime = millis(); // Reset debounce timer
    curLeftRead = leftRead;
  }

  // If enough time has passed, accept the reading as stable
  if ((millis() - lastLeftDebounceTime) > DEBOUNCE_DELAY) {
    // If the stable state has changed
    if (curLeftRead != lastLeftState) {
      lastLeftState = curLeftRead;

      if (curLeftRead == KEY_PRESSED) { // Button pressed (pulled to ground)
        Keyboard.press(KEY_LEFT_ARROW);
      } else { // Button released
        Keyboard.release(KEY_LEFT_ARROW);
      }
    }
  }
}

void doRight() {
  int rightRead = digitalRead(RIGHT_PIN);

  // If the reading changed (due to noise or actual press)
  if (rightRead != curRightRead) {
    lastRightDebounceTime = millis(); // Reset debounce timer
    curRightRead = rightRead;
  }

  // If enough time has passed, accept the reading as stable
  if ((millis() - lastRightDebounceTime) > DEBOUNCE_DELAY) {
    // If the stable state has changed
    if (curRightRead != lastRightState) {
      lastRightState = curRightRead;

      if (curRightRead == KEY_PRESSED) { // Button pressed (pulled to ground)
        Keyboard.press(KEY_RIGHT_ARROW);
      } else { // Button released
        Keyboard.release(KEY_RIGHT_ARROW);
      }
    }
  }
}
