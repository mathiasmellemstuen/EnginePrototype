#ifndef ENGINEPROTOTYPE_EASINGFUNCTIONS_H
#define ENGINEPROTOTYPE_EASINGFUNCTIONS_H

// Standard lerp function
float lerp(const float& startingValue, const float& targetValue, const float& time);
// Sine functions
float easeInSine(const float& startingValue, const float& targetValue, const float& time);
float easeOutSine(const float& startingValue, const float& targetValue, const float& time);
float easeInOutSine(const float& startingValue, const float& targetValue, const float& time);
//Quadratic functions
float easeInQuadratic(const float& startingValue, const float& targetValue, const float& time);
float easeOutQuadratic(const float& startingValue, const float& targetValue, const float& time);
float easeInOutQuadratic(const float& startingValue, const float& targetValue, const float& time);
//Cubic functions
float easeInCubic(const float& startingValue, const float& targetValue, const float& time);
float easeOutCubic(const float& startingValue, const float& targetValue, const float& time);
float easeInOutCubic(const float& startingValue, const float& targetValue, const float& time);
//Quart functions
float easeInQuart(const float& startingValue, const float& targetValue, const float& time);
float easeOutQuart(const float& startingValue, const float& targetValue, const float& time);
float easeInOutQuart(const float& startingValue, const float& targetValue, const float& time);
//Quint functions
float easeInQuint(const float& startingValue, const float& targetValue, const float& time);
float easeOutQuint(const float& startingValue, const float& targetValue, const float& time);
float easeInOutQuint(const float& startingValue, const float& targetValue, const float& time);
//Exponential functions
float easeInExponential(const float& startingValue, const float& targetValue, const float& time);
float easeOutExponential(const float& startingValue, const float& targetValue, const float& time);
float easeInOutExponential(const float& startingValue, const float& targetValue, const float& time);
//Circular functions
float easeInCircular(const float& startingValue, const float& targetValue, const float& time);
float easeOutCircular(const float& startingValue, const float& targetValue, const float& time);
float easeInOutCircular(const float& startingValue, const float& targetValue, const float& time);
//Back functions
float easeInBack(const float& startingValue, const float& targetValue, const float& time);
float easeOutBack(const float& startingValue, const float& targetValue, const float& time);
float easeInOutBack(const float& startingValue, const float& targetValue, const float& time);
//Elastic functions
float easeInElastic(const float& startingValue, const float& targetValue, const float& time);
float easeOutElastic(const float& startingValue, const float& targetValue, const float& time);
float easeInOutElastic(const float& startingValue, const float& targetValue, const float& time);
//Bounce functions
float easeInBounce(const float& startingValue, const float& targetValue, const float& time);
float easeOutBounce(const float& startingValue, const float& targetValue, const float& time);
float easeInOutBounce(const float& startingValue, const float& targetValue, const float& time);

#endif //ENGINEPROTOTYPE_EASINGFUNCTIONS_H
