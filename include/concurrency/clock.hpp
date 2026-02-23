#ifndef CLOCK_HPP
#define CLOCK_HPP

// Para cada semaforo, este vai ter um clock em que ele muda para verde ou
// vermelho a cada x segundos

// Como fazer:

class Clock {

private:
  int timer;
  void toggle();

public:
  Clock(int timer);
  ~Clock();

  void init();
};

#endif