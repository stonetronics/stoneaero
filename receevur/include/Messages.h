typedef struct {
  uint8_t mode;
  int8_t throttle;
  int8_t yaw;
  int8_t pitch;
  int8_t roll;
  uint8_t controlByte;
} Command;

typedef struct {
  int16_t current;
  int16_t cellVoltage[3];
} Report;
