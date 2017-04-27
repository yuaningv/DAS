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

class CFileOperationManager
{
public:
    CFileOperationManager(const QString& filePath);
    ~CFileOperationManager();

    // 读取xml文件
    bool ReadXmlFile();

    // 写xml文件
    bool writeXmlFile();


	//打开xml文件
	bool OpenFile();

	//获取读取文件来源标记：TRUE-XLS，FALSE-二进制文件
	bool GetFromXLSFileFlag();

private:
	QDomDocument *m_doc;
	QFile *m_file;
	QString m_strFilePath;
};

#endif