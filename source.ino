#define DECODE_NEC 1
#define EXCLUDE_EXOTIC_PROTOCOLS
#include <IRremote.h>

#define IR_RECEIVE_PIN  6
#define RED 9
#define GREEN 10
#define BLUE 5

// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

int currentRED = 0;
int currentGREEN = 0;
int currentBLUE = 0;

String p1 = ";";

void setup() {
//    Serial.begin(9600);
    delay(2000);

    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

    pinMode(BLUE, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
}

void loop() {
    if (IrReceiver.decode()) {
      
        IrReceiver.stop();
        delay(8);
        IrReceiver.start(8000);
        IrReceiver.resume();

        if (IrReceiver.decodedIRData.address == 0) {
          
            switch (IrReceiver.decodedIRData.command) {
              
            case 0x46: // Seta para cima
              while(true) {
                for(int i=0;i<=255;i++) {
                  smoothSetRGB(i,i,i, 1);
                  delay(15);
                  if (IrReceiver.decode()) { return; }
                }

                for(int j=0;j<=255;j++) {
                  smoothSetRGB(255-j,255-j,255-j, 1);
                  delay(15);
                  if (IrReceiver.decode()) { return; }
                }
              
                if (IrReceiver.decode()) { return; }
              }
              break;
              
            case 0x15: // Seta para baixo              
              while(true) {
                for(int i=0;i<=255;i++) {
                  smoothSetRGB(i,255-i,0, 1);
                  delay(20);
                  if ( IrReceiver.decode()) { return; }
                }

                for(int i=255;i>=0;i--) {
                  smoothSetRGB(i,255-i,0, 1);
                  delay(20);
                  if (IrReceiver.decode()) { return; }
                }
                
                if (IrReceiver.decode()) { return; }
              }
              break;
              
            case 0x44: // Seta para esquerda
              while(true) {
                for(int i=0;i<=255;i++) {
                  smoothSetRGB(0,255-i,i, 1);
                  delay(20);
                  if (IrReceiver.decode()) { return; }
                }

                for(int i=255;i>=0;i--) {
                  smoothSetRGB(0,255-i,i, 1);
                  delay(20);
                  if (IrReceiver.decode()) { return; }
                }
                
                if (IrReceiver.decode()) { return; }
              }
              break;
              
            case 0x43: // Seta para direita
              while(true) {
                for(int i=0;i<=255;i++) {
                  smoothSetRGB(255-i,0,i, 1);
                  delay(20);
                  if ( IrReceiver.decode() ) { return; }
                }

                for(int i=255;i>=0;i--) {
                  smoothSetRGB(255-i,0,i, 1);
                  delay(20);
                  if (IrReceiver.decode() ) { return; }
                }
                
                if (IrReceiver.decode() ) { return; }
              }
              break;
              
            case 0x40: // OK             
              while(true) {
                smoothSetRGB(random(0,255), random(0,255), random(0,255), 40);

                for (int i=0;i<=random(10000,30000);i++) {
                  delay(1);
                  if (IrReceiver.decode()) { return; }
                }

                if (IrReceiver.decode()) { return; }
              }
              break;
              
            case 0x42: // *
              smoothSetRGB(random(0,255), random(0,255), random(0,255), 1);
              break;
              
            case 0x4A: // #
              smoothSetRGB(0,0,0, 1);
              break;
              
            case 0x52: // 0
              smoothSetRGB(255, 255, 255, 1);
              break;
              
            case 0x16: // 1
              smoothSetRGB(255, 0, 0, 1);
              break;
              
            case 0x19: // 2
              smoothSetRGB(0, 255, 0, 1);
              break;
              
            case 0xD:  // 3
              smoothSetRGB(0, 0, 255, 1);
              break;
              
            case 0xC:  // 4
              smoothSetRGB(255, 255, 0, 1);
              break;
              
            case 0x18: // 5
              smoothSetRGB(255, 0, 255, 1);
              break;
              
            case 0x5E: // 6
              smoothSetRGB(0, 255, 255, 1);
              break;
              
            case 0x8:  // 7
              smoothSetRGB(255, 255, 128, 1);
              break;
              
            case 0x1C: // 8
              smoothSetRGB(255, 128, 255, 1);
              break;
              
            case 0x5A: // 9
              smoothSetRGB(128, 255, 255, 1);
              break;
              
             default: break;
            }
        }
    }
}

static void setRGB(int red, int green, int blue) {
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);

  currentRED = red;
  currentGREEN = green;
  currentBLUE = blue;

  printCurrentRGB();
}

static void smoothSetRGB(int red, int green, int blue, int delayTime) {
  while(currentRED != red || currentGREEN != green || currentBLUE != blue) {
    if(currentRED > red) {
      currentRED -= 1;
    } else if (currentRED < red) {
      currentRED += 1;
    }

    if(currentGREEN > green) {
      currentGREEN -= 1;
    } else if (currentGREEN < green) {
      currentGREEN += 1;
    }

    if(currentBLUE > blue) {
      currentBLUE -= 1;
    } else if (currentBLUE < blue) {
      currentBLUE += 1;
    }

    setRGB(currentRED, currentGREEN, currentBLUE);
    delay(delayTime);
    if (IrReceiver.decode()) { return; }
  }
}

static void printCurrentRGB() {
//  Serial.println("RGB -> " + p1 + currentRED + p1 + currentGREEN + p1 + currentBLUE);
}
