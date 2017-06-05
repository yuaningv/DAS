/*
* File    : CFileOperationManager.h
*
* Date    : 2017-04-25
*
* Author  :
*
* Brief   : 文件操作管理
*
* Note    :
*/

#ifndef _CFILE_OPERATION_MANAGER_H_
#define _CFILE_OPERATION_MANAGER_H_


#include "QtCore/QFile"
#include "QtXml/QDomDocument"
#include "QtCore/QStringList"
#include "QtCore/QObject"
#include "type.h"

class CFileOperationManager : public QObject
{
    Q_OBJECT

public:
    CFileOperationManager();
    CFileOperationManager(const QString& filePath);
    ~CFileOperationManager();

    // 读取xml文件
    bool ReadXmlFile(QMap<int, QList<WidgetProperty_t>>& mapTmpItems, QString& strPath, QString& strTimeBegin, QString& strTimeEnd);

    bool ReadXmlFile(QList<CurveLine_t>& lstTmpItems);

    // 写xml文件
    bool writeXmlFile(QMap<int, QList<WidgetProperty_t>>& mapTmpItems, const QString& strPath, const QString& strTimeBegin, const QString& strTimeEnd);

    // 导出excel文件
    bool writeExcelFile(const QString& strFileName, const QStringList& slstHeader, QList< QList<QVariant> >& lstExcelData);


private:
	QDomDocument *m_doc;
	QFile *m_file;
	QString m_strFilePath;
};

#endif