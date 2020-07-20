/* CDatabaseInit.cpp */

#include "CDatabaseInit.h"

#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>
#include <QSqlError>


#define DB_FILE_PATCH   "database"
#define DB_FILE_NAME    "Regnt.db"

CDatabaseInit::CDatabaseInit()
{

}


CDatabaseInit::~CDatabaseInit()
{

}


bool CDatabaseInit::createFile(QString fileName)
{
    QString sDBPath = QCoreApplication::applicationDirPath();
    QString sDBName = sDBPath + "/" + DB_FILE_PATCH + "/" + fileName;
    QFile file(sDBName);
	if (file.exists())
	{
		if (!file.remove())
		{
            qDebug()<<QString("Fail to remove the old '%1' file!").arg(DB_FILE_NAME).toAscii();
			return false;
		}
	}

	//创建数据库文件
    m_fileName = fileName;
    CSqlQuery query(fileName);

	return true;
}


bool CDatabaseInit::createTable()
{
    QString sFilePath = QCoreApplication::applicationDirPath();
    QString sFileName = "/database/CreateTables.sql";
	QFile file(sFilePath+sFileName);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		qDebug()<<"Fail to open the 'CreateTables.sql' file!";
		return false;
	}
	QString sSQL = file.readAll(); 
	QStringList tableList = sSQL.split(";", QString::SkipEmptyParts);
	tableList.removeLast(); //去掉最后一个空字符串

    CSqlQuery query(m_fileName);
	bool bRet = query.beginTransaction();
	if (bRet)
	{
		foreach (QString strSql, tableList)
		{
			bRet &= query.exec(strSql);
        }

		if (bRet)
			bRet = query.commit();

		if (!bRet)
			query.rollback();
	}
	query.endTransaction();

	return bRet;
}


bool CDatabaseInit::initData()
{
	QStringList dataList;
	if (!parseXml(dataList))
		return false;

    CSqlQuery query(m_fileName);
	bool bRet = query.beginTransaction();
	if (bRet)
	{
		foreach (QString strSql, dataList)
		{
			bRet &= query.exec(strSql);
		}

		if (bRet)
			bRet = query.commit();

		if (!bRet)
			query.rollback();
	}
	query.endTransaction();

	return bRet;
}


bool CDatabaseInit::parseXml(QStringList& lst)
{
    QString sFilePath = QCoreApplication::applicationDirPath();
    QString sFileName = "/database/DatabaseInit.xml";
	QFile file(sFilePath+sFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug()<<"Fail to open the 'DatabaseInit.xml' file!";
		return false;
	}

	QXmlStreamReader xmlReader;
	xmlReader.setDevice(&file);
	while (!xmlReader.atEnd())
	{
		if (xmlReader.isStartElement() && xmlReader.name() == "sql")
		{
			QString sElm = xmlReader.readElementText();
			lst.append(sElm);
		}
		xmlReader.readNextStartElement();

		//Error Check
		if (xmlReader.error() != QXmlStreamReader::NoError &&
			xmlReader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
		{
			QXmlStreamReader::Error e = xmlReader.error();
			qDebug()<< "An error occurred while parsing the file 'DatabaseInit.xml'";
			qDebug()<< "Error = "<< xmlReader.errorString() 
					<< "on line "<< xmlReader.lineNumber()
					<< "column " << xmlReader.columnNumber();

			return false;
		}
	}

	return true;
}
