#pragma once
#include <array>

template<typename t, t v>
struct constant_holder {
	enum class val_holder : t {
		val = v
	};
};

#define constant(value) ((decltype(value))constant_holder<decltype(value), value>::val_holder::val)

constexpr uint32_t seed = 0x45C3370D;
constexpr uint32_t prime = 0x1000193;

inline uint32_t fnv1a_rt(const char* key) {
	auto hash = seed;

#ifdef RELEASE
	hash ^= __readfsdword(0x20);
#endif

	const char* data = const_cast<char*>(key);

	for (int i = 0; i < strlen(key); i++) {
		hash = hash ^ static_cast<uint8_t>(data[i]);
		hash *= prime;
	}

	return hash;
}

inline uint32_t fnv1a_fl_rt(const char* key, uint32_t length) {
	auto hash = seed;

#ifdef RELEASE
	hash ^= __readfsdword(0x20);
#endif

	const char* data = const_cast<char*>(key);

	for (int i = 0; i < length; i++) {
		hash = hash ^ static_cast<uint8_t>(data[i]);
		hash *= prime;
	}

	return hash;
}

constexpr uint32_t fnv1a_ct(const char* str, uint32_t value = seed) noexcept { return !*str ? value : fnv1a_ct(str + 1, static_cast<unsigned>(1ull * (value ^ static_cast<uint8_t>(*str)) * prime)); }

#define fnv1a(s) constant(fnv1a_ct(s))