#pragma once
#include "Image3D.h"
#include <unordered_map>

namespace ImageUtility {
	// Mask generation
	template <typename T>
	Image3D<T>* CreateMask(Image3D<T>* image);
	template <typename T>
	Image3D<T>* Thresholding(Image3D<T>* image, T minimum, T maximum);
	template <typename T>
	void FindEdge(Image3D<T>* image);
	template <typename T>
	Image3D<T>* CalculateChamferDistanceMap(Image3D<T>* image, int isothetic, int diagonal, int diagonal3D);
	template <typename T>
	int CalculateDistance(Image3D<T>* distanceMap, Image3D<T>* binaryMask);
	template <typename T>
	Image3D<T>* CalculateSubtractImage(Image3D<T>* lhs, Image3D<T>* rhs);
}

#include "ImageUtility.cpp"