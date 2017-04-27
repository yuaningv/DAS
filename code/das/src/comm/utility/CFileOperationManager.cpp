#include "CFileOperationManager.h"
#include "QtCore/QDebug"

CFileOperationManager::CFileOperationManager(const QString& filePath)
{
    m_strFilePath = filePath;
    if (filePath.isEmpty())
	{
        m_strFilePath = QString("");
	}
	m_doc = new QDomDocument();
    m_file = new QFile(m_strFilePath);
}

bool CFileOperationManager::OpenFile()
{	
	if (!m_file->open(QIODevice::ReadOnly)) 
		return false;
	if (!m_doc->setContent(m_file))
	{
		m_file->close();
		return false;
	}
	m_file->close();
	return true;
}

CFileOperationManager::~CFileOperationManager()
{

}

// XML文件读取
bool CFileOperationManager::ReadXmlFile()
{
    if (!m_file->open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!m_doc->setContent(m_file))
    {
        m_file->close();
        return false;
    }
    m_file->close();

    QDomElement docElem = m_doc->documentElement();

    QDomNode n = docElem.firstChild();
    while (!n.isNull()) 
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull()) 
        {
            qDebug() << e.tagName(); // the node really is an element.
            QDomNode tmpNode = e.firstChild();
            while (!tmpNode.isNull())
            {
                QDomElement tmpElement = tmpNode.toElement();
                qDebug() << tmpElement.attribute("value");
                tmpNode = tmpNode.nextSibling();
            }
        }
        n = n.nextSibling();
    }

    return true;
}

// 写XML文件
bool CFileOperationManager::writeXmlFile()
{

    return true;
}


bool CFileOperationManager::GetFromXLSFileFlag()
{
	QDomElement root = m_doc->documentElement();
	QDomElement child = root.elementsByTagName("READFILE").item(0).firstChildElement("FromXLSFileFlag");
	if (child.attribute("value") == "TRUE")
	{
		return true;
	}
	else if (child.attribute("value") == "FALSE")
	{
		return false;
	}
	return false;
}

