uint16_t prev = 0;
uint16_t last = 0;

// Определить является текущий момент переходом через вершину на синусоиде
boolean set_and_tick(uint16_t curr) {
    if (prev < last && last > curr) {
        prev = last;
        last = curr;
        return true;
    } else {
        prev = last;
        last = curr;
        return false;
    }
}