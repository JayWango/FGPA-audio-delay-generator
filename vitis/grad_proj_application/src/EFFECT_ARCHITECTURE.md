# Effect Processing Architecture

## Signal Processing Chain

```
Input Signal
    ↓
[ALWAYS-ON] High-Pass Filter (removes low-frequency rumble)
    ↓
[ALWAYS-ON] Automatic Gain Control / Ducking (prevents feedback)
    ↓
[ALWAYS-ON] Input Limiter (prevents clipping on input)
    ↓
[BUTTON-CONTROLLED] Creative Effects (Delay, Chorus, Tremolo, Reverb)
    ↓
[BUTTON-CONTROLLED] Compressor (optional, can be always-on)
    ↓
[ALWAYS-ON] Output Limiter (final safety check)
    ↓
Output Signal
```

---

## 🔴 **ALWAYS-ON Effects** (Critical for System Stability)

These effects **MUST** always be active - they prevent feedback and protect your system.

### 1. **High-Pass Filter on Input** ✅ ALWAYS-ON
- **Purpose**: Removes low-frequency rumble and DC offset
- **Why Always-On**: Low frequencies cause unwanted vibrations and feedback through guitar body
- **User Control**: None needed (set and forget)
- **Implementation**: Simple one-pole filter
- **Position**: First in chain (right after DC removal)

### 2. **Automatic Gain Control (AGC) / Ducking** ✅ ALWAYS-ON
- **Purpose**: Automatically reduces gain when input is loud to prevent feedback
- **Why Always-On**: **CRITICAL** - Without this, the exciter-on-guitar setup will feedback uncontrollably
- **User Control**: None needed (automatic)
- **Implementation**: Envelope follower + gain reduction
- **Position**: Early in chain (after high-pass filter)

### 3. **Input Limiter** ✅ ALWAYS-ON
- **Purpose**: Prevents input signal from exceeding safe levels
- **Why Always-On**: Protects downstream processing from clipping
- **User Control**: None needed
- **Implementation**: Hard/soft clipping
- **Position**: After AGC, before creative effects

### 4. **Output Limiter** ✅ ALWAYS-ON
- **Purpose**: Final safety check before PWM output
- **Why Always-On**: Prevents damage to exciter and clipping artifacts
- **User Control**: None needed
- **Implementation**: Hard clipping to PWM range
- **Position**: Last in chain (before PWM conversion)

---

## 🟢 **BUTTON-CONTROLLED Effects** (Creative Effects)

These effects are **toggleable** via buttons and **adjustable** via encoder.

### **Button 0: Delay** ⏱️
- **Effect**: Adjustable delay/echo
- **Encoder**: Delay time (100-4000 samples)
- **Position**: In creative effects section

### **Button 1: Chorus** 🌊
- **Effect**: Short delay with modulation
- **Encoder**: Modulation rate (0.1-5 Hz)
- **Position**: In creative effects section

### **Button 2: Tremolo** 📊
- **Effect**: Volume modulation
- **Encoder**: Modulation rate (0.5-10 Hz)
- **Position**: In creative effects section

### **Button 3: Reverb** 🏛️
- **Effect**: Multiple delay taps
- **Encoder**: Reverb mix (0-100%)
- **Position**: In creative effects section

### **Button 4: Compressor** 🎚️ (OR Gate)
- **Option A - Compressor**: Dynamic range compression
  - **Encoder**: Compression ratio (1:1 to 4:1)
  - **Why Toggleable**: User may want uncompressed sound for some styles
  - **Default**: ON (recommended)
  
- **Option B - Gate**: Noise gate
  - **Encoder**: Gate threshold
  - **Why Toggleable**: User may want to hear natural decay
  - **Default**: ON (recommended)

**Recommendation**: Use **Compressor** on Button 4, but default it to ON.

---

## Detailed Signal Processing Chain

### Stage 1: Input Processing (Always-On)
```c
// 1. Read input
int32_t raw_input = read_microphone();

// 2. Remove DC offset (already in your code)
int32_t audio_signal = raw_input - dc_bias;

// 3. High-Pass Filter (ALWAYS-ON)
static int32_t hp_filter_state = 0;
int32_t hp_coeff = 240; // Filter coefficient
hp_filter_state = hp_filter_state + ((audio_signal - hp_filter_state) * hp_coeff >> 8);
int32_t filtered_signal = audio_signal - hp_filter_state;

// 4. Scale signal
int32_t scaled_signal = filtered_signal >> 15;

// 5. AGC / Ducking (ALWAYS-ON)
int32_t input_level = (scaled_signal < 0) ? -scaled_signal : scaled_signal;
int32_t max_input = 800; // Threshold
int32_t gain = 256; // Full gain
if (input_level > max_input) {
    gain = 256 - ((input_level - max_input) >> 2);
    if (gain < 128) gain = 128; // Minimum 50%
}
scaled_signal = (scaled_signal * gain) >> 8;

// 6. Input Limiter (ALWAYS-ON)
int32_t input_threshold = 1000;
if (scaled_signal > input_threshold) {
    scaled_signal = input_threshold + ((scaled_signal - input_threshold) >> 2);
} else if (scaled_signal < -input_threshold) {
    scaled_signal = -input_threshold + ((scaled_signal + input_threshold) >> 2);
}
```

