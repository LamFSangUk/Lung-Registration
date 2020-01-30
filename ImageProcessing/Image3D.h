#pragma once
#define NOMINMAX
#include <omp.h>
#include <string>
#include <FileHelper.h>

enum PixelType {
	BACKGROUND = -3,
	FOREGROUND,
	EDGE
};

template <typename T>
class Image3D {
public:
	Image3D(T* buffer, unsigned width, unsigned height, unsigned depth, bool isLittleEndian);
	Image3D(unsigned width, unsigned height, unsigned depth);
	Image3D(const Image3D<T>& image3d);
	~Image3D();

	unsigned getBufferSize() const;
	unsigned getBufferLength() const;
	inline bool isValidIndex(int x, int y, int z) const;
	int get3DIndex(int x, int y, int z) const;
	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned getDepth() const;
	void calculateMinMax();
	std::pair<short, short> getMinMax();
	void printImage(const std::string& path);
	// RGBA 8888
	unsigned char* getSliceAt(int z, T width, T level) const;
	void sliceInterpolate(unsigned int level);
	void clear(T value);
	inline T getInterpolatedAt(float x, float y, float z) const;
	T* getBuffer();

private:

	// TODO : Add a member state about whether it's binary image or not

	T* m_buffer;
	unsigned m_width;
	unsigned m_height;
	unsigned m_depth;
	float m_windowing;
	std::pair<T, T> m_minmax;
};

#include "Image3D.cpp"