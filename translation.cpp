#include "translation.h"
#include "ui_translation.h"


#include <QMessageBox>

#pragma execution_character_set("utf-8") // qt支持显示中文

Translation::Translation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Translation)
{
    ui->setupUi(this);



}

Translation::~Translation()
{
    delete ui;
}


QMap<QString, QList<QStringList>> Translation::getTranslationXML(QString fileName, QJsonObject extend)
{
    QMap<QString, QList<QStringList>> translationXML;

    // 打开文件
    QFile file(fileName);
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", QString("%1文件打开失败！").arg(fileName));
        return translationXML;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return translationXML;
    }

    QDomElement root = doc.documentElement();


    QDomNodeList list = root.elementsByTagName("context");


    /* 获取属性中的值 */
    for (int i = 0; i < list.count(); i++) {
        // 获取链表中的值
        QDomElement element = list.at(i).toElement();
        // 获取唯一键值
        QString key = element.namedItem("name").firstChild().nodeValue();
        QList<QStringList> keyValue;   // 值

        QDomNodeList m_list = element.elementsByTagName("message");
        for (int j = 0; j < m_list.count(); j++) {
            // 获取链表中的值
            QDomElement m_element = m_list.at(j).toElement();
            QDomNode source = m_element.namedItem("source");
            QDomNode translation = m_element.namedItem("translation");

            QStringList childrenNode;
            childrenNode.push_back(source.firstChild().nodeValue());
            childrenNode.push_back(translation.firstChild().nodeValue());

            keyValue.push_back(childrenNode);
        }

        // 绑定键和值
        translationXML[key] = keyValue;
    }

    file.close();

    return translationXML;
}

void Translation::writeExcel(QMap<QString, QList<QStringList>> &translationXML, QJsonObject extend)
{
    // 对应excel表里的行和列
    int row = 1;
    int column = 1;

    QMap<QString, QList<QStringList>>::const_iterator iter = translationXML.cbegin();
    while (iter != translationXML.cend()) {
        QString key = iter.key();
        QList<QStringList> value = iter.value();

        QXlsx::Format formatTitle;
        formatTitle.setFontBold(true);   // 加粗
        formatTitle.setFontSize(20);

        QXlsx::Format formatValue;
        formatValue.setFontSize(12);


        // 写入标题
        m_xlsx.write(row, column, key, formatTitle);
        ui->labelText->setText(QString("写入键：%1").arg(key));
        row++;


        QList<QStringList>::const_iterator it = value.cbegin();
        while (it != value.cend()) {
            QStringList message = *it;

            m_xlsx.write(row, column, message.front(),formatValue);
            ui->labelText->setText(QString("写入值：%1").arg(message.front()));
            column++;   // 第二列
            m_xlsx.write(row, column, message.back(),formatValue);
            ui->labelText->setText(QString("写入值：%1").arg(message.back()));

            // 提前为下一行做准备
            column--;
            row++;

            it++;
        }

        m_xlsx.write(row, column, "###", formatTitle);
        row += 2;

        iter++;
    }

    // 结束标志
    m_xlsx.write(row, column, "######");

    ui->labelText->setText(QString("ts转excel写入完毕"));
}

QMap<QString, QList<QStringList> > Translation::getTranslationExcel(QString fileName, QJsonObject extend)
{
    QMap<QString, QList<QStringList>> translationExcel;

    QXlsx::Document xlsx(fileName);

    // 对应excel表里的行和列
    int row = 1;
    int column = 1;

    while (1) {
        QList<QStringList> value;
        QString title = xlsx.read(row, column).toString();
        if (QLatin1String("######") == title) {
            ui->labelText->setText("excel数据读取写入ts文件 完毕！");
            break;

        } else if ("" == title) {
            ui->labelText->setText("excel数据读取写入ts文件 数据有误！");
            break;

        }

        row++;

        while (1) {
            QStringList list;
            QString message = xlsx.read(row, column).toString();    // 读取原文
            if (QLatin1String("###") == message) {  // 结束条件
                row += 2;       // 为读取下一行做准备
                column = 1;
                break;
            }


            list.append(message);
            column++;

            message = xlsx.read(row, column).toString();        // 读取译文
            list.append(message);
            column++;

            message = xlsx.read(row, column).toString();        // 最终翻译后的译文
            list.append(message);
            row++;
            column = 1;

            value.append(list);
        }


        translationExcel.insert(title, value);
    }


    return translationExcel;
}

