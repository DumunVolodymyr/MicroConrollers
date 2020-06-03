
const int buzzerPin = 34;
byte numbers[11] = { 0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001,
                     0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000,
                     0b10001000,
                   };
int digits[6] = {51, 50, 10, 11, 12, 13};
byte second = 0;
int clok = 0;
int daley = 0;
int bip = 0;
int A = 1;
int sec1 = 0;
int sec2 = 0;
int sec3 = 0;
int minut1 = 0;
int minut2 = 0;
int minut3 = 0;
int hour1 = 0;
int hour2 = 0;
int hour3 = 0;
int minut4 = 0;
int minut5 = 0;
int minut6 = 0;
int hour4 = 0;
int hour5 = 0;
int hour6 = 0;
int i;

ISR(TIMER0_COMPA_vect) {
  second++;
  if (daley > 0) {
    daley--;
  }
  if (second == 45) {
    second = 0;
    sec1++;
    if (sec1 == 60) {
      sec1 = 0;
      minut1++;
      if (minut1 == 60) {
        minut1 = 0;
        hour1++;
        if (hour1 == 24) {
          hour1 = 0;
        }
      }
    }
    if (clok == 1) {
      if (minut4 == minut1 && hour4 == hour1) {
        bip = 1;
        clok = 0;
      }
    }
    if (daley == 0) {
      if (bip == 4) {
        digitalWrite(buzzerPin, LOW);
        bip = 0;
      }
      if (bip == 3) {
        digitalWrite(buzzerPin, HIGH);
        bip = 4;
        daley = 30;
      }
      if (bip == 2) {
        digitalWrite(buzzerPin, LOW);
        bip = 3;
      }
      if (bip == 1) {
        digitalWrite(buzzerPin, HIGH);
        bip = 2;
        daley = 30;
      }
    }
  }
}

void setup() {
  cli();
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = 240;
  TCCR0B |= (1 << WGM02);
  TCCR0B |= (1 << CS00);
  TCCR0B |= (1 << CS02);
  TIMSK0 |= (1 << OCIE0A);
  sei();

  pinMode (18, INPUT);
  digitalWrite(18, HIGH);
  pinMode (19, INPUT);
  digitalWrite(19, HIGH);
  pinMode (20, INPUT);
  digitalWrite(20, HIGH);
  pinMode (21, INPUT);
  digitalWrite(21, HIGH);
  pinMode (51, OUTPUT);
  digitalWrite(51, HIGH);
  pinMode (50, OUTPUT);
  digitalWrite(50, HIGH);
  pinMode (10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode (11, OUTPUT);
  digitalWrite(11, HIGH);
  pinMode (12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode (13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode (22, OUTPUT);
  digitalWrite(22, HIGH);
  pinMode (23, OUTPUT);
  digitalWrite(23, HIGH);
  pinMode (24, OUTPUT);
  digitalWrite(24, HIGH);
  pinMode (25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode (26, OUTPUT);
  digitalWrite(26, HIGH);
  pinMode (27, OUTPUT);
  digitalWrite(27, HIGH);
  pinMode (28, OUTPUT);
  digitalWrite(28, HIGH);
  pinMode (29, OUTPUT);
  digitalWrite(29, HIGH);
  pinMode (buzzerPin, OUTPUT);
}

void loop() {
  if (A == 1) {
    sec2 = sec1 / 10;
    sec3 = sec1 % 10;
    minut2 = minut1 / 10;
    minut3 = minut1 % 10;
    hour2 = hour1 / 10;
    hour3 = hour1 % 10;
    i = hour2;
    PORTA = numbers[i];
    digitalWrite(digits[0], HIGH);
    digitalWrite(digits[0], LOW);
    i = hour3;
    PORTA = numbers[i];
    digitalWrite(29, LOW);
    digitalWrite(digits[1], HIGH);
    digitalWrite(digits[1], LOW);
    i = minut2;
    PORTA = numbers[i];
    digitalWrite(digits[2], HIGH);
    digitalWrite(digits[2], LOW);
    i = minut3;
    PORTA = numbers[i];
    digitalWrite(29, LOW);
    digitalWrite(digits[3], HIGH);
    digitalWrite(digits[3], LOW);
    i = sec2;
    PORTA = numbers[i];
    digitalWrite(digits[4], HIGH);
    digitalWrite(digits[4], LOW);
    if (clok == 0) {
      i = sec3;
      PORTA = numbers[i];
      digitalWrite(digits[5], HIGH);
      digitalWrite(digits[5], LOW);
    }
    if (clok == 1) {
      PORTA = numbers[10];
      digitalWrite(digits[5], HIGH);
      digitalWrite(digits[5], LOW);
    }
  }
  if (A == 2) {
    minut5 = minut4 / 10;
    minut6 = minut4 % 10;
    hour5 = hour4 / 10;
    hour6 = hour4 % 10;
    i = hour5;
    PORTA = numbers[i];
    digitalWrite(digits[0], HIGH);
    digitalWrite(digits[0], LOW);
    i = hour6;
    PORTA = numbers[i];
    digitalWrite(29, LOW);
    digitalWrite(digits[1], HIGH);
    digitalWrite(digits[1], LOW);
    i = minut5;
    PORTA = numbers[i];
    digitalWrite(digits[2], HIGH);
    digitalWrite(digits[2], LOW);
    i = minut6;
    PORTA = numbers[i];
    digitalWrite(29, LOW);
    digitalWrite(digits[3], HIGH);
    digitalWrite(digits[3], LOW);
    PORTA = numbers[0];
    digitalWrite(digits[4], HIGH);
    digitalWrite(digits[4], LOW);
    PORTA = numbers[0];
    digitalWrite(digits[5], HIGH);
    digitalWrite(digits[5], LOW);
  }
  if (daley == 0) {
    if (digitalRead(18) == LOW) {
      if (A == 1) {
        hour1++;
        if (hour1 == 24) {
          hour1 = 0;
        }
      }
      if (A == 2) {
        hour4++;
        if (hour4 == 24) {
          hour4 = 0;
        }
      }
      daley = 20;
    }
    if (digitalRead(19) == LOW) {
      if (A == 1) {
        minut1++;
        if (minut1 == 60) {
          minut1 = 0;
        }
      }
      if (A == 2) {
        minut4++;
        if (minut4 == 60) {
          minut4 = 0;
        }
      }
      daley = 20;
    }
    if (digitalRead(20) == LOW) {
      A++;
      if (A == 3) {
        A = 1;
      }
      daley = 20;
    }
    if (digitalRead(21) == LOW && A == 2) {
      clok++;
      daley = 20;
      if (clok == 2) {
        clok = 0;
      }
    }
  }
}
