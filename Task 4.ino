// YUVRAJ BANSAL
// 2110994799

const byte motionSens = 2;
const byte motionSens2 = 7;
const byte soilMoist = 3;

volatile bool curr_State = false;
volatile bool D7_state = LOW;

const int led_pin = PB5;

const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(motionSens, INPUT_PULLUP);
    pinMode(motionSens2, INPUT_PULLUP);
    pinMode(soilMoist, INPUT);

    // Enable PCIE2 Bit3 = 1 (Port D)
    PCICR |= B00000100;
    // Enable PCINT18  & PCINT23 (Pins D2 & D7)
    PCMSK2 |= B10000000;

    // pinMode(LED_BUILTIN, OUTPUT);
    DDRB |= (1 << led_pin);

    attachInterrupt(digitalPinToInterrupt(motionSens), swap, CHANGE);
    attachInterrupt(digitalPinToInterrupt(soilMoist), soil_moist, RISING);

    TCCR1A = 0;

    // Clear on compare
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);

    // set prescaler 1024
    TCCR1B |= (1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= (1 << CS10);

    // reset timer1 set and compare values
    TCNT1 = t1_load;
    OCR1A = t1_comp;

    // enable timer1 compare interrupt
    TIMSK1 = (1 << OCIE1A);

    // enabling global interrupts
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1 << led_pin);
    Serial.println("Timer Interrupt!");
}

void loop()
{
    delay(1000);
}

void swap()
{
    curr_State = !curr_State;
    digitalWrite(LED_BUILTIN, curr_State);
    Serial.println("Motion Interrupt 1");
}

void soil_moist()
{
    Serial.println("Soil Moisture Interrupt");
}

ISR(PCINT2_vect)
{
    if (digitalRead(motionSens2) == LOW)
    {
        D7_state = !D7_state;
        digitalWrite(LED_BUILTIN, D7_state);
        Serial.println("Motion Interrupt 2");
    }
}