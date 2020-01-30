#pragma once
#define NOMINMAX
#include "Registrator.h"
#include "ImageUtility.h"
#include <glm\gtx\transform.hpp>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <qdebug.h>

template <typename T>
Registrator<T>::Registrator(Image3D<T> * referenceImage, Image3D<T> * floatImage)
	:m_referenceImage(referenceImage),
	m_referenceMask(nullptr),
	m_floatImage(floatImage),
	m_floatMask(nullptr),
	m_subtractImage(nullptr) {
}

template <typename T>
Registrator<T>::~Registrator() {
	if (m_referenceImage) {
		delete m_referenceImage;
	}
	if (m_referenceMask) {
		delete m_referenceMask;
	}
	if (m_floatImage) {
		delete m_floatImage;
	}
	if (m_floatMask) {
		delete m_floatMask;
	}
	if (m_subtractImage) {
		delete m_subtractImage;
	}
}

template <typename T>
void Registrator<T>::Process() {
	m_referenceImage->sliceInterpolate(5);
	m_floatImage->sliceInterpolate(5);

	m_referenceMask = ImageUtility::CreateMask(m_referenceImage);
	m_floatMask = ImageUtility::CreateMask(m_floatImage);

	glm::vec3 referenceCenter = CalculateCenterOfMass(m_referenceMask);
	glm::vec3 centerDifference = referenceCenter - CalculateCenterOfMass(m_floatMask);
	// Initial Transform : Transform both image and mask to use same rotation center (aligned center of mass)
	glm::mat4 transform = glm::translate(centerDifference);

	ImageUtility::FindEdge(m_floatMask);
	ImageUtility::FindEdge(m_referenceMask);

	// TODO : calclulate distance map using reference mask
	Image3D<T>* referenceDistanceMap = m_referenceMask;

	int currentDistance = CalculateTransformedDistance(referenceDistanceMap, m_floatMask, transform);

	/*
	TODO : optimize transform using distance map
	ex) distance = CalculateTransformedDistance(referenceDistanceMap, m_floatMask, transform);
	*/

	// Apply final transform
	TransformImage(m_floatMask, transform, BACKGROUND);
	TransformImage(m_floatImage, transform, -1024);
	m_subtractImage = ImageUtility::CalculateSubtractImage(m_referenceImage, m_floatImage);

	m_referenceMask->calculateMinMax();
	m_referenceImage->calculateMinMax();
	m_floatMask->calculateMinMax();
	m_floatImage->calculateMinMax();
	m_subtractImage->calculateMinMax();
}

template <typename T>
Image3D<T>* Registrator<T>::GetSubtractImage() {
	return m_subtractImage;
}

template <typename T>
Image3D<T>* Registrator<T>::GetReferenceImage() {
	return m_referenceImage;
}

template <typename T>
Image3D<T>* Registrator<T>::GetFloatImage() {
	return m_floatImage;
}

template <typename T>
Image3D<T>* Registrator<T>::GetReferenceMask() {
	return m_referenceMask;
}

template <typename T>
Image3D<T>* Registrator<T>::GetFloatMask() {
	return m_floatMask;
}

template <typename T>
glm::vec3 Registrator<T>::CalculateCenterOfMass(Image3D<T> * image) {
	// sum of position*mass(1) , entire mass
	std::vector<std::pair<glm::vec3, int>> planeMasses(image->getDepth(), std::make_pair(glm::vec3(0.f), 0));
#pragma omp parallel for
	for (int z = 0; z < image->getDepth(); z++) {
		for (int y = 0; y < image->getHeight(); y++) {
			for (int x = 0; x < image->getWidth(); x++) {
				int index = image->get3DIndex(x, y, z);
				if (image->getBuffer()[index] == FOREGROUND) {
					planeMasses[z].first += glm::vec3(x, y, z);
					planeMasses[z].second++;
				}
			}
		}
	}
	glm::vec3 positionMass = {};
	float mass = 0;
	for (std::pair<glm::vec3, int> planeMass : planeMasses) {
		positionMass += planeMass.first;
		mass += planeMass.second;
	}
	return positionMass / mass;
}

template <typename T>
void Registrator<T>::TransformImage(Image3D<T> * image, const glm::vec3& translation, short backgroundColor) {
	TransformImage(image, glm::translate(translation), backgroundColor);
}

template <typename T>
void Registrator<T>::TransformImage(Image3D<T> * image, const glm::vec3& rotation, const glm::vec3& rotationCenter, short backgroundColor) {
	glm::mat4 transform(1.f);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	TransformImage(image, transform, backgroundColor);
}

template <typename T>
void Registrator<T>::TransformImage(Image3D<T> * image, const glm::mat4& transform, short backgroundColor) {
	Image3D<T>* originalImage = new Image3D<T>(*image);
	image->clear(backgroundColor);

	glm::mat4 inverseTransform = glm::inverse(transform);

#pragma omp parallel for
	for (int z = 0; z < image->getDepth(); z++) {
		for (int y = 0; y < image->getHeight(); y++) {
			for (int x = 0; x < image->getWidth(); x++) {
				glm::vec3 position = { x,y,z };
				position = glm::vec3(inverseTransform * glm::vec4(position, 1.f));
				int index = image->get3DIndex(x, y, z);
				if (image->isValidIndex(position.x, position.y, position.z)) {
					image->getBuffer()[index] = originalImage->getInterpolatedAt(position.x, position.y, position.z);
				}
			}
		}
	}

	delete originalImage;
}

template <typename T>
int Registrator<T>::CalculateTransformedDistance(Image3D<T> * distanceMap, Image3D<T> * binaryMask, const glm::mat4& transform) {
	std::vector<int> planeSum(binaryMask->getDepth(), 0);

#pragma omp parallel for
	for (int z = 0; z < binaryMask->getDepth(); z++) {
		for (int y = 0; y < binaryMask->getHeight(); y++) {
			for (int x = 0; x < binaryMask->getWidth(); x++) {
				int index = binaryMask->get3DIndex(x, y, z);
				if (binaryMask->getBuffer()[index] == EDGE) {
					glm::vec3 position = { x,y,z };
					position = glm::vec3(transform * glm::vec4(position, 1.f));
					if (distanceMap->isValidIndex(position.x, position.y, position.z)) {
						planeSum[z] += distanceMap->getInterpolatedAt(position.x, position.y, position.z);
					}
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
glm::mat4 Registrator<T>::GetRotationMatrixAroundPoint(const glm::vec3 & point, float degree, const glm::vec3 & axis) const {
	glm::mat4 transform = glm::translate(point);
	transform *= glm::rotate(glm::radians(degree), axis);
	transform *= glm::translate(-point);
	return transform;
}
