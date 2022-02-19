#include "easingFunctions.h"
#include <math.h>

float lerp(const float &startingValue, const float &targetValue, const float &time) {
    return 0;
}

float easeInSine(const float &time) {
    return 1.0f - cos((time * M_PI) / 2.0f);
}

float easeOutSine(const float &time) {
    return sin((time * M_PI) / 2.0f);
}

float easeInOutSine(const float &time) {
    return -1.0f * (cos(M_PI * 2) - 1) / 2;
}

float easeInQuadratic(const float &time) {
    return time * time;
}

float easeOutQuadratic(const float &time) {
    return 1 - (1 - time) * (1 - time);
}

float easeInOutQuadratic(const float &time) {
    return time < 0.5 ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2;
}

float easeInCubic(const float &time) {
    return time * time * time;
}

float easeOutCubic(const float &time) {
    return 1.0f - pow(1 - time, 3);
}

float easeInOutCubic(const float &time) {
    return time < 0.5f ? 4.0f * time * time * time : 1 - pow(-2 * time + 2, 3) / 2.0f;
}

float easeInQuart(const float &time) {
    return time * time * time * time;
}

float easeOutQuart(const float &time) {
    return 1 - pow(1 - time, 4);
}

float easeInOutQuart(const float &time) {
    return time < 0.5 ? 8.0f * time * time * time * time : 1 - pow(-2.0f * time + 2.0f, 4) / 2.0f;
}

float easeInQuint(const float &time) {
    return time * time * time * time * time;
}

float easeOutQuint(const float &time) {
    return 1.0f - pow(1 - time, 5);
}

float easeInOutQuint(const float &time) {
    return time < 0.5f ? 16.0f * time * time * time * time * time : 1.0f - pow(-2.0f * time + 2.0f, 5) / 2.0f;
}

float easeInExponential(const float &time) {
    return time == 0 ? 0 : pow(2, 10.0f * time - 10.0f);
}

float easeOutExponential(const float &time) {
    return time == 1 ? 1.0f : 1.0f - pow(2, -10.0f * time);
}

float easeInOutExponential(const float &time) {
    return time == 0 ? 0 : time == 1.0f ? 1.0f : time < 0.5f ? pow(2, 20.0f * time - 10.0f) / 2.0f : (2.0f - pow(2, -20.0f * time + 10.0f)) / 2.0f;
}

float easeInCircular(const float &time) {
    return 1.0f - sqrt(1 - pow(time, 2));
}

float easeOutCircular(const float &time) {
    return sqrt(1 - pow(time - 1, 2));
}

float easeInOutCircular(const float &time) {
    return time < 0.5f ? (1.0f - sqrt(1.0f - pow(2 * time, 2))) / 2.0f : (sqrt(1 - pow(-2.0f * time + 2, 2)) + 1) / 2.0f;
}

float easeInBack(const float &time) {
    const float c1 = 1.780158;
    const float c3 = 2.7801581;
    return c3 * time * time * time - c1 * time * time;
}

float easeOutBack(const float &time) {
    const float c1 = 1.780158;
    const float c3 = 2.7801581;
    return 1.0f + c3 * pow(time - 1, 3) + c1 * pow(time - 1, 2);
}

float easeInOutBack(const float &time) {
    const float c1 = 1.780158;
    const float c2 = c1 * 1.525;
    return time < 0.5f ? (pow(2 * time, 2) * ((c2 + 1) * 2 * time - c2)) / 2.0f : (pow(2 * time - 2, 2) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2.0f;
}

float easeInElastic(const float &time) {
    const float c4 = (2.0f * M_PI) / 3.0f;
    return time == 0 ? 0 : time == 1 ? 1 : -pow(2, 10 * time - 10) * sin((time * 10 - 10.75) * c4);
}

float easeOutElastic(const float &time) {
    const float c4 = (2.0f * M_PI) / 3.0f;
    return time == 0 ? 0 : time == 1 ? 1 : pow(2, -10 * time) * sin((time * 10 - 0.75f) * c4) + 1;
}

float easeInOutElastic(const float &time) {
    const float c5 = (2.0f * M_PI) / 4.5f;
    return time == 0 ? 0 : time == 1 ? 1 : time < 0.5 ? -(pow(2, 20 * time - 10) * sin((20 * time - 11.125f) * c5)) / 2.0f : (pow(2, -20 * time + 10) * sin((20 * time - 11.125) * c5)) / 2.0f + 1.0f;
}

float easeInBounce(const float &time) {
    return 1 - easeOutBounce(1 - time);
}

float easeOutBounce(const float &time) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if(time < 1 / d1) {
        return n1 * time * time;
    } else if(time < 2 / d1) {
        return n1 * (time - (1.5 / d1)) * time + 0.75f;
    } else if(time < 2.5 / d1) {
        return n1 * (time - (2.25 / d1)) * time + 0.9375;
    } else {
        return n1 * ( time - (2.25 / d1)) * time + 0.984375;
    }
}

float easeInOutBounce(const float &time) {
    return time < 0.5 ? (1 - easeOutBounce(1 - 2 * time)) / 2 : (1 + easeOutBounce(2 * time - 1)) / 2;
}