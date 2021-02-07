//    Arduino PPM Generator
//    Copyright (C) 2015-2016  Alexandr Kolodkin <alexandr.kolodkin@gmail.com>
//    Changed by SilentResonance
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SimpleModbusSlave.h"
#include "PXX/src/pxx.h"

SimpleModbusSlave slave(1);

#define RC_CHANNEL_MIN 990
#define RC_CHANNEL_MAX 2010

#define SBUS_MIN_OFFSET 192
#define SBUS_MID_OFFSET 992
#define SBUS_MAX_OFFSET 1792
//#define SBUS_MIN_OFFSET 172
//#define SBUS_MID_OFFSET 992
//#define SBUS_MAX_OFFSET 1811
#define SBUS_CHANNEL_NUMBER 16
#define SBUS_PACKET_LENGTH 25
#define SBUS_FRAME_HEADER 0x0f
#define SBUS_FRAME_FOOTER 0x00
#define SBUS_FRAME_FOOTER_V2 0x04
#define SBUS_STATE_FAILSAFE 0x08
#define SBUS_STATE_SIGNALLOSS 0x04
#define SBUS_UPDATE_RATE 15 //ms

#define GUI_CHANNEL_MIN 352
#define GUI_CHANNEL_MAX 1696
// According to https://documents.blackmagicdesign.com/UserManuals/BlackmagicStudioCameraManual.pdf

// Maximum number of channels
#define MAX_COUNT SBUS_CHANNEL_NUMBER

enum State {
  Pulse,             // N-th channel pulse
  StartSync,         // Start of sync pulse
  ContinueSync,      // Sync pulse continuation
  FinishSync         // Sync pulse end
};

// little endian
typedef union {
  unsigned long raw;
  struct {
    word low;
    word high;
  };
} long_t;

typedef union __attribute__ ((packed)) {
  uint16_t raw[];
  struct __attribute__ ((packed)) {
    word quant;               // 1 microsec per system frequency clock
    word max_count;           // Maximum number of channels
    word state;               // 2 - On (inversion) / 1 - On / 0 - Off
    word count;               // Number of channels (0 ... MAX_COUNT)
    word pause;               // Duration of pause (in cycles of the system frequency)
    long_t sync;              // The duration of the synchronization pulse (in cycles of the system frequency)
    word channel[MAX_COUNT];  // Pulse duration (in cycles of the system frequency)
  };
} regs_t;

regs_t tmp;                   // Temporary dataset
regs_t ppm;                   // Working dataset
volatile byte state = Pulse;  // Current state
volatile byte current = 0;    // Current channel number

uint8_t sbusPacket[SBUS_PACKET_LENGTH];
int rcChannels[SBUS_CHANNEL_NUMBER];
uint32_t sbusTime = 0;

//byte const modbus_registers_count = sizeof(regs_t) / sizeof(word);
uint16_t const modbus_registers_count = sizeof(regs_t) / sizeof(uint16_t);

void sbusPreparePacket(uint8_t packet[], int channels[], bool isSignalLoss, bool isFailsafe){

    static int output[SBUS_CHANNEL_NUMBER] = {0};

    /*
     * Map 1000-2000 with middle at 1500 chanel values to
     * 173-1811 with middle at 992 S.BUS protocol requires
     */
    for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
        //output[i] = map(channels[i], RC_CHANNEL_MIN, RC_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
        //output[i] = map(channels[i], GUI_CHANNEL_MIN, GUI_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
        //output[i] = constrain(output[i], SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
        output[i] = channels[i];
    }

    uint8_t stateByte = 0x00;
    if (isSignalLoss) {
        stateByte |= SBUS_STATE_SIGNALLOSS;
    }
    if (isFailsafe) {
        stateByte |= SBUS_STATE_FAILSAFE;
    }
    packet[0] = SBUS_FRAME_HEADER; //Header

    packet[1] = (uint8_t) (output[0] & 0x07FF);
    packet[2] = (uint8_t) ((output[0] & 0x07FF)>>8 | (output[1] & 0x07FF)<<3);
    packet[3] = (uint8_t) ((output[1] & 0x07FF)>>5 | (output[2] & 0x07FF)<<6);
    packet[4] = (uint8_t) ((output[2] & 0x07FF)>>2);
    packet[5] = (uint8_t) ((output[2] & 0x07FF)>>10 | (output[3] & 0x07FF)<<1);
    packet[6] = (uint8_t) ((output[3] & 0x07FF)>>7 | (output[4] & 0x07FF)<<4);
    packet[7] = (uint8_t) ((output[4] & 0x07FF)>>4 | (output[5] & 0x07FF)<<7);
    packet[8] = (uint8_t) ((output[5] & 0x07FF)>>1);
    packet[9] = (uint8_t) ((output[5] & 0x07FF)>>9 | (output[6] & 0x07FF)<<2);
    packet[10] = (uint8_t) ((output[6] & 0x07FF)>>6 | (output[7] & 0x07FF)<<5);
    packet[11] = (uint8_t) ((output[7] & 0x07FF)>>3);
    packet[12] = (uint8_t) ((output[8] & 0x07FF));
    packet[13] = (uint8_t) ((output[8] & 0x07FF)>>8 | (output[9] & 0x07FF)<<3);
    packet[14] = (uint8_t) ((output[9] & 0x07FF)>>5 | (output[10] & 0x07FF)<<6);  
    packet[15] = (uint8_t) ((output[10] & 0x07FF)>>2);
    packet[16] = (uint8_t) ((output[10] & 0x07FF)>>10 | (output[11] & 0x07FF)<<1);
    packet[17] = (uint8_t) ((output[11] & 0x07FF)>>7 | (output[12] & 0x07FF)<<4);
    packet[18] = (uint8_t) ((output[12] & 0x07FF)>>4 | (output[13] & 0x07FF)<<7);
    packet[19] = (uint8_t) ((output[13] & 0x07FF)>>1);
    packet[20] = (uint8_t) ((output[13] & 0x07FF)>>9 | (output[14] & 0x07FF)<<2);
    packet[21] = (uint8_t) ((output[14] & 0x07FF)>>6 | (output[15] & 0x07FF)<<5);
    packet[22] = (uint8_t) ((output[15] & 0x07FF)>>3);

    packet[23] = stateByte; //Flags byte
    packet[24] = SBUS_FRAME_FOOTER; //Footer
}

