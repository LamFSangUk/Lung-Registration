#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include "ui_ImageViewer.h"
#include "Image3D.h"
#include "FileHelper.h"
#include "Registrator.h"

class ImageViewer : public QMainWindow {
	Q_OBJECT

public:
	ImageViewer(QWidget *parent = Q_NULLPTR);
	~ImageViewer();
private:
	// UI related functions
	Image3D<short>* GetReferenceImage() const;
	Image3D<short>* GetFloatImage() const;
	void InitializeView(QGraphicsView* view);
	void InitializeUIComponents();
	void RenderImageToView(QGraphicsView* view, Image3D<short>* image, unsigned depth, short window = 0);

	Ui::ImageViewerClass m_ui;

	Registrator<short>* m_registrator;

	void DepthChanged();
	private slots:
	void SharedShowMaskChanged();
	void ReferenceShowMaskChanged();
	void FloatShowMaskChanged();
	void SharedSliderChanged();
	void ReferenceSliderChanged();
	void FloatSliderChanged();
	void SubtractSliderChanged();
	void SharedSpinBoxChanged();
	void ReferenceSpinBoxChanged();
	void FloatSpinBoxChanged();
	void SubtractSpinboxChanged();
	void WindowSliderChanged();
};
