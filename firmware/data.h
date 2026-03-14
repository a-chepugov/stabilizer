constexpr size_t DATA_SIZE = pow(2, 8);
constexpr size_t DATA_MAX_IDX = DATA_SIZE - 1;
constexpr size_t DATA_SIZE_QUATER = DATA_SIZE / 4;

uint32_t data[DATA_SIZE] = {0};
size_t head = 0;

void data_push(uint16_t value)
{
//    data[head] = value;
    data[head] = (uint32_t)value * (uint32_t)value;
    head = (head + 1) & (DATA_MAX_IDX);
}

uint16_t data_rms() {
  uint16_t count = 0;
  uint32_t sum = 0;
  for (size_t i = 0; i < DATA_SIZE; i++) {
    if (data[i] > 0) {
      count++;
//      sum += (uint32_t)data[i] * (uint32_t)data[i];
      sum += data[i];
    }
  }
  return TEST || count > DATA_SIZE_QUATER
    ? (uint16_t) (sqrt(sum / count))
    : 0
    ;
}

uint16_t data_max() {
  uint32_t max = data[0];
  for (size_t i = 1; i < DATA_SIZE; i++) {
    if (max < data[i]) {
      max = data[i];
    }
  }
//  return max;
  return sqrt(max);
}