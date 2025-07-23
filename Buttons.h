#pragma once

// buttons
const uint16_t BTN_X         = 0x0001;
const uint16_t BTN_O         = 0x0002;
const uint16_t BTN_SQUARE    = 0x0004;
const uint16_t BTN_TRIANGLE  = 0x0008;
const uint16_t BTN_L1        = 0x0010;
const uint16_t BTN_R1        = 0x0020;
const uint16_t BTN_L2        = 0x0040;
const uint16_t BTN_R2        = 0x0080;

// Dpad
const uint8_t PAD_UP           = 0x01;
const uint8_t PAD_DOWN         = 0x02;
const uint8_t PAD_RIGHT        = 0x04;
const uint8_t PAD_LEFT         = 0x08;
// misc
const uint8_t MISC_PS           = 0x01;
const uint8_t MISC_SHARE        = 0x02;
const uint8_t MISC_OPTIONS      = 0x04;

   // x = 0x0001 current connect back ps controller light
    // o = 0x0002
    // square = 0x0004 current connect right haptic motor
    // triangle = 0x0008
    // upDpad = 0x01
    // downDpad = 0x02
    // leftDpad = 0x08
    // rightDpad = 0x04
    // leftStick = up-down(-508, 512) & restRange(-12 , 20) left-right(-508, 512) & restRange(-4, 12)
    // rightStick = up-down(-508, 512) & restRange(-20, 24) left-right(-508, 512) & restRange(-28, 20)
    // leftbumber = 0x0010
    // leftTrigger = 0x0040 current connect brake 0-1020
    // rightBumper = 0x0020
    // rightTrigger = 0x0080 current connect throttle 0-1020
    // share = 0x02 labled misc
    // middlepad = no know assignment (update: if tochpad enabled become second controler same as mouse)
    // options = 0x04 labled misc
    // psBtn =  0x01 labled misc
