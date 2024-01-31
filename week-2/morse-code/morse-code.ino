#define LED_PIN 13
const unsigned long DELAY_TIME = 100;

String morsetable[] = {
    // Letters A-Z (indices 0-25)
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
    "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
    "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
    "-.--", "--..", 
    // Digits start here (indices 26-35)
    ".----", "..--", "...--", "....-", "....",
    "-...", "--...", "---..", "----.", "-----"
};


/* Convert a character to an index into the morse code table.
 *  Rules:
 *    If it's an uppercase letter (A-Z) then return 0-25
 *    If it's a digit (0-9) then return 26-35
 *    Anything else return -1
 *  Assumption:
 *    The character has already been converted to uppercase if it's a letter.
 */
int lookup(char c){

  // If the character is not a valid letter or digit then return -1
  if( !isAlphaNumeric(c) ){
    return -1;
  }


  // Now we do a bit of ASCII trickery to convert letters & digits
  // into positions in our lookup table. See https://www.arduino.cc/reference/en/language/variables/data-types/char/
  // Letters are easier
  if( isAlpha(c) ){
    return c - 'A';
  }

  if( isDigit(c) ){
    // add on 25 because the digits come after the letters in the table, so we skip that far
    return (c - '0') + 25; 
  }

}

// Flashes a character (either '.' or '-')
void flashMorseChar(char c) {
  if(c != '.' && c != '-') return; // Error Guarding
  digitalWrite(LED_PIN, HIGH);
  if(c=='-') {
    delay(3*DELAY_TIME); // Dash Time = 3tu
  } else {
    delay(DELAY_TIME); // Dot Time = 1tu
  }
  digitalWrite(LED_PIN, LOW);
  delay(DELAY_TIME);
}

// Flashes a Space (7 time units)
void flashSpace() {
  digitalWrite(LED_PIN, LOW);
  delay(7*DELAY_TIME);
}

void setup() {
  pinMode(LED_PIN, OUTPUT); // Set LED_PIN to output
  Serial.begin(9600); // Open Serial Port
}

void loop() {
  // Read String
  String morseWord = Serial.readString();
  // Set all to Uppercase
  morseWord.toUpperCase();
  for(char c : morseWord) {
    Serial.println(c); // For Debugging mostly
    if(lookup(c) == -1) {
      // Incorrect Character
      flashSpace();
    } else {
      // The word exists
      for(char mc : morsetable[lookup(c)]) {
        flashMorseChar(mc);
      }
    }
  }
}

