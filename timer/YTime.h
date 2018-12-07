#ifndef __YTIME_H__
#define __YTIME_H__

struct Time {
  unsigned hours, minutes, seconds;


  Time(unsigned long mil) {
    const unsigned secs_hour = 60 * 60;
    unsigned long s = mil / 1000;
    hours   = s / secs_hour;
    s       = s % secs_hour;
    minutes = s / 60;
    seconds = s % 60;
  }

  Time()
    : hours(0), minutes(0), seconds(0) {}

  Time(const Time &y)
    : hours(y.hours), minutes(y.minutes), seconds(y.seconds) {}

  bool operator == (Time y) {
    return y.seconds == seconds &&
           y.minutes == minutes &&
           y.hours   == hours;
  }

  bool operator != (Time y) {
    return !(*this == y);
  }

  unsigned long millis(void) {
    return 1000 * (seconds + minutes * 60 + hours * 60 * 60);
  }


  // Buffer needs 9 bytes of space. Only for less than 100 hours.
  void render(char *buf) {
    const unsigned size = 2 + 1 + 2 + 1 + 2 + 1;  // HH:MM:SS\0
    snprintf(buf,size,"%02u:%02u:%02u",hours % 100,minutes,seconds);
  }
};

#endif
