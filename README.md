# Derivs_Limiter

This library can be used to limit the first and second derivative of a variable as it approaches a target value.

It's easiest to think of in terms of position, velocity, and acceleration. 
If used with a servo, for example, the servo will smoothly move to a target value with a trapezoidal velocity profile.

The formula in this program supports being run at uneven intervals, and allows for editing the target, position, and velocity while it runs since it doesn't rely on calculating a profile ahead of time.

### Usage:

todo

### Notes:

todo

During the part of the profile where velocity is decreased, the acceleration is a bit "spiky."


Here are some graphs created with [this sketch](https://gist.github.com/joshua-8/3209f2f400a0e68dead911b8743fc5f0) made with [Processing](https://processing.org/) program which uses the same formula as I made for this library.

![position s curve, velocity trapezoidal curve and accel graph](https://github.com/joshua-8/Derivs_Limiter/blob/master/docs/graphs.png)

