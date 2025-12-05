#ifndef BSP_H
#define BSP_H

#include "xtmrctr.h" // timer/counter driver
#include "xintc.h" // interrupt controller driver
#include "xparameters.h"
#include "xtmrctr_l.h" // low-level driver for timer/counter
#include "xintc_l.h" // low-level driver for interrupt controller
#include "mb_interface.h" //
#include <xbasic_types.h> //
#include <xio.h> // provides I/O utility macros for r/w to hardware registers

#define RESET_VALUE 2048 // modify this to change frequency of sampling_ISR()
#define NUM_INPUT_SAMPLES 5

// Always-on safety effect parameters
// High-Pass Filter: Removes low-frequency rumble (0-256, higher = less filtering)
#define HP_FILTER_COEFF  0  // Moderate high-pass filtering

// AGC/Ducking: Prevents feedback (0-256, where 256 = 100% gain)
#define AGC_THRESHOLD    100000  // Input level threshold for gain reduction
#define AGC_MIN_GAIN     128  // Minimum gain (50% = 128/256)
#define AGC_REDUCTION_RATE 4  // How fast gain reduces (shift right by this amount)

// Input Limiter: Prevents clipping in processing chain
#define INPUT_LIMIT_THRESHOLD  100000  // Threshold for input limiting

// Output Limiter: Final safety check (already have PWM clipping, but this is explicit)
#define OUTPUT_LIMIT_THRESHOLD  100000  // Max value before PWM conversion (RESET_VALUE/2)

#define BTN_MIDDLE  BTN4_MASK
#define BTN_RIGHT   BTN2_MASK
#define BTN_LEFT    BTN1_MASK
#define BTN_TOP     BTN0_MASK
#define BTN_BOTTOM  BTN3_MASK

#define BTN0_MASK   0x01  // bit 0 --> (BTNU on fpga board)
#define BTN1_MASK   0x02  // bit 1 --> (BTNL on fpga board)
#define BTN2_MASK   0x04  // bit 2 --> (BTNR on fpga board)
#define BTN3_MASK   0x08  // bit 3 --> (BTND on fpga board)
#define BTN4_MASK   0x10  // bit 4 --> (BTNC on fpga board)

#define ENC_A       0x01
#define ENC_B       0x02
#define ENC_BTN     0x04

void BSP_init();

// encoder
void init_btn_gpio();
void init_enc_gpio();
void pushBtn_ISR(void *CallbackRef);
void enc_ISR(void *CallbackRef);

// axi_timer_0
int init_sampling_timer();
void sampling_ISR();

// axi_timer_1
int init_pwm_timer();

#endif