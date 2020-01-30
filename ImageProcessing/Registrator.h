#include "Image3D.h"
#include <glm\glm.hpp> 

#pragma once
template <typename T>
class Registrator {
public:
	Registrator(Image3D<T>* referenceImage, Image3D<T>* floatImage);
	~Registrator();
	// Registrate reference image and float image and returns registrated floating image
	void Process();

	Image3D<T>* GetSubtractImage();
	Image3D<T>* GetReferenceImage();
	Image3D<T>* GetFloatImage();
	Image3D<T>* GetReferenceMask();
	Image3D<T>* GetFloatMask();

private:

	// Registration
	glm::vec3 CalculateCenterOfMass(Image3D<T>* image);
	void TransformImage(Image3D<T>* image, const glm::vec3 & translation, short backgroundColor);
	void TransformImage(Image3D<T>* image, const glm::vec3 & rotation, const glm::vec3 & rotationCenter, short backgroundColor);
	void TransformImage(Image3D<T>* image, const glm::mat4 & transform, short backgroundColor);
	int CalculateTransformedDistance(Image3D<T>* distanceMap, Image3D<T>* binaryMask, const glm::mat4& transform);

	glm::mat4 GetRotationMatrixAroundPoint(const glm::vec3& point, float degree, const glm::vec3& axis) const;

	Image3D<T>* m_referenceImage;
	Image3D<T>* m_floatImage;

	Image3D<T>* m_referenceMask;
	Image3D<T>* m_floatMask;

	Image3D<T>* m_subtractImage;
};

#include "Registrator.cpp"