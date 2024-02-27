#ifndef IS_IOKIT_INCLUDED

#define IS_IOKIT_INCLUDED

#define PULLUP 1
#define OPEN 0

class Output {
 public:
  Output(int pin);

  double operator=(double val);
  int operator<<=(int val);

  byte raw(void);

  operator double();

 private:
  int _pinNumber;
  byte _pinStatus;

  void set(bool val);
  void setPWM(int val);

#ifdef _STM32_DEF_
  bool _pwmExp = false;
#endif
};

class Input {
 public:
  Input(int pin);
  Input(int pin, bool pullup);

  bool digital(void);
  double analog(void);
  int raw(void);

  operator bool();

 private:
  int _pinNumber;
  byte _pinStatus;
};

#endif