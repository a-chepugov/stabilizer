#ifndef TICK_H
#define TICK_H

class Tick {
public:
    uint16_t ticks = 0;
        
    uint16_t old = 0;
    uint16_t mid = 0;    

    boolean tick(uint16_t now) {
        if (old < mid && mid > now) {
            old = mid;
            mid = now;
            ticks++;
            return true;
        } else {
            old = mid;
            mid = now;
            return false;
        }
    }
};

#endif