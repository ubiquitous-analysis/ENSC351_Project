#include "hit_detector.h"
#include "accelerometer.h"
#include <Arduino.h>
#include <math.h>

// calibrate the baseline
#define CALIBRATION_PERIOD_MS       5 // time in ms between samples  
#define CALIBRATION_SAMPLES         50 // number of samples taken to form baseline
#define BASELINE_UPDATE_THRESHOLD   0.15 // tolerance from baseline it has to be to affect avg
#define BASELINE_ALPHA              0.001 // how much its weighted against the avg

// threshold for detecting hits
#define HIT_THRESHOLD_G             0.3 // threshold for what delta is a 'hit'
#define QUIET_THRESHOLD             0.05 // threshold for delta is considered within baseline
#define COOLDOWN_MS                 100 // time in ms that has to pass between detected hits

// initialize local variables
static double baseline = 1.0;
static bool armed = true;
static unsigned long lastHitMs = 0;

// helper function that calculates the magnitude (abs position)
// of the accelerometer 
static double magnitude(const accelerometer_sample_t *s) {
    return sqrt(s->x_g*s->x_g + s->y_g*s->y_g + s->z_g*s->z_g);
}

// initalize ADC, calibrate  baseline from samples
void hitDetector_init(void) {
    accelerometer_init();
    
    accelerometer_sample_t s;
    double sum = 0;
    
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        if (accelerometer_getSample(&s)) {
            sum += magnitude(&s);
        }
        delay(CALIBRATION_PERIOD_MS);
    }

    baseline = sum / CALIBRATION_SAMPLES;
    lastHitMs = millis() - COOLDOWN_MS; // do this so cooldown check passes immediately
    armed = true; // ensures armed
    // hitCount = 0;
}

// flow for detecting hits and updating baseline 
bool hitDetector_scan(bool *isHit) {
    accelerometer_sample_t s;

    // checks if a sample can be fetched from accelerometer
    // 1 = successful, 0 = failed (checks for 0)
    if (!accelerometer_getSample(&s)) {
        return false;
    }

    double mag = magnitude(&s);
    double delta = fabs(mag - baseline); // change in mag from baseline
    unsigned long now = millis();

    // check if change is small enough to update baseline
    // continuously calibrate baseline
    if (delta < BASELINE_UPDATE_THRESHOLD) {
        baseline += BASELINE_ALPHA * (mag - baseline);
    }

    // rearm if cooldown has passed + sensor is below threshold for no activity
    if (!armed) {
        if (((now - lastHitMs) >= COOLDOWN_MS) && (delta < QUIET_THRESHOLD)) {
            armed = true;
        }
        return false;
    }

    // check if a hit is detected
    if (armed && delta > HIT_THRESHOLD_G) {
        // check if the disturbance is greater than our hit threshold
        armed = false;
        lastHitMs = now;
        Serial.printf("HIT DETECTED: delta = %.2fg\n", delta);
        if (isHit != nullptr) { // double checks whether isHit traces to valid memory block
            *isHit = true;
        }
        return true; // hit occured
    }
    return false; // hit did not occur
}
