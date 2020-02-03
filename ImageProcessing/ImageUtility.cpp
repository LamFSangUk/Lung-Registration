#pragma once
#include "ImageUtility.h"
#include "ConnectedComponent.h"
#include <queue>

namespace ImageUtility {
	template <typename T>
	Image3D<T>* CreateMask(Image3D<T> * image, int label = 0) {
		Image3D<T>* mask = new Image3D<T>(*image);
		Thresholding<T>(mask, -1024, -400);

		/*
		TODO :
		Connected Component Labeling
		Create Largest Set Mask
		(FOREGROUND, BACKGROUND)
		*/
		// TODO: need background regioning
 		BorderRegionGrowing<T>(mask);
		ConnectedComponentAnalyze<T>(mask, label);

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
	void BorderRegionGrowing(Image3D<T> * image) {

		// Left plane
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				int x = 0;

				RegionGrowing(image, x, y, z);
			}
		}

		// Right plane
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				int x = image->getWidth() - 1;

				RegionGrowing(image, x, y, z);
			}
		}
	}

	template <typename T>
	void RegionGrowing(Image3D<T> * image, int x, int y, int z) {

		const int neighborX[] = { -1, 0, 0, 1, 0, 0 };
		const int neighborY[] = { 0, -1, 0, 0, 1, 0 };
		const int neighborZ[] = { 0, 0, -1, 0, 0, 1 };
		
		// TODO: check cur is bg
		int index = image->get3DIndex(x, y, z);
		if (image->getBuffer()[index] == BACKGROUND) return;	// Not for Background
		image->getBuffer()[index] = BACKGROUND;

		// Queue structure for region growing
		std::queue<glm::vec3> q;

		q.push(glm::vec3(x, y, z));


		while (!q.empty()) {
			// Get the front of queue
			glm::vec3 cur = q.front();
			q.pop();

			for (int i = 0; i < 6; i++) {
				int nextX = cur.x + neighborX[i];
				int nextY = cur.y + neighborY[i];
				int nextZ = cur.z + neighborZ[i];

				if (image->isValidIndex(nextX, nextY, nextZ)) {

					int nextIndex = image->get3DIndex(nextX, nextY, nextZ);
					if (image->getBuffer()[nextIndex] == BACKGROUND) continue;	// Not for Background

					image->getBuffer()[nextIndex] = BACKGROUND;
					q.push(glm::vec3(nextX, nextY, nextZ));
				}
			}
		}
	}

	template <typename T>
	void ConnectedComponentAnalyze(Image3D<T> * image, int pre_defined_label) {

		ComponentGroup comps;

		// 4-connectivity neighbor
		const int neighborX[] = { -1, 0, 0 };
		const int neighborY[] = { 0, -1, 0 };
		const int neighborZ[] = { 0, 0, -1 };

		int currentLabelCount = 0;

		// First Pass
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				for (int x = 0; x < image->getWidth(); x++) {
					int index = image->get3DIndex(x, y, z);
					if (image->getBuffer()[index] == BACKGROUND) continue;	// Not for Background

					T minLabel = SHRT_MAX;
					int neighborCount = 0;
					T singleLabel = BACKGROUND;
					int singleLabelCount = 0;
					for (int i = 0; i < 3; i++) {	 // 4-connectivity
						if (image->isValidIndex(x + neighborX[i], y + neighborY[i], z + neighborZ[i])) {

							int neighborIndex = image->get3DIndex(x + neighborX[i], y + neighborY[i], z + neighborZ[i]);

							if (image->getBuffer()[neighborIndex] != BACKGROUND) {
								T neighborLabel = image->getBuffer()[neighborIndex];

								neighborCount++;

								// check the neighbors have single values 
								if (singleLabel != neighborLabel) {
									singleLabel = neighborLabel;
									singleLabelCount++;

									if (singleLabel < minLabel) minLabel = singleLabel;
								}
							}
						}
					}

					if (neighborCount == 0) {	// if no neighbor pixel assigned.
						comps.MakeNewComponent(++currentLabelCount);
						image->getBuffer()[index] = currentLabelCount;
					}
					else if (singleLabelCount == 1) {	// If neighbors have single label
						image->getBuffer()[index] = singleLabel;
						comps.AddNewElement(singleLabel);
					}
					else {	// diffrent label
						image->getBuffer()[index] = minLabel;
						comps.AddNewElement(minLabel);

						for (int i = 0; i < 3; i++) {	 // 4-connectivity
							if (image->isValidIndex(x + neighborX[i], y + neighborY[i], z + neighborZ[i])) {

								int neighborIndex = image->get3DIndex(x + neighborX[i], y + neighborY[i], z + neighborZ[i]);

								if (image->getBuffer()[neighborIndex] != BACKGROUND) {
									T neighborLabel = image->getBuffer()[neighborIndex];

									comps.Merge(neighborLabel, minLabel);
								}
							}
						}
					}

				}
			}
		}
		std::cout << "Pass 1 complete, comp size :" << comps.components.size() << std::endl;

		// Second Pass
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				for (int x = 0; x < image->getWidth(); x++) {
					int index = image->get3DIndex(x, y, z);

					if (image->getBuffer()[index] == BACKGROUND) continue;

					int label = image->getBuffer()[index];

					Component root = comps.Find(comps.components[label - 1]);
					image->getBuffer()[index] = root.label;
				}
			}
		}


//		// Remove Background
//		std::sort(comps.components.begin(), comps.components.end(), std::greater<Component>());
		
		for (int i = 0; i < 20; i++) {
			std::cout << "comp_label : " << comps.components[i].label << " size : " << comps.components[i].size << std::endl;
		}

		// TODO: set the biggest
		std::cout << comps.components[pre_defined_label - 1].label << std::endl;
		std::cout << comps.components[pre_defined_label - 1].parent << std::endl;

		const int interest_label = comps.Find(comps.components[pre_defined_label - 1]).label;
		std::cout << interest_label << std::endl;

#pragma omp parallel for
		for (int z = 0; z < image->getDepth(); z++) {
			for (int y = 0; y < image->getHeight(); y++) {
				for (int x = 0; x < image->getWidth(); x++) {
					int index = image->get3DIndex(x, y, z);

					if (image->getBuffer()[index] != BACKGROUND) {
						if (image->getBuffer()[index] == interest_label) {
							image->getBuffer()[index] = FOREGROUND;
						}
						else {
							image->getBuffer()[index] = BACKGROUND;
						}
					}
				}
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