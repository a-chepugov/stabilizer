 #ifndef RANGE_H
 #define RANGE_H

 template<typename T>
 class Range {
 public:
     const T lo;
     const T hi;
 
     constexpr Range(T lo, T hi)
       : lo(lo), hi(hi) {}
 
     bool contains(T value) const {
       return value >= lo && value <= hi;
     }
 
     T length() const {
       return hi - lo;
     }
 
     template<typename T2, typename F>
     constexpr Range<T2> map(F f) const {
       return Range<T2>(f(lo), f(hi));
     }
 };

 #endif // RANGE_H

