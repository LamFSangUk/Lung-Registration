#pragma once
namespace MathUtility {
	__forceinline float LinearInterpolation(float x, float x1, float x2, float q00, float q01) {
		return ((x2 - x) / (x2 - x1)) * q00 + ((x - x1) / (x2 - x1)) * q01;
	}

	__forceinline float BiLinearInterpolation(float x, float y, float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2) {
		float r1 = LinearInterpolation(x, x1, x2, q11, q21);
		float r2 = LinearInterpolation(x, x1, x2, q12, q22);

		return LinearInterpolation(y, y1, y2, r1, r2);
	}

	__forceinline float TriLinearInterpolation(float x, float y, float z, float q000, float q001, float q010, float q011, float q100, float q101, float q110, float q111, float x1, float x2, float y1, float y2, float z1, float z2) {
		float x00 = LinearInterpolation(x, x1, x2, q000, q100);
		float x10 = LinearInterpolation(x, x1, x2, q010, q110);
		float x01 = LinearInterpolation(x, x1, x2, q001, q101);
		float x11 = LinearInterpolation(x, x1, x2, q011, q111);
		float r0 = LinearInterpolation(y, y1, y2, x00, x01);
		float r1 = LinearInterpolation(y, y1, y2, x10, x11);

		return LinearInterpolation(z, z1, z2, r0, r1);
	}
}