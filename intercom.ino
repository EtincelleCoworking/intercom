#define LONGPRESS_ONLY

#define CHEAT_PRESS_NUM             4
#define CHEAT_PRESS_INTERVAL_MS     3000
#define DO_BUZZER_DELAY_MS          3000
#define DO_BUTTON_DELAY_MS          3000

#define PIN_INTERCOM_BUTTON         2
#define PIN_INTERCOM_BUZZER         3
#define PIN_INTERCOM_DO_BUZZ        4

void open_door() {
    Serial.println(F("OPEN DOOR"));
    digitalWrite(PIN_INTERCOM_BUTTON, HIGH);
    delay(DO_BUTTON_DELAY_MS);
    digitalWrite(PIN_INTERCOM_BUTTON, LOW);
}

void ring_buzzer() {
    Serial.println(F("RING BUZZER"));
    digitalWrite(PIN_INTERCOM_DO_BUZZ, HIGH);
    delay(DO_BUZZER_DELAY_MS);
    digitalWrite(PIN_INTERCOM_DO_BUZZ, LOW);
}

void setup() {
    Serial.begin(9600);
    pinMode(PIN_INTERCOM_BUTTON, OUTPUT);
    pinMode(PIN_INTERCOM_DO_BUZZ, OUTPUT);
    pinMode(PIN_INTERCOM_BUZZER, INPUT);
    Serial.println(F("SETUP DONE"));
}

void loop() {
    
    unsigned long ms = millis();
    
    if(digitalRead(PIN_INTERCOM_BUZZER) == HIGH) {
#ifdef LONGPRESS_ONLY
        while (digitalRead(PIN_INTERCOM_BUZZER)) {
            // wait till buzz is unpressed
            delay(10);
        }
        
        if(millis() - ms > CHEAT_PRESS_INTERVAL_MS) {
            // pressed long enough, open doors
            open_door();
        }
#else
        unsigned char presses = 0;
        bool pressed;
        for (presses = 0; millis() - ms < CHEAT_PRESS_INTERVAL_MS; ) {
            while (digitalRead(PIN_INTERCOM_BUZZER) == HIGH) {
                // wait till buzz is unpressed
                delay(10);
                pressed = true;
            }
            
            if(pressed) {
                presses++;
                pressed = false;
                Serial.print(presses);
                Serial.println(F(" PRESSES"));
            }
        }
        
        if(presses == CHEAT_PRESS_NUM) {
            // pressed the right number of times during the given interval, open doors
            open_door();
        }
#endif
        else {
            // not press long enough, make buzz ring
            ring_buzzer();
        }
    }
}
