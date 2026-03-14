// Линейная интерполяция положения точки как процент от длины AB (0..1)
constexpr float spacing(float a, float b, float passed) {
    return a * (1 - passed) + b * passed;
}  