#pragma once
#include "ImageUtility.h"
namespace ImageUtility {
	template <typename T>
	Image3D<T>* CreateMask(Image3D<T> * image) {
		Image3D<T>* mask = new Image3D<T>(*image);
		Thresholding<T>(mask, -1024, -400);

		/*
		TODO :
		Connected Component Labeling
		Create Largest Set Mask
		(FOREGROUND, BACKGROUND)
		*/

		return mask;
	}

	template <typename T>
	Image3D<T>* Thresholding(Image3D<T> * image, T minimum, T maximum) {
#pragma omp parallel for
		for (int i = 0; i < image->getBufferSize(); i++) {
			auto value = image->getBuffer()[i];
			if (value >= minimum  && value <= maximum) {
				image->getBuffer()[i] = FOREGROUND;
			}
			else {
				image->getBuffer()[i] = BACKGROUND;
			}
		}
	}

	template <typename T>
	void FindEdge(Image3D<T> * image) {
		const int neighborX[] = { -1, 0, 1, 0 };
		const int neighborY[] = { 0, -1, 0, 1 };

#pragma omp parallel for
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				for (int x = 0; x < image->getWidth(); x++) {
					int index = image->get3DIndex(x, y, z);
					if (image->getBuffer()[index] != BACKGROUND) {
						for (int i = 0; i < 4; i++) {
							if (image->isValidIndex(x + neighborX[i], y + neighborY[i], z)) {
								int neighborIndex = image->get3DIndex(x + neighborX[i], y + neighborY[i], z);
								if (image->getBuffer()[neighborIndex] == BACKGROUND) {
									image->getBuffer()[index] = EDGE;
								}
							}
						}
					}
				}
			}
		}


#pragma omp parallel for
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				for (int x = 0; x < image->getWidth(); x++) {
					int index = image->get3DIndex(x, y, z);
					if (image->getBuffer()[index] == FOREGROUND) {
						image->getBuffer()[index] = BACKGROUND;
					}
				}
			}
		}
	}

	template <typename T>
	int CalculateDistance(Image3D<T> * distanceMap, Image3D<T> * binaryMask) {
		std::vector<int> planeSum(binaryMask->getDepth(), 0);

#pragma omp parallel for
		for (int z = 0; z < binaryMask->getDepth(); z++) {
			for (int y = 0; y < binaryMask->getHeight(); y++) {
				for (int x = 0; x < binaryMask->getWidth(); x++) {
					int index = binaryMask->get3DIndex(x, y, z);
					if (binaryMask->getBuffer()[index] == EDGE && distanceMap->isValidIndex(x, y, z)) {
						planeSum[z] += distanceMap->getBuffer()[distanceMap->get3DIndex(x, y, z)];
					}
				}
			}
		}

		int sum = 0;
		for (int partialSum : planeSum) {
			sum += partialSum;
		}

		return sum;
	}

	template <typename T>
	Image3D<T> * CalculateSubtractImage(Image3D<T> * lhs, Image3D<T> * rhs) {
		Image3D<T>* subtractedImage = new Image3D<T>(*lhs);

		int width = std::min(lhs->getWidth(), rhs->getWidth());
		int height = std::min(lhs->getHeight(), rhs->getHeight());
		int depth = std::min(lhs->getDepth(), rhs->getDepth());

#pragma omp parallel for
		for (int z = 0; z < depth; z++) {
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					subtractedImage->getBuffer()[subtractedImage->get3DIndex(x, y, z)] -= rhs->getBuffer()[rhs->get3DIndex(x, y, z)];
				}
			}
		}

		return subtractedImage;
	}
}