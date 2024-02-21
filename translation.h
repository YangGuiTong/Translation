#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QWidget>
#include <QMap>

#include <QtXml>
#include <QDomDocument>

#include <QJsonObject>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"




namespace Ui {
class Translation;
}

class Translation : public QWidget
{
    Q_OBJECT

public:
    explicit Translation(QWidget *parent = 0);
    ~Translation();

    // 读取ts文件数据写入excel
    QMap<QString, QList<QStringList>> getTranslationXML(QString fileName, QJsonObject extend = QJsonObject());
    void writeExcel(QMap<QString, QList<QStringList>> &translationXML, QJsonObject extend = QJsonObject());

    // 读取excel文件数据写入ts
    QMap<QString, QList<QStringList>> getTranslationExcel(QString fileName, QJsonObject extend = QJsonObject());
    void writeXML(QMap<QString, QList<QStringList>> &translationExcel, QJsonObject extend = QJsonObject());

    // ts转ts
    void TsTransitionTs(QString origin_filename, QString target_filename);
    void TsTransitionTs_2(QString origin_filename, QString target_filename);

private slots:
    void on_btnTsTExcel_clicked();

    void on_btnExcelTTs_clicked();

    void on_btnTsTTs_clicked();

    void on_btnTsTTs_2_clicked();

private:
    Ui::Translation *ui;
    QXlsx::Document m_xlsx;
};

#endif // TRANSLATION_H
