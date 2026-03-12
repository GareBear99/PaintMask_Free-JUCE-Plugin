#include "EuclideanRhythm.h"
std::vector<int> EuclideanRhythm::generate(int pulses, int steps) const
{
    std::vector<int> out;
    if (steps <= 0) return out;
    out.resize((size_t) steps, 0);
    if (pulses <= 0) return out;
    int bucket = 0;
    for (int i = 0; i < steps; ++i)
    {
        bucket += pulses;
        if (bucket >= steps) { bucket -= steps; out[(size_t) i] = 1; }
    }
    return out;
}
