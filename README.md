# stoneaero
Flight controller + transmitter hardware using Arduino Nano, NRF24L01, MPU6050, etc

by now, basic transmitter/receiver operation is implemented.

transmitter
-basic joystick operation
-throttle on potentiometer

receiver
-basic servo signal forwarding from rf module
-automatic throttle cut and circular descent when there is no signal received for 1.5s
