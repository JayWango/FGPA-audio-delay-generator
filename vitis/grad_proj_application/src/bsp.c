#include "bsp.h"
#include "delay.h"

XIntc sys_intc;
XTmrCtr sampling_tmr; // axi_timer_0
XTmrCtr pwm_tmr; // axi_timer_1

volatile u32 circular_buffer[BUFFER_SIZE] = {0};
volatile u32 read_head = 0;
volatile u32 write_head = 0;

// variables used in sampling_ISR() for printing statistics and collecting the DC offset of the raw data
volatile static u32 count = 0;
static int32_t dc_bias = 0;
static int first_run = 1; // just a simple flag

void BSP_init() {
	// interrupt controller
	XIntc_Initialize(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	XIntc_Start(&sys_intc, XIN_REAL_MODE);
	init_pwm_timer();
	init_sampling_timer();
}

void sampling_ISR() {
    // refer to stream_grabber.c from lab3a for why this is necessary
	// BASEADDR + 4 is the offset of where you "select" which index to read from the stream grabber
	// BASEADDR + 8 is the offset of where you actually read the raw data of the mic
    Xil_Out32(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR + 4, 0);
    u32 raw_data = Xil_In32(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR + 8);

    // cast the u32 to int32 so that the raw data is a bipolar signal (audio waves have pos/neg values)
    int32_t curr_sample = (int32_t) raw_data;

    // set first sample from mic as the dc_bias
    if (first_run) {
        dc_bias = curr_sample;
        first_run = 0;
    }

	// self note: try to comment out this exponential moving average and see how it affects our audio signal
    // Exponential Moving Average
	// this line expands to: dc_bias = dc_bias + ((curr_sample - dc_bias) >> 10);
    dc_bias += (curr_sample - dc_bias) >> 10;

    // remove the DC offset from the current sample
    int32_t audio_signal = curr_sample - dc_bias;

    // now that we preserve the sign, we can shift safely
	// scale the signal down to a nice number ideally between -1133 and 1133
    int32_t scaled_signal = audio_signal >> 15;

    // re-center for PWM (unsigned output between 0 to 2267)
    // we add the mid-point of the PWM ticks (2267/2 = 1133) to turn the signed AC wave into a positive DC wave
    int32_t pwm_sample = scaled_signal + (RESET_VALUE / 2);

    // clip the audio for safety 
    if (pwm_sample < 0) pwm_sample = 0;
    if (pwm_sample > RESET_VALUE) pwm_sample = RESET_VALUE;

    // print data - remove in the final product to make this ISR faster (printing in ISR is generally bad)
    count++;
	if (count >= 44100) {
		xil_printf("raw_sample:    %lu\r\n", raw_data);
		xil_printf("curr_sample:   %ld\r\n", curr_sample);
		xil_printf("dc_bias: 	   %ld\r\n", dc_bias);
		xil_printf("audio_signal:  %ld\r\n", audio_signal);
		xil_printf("scaled_signal: %ld\r\n", scaled_signal);
		xil_printf("pwm_sample:    %ld\r\n", pwm_sample);
		xil_pritnf("\r\n");
		count = 0;
	}

	// set the duty cycle of the PWM signal
    XTmrCtr_SetResetValue(&pwm_tmr, 1, pwm_sample);

    // need to write some value to baseaddr of stream grabber to reset it for the next sample
    Xil_Out32(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR, 0);

    // clear the interrupt flag to enable the interrupt to trigger again; csr = control status register
    Xuint32 csr = XTmrCtr_ReadReg(sampling_tmr.BaseAddress, 0, XTC_TCSR_OFFSET);
    XTmrCtr_WriteReg(sampling_tmr.BaseAddress, 0, XTC_TCSR_OFFSET, csr | XTC_CSR_INT_OCCURED_MASK);
}

int init_sampling_timer() {
	XStatus Status;
	Status = XST_SUCCESS;
	Status = XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR,
			(XInterruptHandler) sampling_ISR, &sampling_tmr);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected to Interrupt Controller!\r\n");

	/*
	 * Enable the interrupt for the timer counter
	 */
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);
	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XTmrCtr_Initialize(&sampling_tmr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Timer initialization failed...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Initialized Timer!\r\n");
	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&sampling_tmr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	// clk cycles / 100 Mhz = period
	XTmrCtr_SetResetValue(&sampling_tmr, 0, 0xFFFFFFFF - RESET_VALUE);// 2267 clk cycles @ 100MHz = 22.67 us
	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&sampling_tmr, 0);

	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */
	microblaze_register_handler(
			(XInterruptHandler) XIntc_DeviceInterruptHandler,
			(void*) XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);

	// refer to stream_grabber.c from lab3a; need to write some value to the base address to reinitialize the stream grabber
	Xil_Out32(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR, 0);

	xil_printf("Interrupts enabled!\r\n");
	microblaze_enable_interrupts();

	return XST_SUCCESS;
}

int init_pwm_timer() {
	XStatus Status;

	// Initialize the PWM Timer instance
	Status = XTmrCtr_Initialize(&pwm_tmr, XPAR_AXI_TIMER_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Configure the timer for PWM Mode
	// Generate Output, and Down Counting (easier for duty cycle)
	XTmrCtr_SetOptions(&pwm_tmr, 0, XTC_EXT_COMPARE_OPTION | XTC_DOWN_COUNT_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetOptions(&pwm_tmr, 1, XTC_EXT_COMPARE_OPTION | XTC_DOWN_COUNT_OPTION | XTC_AUTO_RELOAD_OPTION);

	// Set the Period (Frequency) in the first register (TLR0)
	// We match the sampling frequency: 2267 ticks
	// Side Note: we can decrease 2267 to a smaller number to increase the amount of 'pwm cycles' in one sampling cycle; this leads to a smoother signal because of analog filtering
	// think of channel 0 of the pwm_tmr as modifying the "Auto Reload Register (ARR)" of STM32 timers
	XTmrCtr_SetResetValue(&pwm_tmr, 0, 6000);

	// Set the Duty Cycle (High Time) in the second register (TLR1)
	// Start with 50% duty cycle (silence)
	// think of channel 1 of the pwm_tmr as modifying the "Capture Compare Register (CCR)" of STM32 timers
	XTmrCtr_SetResetValue(&pwm_tmr, 1, 6000 / 2);

	// This function sets the specific bits in the Control Status Register to turn on PWM
	XTmrCtr_PwmEnable(&pwm_tmr);

	// Start the PWM generation
	XTmrCtr_Start(&pwm_tmr, 0);

	xil_printf("PWM Timer successfully initialized!\r\n");

	return XST_SUCCESS;
}
