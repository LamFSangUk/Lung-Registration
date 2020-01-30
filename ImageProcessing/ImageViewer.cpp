#define NOMINMAX
#include "ImageViewer.h"
#include "FileHelper.h"
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <fstream>
#include <ctime>

ImageViewer::ImageViewer(QWidget *parent)
	: QMainWindow(parent),
	m_registrator(nullptr) {
	m_ui.setupUi(this);

	InitializeView(m_ui.referenceView);
	InitializeView(m_ui.floatView);
	InitializeView(m_ui.subtractView);


	std::pair<char*, uint32_t> referenceImage = FileHelper::FileToBuffer(L"volume1_512x512x56.raw");
	std::pair<char*, uint32_t> floatImage = FileHelper::FileToBuffer(L"volume2_512x512x58.raw");


	m_registrator = new Registrator<short>(new Image3D<short>(reinterpret_cast<short*>(referenceImage.first), 512, 512, 56, false), new Image3D<short>(reinterpret_cast<short*>(floatImage.first), 512, 512, 58, false));

	m_registrator->Process();

	RenderImageToView(m_ui.floatView, GetFloatImage(), 0, m_ui.windowSlider->value());
	RenderImageToView(m_ui.referenceView, GetReferenceImage(), 0, m_ui.windowSlider->value());
	RenderImageToView(m_ui.subtractView, m_registrator->GetSubtractImage(), 0);


	InitializeUIComponents();
}

ImageViewer::~ImageViewer() {
	if (m_registrator) {
		delete m_registrator;
	}
}

Image3D<short> * ImageViewer::GetReferenceImage() const {
	return m_ui.referenceShowMask->isChecked() ? m_registrator->GetReferenceMask() : m_registrator->GetReferenceImage();
}

Image3D<short> * ImageViewer::GetFloatImage() const {
	return m_ui.floatShowMask->isChecked() ? m_registrator->GetFloatMask() : m_registrator->GetFloatImage();
}


void ImageViewer::InitializeView(QGraphicsView * view) {
	QGraphicsScene *scnImage = new QGraphicsScene(view);
	scnImage->setSceneRect(view->rect());
	view->setScene(scnImage);

	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageViewer::InitializeUIComponents() {
	m_ui.referenceSlider->setMinimum(0);
	m_ui.referenceSlider->setMaximum(m_registrator->GetReferenceImage()->getDepth() - 1);

	m_ui.floatSlider->setMinimum(0);
	m_ui.floatSlider->setMaximum(m_registrator->GetFloatImage()->getDepth() - 1);

	m_ui.referenceSpinBox->setMinimum(0);
	m_ui.referenceSpinBox->setMaximum(m_registrator->GetReferenceImage()->getDepth() - 1);

	m_ui.floatSpinBox->setMinimum(0);
	m_ui.floatSpinBox->setMaximum(m_registrator->GetFloatImage()->getDepth() - 1);

	m_ui.sharedSlider->setMinimum(0);
	m_ui.sharedSlider->setMaximum(std::max(m_registrator->GetReferenceImage()->getDepth(), m_registrator->GetFloatImage()->getDepth()) - 1);

	m_ui.sharedSpinBox->setMinimum(0);
	m_ui.sharedSpinBox->setMaximum(std::max(m_registrator->GetReferenceImage()->getDepth(), m_registrator->GetFloatImage()->getDepth()) - 1);

	m_ui.subtractSlider->setMinimum(0);
	m_ui.subtractSlider->setMaximum(m_registrator->GetSubtractImage()->getDepth() - 1);

	m_ui.subtractSpinBox->setMinimum(0);
	m_ui.subtractSpinBox->setMaximum(m_registrator->GetSubtractImage()->getDepth() - 1);

	m_ui.windowSlider->setMinimum(-2000);
	m_ui.windowSlider->setMaximum(2000);

	QObject::connect(
		m_ui.referenceSlider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(ReferenceSliderChanged()));

	QObject::connect(
		m_ui.floatSlider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(FloatSliderChanged()));

	QObject::connect(
		m_ui.sharedSlider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(SharedSliderChanged()));

	QObject::connect(
		m_ui.subtractSlider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(SubtractSliderChanged()));

	QObject::connect(
		m_ui.referenceSpinBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(ReferenceSpinBoxChanged()));

	QObject::connect(
		m_ui.floatSpinBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(FloatSpinBoxChanged()));

	QObject::connect(
		m_ui.sharedSpinBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(SharedSpinBoxChanged()));

	QObject::connect(
		m_ui.subtractSpinBox,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(SubtractSpinboxChanged()));

	QObject::connect(
		m_ui.referenceShowMask,
		SIGNAL(clicked(bool)),
		this,
		SLOT(ReferenceShowMaskChanged()));

	QObject::connect(
		m_ui.floatShowMask,
		SIGNAL(clicked(bool)),
		this,
		SLOT(FloatShowMaskChanged()));

	QObject::connect(
		m_ui.sharedShowMask,
		SIGNAL(clicked(bool)),
		this,
		SLOT(SharedShowMaskChanged()));

	QObject::connect(
		m_ui.windowSlider,
		SIGNAL(valueChanged(int)),
		this,
		SLOT(WindowSliderChanged()));
}

void ImageViewer::RenderImageToView(QGraphicsView * view, Image3D<short>* image, unsigned depth, short window) {
	QPixmap pixmap(view->rect().width(), view->rect().height());
	QPainter painter(&pixmap);
	// Painter initial setting
	painter.setBackgroundMode(Qt::TransparentMode);
	painter.fillRect(pixmap.rect(), Qt::black);
	unsigned char* buffer = image->getSliceAt(depth, (image->getMinMax().second - image->getMinMax().first), window);

	painter.drawImage(QRect(0, 0, image->getWidth(), image->getHeight()), QImage(buffer, (int)image->getWidth(), (int)image->getHeight(), QImage::Format_RGBA8888));
	painter.end();
	view->scene()->clear();
	view->scene()->addPixmap(pixmap);
	delete[] buffer;
}

void ImageViewer::DepthChanged() {
	m_ui.referenceSpinBox->blockSignals(true);
	m_ui.referenceSlider->blockSignals(true);
	m_ui.floatSpinBox->blockSignals(true);
	m_ui.floatSlider->blockSignals(true);
	m_ui.subtractSpinBox->blockSignals(true);
	m_ui.subtractSlider->blockSignals(true);
	m_ui.referenceSpinBox->setValue(m_ui.sharedSlider->value());
	m_ui.referenceSlider->setValue(m_ui.sharedSlider->value());
	m_ui.floatSpinBox->setValue(m_ui.sharedSlider->value());
	m_ui.floatSlider->setValue(m_ui.sharedSlider->value());
	m_ui.subtractSpinBox->setValue(m_ui.sharedSlider->value());
	m_ui.subtractSlider->setValue(m_ui.sharedSlider->value());
	m_ui.referenceSpinBox->blockSignals(false);
	m_ui.floatSpinBox->blockSignals(false);
	m_ui.referenceSlider->blockSignals(false);
	m_ui.floatSlider->blockSignals(false);
	m_ui.subtractSpinBox->blockSignals(false);
	m_ui.subtractSlider->blockSignals(false);

	bool useReferenceMask = m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked();
	bool useFloatMask = m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked();

	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), useReferenceMask ? FOREGROUND : m_ui.windowSlider->value());
	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value(), useFloatMask ? FOREGROUND : m_ui.windowSlider->value());
	RenderImageToView(m_ui.subtractView, m_registrator->GetSubtractImage(), m_ui.subtractSpinBox->value());
}

