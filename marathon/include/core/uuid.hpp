#pragma once

#include <cstdint>
#include <random>
#include <cmath>

namespace marathon {

//// TODO:
// Add validation to ensure no possible reusage or UUID
// Remove copying UUID, feels bad

// Universal Unique Identifier
// int64_t wrapper
// 0 : Invalid
// > 0 : from file
// < 0 : at runtime
struct UUID {
private:
    int64_t _uuid = 0;

    // Non-deterministic random number
    // Seed the pseudo number gen
    static inline std::random_device _trueRandom;

    // Psuedo random generator
    // Mersenne Twister algorithm
    // 2^19937 - 1 before repeating
	static inline std::mt19937_64 _psuedoGenerator{_trueRandom()};

    // even chance across whole UUID space
	static inline std::uniform_int_distribution<int64_t> _uniformDistributer;

public:

	UUID(bool fromFile=false)
    : _uuid(fromFile 
            ? std::abs(_uniformDistributer(_psuedoGenerator)) 
            : -std::abs(_uniformDistributer(_psuedoGenerator))) {}

	UUID(int64_t uuid)
		: _uuid(uuid) {}


    operator int64_t() const { return (int64_t)_uuid; }

};

} // marathon