### Stage 2: Creative Effects (Button-Controlled)
```c
int32_t processed_signal = scaled_signal;

// Apply creative effects based on button states
if (effect_enabled[0]) { // Delay
    processed_signal = apply_delay(processed_signal);
}

if (effect_enabled[1]) { // Chorus
    processed_signal = apply_chorus(processed_signal);
}

if (effect_enabled[2]) { // Tremolo
    processed_signal = apply_tremolo(processed_signal);
}

if (effect_enabled[3]) { // Reverb
    processed_signal = apply_reverb(processed_signal);
}
```

### Stage 3: Optional Effects (Button-Controlled)
```c
if (effect_enabled[4]) { // Compressor
    processed_signal = apply_compressor(processed_signal);
}
```

### Stage 4: Output Processing (Always-On)
```c
// Output Limiter (ALWAYS-ON)
int32_t output_threshold = 1133; // Max PWM value
if (processed_signal > output_threshold) {
    processed_signal = output_threshold;
} else if (processed_signal < -output_threshold) {
    processed_signal = -output_threshold;
}

// Convert to PWM
int32_t pwm_sample = processed_signal + (RESET_VALUE / 2);

// Final safety clip
if (pwm_sample < 0) pwm_sample = 0;
if (pwm_sample > RESET_VALUE) pwm_sample = RESET_VALUE;
```

---

## Why This Architecture?

### Always-On Effects Are Critical Because:

1. **High-Pass Filter**:
   - Removes low-frequency rumble that causes feedback
   - Removes DC offset that can cause problems
   - No musical reason to disable it

2. **AGC / Ducking**:
   - **ESSENTIAL** for exciter-on-guitar setup
   - Prevents runaway feedback
   - Automatic - user shouldn't need to control it
   - System won't work without it

3. **Input Limiter**:
   - Protects downstream processing
   - Prevents clipping artifacts
   - No musical reason to disable it

4. **Output Limiter**:
   - Protects exciter hardware
   - Prevents PWM clipping
   - Final safety check

### Button-Controlled Effects Are Creative Because:

1. **Delay, Chorus, Tremolo, Reverb**:
   - Musical choices
   - User may want them on/off
   - Adjustable parameters add value

2. **Compressor**:
   - Can be toggleable (user preference)
   - But should default to ON
   - Adjustable ratio is useful

---

## Updated Button Assignments

Given this architecture:

### **Button 0: Delay** ⏱️
- Toggle delay effect on/off
- Encoder adjusts delay time

### **Button 1: Chorus** 🌊
- Toggle chorus effect on/off
- Encoder adjusts modulation rate

### **Button 2: Tremolo** 📊
- Toggle tremolo effect on/off
- Encoder adjusts modulation rate

### **Button 3: Reverb** 🏛️
- Toggle reverb effect on/off
- Encoder adjusts reverb mix

### **Button 4: Compressor** 🎚️
- Toggle compressor on/off (default: ON)
- Encoder adjusts compression ratio

---

## Implementation Priority

### Phase 1: Always-On Effects (Critical)
1. ✅ High-Pass Filter
2. ✅ AGC / Ducking
3. ✅ Input Limiter
4. ✅ Output Limiter

**These MUST be implemented first** - system won't work properly without them.

### Phase 2: Button-Controlled Effects
1. Delay (already implemented)
2. Compressor
3. Chorus
4. Tremolo
5. Reverb

---

## Summary

**Always-On (4 effects)**:
- High-Pass Filter
- AGC / Ducking
- Input Limiter
- Output Limiter

**Button-Controlled (5 effects)**:
- Delay (Button 0)
- Chorus (Button 1)
- Tremolo (Button 2)
- Reverb (Button 3)
- Compressor (Button 4, default ON)

**Total**: 9 effects (4 always-on, 5 toggleable)

This architecture ensures system stability while giving users creative control!

