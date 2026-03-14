class Period {
    public:
        uint32_t duration = 0;
        uint16_t rms = 0;

        explicit Period() {};

        explicit Period(const uint32_t duration, const uint16_t rms) : duration(duration), rms(rms) {};
};