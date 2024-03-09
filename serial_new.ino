const unsigned int MAX_MESSAGE_LENGTH = 12;                                     // max length of message
const unsigned int MESSAGE_MASK_LENGTH = 3;                                     // length of search mask
byte MESSAGE_MASK[MESSAGE_MASK_LENGTH] = {46, 1, 8};                            // equivalent of 2E 1 8 in message, mask to search by. To get hex equivalent use converter from hex to int

void setup() {    
  pinMode(13, OUTPUT);   
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    static byte message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
    byte inByte = Serial.read();
    if (!Serial.availableForWrite()) {
      Serial.flush();
    }
    Serial.write(inByte);
    if (message_pos < MESSAGE_MASK_LENGTH && inByte != MESSAGE_MASK[message_pos] ) {
      message_pos = 0;
      digitalWrite(13, LOW);
      continue;
    }
    if (message_pos < MAX_MESSAGE_LENGTH) {
      digitalWrite(13, HIGH);
      message[message_pos] = inByte;
      message_pos++;
    } else {
      printSerialMessage(message);
      digitalWrite(13, LOW);
      message_pos = 0;
    }
  }
}

void printSerialMessage(byte message[MAX_MESSAGE_LENGTH]) {
  for (int i = 0; i < MAX_MESSAGE_LENGTH; i++)             // this loop is only for debugging, which also makes impossible to work canbus or whatever with head unit
          {
            Serial.print("Position: ");                           // printing index of byte
            Serial.println(i);
            Serial.print("Byte: ");                               // and value of byte
            Serial.println(message[i]);
          }
      Serial.println("End");
}