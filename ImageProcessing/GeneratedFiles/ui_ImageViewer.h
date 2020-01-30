/********************************************************************************
** Form generated from reading UI file 'ImageViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEWER_H
#define UI_IMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageViewerClass
{
public:
    QWidget *mainWidget;
    QGraphicsView *referenceView;
    QGraphicsView *floatView;
    QSlider *referenceSlider;
    QSlider *floatSlider;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *referenceSliderLabel;
    QLabel *label_5;
    QSpinBox *referenceSpinBox;
    QLabel *floatSliderLabel;
    QSpinBox *floatSpinBox;
    QCheckBox *referenceShowMask;
    QCheckBox *floatShowMask;
    QGroupBox *groupBox;
    QSlider *windowSlider;
    QLabel *referenceSliderLabel_2;
    QLabel *sharedSliderName;
    QSlider *sharedSlider;
    QSpinBox *sharedSpinBox;
    QCheckBox *sharedShowMask;
    QGraphicsView *subtractView;
    QLabel *subtractSliderLabel;
    QSlider *subtractSlider;
    QSpinBox *subtractSpinBox;
    QLabel *label_6;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageViewerClass)
    {
        if (ImageViewerClass->objectName().isEmpty())
            ImageViewerClass->setObjectName(QString::fromUtf8("ImageViewerClass"));
        ImageViewerClass->resize(1632, 689);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImageViewerClass->sizePolicy().hasHeightForWidth());
        ImageViewerClass->setSizePolicy(sizePolicy);
        mainWidget = new QWidget(ImageViewerClass);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        sizePolicy.setHeightForWidth(mainWidget->sizePolicy().hasHeightForWidth());
        mainWidget->setSizePolicy(sizePolicy);
        mainWidget->setMinimumSize(QSize(1632, 636));
        mainWidget->setMaximumSize(QSize(1632, 636));
        referenceView = new QGraphicsView(mainWidget);
        referenceView->setObjectName(QString::fromUtf8("referenceView"));
        referenceView->setGeometry(QRect(20, 30, 512, 512));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(referenceView->sizePolicy().hasHeightForWidth());
        referenceView->setSizePolicy(sizePolicy1);
        floatView = new QGraphicsView(mainWidget);
        floatView->setObjectName(QString::fromUtf8("floatView"));
        floatView->setGeometry(QRect(560, 30, 512, 512));
        sizePolicy.setHeightForWidth(floatView->sizePolicy().hasHeightForWidth());
        floatView->setSizePolicy(sizePolicy);
        referenceSlider = new QSlider(mainWidget);
        referenceSlider->setObjectName(QString::fromUtf8("referenceSlider"));
        referenceSlider->setGeometry(QRect(60, 550, 321, 22));
        referenceSlider->setOrientation(Qt::Horizontal);
        floatSlider = new QSlider(mainWidget);
        floatSlider->setObjectName(QString::fromUtf8("floatSlider"));
        floatSlider->setGeometry(QRect(600, 550, 321, 22));
        floatSlider->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(mainWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 10, 101, 16));
        label_4 = new QLabel(mainWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(660, 20, 91, 16));
        referenceSliderLabel = new QLabel(mainWidget);
        referenceSliderLabel->setObjectName(QString::fromUtf8("referenceSliderLabel"));
        referenceSliderLabel->setGeometry(QRect(20, 550, 41, 21));
        label_5 = new QLabel(mainWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(560, 10, 91, 16));
        referenceSpinBox = new QSpinBox(mainWidget);
        referenceSpinBox->setObjectName(QString::fromUtf8("referenceSpinBox"));
        referenceSpinBox->setGeometry(QRect(390, 550, 42, 22));
        floatSliderLabel = new QLabel(mainWidget);
        floatSliderLabel->setObjectName(QString::fromUtf8("floatSliderLabel"));
        floatSliderLabel->setGeometry(QRect(560, 550, 41, 21));
        floatSpinBox = new QSpinBox(mainWidget);
        floatSpinBox->setObjectName(QString::fromUtf8("floatSpinBox"));
        floatSpinBox->setGeometry(QRect(930, 550, 42, 22));
        referenceShowMask = new QCheckBox(mainWidget);
        referenceShowMask->setObjectName(QString::fromUtf8("referenceShowMask"));
        referenceShowMask->setGeometry(QRect(440, 550, 91, 21));
        floatShowMask = new QCheckBox(mainWidget);
        floatShowMask->setObjectName(QString::fromUtf8("floatShowMask"));
        floatShowMask->setGeometry(QRect(980, 550, 91, 21));
        groupBox = new QGroupBox(mainWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(300, 580, 1031, 51));
        windowSlider = new QSlider(groupBox);
        windowSlider->setObjectName(QString::fromUtf8("windowSlider"));
        windowSlider->setGeometry(QRect(70, 20, 311, 22));
        windowSlider->setOrientation(Qt::Horizontal);
        referenceSliderLabel_2 = new QLabel(groupBox);
        referenceSliderLabel_2->setObjectName(QString::fromUtf8("referenceSliderLabel_2"));
        referenceSliderLabel_2->setGeometry(QRect(10, 20, 51, 21));
        sharedSliderName = new QLabel(groupBox);
        sharedSliderName->setObjectName(QString::fromUtf8("sharedSliderName"));
        sharedSliderName->setGeometry(QRect(390, 20, 41, 21));
        sharedSlider = new QSlider(groupBox);
        sharedSlider->setObjectName(QString::fromUtf8("sharedSlider"));
        sharedSlider->setGeometry(QRect(440, 20, 431, 22));
        sharedSlider->setOrientation(Qt::Horizontal);
        sharedSpinBox = new QSpinBox(groupBox);
        sharedSpinBox->setObjectName(QString::fromUtf8("sharedSpinBox"));
        sharedSpinBox->setGeometry(QRect(880, 20, 42, 22));
        sharedShowMask = new QCheckBox(groupBox);
        sharedShowMask->setObjectName(QString::fromUtf8("sharedShowMask"));
        sharedShowMask->setGeometry(QRect(930, 20, 91, 21));
        subtractView = new QGraphicsView(mainWidget);
        subtractView->setObjectName(QString::fromUtf8("subtractView"));
        subtractView->setGeometry(QRect(1100, 30, 512, 512));
        sizePolicy.setHeightForWidth(subtractView->sizePolicy().hasHeightForWidth());
        subtractView->setSizePolicy(sizePolicy);
        subtractSliderLabel = new QLabel(mainWidget);
        subtractSliderLabel->setObjectName(QString::fromUtf8("subtractSliderLabel"));
        subtractSliderLabel->setGeometry(QRect(1100, 550, 41, 21));
        subtractSlider = new QSlider(mainWidget);
        subtractSlider->setObjectName(QString::fromUtf8("subtractSlider"));
        subtractSlider->setGeometry(QRect(1140, 550, 411, 22));
        subtractSlider->setOrientation(Qt::Horizontal);
        subtractSpinBox = new QSpinBox(mainWidget);
        subtractSpinBox->setObjectName(QString::fromUtf8("subtractSpinBox"));
        subtractSpinBox->setGeometry(QRect(1560, 550, 42, 22));
        label_6 = new QLabel(mainWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(1100, 10, 91, 16));
        ImageViewerClass->setCentralWidget(mainWidget);
        groupBox->raise();
        referenceView->raise();
        floatView->raise();
        referenceSlider->raise();
        floatSlider->raise();
        label_3->raise();
        label_4->raise();
        referenceSliderLabel->raise();
        label_5->raise();
        referenceSpinBox->raise();
        floatSliderLabel->raise();
        floatSpinBox->raise();
        referenceShowMask->raise();
        floatShowMask->raise();
        subtractView->raise();
        subtractSliderLabel->raise();
        subtractSlider->raise();
        subtractSpinBox->raise();
        label_6->raise();
        menuBar = new QMenuBar(ImageViewerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1632, 21));
        ImageViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageViewerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ImageViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ImageViewerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImageViewerClass->setStatusBar(statusBar);

        retranslateUi(ImageViewerClass);

        QMetaObject::connectSlotsByName(ImageViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageViewerClass)
    {
        ImageViewerClass->setWindowTitle(QApplication::translate("ImageViewerClass", "ImageViewer", nullptr));
        label_3->setText(QApplication::translate("ImageViewerClass", "Reference Image", nullptr));
        label_4->setText(QString());
        referenceSliderLabel->setText(QApplication::translate("ImageViewerClass", "Depth", nullptr));
        label_5->setText(QApplication::translate("ImageViewerClass", "Float Image", nullptr));
        floatSliderLabel->setText(QApplication::translate("ImageViewerClass", "Depth", nullptr));
        referenceShowMask->setText(QApplication::translate("ImageViewerClass", "Show Mask", nullptr));
        floatShowMask->setText(QApplication::translate("ImageViewerClass", "Show Mask", nullptr));
        groupBox->setTitle(QApplication::translate("ImageViewerClass", "Shared Control", nullptr));
        referenceSliderLabel_2->setText(QApplication::translate("ImageViewerClass", "Window", nullptr));
        sharedSliderName->setText(QApplication::translate("ImageViewerClass", "Depth", nullptr));
        sharedShowMask->setText(QApplication::translate("ImageViewerClass", "Show Mask", nullptr));
        subtractSliderLabel->setText(QApplication::translate("ImageViewerClass", "Depth", nullptr));
        label_6->setText(QApplication::translate("ImageViewerClass", "Subtract Image", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageViewerClass: public Ui_ImageViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEWER_H