/*
 * 说明：因为translationExcel与读取到的xml数据都是一一对应的，所以下面操作都是按照这个对应关系去处理；
 *      所以不可以改变原有的excel格式和ts格式，否则数据会乱掉！
 */
void Translation::writeXML(QMap<QString, QList<QStringList> > &translationExcel, QJsonObject extend)
{
    // 打开文件
    QFile file("translation_zh_cn.ts");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    QDomDocument doc;

    // 将doc与file关联起来
    // 这个函数从IO设备dev中读取XML文档，如果内容被成功解析，则返回true;否则返回false。
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    // 关联后可以关闭文件了
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList contextList = root.elementsByTagName("context");  // 获得xml中的所有context

    int mapIndex = 0;   // 对应关系
    QMap<QString, QList<QStringList>>::const_iterator iter = translationExcel.cbegin();
    while (iter != translationExcel.cend()) {
        QString keyName = iter.key();                       // name值
        QList<QStringList> valueMessage = iter.value();     // excel中的context下的所有message数据

        // 拿取xml中的所有message
        QDomElement elementMessage = contextList.at(mapIndex).toElement();
        QDomNodeList messageList = elementMessage.elementsByTagName("message");


        for (int i = 0; i < valueMessage.size(); i++) {
            QStringList messageExcel = valueMessage.at(i);  // 第一个message
            if (3 != messageExcel.size()) {
                QMessageBox::critical(this, "错误", "读取excel数据没有3列");
                return;
            }
            QString trans = messageExcel.at(2);     // 拿到译文
            qDebug() << "译文：" << trans;

            // 如果译文为空，则进行下一行操作
            if (QLatin1String("") ==  trans) {
                continue;
            }

            QDomElement messageXML = messageList.at(i).toElement();
            QDomNode  translation = messageXML.namedItem("translation");
            qDebug() << translation.firstChild().nodeValue();
            // 尖括号之间的内容作为子节点出现(修改前)
            QDomNode oldNode = translation.firstChild();
            // 修改尖括号之间的值
            translation.firstChild().setNodeValue(trans);
            // 尖括号之间的内容作为子节点出现(修改后)
            QDomNode newNode = translation.firstChild();
            // 将新旧内容子节点进行替换
            translation.replaceChild(newNode, oldNode);
        }

        iter++;
        mapIndex++;
    }

    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }

    // 输出到文件
    QTextStream stream(&file);
    doc.save(stream, 4);	// 缩进4格
    file.close();
}

