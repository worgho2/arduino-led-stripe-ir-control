#define DECODE_NEC 1
#define EXCLUDE_EXOTIC_PROTOCOLS
#include <IRremote.h>

#define IR_RECEIVE_PIN  6

#define RED 9
#define GREEN 10
#define BLUE 5

int currentRED = 0;
int currentGREEN = 0;
int currentBLUE = 0;

void setup() {
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  Serial.begin(9600);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  setRGB(currentRED, currentGREEN, currentBLUE);
}

void loop() {
  if (IrReceiver.decode()) {  
    if (IrReceiver.decodedIRData.address == 0) {
      switch (IrReceiver.decodedIRData.command) {

      case 0x46: // arrow up
        while(true) {
          for(int i=0;i<=255;i++) {
            IrReceiver.resume();
            smoothSetRGB(i,i,i);
            delay(20);
            if (didReceiveCommand() && i>=30) return;
          }

          for(int i=0;i<=255;i++) {
            IrReceiver.resume();
            smoothSetRGB(255-i,255-i,255-i);
            delay(20);
            if (didReceiveCommand() && i<=225) return;
          }
        }
        break;

      case 0x15: // arrow down
        while(true) {
          for(int i=0;i<=255;i++) {
            IrReceiver.resume();
            smoothSetRGB(i,255-i,0);
            delay(20);
            if (didReceiveCommand() && i>=30) return;
          }

          for(int i=255;i>=0;i--) {
            IrReceiver.resume();
            smoothSetRGB(i,255-i,0);
            delay(20);
            if (didReceiveCommand() && i<=225) return;
          }
        }
        break;

      case 0x44: // arrow left
        while(true) {
          for(int i=0;i<=255;i++) {
            IrReceiver.resume();
            smoothSetRGB(0,255-i,i);
            delay(20);
            if (didReceiveCommand() && i>=30) return;
          }

          for(int i=255;i>=0;i--) {
            IrReceiver.resume();
            smoothSetRGB(0,255-i,i);
            delay(20);
            if (didReceiveCommand() && i<=225) return;
          }
        }
        break;

      case 0x43: // arrow right
        while(true) {
          for(int i=0;i<=255;i++) {
            IrReceiver.resume();
            smoothSetRGB(255-i,0,i);
            delay(20);
            if (didReceiveCommand() && i>=30) return;
          }

          for(int i=255;i>=0;i--) {
            IrReceiver.resume();
            smoothSetRGB(255-i,0,i);
            delay(20);
            if (didReceiveCommand() && i<=225) return;
          }
        }
        break;

      case 0x40: // ok
        while(true) {
          smoothSetRGB(random(0,255), random(0,255), random(0,255));

          for (int i=0;i<=2000;i++) {
            IrReceiver.resume();
            delay(1);
            if (didReceiveCommand() && i>=500) return;
          }
        }
        break;

      case 0x42: // *
        IrReceiver.resume();
        smoothSetRGB(random(0,255), random(0,255), random(0,255));
        break;

      case 0x4A: // #
        IrReceiver.resume();
        smoothSetRGB(0,0,0);
        break;

      case 0x52: // 0
        IrReceiver.resume();
        smoothSetRGB(255, 255, 255);
        break;

      case 0x16: // 1
        IrReceiver.resume();
        smoothSetRGB(255, 0, 0);
        break;

      case 0x19: // 2
        IrReceiver.resume();
        smoothSetRGB(0, 255, 0);
        break;

      case 0xD:  // 3
        IrReceiver.resume();
        smoothSetRGB(0, 0, 255);
        break;

      case 0xC:  // 4
        IrReceiver.resume();
        smoothSetRGB(255, 255, 0);
        break;

      case 0x18: // 5
        IrReceiver.resume();
        smoothSetRGB(255, 0, 255);
        break;

      case 0x5E: // 6
        IrReceiver.resume();
        smoothSetRGB(0, 255, 255);
        break;

      case 0x8:  // 7
        IrReceiver.resume();
        smoothSetRGB(255, 0, 150);
        break;

      case 0x1C: // 8
        IrReceiver.resume();
        smoothSetRGB(255, 128, 255);
        break;

      case 0x5A: // 9
        IrReceiver.resume();
        smoothSetRGB(128, 255, 255);
        break;

       default:
        IrReceiver.resume();
        break;
      }
    }
  }
}

void setRGB(int red, int green, int blue) {
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);

  currentRED = red;
  currentGREEN = green;
  currentBLUE = blue;
}

void smoothSetRGB(int red, int green, int blue) {
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
    delay(1);
  }
}

bool didReceiveCommand() {
  if (IrReceiver.decode() && IrReceiver.decodedIRData.address == 0) {
    return true;
  }
  return false;
}

void printCurrentRGB() {
  String p1 = ";";
  Serial.println("RGB -> " + p1 + currentRED + p1 + currentGREEN + p1 + currentBLUE);
}
