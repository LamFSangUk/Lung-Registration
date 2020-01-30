#include <cstdint>

#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

namespace Interpolation {
	__forceinline std::float_t LinearInterpolation(std::float_t x, std::float_t x1, std::float_t x2, std::float_t q00, std::float_t q01) {
		return ((x2 - x) / (x2 - x1)) * q00 + ((x - x1) / (x2 - x1)) * q01;
	}

	__forceinline std::float_t BiLinearInterpolation(std::float_t x, std::float_t y, std::float_t q11, std::float_t q12, std::float_t q21, std::float_t q22, std::float_t x1, std::float_t x2, std::float_t y1, std::float_t y2) {
		std::float_t r1 = LinearInterpolation(x, x1, x2, q11, q21);
		std::float_t r2 = LinearInterpolation(x, x1, x2, q12, q22);

		return LinearInterpolation(y, y1, y2, r1, r2);
	}

	__forceinline std::float_t TriLinearInterpolation(std::float_t x, std::float_t y, std::float_t z, std::float_t q000, std::float_t q001, std::float_t q010, std::float_t q011, std::float_t q100, std::float_t q101, std::float_t q110, std::float_t q111, std::float_t x1, std::float_t x2, std::float_t y1, std::float_t y2, std::float_t z1, std::float_t z2) {
		std::float_t x00 = LinearInterpolation(x, x1, x2, q000, q100);
		std::float_t x10 = LinearInterpolation(x, x1, x2, q010, q110);
		std::float_t x01 = LinearInterpolation(x, x1, x2, q001, q101);
		std::float_t x11 = LinearInterpolation(x, x1, x2, q011, q111);
		std::float_t r0 = LinearInterpolation(y, y1, y2, x00, x01);
		std::float_t r1 = LinearInterpolation(y, y1, y2, x10, x11);

		return LinearInterpolation(z, z1, z2, r0, r1);
	}

	__forceinline std::double_t LinearInterpolation(std::double_t x, std::double_t x1, std::double_t x2, std::double_t q00, std::double_t q01) {
		return ((x2 - x) / (x2 - x1)) * q00 + ((x - x1) / (x2 - x1)) * q01;
	}

	__forceinline std::double_t BiLinearInterpolation(std::double_t x, std::double_t y, std::double_t q11, std::double_t q12, std::double_t q21, std::double_t q22, std::double_t x1, std::double_t x2, std::double_t y1, std::double_t y2) {
		std::double_t r1 = LinearInterpolation(x, x1, x2, q11, q21);
		std::double_t r2 = LinearInterpolation(x, x1, x2, q12, q22);

		return LinearInterpolation(y, y1, y2, r1, r2);
	}

	__forceinline std::double_t TriLinearInterpolation(std::double_t x, std::double_t y, std::double_t z, std::double_t q000, std::double_t q001, std::double_t q010, std::double_t q011, std::double_t q100, std::double_t q101, std::double_t q110, std::double_t q111, std::double_t x1, std::double_t x2, std::double_t y1, std::double_t y2, std::double_t z1, std::double_t z2) {
		std::double_t x00 = LinearInterpolation(x, x1, x2, q000, q100);
		std::double_t x10 = LinearInterpolation(x, x1, x2, q010, q110);
		std::double_t x01 = LinearInterpolation(x, x1, x2, q001, q101);
		std::double_t x11 = LinearInterpolation(x, x1, x2, q011, q111);
		std::double_t r0 = LinearInterpolation(y, y1, y2, x00, x01);
		std::double_t r1 = LinearInterpolation(y, y1, y2, x10, x11);

		return LinearInterpolation(z, z1, z2, r0, r1);
	}
}

#endif