// Controller initialization
void setup() {
  // Peripheral initialization
  pinMode(9, OUTPUT);         // Debug
  pinMode(10, OUTPUT);        // PPM
  digitalWrite(10, HIGH);

  PXX.begin();

  // Initializing Channel Values
  tmp.state        = 0;
  tmp.max_count    = MAX_COUNT;
  tmp.count        = 8;
  tmp.quant        = F_CPU / 1000000;
  tmp.pause        = F_CPU / 1000000 * 200;
  tmp.sync.raw     = F_CPU / 1000000 * 22500 - F_CPU / 1000000 * 300 * 8;
  
  // Channel duration 300 µs
  for (byte i = 0; i < MAX_COUNT; i++) tmp.channel[i] = 300 * (unsigned long) tmp.quant;

  //Maybe a hint on https://www.rcgroups.com/forums/showthread.php?2390656-RC-to-HengLong-with-arduino
//  timing:
//24ms cycletime
//4ms gap
//1ms pulse
//data in 0.3ms pulses


  // Настраиваем MODBUS
  slave.setup(115200);

  for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
      rcChannels[i] = 1500;
  }
  Serial1.begin(100000, SERIAL_8E2);
}

// Main loop
void loop() {
  uint32_t currentMillis = millis();
  word lastState = tmp.state;
  slave.loop(tmp.raw, modbus_registers_count);
  //if (lastState != tmp.state) {
    //tmp.state > 0 ? Start() : Stop();
    if(tmp.state > 0) {
//      for(int i = 0; i < MAX_COUNT; i++){
//        debugSerial.print(tmp.channel[i],DEC);
//      }
//      debugSerial.println("");
      PXX.prepare(tmp.channel);
      PXX.send();
      
      /*
       * Here you can modify values of rcChannels while keeping it in 1000:2000 range
       */
      if (currentMillis > sbusTime) {
          sbusPreparePacket(sbusPacket, tmp.channel, false, false);
          Serial1.write(sbusPacket, SBUS_PACKET_LENGTH);  
          sbusTime = currentMillis + SBUS_UPDATE_RATE;
      }
      delay(2);
    }
  //}
}

// Run generation
void Start() {
  
  cli();                                           // Disable global interrupt
  ppm = tmp;          
  TIMSK1 = B00000001;                              // Enable Timer Interrupt
  TCCR1A = ppm.state == 2 ? B00110011 : B00100011; // FAST PWM MODE 15
  TCCR1B = B00011001;                              // Prescaler = 1
  TCCR1C = B00000000;                              //
  OCR1A  = ppm.channel[0];                         // Duration of the first impulse with pause
  OCR1B  = ppm.channel[0] - ppm.pause;             // The duration of the first pulse without a pause
  current = 1;                                     //
  sei();                                           // Reenable global interrupt
}

// Stop generation
void Stop() {
  cli();             // Disable global interrupt
  TCCR1B = 0;        // Stop the timer
  TIMSK1 = 0;        // Disable timer interrupt
  sei();             // Reenable global interrupt
}

// Timer Overflow interrupt
ISR(TIMER1_OVF_vect) {

  switch (state) {
    
  // Импульс n-го канала
  case Pulse:   
    OCR1A = ppm.channel[current];                 // The duration of the current pulse with pause
    OCR1B = ppm.channel[current] - ppm.pause;     // The duration of the current pulse without a pause

    // We proceed to the formation of a clock pulse
    if (++current == ppm.count) state = (ppm.sync.high == 0) ? FinishSync : ppm.sync.low > ppm.pause ? ContinueSync : StartSync;                               
    break;

  // If the minor ppm.sync.low is less than ppm.pause, then we start the impulse with the duration ppm.pause and
  // on the next pass ppm.sync.low will be more than ppm.pause
  case StartSync:
    OCR1B = OCR1A = ppm.pause;
    ppm.sync.raw -= ppm.pause;
    state = ppm.sync.high ? ContinueSync : FinishSync;
    break;

  // If ppm.sync.high> 0 start the pulse of maximum duration
  case ContinueSync: 
    OCR1A = OCR1B = 0xFFFF;       
    if (--ppm.sync.high == 0) state = FinishSync;
    break;

  // Completion of sync pulse
  case FinishSync: 
    OCR1A = ppm.sync.low;
    OCR1B = ppm.sync.low - ppm.pause;
    ppm = tmp;
    state = Pulse;
    current = 0;
  }
}
