#include <cmath>
#include <limits>

// ================
// COMPILE-TIME RNG
// ================

#define M_PI           3.14159265358979323846  /* pi */

struct Mutation {
private:
    // common types
    typedef unsigned long long ull;
    typedef unsigned int ui;

    // arithmetic simplification functions
    static constexpr ull sq(ull x) { return x * x; }
    static constexpr ull sm(ull x) { return sq(x) + x; }
    static constexpr ull sh(ull x) { return (x >> 32) | (x << 32); }

public:
    // normal prng's are hard to use here, since we can't easily modify our state
    // we need to use a counter-based rng, to use __COUNTER__ as our state instead
    // https://en.wikipedia.org/wiki/Counter-based_random_number_generator_(CBRNG)
    // we use Widynski's Squares method to achieve this: https://arxiv.org/abs/2004.06278
    static constexpr ui Widynski_Squares(ull count, ull seed) {
        unsigned long long cs = (count + 1) * seed;
        return (sq(sh(sq(sh(sm(cs))) + cs + seed)) + cs) >> 32;
    }

    // we define our seed based off of the __DATE__ and __TIME__ macros
    // this allows us to have different compile-time seed values
    static constexpr ull Day =
        (__DATE__[5] - '0') +
        (__DATE__[4] == ' ' ? 0 : __DATE__[4] - '0') * 10;

    static constexpr ull Month =
        (__DATE__[1] == 'a' && __DATE__[2] == 'n') * 1 +
        (__DATE__[2] == 'b') * 2 +
        (__DATE__[1] == 'a' && __DATE__[2] == 'r') * 3 +
        (__DATE__[1] == 'p' && __DATE__[2] == 'r') * 4 +
        (__DATE__[2] == 'y') * 5 +
        (__DATE__[1] == 'u' && __DATE__[2] == 'n') * 6 +
        (__DATE__[2] == 'l') * 7 +
        (__DATE__[2] == 'g') * 8 +
        (__DATE__[2] == 'p') * 9 +
        (__DATE__[2] == 't') * 10 +
        (__DATE__[2] == 'v') * 11 +
        (__DATE__[2] == 'c') * 12;

    static constexpr ull Year =
        (__DATE__[9] - '0') +
        (__DATE__[10] - '0') * 10;

    static constexpr ull Time =
        (__TIME__[0] - '0') * 1 +
        (__TIME__[1] - '0') * 10 +
        (__TIME__[3] - '0') * 100 +
        (__TIME__[4] - '0') * 1000 +
        (__TIME__[6] - '0') * 10000 +
        (__TIME__[7] - '0') * 100000;

#ifndef __Mutation_RANDOM_SEED__
    static constexpr ull Seed =
        Time +
        100000ll * Day +
        10000000ll * Month +
        1000000000ll * Year;
#else
    static constexpr ull Seed = __Mutation_RANDOM_SEED__;
#endif
};

// =====================
// MutationMORPHIC FUNCTIONS
// =====================

// various random types
#define Mutation_uint() (Mutation::Widynski_Squares(__COUNTER__, Mutation::Seed))
#define Mutation_int() ((int)Mutation_uint())
#define Mutation_ull() (((unsigned long long)Mutation_int() << 32) ^ Mutation_int())
#define Mutation_ll() ((long long)Mutation_ull())
#define Mutation_float() (static_cast<float>(Mutation_uint()) / static_cast<float>(UINT_MAX))
#define Mutation_double() (static_cast<double>(Mutation_ull()) / static_cast<double>(ULLONG_MAX))

// random number modulo max
#define Mutation_random(max) (Mutation_uint() % max)

// random no-ops, inserts junk code
#define Mutation_junk() { \
    int chance = Mutation_random(21); \
    if (chance == 0) { volatile int value = Mutation_random(10000); } \
    if (chance == 1) { volatile float value = Mutation_random(1000); } \
    if (chance == 2) { volatile double value = Mutation_random(1000); } \
    if (chance == 3) { volatile char value = Mutation_random(100000); } \
    if (chance == 4) { volatile int v[4] = {Mutation_random(1000), Mutation_random(1000), Mutation_random(1000), Mutation_random(1000)}; } \
    if (chance == 5) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] + v[2]; } \
    if (chance == 6) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] * v[2]; } \
    if (chance == 7) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] | v[2]; } \
    if (chance == 8) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] ^ v[2]; } \
    if (chance == 9) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] & v[2]; } \
    if (chance == 10) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] - v[2]; } \
    if (chance == 11) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[1] / (v[2] + 1); } \
    if (chance == 12) { volatile int v[2] = {Mutation_random(10000), Mutation_random(10000)}; volatile int vo = v[2] % (v[1] + 1); } \
    if (chance == 13) { volatile int v1 = Mutation_random(10000), v2 = v1 + Mutation_random(10000); } \
    if (chance == 14) { volatile int v1 = Mutation_random(10000), v2 = v1 * Mutation_random(10000); } \
    if (chance == 15) { volatile int v1 = Mutation_random(10000), v2 = v1 | Mutation_random(10000); } \
    if (chance == 16) { volatile int v1 = Mutation_random(10000), v2 = v1 ^ Mutation_random(10000); } \
    if (chance == 17) { volatile int v1 = Mutation_random(10000), v2 = v1 & Mutation_random(10000); } \
    if (chance == 18) { volatile int v1 = Mutation_random(10000), v2 = v1 - Mutation_random(10000); } \
    if (chance == 19) { volatile int v1 = Mutation_random(10000), v2 = v1 / (Mutation_random(10000) + 1); } \
    if (chance == 20) { volatile int v1 = Mutation_random(10000), v2 = v1 % (Mutation_random(10000) + 1); } \
}

// random order of operations for two functions
#define Mutation_random_order(f1,f2) { \
    int chance = Mutation_random(2); \
    if (chance == 0) { f1; f2; } \
    else { f2; f1; } \
}

// every `c` calls, on average the function `f` will only get executed once
#define Mutation_random_chance(c,f) { \
    int chance = Mutation_random(c); \
    if (chance == 0) { f; } \
}

// random normal distribution
#define Mutation_normal(sigma,mu) (Mutation::BoxMuller(Mutation_double(),Mutation_double(),sigma,mu))