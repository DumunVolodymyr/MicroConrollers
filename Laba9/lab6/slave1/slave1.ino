#define BAUD 14400
#define MODE 2
#define TRANSMIT HIGH
#define RECEIVE LOW
#define ID 11

char symbol;
int i = 0;
char* bufferPtr;

void setup(void) {
  Serial.begin(BAUD);
  pinMode(MODE, OUTPUT);
  digitalWrite(MODE, RECEIVE);
}


void loop() {
  i = 0;
  symbol = 0;
  if (Serial.available()) {
    bufferPtr = (char*)malloc(50 * sizeof(char));
    delay(5);
    while (Serial.available()) {
      symbol = Serial.read();
      if (symbol == ';') {
        bufferPtr[i++] = '\0';
        i = 0;
        int c;
        sscanf(bufferPtr, "%d", &c);
        if (c != ID) {
          while (Serial.available()){Serial.read();}
          free(bufferPtr);
          return;
        }
      } else {
        bufferPtr[i++] = symbol;
      }
    }
    if (i > 0){
      bufferPtr[i++] = '\0';
    }
  }
  if (i > 0) {
    if (!strcmp(bufferPtr, "Give")) {
      char message[] = "05.09.2000";
      sendMessage(1, message, sizeof(message));
    }
  }
  free(bufferPtr);
  //delay(200);
}

void sendMessage(int recevierId, char str[], long sizeOf) {
  char buffer[20];
  digitalWrite(MODE, TRANSMIT);
  long crc = calculateAlgorithmicImplementationGenibusCrc(str, sizeOf / sizeof(char) - 1, 0x7, 0x00, false, false, 0x55, 8);
  delay(60);
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  
  delay(50);
  str[0] ^= (1<<1);
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  
  delay(50);
  str[0] ^= (1<<1);
  str[strlen(str) - 1] ^= (1<<3);
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  
  delay(50);
  str[strlen(str) - 1] ^= (1<<3);
  str[2] ^= (1<<3) | (1<<7);
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  
  delay(50);
  str[2] ^= (1<<3) | (1<<7);
  str[4] ^= (1<<1) | (1<<2) | (1<<5);
  sprintf(buffer, "%d;%d:%lx!%s*", recevierId, ID, crc, str);
  Serial.print(buffer);
  
  delay(60);
  digitalWrite(MODE, RECEIVE);
}

long calculateAlgorithmicImplementationGenibusCrc(const char data[], const unsigned long length, unsigned long crc,
        const unsigned long poly, const bool refIn, const bool refOut, const unsigned long xorOut,
        const unsigned long crcBitLength) {
    unsigned long greatestBit = 1 << (crcBitLength - 1);
    unsigned long cutter = (1 << crcBitLength) - 1;
    unsigned long table[256];

    for (unsigned long i = 0; i < 256; i++) {
        unsigned long res = i;

        if (crcBitLength > 8) {
            res <<= (crcBitLength - 8);
        }

        for (unsigned int j = 0; j < 8; j++) {
            if ((res & greatestBit) != 0) {
                res = (res << 1) ^ poly;
            }
            else {
                res <<= 1;
            }
        }

        table[i] = res & cutter;
    }

    unsigned long shift = (crcBitLength - 8 < 0) ? 0 : (crcBitLength - 8);
    for (unsigned long i = 0; i < length; i++) {
        unsigned long temp = data[i];
        if (refIn) {
            temp = reverseBits(temp, 8);
        }
        crc = table[(crc >> shift) ^ temp] ^ (crc << 8);
        crc &= cutter;
    }

    if (refOut) {
        crc = reverseBits(crc, crcBitLength);
    }

    return crc ^ xorOut;
}

unsigned long reverseBits(unsigned long value, const unsigned long size) {
    unsigned long newValue = 0;

    for (unsigned long i = size - 1; i >= 0; i--) {
        newValue |= (value & 1) << i;
        value >>= 1;
    }
    return newValue;
}
