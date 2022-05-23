#ifndef ENGINEPROTOTYPE_EASINGFUNCTIONS_H
#define ENGINEPROTOTYPE_EASINGFUNCTIONS_H

// Standard lerp function
float lerp(const float& startingValue, const float& targetValue, const float& time);
// Sine functions
float easeInSine(const float& time);
float easeOutSine(const float& time);
float easeInOutSine(const float& time);
//Quadratic functions
float easeInQuadratic(const float& time);
float easeOutQuadratic(const float& time);
float easeInOutQuadratic(const float& time);
//Cubic functions
float easeInCubic(const float& time);
float easeOutCubic(const float& time);
float easeInOutCubic(const float& time);
//Quart functions
float easeInQuart(const float& time);
float easeOutQuart(const float& time);
float easeInOutQuart(const float& time);
//Quint functions
float easeInQuint(const float& time);
float easeOutQuint(const float& time);
float easeInOutQuint(const float& time);
//Exponential functions
float easeInExponential(const float& time);
float easeOutExponential(const float& time);
float easeInOutExponential(const float& time);
//Circular functions
float easeInCircular(const float& time);
float easeOutCircular(const float& time);
float easeInOutCircular(const float& time);
//Back functions
float easeInBack(const float& time);
float easeOutBack(const float& time);
float easeInOutBack(const float& time);
//Elastic functions
float easeInElastic(const float& time);
float easeOutElastic(const float& time);
float easeInOutElastic(const float& time);
//Bounce functions
float easeInBounce(const float& time);
float easeOutBounce(const float& time);
float easeInOutBounce(const float& time);

#endif //ENGINEPROTOTYPE_EASINGFUNCTIONS_H
