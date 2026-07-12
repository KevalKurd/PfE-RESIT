https://github.com/KevalKurd/PfE-RESIT.git
# ADC Sensor Log Analysis

This project was completed for the UWE Bristol Programming for Engineers coursework.

## Repository

GitHub Repository:

https://github.com/KevalKurd/PfE-RESIT.git

##_ Build Instructions

### CLion

1. Open the project in CLion.
2. Allow CMake to configure automatically.
3. Place `adc_sensor_log.bin` in the project directory.
4. Run the `CW` target with the filename as the program argument.

### GCC

Compile using:

```bash
gcc main.c adc.c io.c stats.c -o CW -lm
```

Run using:

```bash
./CW adc_sensor_log.bin
```