/********************************************************************************
** Form generated from reading UI file 'UIMain.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIMAIN_H
#define UI_UIMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UIMainClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *gruopOption;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEditStop;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEditStart;
    QCheckBox *checkBoxPause;
    QSpinBox *spinBoxAlignment;
    QCheckBox *checkBoxAlignment;
    QPushButton *buttonOpen;
    QGroupBox *groupParam;
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QLabel *label_2;
    QLabel *label;
    QPushButton *buttonNextScan;
    QLabel *labelProcessInfo;
    QLabel *labelIcon;
    QLabel *labelFound;
    QComboBox *comboBoxValueType;
    QPushButton *buttonNewScan;
    QPushButton *pushButton_4;
    QComboBox *comboBoxScanType;
    QOpenGLWidget *openGLWidget;
    QTreeWidget *treeWidgetResults;
    QTreeWidget *treeWidgetWatchList;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UIMainClass)
    {
        if (UIMainClass->objectName().isEmpty())
            UIMainClass->setObjectName(QStringLiteral("UIMainClass"));
        UIMainClass->resize(852, 619);
        centralWidget = new QWidget(UIMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gruopOption = new QGroupBox(centralWidget);
        gruopOption->setObjectName(QStringLiteral("gruopOption"));
        gruopOption->setEnabled(true);
        gridLayout_3 = new QGridLayout(gruopOption);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lineEditStop = new QLineEdit(gruopOption);
        lineEditStop->setObjectName(QStringLiteral("lineEditStop"));
        lineEditStop->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(lineEditStop, 1, 1, 1, 1);

        label_3 = new QLabel(gruopOption);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFrameShape(QFrame::NoFrame);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(gruopOption);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        lineEditStart = new QLineEdit(gruopOption);
        lineEditStart->setObjectName(QStringLiteral("lineEditStart"));
        lineEditStart->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(lineEditStart, 0, 1, 1, 1);

        checkBoxPause = new QCheckBox(gruopOption);
        checkBoxPause->setObjectName(QStringLiteral("checkBoxPause"));
        checkBoxPause->setChecked(true);

        gridLayout_3->addWidget(checkBoxPause, 3, 0, 1, 2);

        spinBoxAlignment = new QSpinBox(gruopOption);
        spinBoxAlignment->setObjectName(QStringLiteral("spinBoxAlignment"));
        spinBoxAlignment->setMinimum(1);
        spinBoxAlignment->setMaximum(4096);
        spinBoxAlignment->setValue(4);

        gridLayout_3->addWidget(spinBoxAlignment, 2, 1, 1, 1);

        checkBoxAlignment = new QCheckBox(gruopOption);
        checkBoxAlignment->setObjectName(QStringLiteral("checkBoxAlignment"));
        checkBoxAlignment->setChecked(true);

        gridLayout_3->addWidget(checkBoxAlignment, 2, 0, 1, 1);


        gridLayout->addWidget(gruopOption, 9, 2, 1, 3);

        buttonOpen = new QPushButton(centralWidget);
        buttonOpen->setObjectName(QStringLiteral("buttonOpen"));

        gridLayout->addWidget(buttonOpen, 0, 0, 2, 1);

        groupParam = new QGroupBox(centralWidget);
        groupParam->setObjectName(QStringLiteral("groupParam"));
        groupParam->setFlat(false);
        verticalLayout = new QVBoxLayout(groupParam);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        gridLayout->addWidget(groupParam, 8, 2, 1, 3);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximum(1);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        gridLayout->addWidget(progressBar, 1, 1, 1, 4);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 7, 2, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 6, 2, 1, 1);

        buttonNextScan = new QPushButton(centralWidget);
        buttonNextScan->setObjectName(QStringLiteral("buttonNextScan"));
        buttonNextScan->setEnabled(false);

        gridLayout->addWidget(buttonNextScan, 5, 3, 1, 1);

        labelProcessInfo = new QLabel(centralWidget);
        labelProcessInfo->setObjectName(QStringLiteral("labelProcessInfo"));

        gridLayout->addWidget(labelProcessInfo, 0, 2, 1, 3);

        labelIcon = new QLabel(centralWidget);
        labelIcon->setObjectName(QStringLiteral("labelIcon"));
        labelIcon->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelIcon, 0, 1, 1, 1);

        labelFound = new QLabel(centralWidget);
        labelFound->setObjectName(QStringLiteral("labelFound"));

        gridLayout->addWidget(labelFound, 2, 0, 1, 1);

        comboBoxValueType = new QComboBox(centralWidget);
        comboBoxValueType->setObjectName(QStringLiteral("comboBoxValueType"));

        gridLayout->addWidget(comboBoxValueType, 7, 3, 1, 1);

        buttonNewScan = new QPushButton(centralWidget);
        buttonNewScan->setObjectName(QStringLiteral("buttonNewScan"));
        buttonNewScan->setEnabled(false);
        buttonNewScan->setCheckable(false);

        gridLayout->addWidget(buttonNewScan, 5, 2, 1, 1);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 10, 4, 1, 1);

        comboBoxScanType = new QComboBox(centralWidget);
        comboBoxScanType->setObjectName(QStringLiteral("comboBoxScanType"));

        gridLayout->addWidget(comboBoxScanType, 6, 3, 1, 1);

        openGLWidget = new QOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 5, 4, 3, 1);

        treeWidgetResults = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidgetResults->setHeaderItem(__qtreewidgetitem);
        treeWidgetResults->setObjectName(QStringLiteral("treeWidgetResults"));

        gridLayout->addWidget(treeWidgetResults, 5, 0, 5, 2);

        treeWidgetWatchList = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeWidgetWatchList->setHeaderItem(__qtreewidgetitem1);
        treeWidgetWatchList->setObjectName(QStringLiteral("treeWidgetWatchList"));

        gridLayout->addWidget(treeWidgetWatchList, 10, 0, 1, 4);

        UIMainClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(UIMainClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 852, 23));
        UIMainClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UIMainClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        UIMainClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(UIMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        UIMainClass->setStatusBar(statusBar);

        retranslateUi(UIMainClass);

        QMetaObject::connectSlotsByName(UIMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *UIMainClass)
    {
        UIMainClass->setWindowTitle(QApplication::translate("UIMainClass", "ZheatEngine", Q_NULLPTR));
        gruopOption->setTitle(QApplication::translate("UIMainClass", "Memory Scan Options", Q_NULLPTR));
        lineEditStop->setText(QApplication::translate("UIMainClass", "7FFFFFFFFFFFFFFF", Q_NULLPTR));
        label_3->setText(QApplication::translate("UIMainClass", "Start", Q_NULLPTR));
        label_4->setText(QApplication::translate("UIMainClass", "Stop", Q_NULLPTR));
        lineEditStart->setText(QApplication::translate("UIMainClass", "0000000000000000", Q_NULLPTR));
        checkBoxPause->setText(QApplication::translate("UIMainClass", "Pause the game while searching", Q_NULLPTR));
        checkBoxAlignment->setText(QApplication::translate("UIMainClass", "Aligned Only", Q_NULLPTR));
        buttonOpen->setText(QApplication::translate("UIMainClass", "Open", Q_NULLPTR));
        groupParam->setTitle(QApplication::translate("UIMainClass", "Scan Parameters", Q_NULLPTR));
        label_2->setText(QApplication::translate("UIMainClass", "Value Type", Q_NULLPTR));
        label->setText(QApplication::translate("UIMainClass", "Scan Type", Q_NULLPTR));
        buttonNextScan->setText(QApplication::translate("UIMainClass", "Next Scan", Q_NULLPTR));
        labelProcessInfo->setText(QApplication::translate("UIMainClass", "No Process Selected", Q_NULLPTR));
        labelIcon->setText(QString());
        labelFound->setText(QString());
        buttonNewScan->setText(QApplication::translate("UIMainClass", "First Scan", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("UIMainClass", "Add Address Manually", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class UIMainClass: public Ui_UIMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIMAIN_H
