#ifndef DELAY_H
#define DELAY_H

// this file contains all the definitions for the circular buffer

#define BUFFER_SIZE 16000
#define READ_START  2000  // Initial delay: 2000 samples

// Delay time limits (in samples)
// At ~48kHz: 100 samples ≈ 2ms, 8000 samples ≈ 167ms
#define DELAY_SAMPLES_MIN  200   // Minimum delay (~4ms)
#define DELAY_SAMPLES_MAX  14000  // Maximum delay (~292ms)
#define DELAY_SAMPLES_DEFAULT 8000  // Default delay (~167ms)

// Delay adjustment step (samples per encoder click)
#define DELAY_ADJUST_STEP  100  // ~2ms per click

#define WET_MIX  192
#define DRY_MIX  64

extern volatile u32 circular_buffer[BUFFER_SIZE];
extern volatile u32 read_head;
extern volatile u32 write_head;

// Delay control state
extern volatile u8 delay_enabled;      // 0 = delay off, 1 = delay on
extern volatile u32 delay_samples;     // Current delay time in samples

#endif
