/**
 * Authors @lluiscampos, @larsewi, @ThomasRyd & @kjaskiewiczz
 */

#define LED_GAME_START 8
#define LED_GAME_STOP  2
#define LED_PLAYER_ONE 3
#define LED_PLAYER_TWO 4

#define BTN_PLAYER_ONE 5
#define BTN_PLAYER_TWO 6
#define BTN_START_GAME 7

#define SEG_A 12
#define SEG_G 13
#define SEG_D 9
#define DIG_1 10
#define DIG_4 11

enum state {
  GAME_START = 0,
  GAME_WAIT,
  GAME_REACT,
  GAME_PLAYER_ONE,
  GAME_PLAYER_TWO,
  GAME_DRAW,
};

enum state game_state;
const long wait_min = 1;
const long wait_max = 10;
long wait_time;
unsigned long start_time;
int player_one_score;
int player_two_score;

bool update_player_one = false;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("Setting up GPIO's");
  pinMode(LED_GAME_START, OUTPUT);
  pinMode(LED_GAME_STOP,  OUTPUT);
  pinMode(LED_PLAYER_ONE, OUTPUT);
  pinMode(BTN_PLAYER_ONE, INPUT );
  pinMode(LED_PLAYER_TWO, OUTPUT);
  pinMode(BTN_PLAYER_TWO, INPUT );
  pinMode(BTN_START_GAME, INPUT );

  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(DIG_1, OUTPUT);
  pinMode(DIG_4, OUTPUT);

  player_one_score = 0;
  player_two_score = 0;
  update_player_one = true;

  Serial.println("Setting state GAME_START");
  game_state = GAME_START;
}

void update_game_state() {
  switch (game_state) {
    case GAME_START:
    {
      // Wait for game start button
      if(digitalRead(BTN_START_GAME) == LOW) {
        break;
      }

      if ((player_one_score >= 3) || (player_two_score >= 3)) {
        player_one_score = 0;
        player_two_score = 0;
      }

      // Set all LED's to desired values
      Serial.println("Setting LED_PLAYER_ONE to LOW");
      digitalWrite(LED_PLAYER_ONE, LOW);
      Serial.println("Setting LED_PLAYER_TWO to LOW");
      digitalWrite(LED_PLAYER_TWO, LOW);
      Serial.println("Setting LED_START to LOW");
      digitalWrite(LED_GAME_START, LOW); 
      Serial.println("Setting LED_STOP to HIGH");
      digitalWrite(LED_GAME_STOP, HIGH);

      // Compute the random wait time
      start_time = millis();
      wait_time = random(wait_min, wait_max) * 1000;
      Serial.println("Computed random wait time");
      Serial.println(wait_time);

      // Enter waiting state
      Serial.println("Entering state GAME_WAIT");
      game_state = GAME_WAIT;
      break;
    }
    case GAME_WAIT:
    {
      // Check if the random wait time is exceeded
      unsigned long now = millis();
      if ((now - start_time) < (unsigned long)wait_time) {
        // If you click to early you loose, if both click it's a draw
        if ((digitalRead(BTN_PLAYER_ONE) == HIGH) && (digitalRead(BTN_PLAYER_TWO) == HIGH)) {
          Serial.println("Setting state GAME_DRAW");
          game_state = GAME_DRAW;
        }
        else if (digitalRead(BTN_PLAYER_ONE) == HIGH) {
          Serial.println("Setting state GAME_PLAYER_TWO");
          game_state = GAME_PLAYER_TWO;
        }
        else if (digitalRead(BTN_PLAYER_TWO) == HIGH) {
          Serial.println("Setting state GAME_PLAYER_ONE");
          game_state = GAME_PLAYER_ONE;
        }
        break;
      }

      // Wait time exeeded, switch to green LED to signal the players
      Serial.println("Setting LED_STOP to LOW");
      digitalWrite(LED_GAME_STOP, LOW);
      Serial.println("Setting LED_START to HIGH");
      digitalWrite(LED_GAME_START, HIGH);

      // Change to react state
      Serial.println("Entering state GAME_REACT");
      game_state = GAME_REACT;
      break;
    }
    case GAME_REACT:
    {
      // The first person to click wins, if both click at the same time it's a draw
      if ((digitalRead(BTN_PLAYER_ONE) == HIGH) && (digitalRead(BTN_PLAYER_TWO) == HIGH)) {
        Serial.println("Setting state GAME_DRAW");
        game_state = GAME_DRAW;
      }
      else if (digitalRead(BTN_PLAYER_ONE) == HIGH) {
        Serial.println("Setting state GAME_PLAYER_ONE");
        game_state = GAME_PLAYER_ONE;
      }
      else if (digitalRead(BTN_PLAYER_TWO) == HIGH) {
        Serial.println("Setting state GAME_PLAYER_TWO");
        game_state = GAME_PLAYER_TWO;
      }
      break;
    }
    case GAME_PLAYER_ONE:
    {
      Serial.println("Setting LED_PLAYER_ONE to HIGH");
      digitalWrite(LED_PLAYER_ONE, HIGH);
      player_one_score += 1;
      Serial.println("Setting state GAME_START");
      game_state = GAME_START;
      break;
    }
    case GAME_PLAYER_TWO:
    {
      Serial.println("Setting LED_PLAYER_TWO to HIGH");
      digitalWrite(LED_PLAYER_TWO, HIGH);
      player_two_score += 1;
      Serial.println("Setting state GAME_START");
      game_state = GAME_START;
      break;
    }
    case GAME_DRAW:
    {
      Serial.println("Setting LED_PLAYER_ONE to HIGH");
      digitalWrite(LED_PLAYER_ONE, HIGH);
      Serial.println("Setting LED_PLAYER_TWO to HIGH");
      digitalWrite(LED_PLAYER_TWO, HIGH);
      player_one_score += 1;
      player_two_score += 1;
      Serial.println("Setting state GAME_START");
      game_state = GAME_START;
      break;
    }
  }
}

void update_digit(int score, int digit) {
  // Set all segments to LOW
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_D, LOW);

  // Chose which digit to update
  if (digit == 1) {
    digitalWrite(DIG_1, HIGH);
    digitalWrite(DIG_4, LOW);
  }
  else if (digit == 4) {
    digitalWrite(DIG_1, LOW);
    digitalWrite(DIG_4, HIGH);
  }

  // Display points
  switch (score) {
    case 0:
    {
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_D, LOW);
      break;
    }
    case 1:
    {
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_D, LOW);
      break;
    }
    case 2:
    {
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_G, HIGH);
      digitalWrite(SEG_D, LOW);
      break;
    }
    case 3:
    {
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_G, HIGH);
      digitalWrite(SEG_D, HIGH);
      break;
    }
  }
}

void update_display() {
  if (update_player_one) {
    update_digit(player_one_score, 1);
  }
  else {
    update_digit(player_two_score, 4);
  }
  update_player_one = !update_player_one;
}

// the loop function runs over and over again forever
void loop() {
  update_game_state();
  update_display();
}
