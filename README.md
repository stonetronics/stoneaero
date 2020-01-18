# stoneaero
Flight controller + transmitter hardware using Arduino Nano, NRF24L01, MPU6050, etc


basic transmitter/receiver code is contained in these folders:

* transmitter
  * basic joystick operation
  * throttle on potentiometer

* receiver
  * basic servo signal forwarding from rf module
  * automatic throttle cut and circular descent when there is no signal received for 1.5s



more advanced functions are contained in receevur/trunsmeetur

* hw
  * eagle files for both pcbs
  * receevur
    * ATmega328p
    * 7 servo channels
    * 3S lipo voltage measurement
    * current measurement
    * MPU9250/MPU6050 header
    * NRF24L01+PA+LNA module
  * trunsmeetur
    * Arduino Mega
    * 16x2 generic lcd
    * buttons for lcd menu
    * NRF24L01+PA+LNA module
    * pinheaders for connecting joysticks, switches etc
    * designed to replace the electronics on a "modelcraft N-6" transmitter

* receevur
  * servo signals
    * airbrake mode for ailerons
  * current/voltage reporting

* trunsmeetur
  * lcd display
    * current/voltage report display
    * joystick settings
  * controls for servo signals

feel free to edit

