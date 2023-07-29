/********************************************************************************
** Form generated from reading UI file 'GoProSurfTWlFLj.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef GOPROSURFTWLFLJ_H
#define GOPROSURFTWLFLJ_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *filesVerticalLayout;
    QTreeView *directoriesTreeView;
    QGroupBox *groupBox;
    QGridLayout *detectGridLayout;
    QLabel *minSpeedlabel;
    QSpinBox *minDurationSpinBox;
    QDoubleSpinBox *minSpeedSpinBox;
    QLabel *minDurationlabel;
    QTableView *filesTable;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *exportButton;
    QPushButton *deleteButton;
    QPushButton *deleteEmptyButton;
    QTableView *wavesTable;
    QVBoxLayout *videoLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QVideoWidget *videoWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *playerButton;
    QSlider *playerSlider;
    QChartView *chartView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        filesVerticalLayout = new QVBoxLayout();
        filesVerticalLayout->setObjectName(QString::fromUtf8("filesVerticalLayout"));
        directoriesTreeView = new QTreeView(centralwidget);
        directoriesTreeView->setObjectName(QString::fromUtf8("directoriesTreeView"));

        filesVerticalLayout->addWidget(directoriesTreeView);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        detectGridLayout = new QGridLayout(groupBox);
        detectGridLayout->setObjectName(QString::fromUtf8("detectGridLayout"));
        minSpeedlabel = new QLabel(groupBox);
        minSpeedlabel->setObjectName(QString::fromUtf8("minSpeedlabel"));

        detectGridLayout->addWidget(minSpeedlabel, 0, 0, 1, 1);

        minDurationSpinBox = new QSpinBox(groupBox);
        minDurationSpinBox->setObjectName(QString::fromUtf8("minDurationSpinBox"));
        minDurationSpinBox->setMinimum(1);
        minDurationSpinBox->setMaximum(999);
        minDurationSpinBox->setValue(5);

        detectGridLayout->addWidget(minDurationSpinBox, 1, 2, 1, 1);

        minSpeedSpinBox = new QDoubleSpinBox(groupBox);
        minSpeedSpinBox->setObjectName(QString::fromUtf8("minSpeedSpinBox"));
        minSpeedSpinBox->setDecimals(1);
        minSpeedSpinBox->setMinimum(0.100000000000000);
        minSpeedSpinBox->setSingleStep(0.100000000000000);
        minSpeedSpinBox->setValue(2.000000000000000);

        detectGridLayout->addWidget(minSpeedSpinBox, 1, 0, 1, 1);

        minDurationlabel = new QLabel(groupBox);
        minDurationlabel->setObjectName(QString::fromUtf8("minDurationlabel"));

        detectGridLayout->addWidget(minDurationlabel, 0, 2, 1, 1);


        filesVerticalLayout->addWidget(groupBox);

        filesTable = new QTableView(centralwidget);
        filesTable->setObjectName(QString::fromUtf8("filesTable"));
        filesTable->setSelectionMode(QAbstractItemView::SingleSelection);
        filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        filesTable->verticalHeader()->setVisible(false);

        filesVerticalLayout->addWidget(filesTable);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));

        filesVerticalLayout->addWidget(progressBar);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        exportButton = new QPushButton(centralwidget);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));

        horizontalLayout_3->addWidget(exportButton);

        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setEnabled(false);
        deleteButton->setStyleSheet(QString::fromUtf8("color: rgb(237, 51, 59);"));

        horizontalLayout_3->addWidget(deleteButton);

        deleteEmptyButton = new QPushButton(centralwidget);
        deleteEmptyButton->setObjectName(QString::fromUtf8("deleteEmptyButton"));
        deleteEmptyButton->setEnabled(false);
        deleteEmptyButton->setStyleSheet(QString::fromUtf8("color: rgb(224, 27, 36);"));

        horizontalLayout_3->addWidget(deleteEmptyButton);


        filesVerticalLayout->addLayout(horizontalLayout_3);

        wavesTable = new QTableView(centralwidget);
        wavesTable->setObjectName(QString::fromUtf8("wavesTable"));
        wavesTable->setSelectionMode(QAbstractItemView::SingleSelection);
        wavesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        filesVerticalLayout->addWidget(wavesTable);

        filesVerticalLayout->setStretch(0, 3);
        filesVerticalLayout->setStretch(2, 3);
        filesVerticalLayout->setStretch(5, 2);

        horizontalLayout->addLayout(filesVerticalLayout);

        videoLayout = new QVBoxLayout();
        videoLayout->setObjectName(QString::fromUtf8("videoLayout"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        videoWidget = new QVideoWidget(frame);
        videoWidget->setObjectName(QString::fromUtf8("videoWidget"));
        sizePolicy.setHeightForWidth(videoWidget->sizePolicy().hasHeightForWidth());
        videoWidget->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(videoWidget, 0, 0, 1, 1);


        videoLayout->addWidget(frame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        playerButton = new QPushButton(centralwidget);
        playerButton->setObjectName(QString::fromUtf8("playerButton"));
        playerButton->setEnabled(false);
        playerButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(playerButton);

        playerSlider = new QSlider(centralwidget);
        playerSlider->setObjectName(QString::fromUtf8("playerSlider"));
        playerSlider->setEnabled(false);
        playerSlider->setSingleStep(20);
        playerSlider->setPageStep(5000);
        playerSlider->setOrientation(Qt::Horizontal);
        playerSlider->setTickPosition(QSlider::TicksAbove);
        playerSlider->setTickInterval(1000);

        horizontalLayout_2->addWidget(playerSlider);


        videoLayout->addLayout(horizontalLayout_2);

        chartView = new QChartView(centralwidget);
        chartView->setObjectName(QString::fromUtf8("chartView"));

        videoLayout->addWidget(chartView);

        videoLayout->setStretch(0, 4);
        videoLayout->setStretch(2, 1);

        horizontalLayout->addLayout(videoLayout);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 3);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GoPro Surf", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Detect options:", nullptr));
        minSpeedlabel->setText(QCoreApplication::translate("MainWindow", "Min speed (m/s):", nullptr));
        minDurationlabel->setText(QCoreApplication::translate("MainWindow", "Min. duration (s):", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export all", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
#if QT_CONFIG(tooltip)
        deleteEmptyButton->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        deleteEmptyButton->setText(QCoreApplication::translate("MainWindow", "Delete empty", nullptr));
        playerButton->setText(QCoreApplication::translate("MainWindow", "\342\226\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // GOPROSURFTWLFLJ_H
