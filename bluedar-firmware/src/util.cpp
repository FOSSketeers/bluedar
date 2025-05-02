#include <Arduino.h>

String getBleAppearanceString(uint16_t appearanceId) {
    switch (appearanceId) {
        case 0x0000:
            return "UNKNOWN";
        case 0x0040:
            return "GENERIC_PHONE";
        case 0x0080:
            return "GENERIC_COMPUTER";
        case 0x00C0:
            return "GENERIC_WATCH";
        case 0x00C1:
            return "SPORTS_WATCH";
        case 0x0100:
            return "GENERIC_CLOCK";
        case 0x0140:
            return "GENERIC_DISPLAY";
        case 0x0180:
            return "GENERIC_REMOTE";
        case 0x01C0:
            return "GENERIC_EYEGLASSES";
        case 0x0200:
            return "GENERIC_TAG";
        case 0x0240:
            return "GENERIC_KEYRING";
        case 0x0280:
            return "GENERIC_MEDIA_PLAYER";
        case 0x02C0:
            return "GENERIC_BARCODE_SCANNER";
        case 0x0300:
            return "GENERIC_THERMOMETER";
        case 0x0301:
            return "THERMOMETER_EAR";
        case 0x0340:
            return "GENERIC_HEART_RATE";
        case 0x0341:
            return "HEART_RATE_BELT";
        case 0x0380:
            return "GENERIC_BLOOD_PRESSURE";
        case 0x0381:
            return "BLOOD_PRESSURE_ARM";
        case 0x0382:
            return "BLOOD_PRESSURE_WRIST";
        case 0x03C0:
            return "GENERIC_HID";
        case 0x03C1:
            return "HID_KEYBOARD";
        case 0x03C2:
            return "HID_MOUSE";
        case 0x03C3:
            return "HID_JOYSTICK";
        case 0x03C4:
            return "HID_GAMEPAD";
        case 0x03C5:
            return "HID_DIGITIZER_TABLET";
        case 0x03C6:
            return "HID_CARD_READER";
        case 0x03C7:
            return "HID_DIGITAL_PEN";
        case 0x03C8:
            return "HID_BARCODE_SCANNER";
        case 0x0400:
            return "GENERIC_GLUCOSE";
        case 0x0440:
            return "GENERIC_WALKING";
        case 0x0441:
            return "WALKING_IN_SHOE";
        case 0x0442:
            return "WALKING_ON_SHOE";
        case 0x0443:
            return "WALKING_ON_HIP";
        case 0x0480:
            return "GENERIC_CYCLING";
        case 0x0481:
            return "CYCLING_COMPUTER";
        case 0x0482:
            return "CYCLING_SPEED";
        case 0x0483:
            return "CYCLING_CADENCE";
        case 0x0484:
            return "CYCLING_POWER";
        case 0x0485:
            return "CYCLING_SPEED_CADENCE";
        case 0x0C40:
            return "GENERIC_PULSE_OXIMETER";
        case 0x0C41:
            return "PULSE_OXIMETER_FINGERTIP";
        case 0x0C42:
            return "PULSE_OXIMETER_WRIST";
        case 0x0C80:
            return "GENERIC_WEIGHT";
        case 0x0CC0:
            return "GENERIC_PERSONAL_MOBILITY_DEVICE";
        case 0x0CC1:
            return "POWERED_WHEELCHAIR";
        case 0x0CC2:
            return "MOBILITY_SCOOTER";
        case 0x0D00:
            return "GENERIC_CONTINUOUS_GLUCOSE_MONITOR";
        case 0x0D40:
            return "GENERIC_INSULIN_PUMP";
        case 0x0D41:
            return "INSULIN_PUMP_DURABLE_PUMP";
        case 0x0D44:
            return "INSULIN_PUMP_PATCH_PUMP";
        case 0x0D48:
            return "INSULIN_PEN";
        case 0x0D80:
            return "GENERIC_MEDICATION_DELIVERY";
        case 0x1440:
            return "GENERIC_OUTDOOR_SPORTS";
        case 0x1441:
            return "OUTDOOR_SPORTS_LOCATION";
        case 0x1442:
            return "OUTDOOR_SPORTS_LOCATION_AND_NAV";
        case 0x1443:
            return "OUTDOOR_SPORTS_LOCATION_POD";
        case 0x1444:
            return "OUTDOOR_SPORTS_LOCATION_POD_AND_NAV";
        default:
            return "THIS SHOULD NEVER HAPPEN";
    }
}

