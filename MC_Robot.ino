#include <Bluepad32.h>
#include "Buttons.h"

const int builtInLed = 2;

int setTurnSpeed = 1;

const int m1Rpwm = 23; // d23 swivel base
const int m1Lpwm = 22; // d22 swivel base
const int m2Rpwm = 21; // d21 shoulder arm
const int m2Lpwm = 19; // d19 shoulder arm
const int m3Rpwm = 32; // d32 forward arm
const int m3Lpwm = 33; // d33 forward arm
const int m4Rpwm = 25; // d25 clamp hand
const int m4Lpwm = 26; // d26 clamp hand

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis 
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

void dumpMouse(ControllerPtr ctl) {
    Serial.printf("idx=%d, buttons: 0x%04x, scrollWheel=0x%04x, delta X: %4d, delta Y: %4d\n",
                   ctl->index(),        // Controller Index
                   ctl->buttons(),      // bitmask of pressed buttons
                   ctl->scrollWheel(),  // Scroll Wheel
                   ctl->deltaX(),       // (-511 - 512) left X Axis
                   ctl->deltaY()        // (-511 - 512) left Y axis
    );
}

void processGamepad(ControllerPtr ctl) {
    int throttle = ctl->throttle(); // Range: 0 - 1023
    int reverse = ctl->brake();       // Range: 0 - 1023

    if (ctl->miscButtons() == MISC_SHARE){
        setTurnSpeed++;
        if (setTurnSpeed > 3) setTurnSpeed = 1;
    }

    int maxPwm = 85;
    if (setTurnSpeed == 2) maxPwm = 127;
    else if (setTurnSpeed == 3) maxPwm = 255;

    
   
    switch (setTurnSpeed) {
        case 1:
            // Green
            ctl->setColorLED(0, 255, 0);
            break;
        case 2:
            // Blue
            ctl->setColorLED(0, 0, 255);
            break;
        case 3:
            // Red
            ctl->setColorLED(255, 0, 0);
            break;
    }

    if ((ctl->buttons() & BTN_R2) && (ctl->buttons() & BTN_L2)) {
        analogWrite(m1Rpwm, 0);
        analogWrite(m1Lpwm, 0);
    } else if (ctl->buttons() & BTN_R2) {
        int pwm = map(throttle, 0, 1023, 30, maxPwm);
        analogWrite(m1Lpwm, 0);
        analogWrite(m1Rpwm, pwm);
    } else if (ctl->buttons() & BTN_L2) {
        int pwm = map(reverse, 0, 1023, 30, maxPwm);
        analogWrite(m1Rpwm, 0);
        analogWrite(m1Lpwm, pwm);
    } else {
        analogWrite(m1Rpwm, 0);
        analogWrite(m1Lpwm, 0);
    }


    if (ctl->axisY() > 70) {
        analogWrite(m2Lpwm, 0);
        analogWrite(m2Rpwm, 255);  
    } else if (ctl->axisY() < -70) {
        analogWrite(m2Rpwm, 0);
        analogWrite(m2Lpwm, 255);
    } else {
        analogWrite(m2Rpwm, 0);
        analogWrite(m2Lpwm, 0);
    }

    if (ctl->axisRY() > 70) {
        analogWrite(m3Lpwm, 0);
        analogWrite(m3Rpwm, 255);  
    } else if (ctl->axisRY() < -70) {
        analogWrite(m3Rpwm, 0);
        analogWrite(m3Lpwm, 255);
    } else {
        analogWrite(m3Rpwm, 0);
        analogWrite(m3Lpwm, 0);
    }


    if (ctl->buttons() == BTN_L1) {
        analogWrite(m4Rpwm, 0);
        analogWrite(m4Lpwm, 255);
    } else if (ctl->buttons() == BTN_R1) {
        analogWrite(m4Lpwm, 0);
        analogWrite(m4Rpwm, 255);
    } else {
        analogWrite(m4Lpwm, 0);
        analogWrite(m4Rpwm, 0);
    }

    if (ctl->buttons() == BTN_X) {
        Serial.print("button pressed");
        digitalWrite(builtInLed, HIGH);
    }
    if (ctl->buttons() != BTN_X) {
        digitalWrite(builtInLed, LOW);
    }


    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    // dumpGamepad(ctl);
}

void processMouse(ControllerPtr ctl) {
    // This is just an example.
    if (ctl->scrollWheel() > 0) {
        // Do Something
    } else if (ctl->scrollWheel() < 0) {
        // Do something else
    }

    // See "dumpMouse" for possible things to query.
    dumpMouse(ctl);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            } else if (myController->isMouse()) {
                processMouse(myController);
            } else {
                Serial.println("Unsupported controller");
            }
        }
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);
    pinMode(builtInLed, OUTPUT);
    pinMode(m1Rpwm, OUTPUT);
    pinMode(m1Lpwm, OUTPUT);
    pinMode(m2Rpwm, OUTPUT);
    pinMode(m2Lpwm, OUTPUT);
    pinMode(m3Rpwm, OUTPUT);
    pinMode(m3Lpwm, OUTPUT);
    pinMode(m4Rpwm, OUTPUT);
    pinMode(m4Lpwm, OUTPUT);


    digitalWrite(m1Rpwm, LOW);
    digitalWrite(m1Lpwm, LOW);
    digitalWrite(m2Lpwm, LOW);
    digitalWrite(m2Rpwm, LOW);
    digitalWrite(m3Rpwm, LOW);
    digitalWrite(m3Lpwm, LOW);
    digitalWrite(m4Rpwm, LOW);
    digitalWrite(m4Lpwm, LOW);
    

    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    BP32.setup(&onConnectedController, &onDisconnectedController);

    
    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    // BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    //     vTaskDelay(1);
    delay(150);
}
