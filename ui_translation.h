/********************************************************************************
** Form generated from reading UI file 'translation.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSLATION_H
#define UI_TRANSLATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Translation
{
public:
    QPushButton *btnTsTExcel;
    QPushButton *btnExcelTTs;
    QLabel *labelText;
    QPushButton *btnTsTTs;
    QTextEdit *textEdit;
    QPushButton *btnTsTTs_2;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Translation)
    {
        if (Translation->objectName().isEmpty())
            Translation->setObjectName(QStringLiteral("Translation"));
        Translation->resize(429, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Translation->sizePolicy().hasHeightForWidth());
        Translation->setSizePolicy(sizePolicy);
        Translation->setMinimumSize(QSize(429, 400));
        Translation->setMaximumSize(QSize(429, 400));
        btnTsTExcel = new QPushButton(Translation);
        btnTsTExcel->setObjectName(QStringLiteral("btnTsTExcel"));
        btnTsTExcel->setGeometry(QRect(90, 120, 93, 28));
        sizePolicy.setHeightForWidth(btnTsTExcel->sizePolicy().hasHeightForWidth());
        btnTsTExcel->setSizePolicy(sizePolicy);
        btnExcelTTs = new QPushButton(Translation);
        btnExcelTTs->setObjectName(QStringLiteral("btnExcelTTs"));
        btnExcelTTs->setGeometry(QRect(300, 120, 93, 28));
        sizePolicy.setHeightForWidth(btnExcelTTs->sizePolicy().hasHeightForWidth());
        btnExcelTTs->setSizePolicy(sizePolicy);
        labelText = new QLabel(Translation);
        labelText->setObjectName(QStringLiteral("labelText"));
        labelText->setGeometry(QRect(20, 170, 391, 41));
        sizePolicy.setHeightForWidth(labelText->sizePolicy().hasHeightForWidth());
        labelText->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Adobe Arabic"));
        font.setPointSize(12);
        labelText->setFont(font);
        btnTsTTs = new QPushButton(Translation);
        btnTsTTs->setObjectName(QStringLiteral("btnTsTTs"));
        btnTsTTs->setGeometry(QRect(90, 50, 93, 28));
        textEdit = new QTextEdit(Translation);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(20, 220, 391, 171));
        btnTsTTs_2 = new QPushButton(Translation);
        btnTsTTs_2->setObjectName(QStringLiteral("btnTsTTs_2"));
        btnTsTTs_2->setGeometry(QRect(300, 50, 93, 28));
        label = new QLabel(Translation);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 72, 15));
        label_2 = new QLabel(Translation);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 60, 72, 15));

        retranslateUi(Translation);

        QMetaObject::connectSlotsByName(Translation);
    } // setupUi

    void retranslateUi(QWidget *Translation)
    {
        Translation->setWindowTitle(QApplication::translate("Translation", "Translation", Q_NULLPTR));
        btnTsTExcel->setText(QApplication::translate("Translation", "ts\350\275\254excel", Q_NULLPTR));
        btnExcelTTs->setText(QApplication::translate("Translation", "excel\350\275\254ts", Q_NULLPTR));
        labelText->setText(QString());
        btnTsTTs->setText(QApplication::translate("Translation", "ts\350\275\254ts", Q_NULLPTR));
        btnTsTTs_2->setText(QApplication::translate("Translation", "ts\350\275\254ts", Q_NULLPTR));
        label->setText(QApplication::translate("Translation", "\346\234\252\344\274\230\345\214\226\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("Translation", "\345\267\262\344\274\230\345\214\226\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Translation: public Ui_Translation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSLATION_H
