#pragma once
#include "Image3D.h"
#include <cmath>
#include "MathUtility.h"
#include "ByteSwap.hpp"
#include <algorithm>
#include <fstream>

template <typename T>
Image3D<T>::Image3D(T * buffer, unsigned width, unsigned height, unsigned depth, bool isLittleEndian)
	:m_buffer(buffer),
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_minmax(std::make_pair(std::numeric_limits<T>::max(), std::numeric_limits<T>::min())) {
	if (!isLittleEndian) {
#pragma omp parallel for
		for (int i = 0; i < width*height*depth; i++) {
			m_buffer[i] = ByteSwap::swap_endian<T>(m_buffer[i]);
		}
	}
}

template <typename T>
Image3D<T>::Image3D(unsigned width, unsigned height, unsigned depth)
	:m_type(type),
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_minmax(BACKGROUND, FOREGROUND) {
	m_buffer = new char[getBufferLength()];
#pragma omp parallel for
	for (int i = 0; i < width*height*depth; i++) {
		m_buffer[i] = BACKGROUND;
	}
}

template <typename T>
Image3D<T>::Image3D(const Image3D<T>& image3d)
	:m_width(image3d.m_width),
	m_height(image3d.m_height),
	m_depth(image3d.m_depth),
	m_minmax(image3d.m_minmax) {
	m_buffer = new T[image3d.getBufferSize()];
	std::memcpy(m_buffer, image3d.m_buffer, getBufferLength());
}

template <typename T>
Image3D<T>::~Image3D() {
	if (m_buffer) {
		delete[] m_buffer;
	}
}

template <typename T>
unsigned Image3D<T>::getBufferSize() const {
	return m_width * m_height * m_depth;
}

template <typename T>
unsigned Image3D<T>::getBufferLength() const {
	return sizeof(T) * m_width * m_height * m_depth;
}

template <typename T>
inline bool Image3D<T>::isValidIndex(int x, int y, int z) const {
	return x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth;
}

template <typename T>
int Image3D<T>::get3DIndex(int x, int y, int z) const {
	return getWidth()*getHeight()*z + getWidth()*y + x;
}

template <typename T>
unsigned Image3D<T>::getWidth() const {
	return m_width;
}

template <typename T>
unsigned Image3D<T>::getHeight() const {
	return m_height;
}

template <typename T>
unsigned Image3D<T>::getDepth() const {
	return m_depth;
}

template <typename T>
void Image3D<T>::calculateMinMax() {
	omp_lock_t minWriteLock;
	omp_lock_t maxWriteLock;

	omp_init_lock(&minWriteLock);
	omp_init_lock(&maxWriteLock);

	m_minmax = std::make_pair(std::numeric_limits<T>::max(), std::numeric_limits<T>::min());

#pragma omp parallel for
	for (int i = 0; i < getBufferSize(); i++) {
		auto value = getBuffer()[i];
		if (value < m_minmax.first) {
			omp_set_lock(&minWriteLock);
			m_minmax.first = value;
			omp_unset_lock(&minWriteLock);
		}
		if (value > m_minmax.second) {
			omp_set_lock(&maxWriteLock);
			m_minmax.second = value;
			omp_unset_lock(&maxWriteLock);
		}
	}

	omp_destroy_lock(&minWriteLock);
	omp_destroy_lock(&maxWriteLock);
}

template <typename T>
std::pair<short, short> Image3D<T>::getMinMax() {
	return m_minmax;
}

template <typename T>
void Image3D<T>::printImage(const std::string & path) {
	std::ofstream ofs(path, std::ofstream::binary);
	ofs.write(reinterpret_cast<char*>(m_buffer), getBufferLength());
	ofs.close();
}

template<typename T>
unsigned char* Image3D<T>::getSliceAt(int z, T width, T level) const {
	unsigned char* newBuffer = new unsigned char[m_width * m_height * 4];

	auto windowing = [width, level](T value) {
		if (value < level - width * 0.5) {
			return 0.f;
		}
		else if (value > level + width * 0.5) {
			return 1.f;
		}
		else {
			return (float)(value - (level - width * 0.5)) / width;
		}
	};

#pragma omp parallel for
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			newBuffer[(y * m_width + x) * 4] = 255.f * windowing(m_buffer[get3DIndex(x, y, z)]);
			newBuffer[(y * m_width + x) * 4 + 1] = 255.f * windowing(m_buffer[get3DIndex(x, y, z)]);
			newBuffer[(y * m_width + x) * 4 + 2] = 255.f * windowing(m_buffer[get3DIndex(x, y, z)]);
			newBuffer[(y * m_width + x) * 4 + 3] = 255.f * windowing(m_buffer[get3DIndex(x, y, z)]);
		}
	}

	return newBuffer;
}

template<typename T>
void Image3D<T>::sliceInterpolate(unsigned int level) {
	if (level > 1) {
		T* newBuffer = new T[getBufferSize() * level];
		const float delta = 1.f / level;
#pragma omp parallel for
		for (int z = 0; z < m_depth - 1; z++) {
			for (int y = 0; y < m_height; y++) {
				for (int x = 0; x < m_width; x++) {
					for (int i = 0; i < level; i++) {
						newBuffer[get3DIndex(x, y, z * level + i)] =
							m_buffer[get3DIndex(x, y, z)] * (1 - delta * i) +
							m_buffer[get3DIndex(x, y, z + 1)] * delta * i;
					}
				}
			}
		}
		delete[] m_buffer;
		m_buffer = newBuffer;
		m_depth *= level;
	}
}

template<typename T>
void Image3D<T>::clear(T value) {
#pragma omp parallel for
	for (int i = 0; i < getBufferSize(); i++) {
		getBuffer()[i] = value;
	}
	m_minmax = std::make_pair(value, value);
}

template<typename T>
inline T Image3D<T>::getInterpolatedAt(float x, float y, float z) const {
	T val = m_buffer[get3DIndex(x, y, z)];
	return MathUtility::TriLinearInterpolation(x, y, z,
		val,
		isValidIndex(x, y + 1, z) ? m_buffer[get3DIndex(x, y + 1, z)] : val,
		isValidIndex(x, y + 1, z + 1) ? m_buffer[get3DIndex(x, y + 1, z + 1)] : val,
		isValidIndex(x + 1, y, z) ? m_buffer[get3DIndex(x + 1, y, z)] : val,
		isValidIndex(x + 1, y, z + 1) ? m_buffer[get3DIndex(x + 1, y, z + 1)] : val,
		isValidIndex(x + 1, y + 1, z) ? m_buffer[get3DIndex(x + 1, y + 1, z)] : val,
		isValidIndex(x + 1, y + 1, z) ? m_buffer[get3DIndex(x + 1, y + 1, z)] : val,
		isValidIndex(x + 1, y + 1, z + 1) ? m_buffer[get3DIndex(x + 1, y + 1, z + 1)] : val,
		std::floor(x),
		std::floor(x) + 1,
		std::floor(y),
		std::floor(y) + 1,
		std::floor(z),
		std::floor(z) + 1);
}

template<typename T>
T * Image3D<T>::getBuffer() {
	return m_buffer;
}