void ImageViewer::SharedShowMaskChanged() {
	m_ui.floatShowMask->blockSignals(true);
	m_ui.referenceShowMask->blockSignals(true);
	m_ui.floatShowMask->setChecked(m_ui.sharedShowMask->isChecked());
	m_ui.referenceShowMask->setChecked(m_ui.sharedShowMask->isChecked());
	m_ui.floatShowMask->blockSignals(false);
	m_ui.referenceShowMask->blockSignals(false);

	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::ReferenceShowMaskChanged() {
	//RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value());
	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::FloatShowMaskChanged() {
	//RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value());
	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value(), m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::SharedSliderChanged() {
	m_ui.sharedSpinBox->blockSignals(true);
	m_ui.sharedSpinBox->setValue(m_ui.sharedSlider->value());
	m_ui.sharedSpinBox->blockSignals(false);

	DepthChanged();
}
void ImageViewer::ReferenceSliderChanged() {
	m_ui.referenceSpinBox->blockSignals(true);
	m_ui.referenceSpinBox->setValue(m_ui.referenceSlider->value());
	m_ui.referenceSpinBox->blockSignals(false);

	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::FloatSliderChanged() {
	m_ui.floatSpinBox->blockSignals(true);
	m_ui.floatSpinBox->setValue(m_ui.floatSlider->value());
	m_ui.floatSpinBox->blockSignals(false);

	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::SubtractSliderChanged() {
	m_ui.subtractSpinBox->blockSignals(true);
	m_ui.subtractSpinBox->setValue(m_ui.subtractSlider->value());
	m_ui.subtractSpinBox->blockSignals(false);

	RenderImageToView(m_ui.subtractView, m_registrator->GetSubtractImage(), m_ui.subtractSlider->value());
}

void ImageViewer::SharedSpinBoxChanged() {
	m_ui.sharedSlider->blockSignals(true);
	m_ui.sharedSlider->setValue(m_ui.sharedSpinBox->value());
	m_ui.sharedSlider->blockSignals(false);

	DepthChanged();
}

void ImageViewer::ReferenceSpinBoxChanged() {
	m_ui.referenceSlider->blockSignals(true);
	m_ui.referenceSlider->setValue(m_ui.referenceSpinBox->value());
	m_ui.referenceSlider->blockSignals(false);

	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSpinBox->value(), m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::FloatSpinBoxChanged() {
	m_ui.floatSlider->blockSignals(true);
	m_ui.floatSlider->setValue(m_ui.floatSpinBox->value());
	m_ui.floatSlider->blockSignals(false);

	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value(), m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}

void ImageViewer::SubtractSpinboxChanged() {
	m_ui.subtractSlider->blockSignals(true);
	m_ui.subtractSlider->setValue(m_ui.subtractSpinBox->value());
	m_ui.subtractSlider->blockSignals(false);

	RenderImageToView(m_ui.subtractView, m_registrator->GetSubtractImage(), m_ui.subtractSpinBox->value());
}

void ImageViewer::WindowSliderChanged() {
	//RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), m_ui.windowSlider->value());
	//RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value(), m_ui.windowSlider->value());	
	RenderImageToView(m_ui.referenceView, GetReferenceImage(), m_ui.referenceSlider->value(), m_ui.sharedShowMask->isChecked() || m_ui.referenceShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
	RenderImageToView(m_ui.floatView, GetFloatImage(), m_ui.floatSpinBox->value(), m_ui.sharedShowMask->isChecked() || m_ui.floatShowMask->isChecked() ? FOREGROUND : m_ui.windowSlider->value());
}