void Translation::TsTransitionTs(QString origin_filename, QString target_filename)
{
    QMap<QString, QList<QStringList>> origin_map = getTranslationXML(origin_filename);
    if (0 >= origin_map.size()) {
        QMessageBox::information(NULL, "提示", QString("文件 %1 文件读取有误！")
                                 .arg(origin_filename));
        return;
    }


    // 打开文件
    QFile target_file(target_filename);
    if (!target_file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", QString("文件%1打开失败！").arg(target_filename));
        return;
    }


    QDomDocument target_doc;
    // 将doc与file关联起来
    // 这个函数从IO设备dev中读取XML文档，如果内容被成功解析，则返回true;否则返回false。
    if (!target_doc.setContent(&target_file)) {
        QMessageBox::information(NULL, "提示", QString("操作的文件%1不是XML文件！").arg(target_filename));
        target_file.close();
        return;
    }
    // 关联后可以关闭文件了
    target_file.close();


    QDomElement target_root = target_doc.documentElement();
    // 获取所有context节点
    QDomNodeList contextList = target_root.elementsByTagName("context");

    for (int i = 0; i < contextList.size(); i++) {
        QDomElement element = contextList.at(i).toElement();
        // 获取唯一键值
        QString key = element.namedItem("name").firstChild().nodeValue();

        QDomNodeList m_list = element.elementsByTagName("message");
        for (int j = 0; j < m_list.count(); j++) {
            // 获取链表中的值
            QDomElement m_element = m_list.at(j).toElement();
            QDomNode source = m_element.namedItem("source");
            QDomNode translation = m_element.namedItem("translation");
            QString sourceStr = source.firstChild().nodeValue();
            QString translationStr = translation.firstChild().nodeValue();


            QList<QStringList> oriQstrList = origin_map[key];   // 根据键值，获取相应数据
            for (int j = 0; j < oriQstrList.size(); j++) {
                QStringList origin_message_strList = oriQstrList.at(j);
                QString origin_source = origin_message_strList.front();     // 获取源文件的sourec项
                QString origin_translation = origin_message_strList.back(); // 获取源文件的translation项

                qDebug() << origin_source << " == " << sourceStr;
                // 两边source相等
                if (origin_source == sourceStr) {
                    // 删除子属性
                    m_element.removeChild(translation);

                    // 在添加回来，不这样做，有些会没法成功
                    QDomElement description = target_doc.createElement("translation");	// 创建节点
                    QDomText text = target_doc.createTextNode(origin_translation);	// 创建节点尖括号间的值
                    description.appendChild(text);	// 尖括号间的值的对象被添加到2级子节点中
                    m_element.appendChild(description);	// 2级子节点被添加到1级子节点中
                    break;
                }
            }
        }
    }


    if (!target_file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", QString("文件%1打开失败！").arg(target_filename));
        return;
    }

    // 输出到文件
    QTextStream stream(&target_file);
    target_doc.save(stream, 4);	// 缩进4格
    target_file.close();
}


// 这是一个有问题的优化，只要有一条没有适配，该key项内的所有子项都没有拷贝过来，待优化...
void Translation::TsTransitionTs_2(QString origin_filename, QString target_filename)
{
    QMap<QString, QList<QStringList>> origin_map = getTranslationXML(origin_filename);
    if (0 >= origin_map.size()) {
        QMessageBox::information(NULL, "提示", QString("文件 %1 文件读取有误！")
                                 .arg(origin_filename));
        return;
    }


    // 打开文件
    QFile target_file(target_filename);
    if (!target_file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", QString("文件%1打开失败！").arg(target_filename));
        return;
    }


    QDomDocument target_doc;
    // 将doc与file关联起来
    // 这个函数从IO设备dev中读取XML文档，如果内容被成功解析，则返回true;否则返回false。
    if (!target_doc.setContent(&target_file)) {
        QMessageBox::information(NULL, "提示", QString("操作的文件%1不是XML文件！").arg(target_filename));
        target_file.close();
        return;
    }
    // 关联后可以关闭文件了
    target_file.close();


    QDomElement target_root = target_doc.documentElement();
    // 获取所有context节点
    QDomNodeList contextList = target_root.elementsByTagName("context");

    for (int i = 0; i < contextList.size(); i++) {
        QDomElement element = contextList.at(i).toElement();
        // 获取唯一键值
        QString key = element.namedItem("name").firstChild().nodeValue();

        QList<QStringList> oriQstrList = origin_map[key];   // 根据键值，获取相应数据

        int origin_index = 0;
        int target_index = 0;

        QDomNodeList m_list = element.elementsByTagName("message");
        if (oriQstrList.size() != m_list.count()) {
            ui->textEdit->append(QString("key: %1").arg(key));
            ui->textEdit->append(QString("源文件当前key的大小: %1   目标文件当前key的大小: %2")
                                 .arg(oriQstrList.size()).arg(m_list.count()));
            qDebug() << "kye: " << key;
            qDebug() << "oriQstrList.size(): " << oriQstrList.size() << "  m_list.count(): " << m_list.count();
        }
        for (int j = 0; j < m_list.count(); j++) {
index_:
            // 获取链表中的值
            QDomElement m_element = m_list.at(j + target_index).toElement();
            QDomNode source = m_element.namedItem("source");
            QDomNode translation = m_element.namedItem("translation");
            QString sourceStr = source.firstChild().nodeValue();
            QString translationStr = translation.firstChild().nodeValue();

            QStringList origin_message_strList = oriQstrList.at(j + origin_index);
            QString origin_source = origin_message_strList.front();     // 获取源文件的sourec项
            QString origin_translation = origin_message_strList.back(); // 获取源文件的translation项

            // 两边source相等
            if (origin_source == sourceStr) {
                // 删除子属性
                m_element.removeChild(translation);

                // 在添加回来，不这样做，有些会没法成功
                QDomElement description = target_doc.createElement("translation");	// 创建节点
                QDomText text = target_doc.createTextNode(origin_translation);	// 创建节点尖括号间的值
                description.appendChild(text);	// 尖括号间的值的对象被添加到2级子节点中
                m_element.appendChild(description);	// 2级子节点被添加到1级子节点中
                //break;
            }
            else
            {
               ui->textEdit->append(QString("%1 != %2；已做处理").arg(origin_source).arg(sourceStr));
               qDebug() << origin_source << " != " << sourceStr << "；已做处理";
                // 根据谁大，谁的的索引值需要加1
               if (oriQstrList.size() > m_list.count()) {
                   origin_index++;
               } else {
                   target_index++;
               }
               goto index_; // 从新执行一遍逻辑
            }
        }
    }


    if (!target_file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", QString("文件%1打开失败！").arg(target_filename));
        return;
    }

    // 输出到文件
    QTextStream stream(&target_file);
    target_doc.save(stream, 4);	// 缩进4格
    target_file.close();
}


