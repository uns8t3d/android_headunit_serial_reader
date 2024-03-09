/*
This small sketch is an example of reading and transmitting data via Android Head Unit and CAN decoder.
The main idea is to provide base functionality of communication between head unit and CAN decoder and also get ability to read some messages and use it for own purposes.
This sketch connects in between of head unit and CAN adapter via UART (Serial) interface, TX from CAN adapter connects to RX of Arduino and RX of head unit connects to TX of Arduino.

To filter the necessary message you need to know it is exact length and assign it to MAX_MESSAGE_LENGTH const as well as you need to know how to determine the message you want to keep,
for this purposes here available two more configurable variables, MESSAGE_MASK_LENGTH, which means how much bytes should be checked to think that the current message is what we looking for, along with this
the variable MESSAGE_MASK, which is a bytes array with length of MESSAGE_MASK_LENGTH should have exact numbers of bytes as MESSAGE_MASK_LENGTH variable.

First step to determine what you should paste in config vars you need toenable CAN BUS Debug mode on your headunit (may have a different name if different vendors) and look on the data that you receive. Length
will be amount of HEX symbols, for ex. 2E 01 08 AA CE 3A 2E 18 has length 8. And for the mask and mask length for ex. we will look on first 3 bytes, so we assign 3 to MESSAGE_MASK_LENGTH.
For the MESSAGE_MASK you should convert HEX values to int and paste it to array initialization. In example 46, 1, 8 is decimal interpritation of 2E 01 08.

Good luck!
*/



const unsigned int MAX_MESSAGE_LENGTH = 12;                                     // max length of message
const unsigned int MESSAGE_MASK_LENGTH = 3;                                     // length of search mask
byte MESSAGE_MASK[MESSAGE_MASK_LENGTH] = {46, 1, 8};                            // equivalent of 2E 1 8 in message, mask to search by. To get hex equivalent use converter from hex to int

void setup() {    
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    static byte message[MAX_MESSAGE_LENGTH];                                  // here we will store message as array of bytes
    static unsigned int message_pos = 0;                                      // byte position and counter
    byte inByte = Serial.read();                                              // read incoming byte
    if (!Serial.availableForWrite()) {                                        // check if buffer available for write (for ex. not overflow)
      Serial.flush();                                                         // if not available try to flush it, helps when overflowed
    }
    Serial.write(inByte);                                                     // and immediately write this byte 
    if (message_pos < MESSAGE_MASK_LENGTH && inByte != MESSAGE_MASK[message_pos] ) {          // checking if current message confronts to our message mask and mask length, if not we reset the counter and do not do any further actions
      message_pos = 0;
      continue;
    }
    if (message_pos < MAX_MESSAGE_LENGTH) {                       // if message receiving not completed (not all bytes read) we keep current byte in it's position and increment counter
      message[message_pos] = inByte;
      message_pos++;
    } else {                                                      // else we advice that message read complete
      printSerialMessage(message);                                // in this example we print all the message to Serial Monitor
      message_pos = 0;                                            // and reset the counter
    }
  }
}

void printSerialMessage(byte message[MAX_MESSAGE_LENGTH]) {           // method to print serial message. On printing message[i] it's possible to specify format of output, for example HEX, looks like Serial.println(message[i], HEX);
  for (int i = 0; i < MAX_MESSAGE_LENGTH; i++)             // this loop is only for debugging, which also makes impossible to work canbus or whatever with head unit
          {
            Serial.print("Position: ");                           // printing index of byte
            Serial.println(i);
            Serial.print("Byte: ");                               // and value of byte
            Serial.println(message[i]);
          }
      Serial.println("-----------------------------");
}