String getBleAddressTypeString(int addrType) {
    switch (addrType) {
        case 0x00:
            return "Public Device Address";
        case 0x01:
            return "Random Device Address";
        case 0x02:
            return "Resolvable Private Address with public identity addr";
        case 0x03:
            return "Resolvable Private Address with random identity addr";
        default:
            return "THIS SHOULD NEVER HAPPEN";
    }
}

String getBtcClassOfDeviceString(uint32_t codType) {
    switch (codType) {
        case 0x000000:
            return "none";
        case 0x000020:
            return "Unknown";
        case 0x000104:
            return "Desktop Workstation";
        case 0x00010c:
            return "Laptop";
        case 0x000116:
            return "Palm sized PC/PDA";
        case 0x000204:
            return "Cellular";
        case 0x000208:
            return "Cordless";
        case 0x000408:
            return "Hands-free Device";
        case 0x00041c:
            return "Portable Audio";
        case 0x000424:
            return "Set-top Box";
        case 0x00043c:
            return "Video Display";
        case 0x000508:
            return "Joystick";
        case 0x000540:
            return "Keyboard";
        case 0x000584:
            return "Joystick";
        case 0x000704:
            return "Wrist Watch";
        case 0x000908:
            return "Thermometer";
        case 0x001101:
            return "none";
        case 0x001f00:
            return "none";
        case 0x001f1f:
            return "Unknown";
        case 0x0020fc:
            return "Unknown";
        case 0x00250c:
            return "Remote Control";
        case 0x002540:
            return "Keyboard";
        case 0x002580:
            return "Pointing Device";
        case 0x002594:
            return "Digitizer Tablet";
        case 0x01020c:
            return "Smart phone";
        case 0x010500:
            return "Smart phone";
        case 0x011f00:
            return "none";
        case 0x020000:
            return "Desktop Workstation";
        case 0x020104:
            return "Desktop Workstation";
        case 0x02010c:
            return "Laptop";
        case 0x020110:
            return "Handheld PC/PDA";
        case 0x020300:
            return "Fully available";
        case 0x022100:
            return "Uncategorized";
        case 0x040600:
            return "Uncategorized";
        case 0x040640:
            return "Scanner";
        case 0x040680:
            return "Printer";
        case 0x060104:
            return "Desktop Workstation";
        case 0x06010c:
            return "Laptop";
        case 0x080208:
            return "Cordless";
        case 0x08043c:
            return "Video Display";
        case 0x080500:
            return "Uncategorized";
        case 0x080618:
            return "Display";
        case 0x0a0110:
            return "Handheld PC/PDA";
        case 0x0a041c:
            return "Portable Audio";
        case 0x0c010c:
            return "Laptop";
        case 0x0c010f:
            return "Laptop";
        case 0x100000:
            return "none";
        case 0x10010c:
            return "Laptop";
        case 0x100114:
            return "Palm sized PC/PDA";
        case 0x100204:
            return "Cellular";
        case 0x100208:
            return "Cordless";
        case 0x102104:
            return "Desktop Workstation";
        case 0x10210c:
            return "Laptop";
        case 0x120104:
            return "Desktop Workstation";
        case 0x12010c:
            return "Laptop";
        case 0x120114:
            return "Palm sized PC/PDA";
        case 0x120300:
            return "none";
        case 0x140680:
            return "Printer";
        case 0x18010c:
            return "Laptop";
        case 0x180204:
            return "Cellular";
        case 0x1a010c:
            return "Laptop";
        case 0x1a0110:
            return "Handheld PC/PDA";
        case 0x1a0114:
            return "Palm sized PC/PDA";
        case 0x1c010c:
            return "Laptop";
        case 0x1c010e:
            return "Laptop";
        case 0x200204:
            return "Cellular";
        case 0x200404:
            return "Wearable Headset Device";
        case 0x200408:
            return "Hands-free Device";
        case 0x22011C:
            return "Tablet";
        case 0x240400:
            return "none";
        case 0x240404:
            return "Wearable Headset Device";
        case 0x240408:
            return "Hands-free Device";
        case 0x240414:
            return "Loudspeaker";
        case 0x240704:
            return "Wrist Watch";
        case 0x280408:
            return "Hands-free Device";
        case 0x2a010c:
            return "Laptop";
        case 0x2a0110:
            return "Handheld PC/PDA";
        case 0x2c0404:
            return "Wearable Headset Device";
        case 0x2e010c:
            return "Laptop";
        case 0x300204:
            return "Cellular";
        case 0x300408:
            return "Hands-free Device";
        case 0x302104:
            return "Desktop Workstation";
        case 0x30210c:
            return "Laptop";
        case 0x340404:
            return "Wearable Headset Device";
        case 0x340408:
            return "Hands-free Device";
        case 0x36010c:
            return "Laptop";
        case 0x380104:
            return "Desktop Workstation";
        case 0x38010c:
            return "Laptop";
        case 0x380408:
            return "Hands-free Device";
        case 0x3a0104:
            return "Desktop Workstation";
        case 0x3a010c:
            return "Laptop";
        case 0x3c0104:
            return "Desktop Workstation";
        case 0x3c010c:
            return "Laptop";
        case 0x3e0104:
            return "Desktop Workstation";
        case 0x3e0108:
            return "Server-Class Computer";
        case 0x3e010c:
            return "Laptop";
        case 0x400204:
            return "Cellular";
        case 0x400210:
            return "Wired modem or voice gateway";
        case 0x402204:
            return "Cellular";
        case 0x41031c:
            return "Fully available";
        case 0x422100:
            return "none";
        case 0x4a0100:
            return "Uncategorized";
        case 0x4a0204:
            return "Cellular";
        case 0x500204:
            return "Cellular";
        case 0x50020c:
            return "Smart phone";
        case 0x520204:
            return "Cellular";
        case 0x520208:
            return "Cordless";
        case 0x52020c:
            return "Palm sized PC/PDA | Smart phone";
        case 0x522204:
            return "Cellular";
        case 0x580204:
            return "Cellular";
        case 0x58020c:
            return "Smart phone";
        case 0x5a0100:
            return "Uncategorized";
        case 0x5a0204:
            return "Cellular";
        case 0x5a0208:
            return "Cordless";
        case 0x5a020c:
            return "Smart phone";
        case 0x5a2204:
            return "Cellular";
        case 0x5e0204:
            return "Cellular";
        case 0x62020c:
            return "Smart phone";
        case 0x6a041c:
            return "Portable Audio";
        case 0x6e010c:
            return "Laptop";
        case 0x70020c:
            return "Smart phone";
        case 0x700404:
            return "Wearable Headset Device";
        case 0x720204:
            return "Cellular";
        case 0x72020c:
            return "Smart phone";
        case 0x720408:
            return "Hands-free Device";
        case 0x722404:
            return "Wearable Headset Device";
        case 0x740408:
            return "Hands-free Device";
        case 0x760408:
            return "Hands-free Device";
        case 0x78020c:
            return "Smart phone";
        case 0x7a010c:
            return "Laptop";
        case 0x7a020c:
            return "Smart phone";
        case 0x7e0104:
            return "Desktop Workstation";
        case 0x7e010c:
            return "Laptop";
        case 0x920100:
            return "Uncategorized";
        case 0x9a0100:
            return "Uncategorized";
        case 0x9a0114:
            return "Palm sized PC/PDA";
        case 0xb16404:
            return "Wearable Headset Device";
        case 0xb50404:
            return "Wearable Headset Device";
        case 0xb56404:
            return "Wearable Headset Device";
        case 0xd20110:
            return "Handheld PC/PDA";
        case 0xf40404:
            return "Hands-free Device";
        case 0xf62404:
            return "Wearable Headset Device";
        case 0xff0104:
            return "Desktop Workstation";
        case 0xff010c:
            return "Laptop";
        default:
            return "UNKNOWN";
    }
}