// ts转excel
void Translation::on_btnTsTExcel_clicked()
{
    ui->labelText->setText("ts数据读取写入excel文件");

    m_xlsx.setColumnWidth(1, 100);
    m_xlsx.setColumnWidth(2, 100);

    QMap<QString, QList<QStringList>> translationXML = getTranslationXML("translation_zh_cn.ts");
    qDebug() << "size = " << translationXML.size();
    if (0 >= translationXML.size()) {
        qDebug() << "xml可能没有数据...";
        return;
    }

    writeExcel(translationXML);

    m_xlsx.saveAs("Text.xlsx");
}

// excel转ts
void Translation::on_btnExcelTTs_clicked()
{
    ui->labelText->setText("excel数据读取写入ts文件");

    QMap<QString, QList<QStringList>> translationExcel = getTranslationExcel("Text.xlsx");
    qDebug() << "size = " << translationExcel.size();
    if (0 >= translationExcel.size()) {
        qDebug() << "excel可能没有数据...";
        return;
    }

    writeXML(translationExcel);
}

// ts 转 ts n^3
void Translation::on_btnTsTTs_clicked()
{
    ui->labelText->setText("正在转化，请稍等...");
    TsTransitionTs("translation_zh_cn_origin.ts", "translation_zh_cn_target.ts");
    ui->labelText->setText("ts转ts结束，请查看 translation_zh_cn_target.ts 文件。");
    ui->textEdit->append("未优化：ts转ts完成，请查看 translation_zh_cn_target.ts 文件");
}

// ts 转 ts n^2
void Translation::on_btnTsTTs_2_clicked()
{
    ui->labelText->setText("正在转化，请稍等...");
    TsTransitionTs_2("translation_zh_cn_origin.ts", "translation_zh_cn_target.ts");
    ui->labelText->setText("ts转ts结束，请查看 translation_zh_cn_target.ts 文件。");
    ui->textEdit->append("已优化：ts转ts完成，请查看 translation_zh_cn_target.ts 文件");
}
