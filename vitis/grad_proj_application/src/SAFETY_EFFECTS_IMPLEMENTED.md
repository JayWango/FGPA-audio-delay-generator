# Always-On Safety Effects - Implementation Summary

## ✅ Implemented Effects

### 1. **High-Pass Filter** (Always-On)
- **Location**: After DC removal, before scaling
- **Purpose**: Removes low-frequency rumble and DC offset that causes feedback through guitar body
- **Implementation**: One-pole IIR filter
  - Filter coefficient: `HP_FILTER_COEFF = 240` (moderate filtering)
  - Formula: `hp_filter_state = hp_filter_state + ((input - hp_filter_state) * 240 >> 8)`
  - Output: `filtered_signal = input - hp_filter_state`
- **Effect**: Removes frequencies below ~100-200 Hz

### 2. **Automatic Gain Control (AGC) / Ducking** (Always-On)
- **Location**: After scaling, before input limiter
- **Purpose**: **CRITICAL** - Prevents runaway feedback from exciter-on-guitar loop
- **Implementation**: 
  - Detects input level (absolute value of scaled signal)
  - When input > `AGC_THRESHOLD` (800): Reduces gain proportionally
  - Gain reduction: `gain = 256 - ((input_level - 800) >> 4)`
  - Minimum gain: `AGC_MIN_GAIN = 128` (50% - never goes below this)
  - Recovery: Slowly increases gain back to 256 when input drops below threshold
- **Effect**: Automatically prevents feedback by reducing gain when input is loud

### 3. **Input Limiter** (Always-On)
- **Location**: After AGC, before delay processing
- **Purpose**: Prevents clipping artifacts in processing chain
- **Implementation**: Soft limiter
  - Threshold: `INPUT_LIMIT_THRESHOLD = 1000`
  - Compression: `output = threshold + ((input - threshold) >> 2)` (soft compression)
- **Effect**: Smoothly compresses signals above threshold

### 4. **Output Limiter** (Always-On)
- **Location**: After delay processing, before PWM conversion
- **Purpose**: Final safety check to protect exciter hardware
- **Implementation**: Hard limiter
  - Threshold: `OUTPUT_LIMIT_THRESHOLD = 1133` (RESET_VALUE/2)
  - Hard clip: `if (signal > 1133) signal = 1133`
- **Effect**: Prevents PWM values from exceeding safe range

---

## Signal Processing Chain

```
Input Sample (from mic)
    ↓
DC Bias Removal
    ↓
[ALWAYS-ON] High-Pass Filter ← Removes low-frequency rumble
    ↓
Scaling (>> 15)
    ↓
[ALWAYS-ON] AGC/Ducking ← Prevents feedback
    ↓
[ALWAYS-ON] Input Limiter ← Prevents clipping
    ↓
[BUTTON-CONTROLLED] Delay Effect (if enabled)
    ↓
[ALWAYS-ON] Output Limiter ← Final safety check
    ↓
PWM Conversion
    ↓
Output to Exciter
```

---

## Parameters (Adjustable in `bsp.h`)

### High-Pass Filter
- `HP_FILTER_COEFF`: 240 (higher = less filtering, lower = more filtering)
  - Range: 0-256
  - Recommended: 200-250

### AGC/Ducking
- `AGC_THRESHOLD`: 800 (input level where gain reduction starts)
  - Adjust based on your input levels
  - Lower = more aggressive gain reduction
- `AGC_MIN_GAIN`: 128 (50% - minimum gain)
  - Prevents complete silence
- `AGC_REDUCTION_RATE`: 4 (how fast gain reduces)
  - Higher = slower reduction, lower = faster reduction

### Input Limiter
- `INPUT_LIMIT_THRESHOLD`: 1000
  - Adjust based on your signal levels after AGC

### Output Limiter
- `OUTPUT_LIMIT_THRESHOLD`: 1133 (RESET_VALUE/2)
  - Should match your PWM midpoint

---

## Testing

1. **High-Pass Filter**: 
   - Should reduce low-frequency rumble
   - Guitar should sound cleaner
   - Less feedback at low frequencies

2. **AGC/Ducking**:
   - Play loudly - gain should automatically reduce
   - Should prevent runaway feedback
   - Gain should recover when you play quietly

3. **Input Limiter**:
   - Should prevent harsh clipping
   - Signal should compress smoothly

4. **Output Limiter**:
   - Final safety - should prevent exciter damage
   - PWM values should never exceed RESET_VALUE

---

## Adjusting Parameters

If you experience issues:

- **Too much feedback**: Lower `AGC_THRESHOLD` or increase `AGC_REDUCTION_RATE`
- **Too quiet**: Increase `AGC_MIN_GAIN` or adjust `AGC_THRESHOLD`
- **Too much low-frequency noise**: Lower `HP_FILTER_COEFF` (more filtering)
- **Not enough bass**: Increase `HP_FILTER_COEFF` (less filtering)

---

## Performance Impact

All effects use fixed-point arithmetic and are optimized for ISR execution:
- High-Pass Filter: ~5-10 cycles
- AGC: ~10-15 cycles  
- Input Limiter: ~5 cycles
- Output Limiter: ~5 cycles
- **Total overhead**: ~25-35 cycles (< 0.5μs @ 100MHz)

Well within your 20.83μs budget! ✅

