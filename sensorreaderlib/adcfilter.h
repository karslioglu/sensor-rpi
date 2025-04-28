#ifndef ADCFILTER_H
#define ADCFILTER_H

class ADCFilter {
public:
    explicit ADCFilter(float alpha = 0.2f) : alpha(alpha), filteredValue(0.0f), initialized(false) {}

    float filter(float newValue) {
        if (!initialized) {
            filteredValue = newValue;
            initialized = true;
        } else {
            filteredValue = alpha * newValue + (1.0f - alpha) * filteredValue;
        }
        return filteredValue;
    }

private:
    float alpha;
    float filteredValue;
    bool initialized;
};

#endif // ADCFILTER_H
