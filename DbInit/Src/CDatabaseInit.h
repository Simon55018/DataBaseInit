/* CDatabaseInit.h */

#ifndef __CDATABASEINIT_H__
#define __CDATABASEINIT_H__

#include <QStringList>
#include "../DbBaseLib/CSqlQuery.h"

class CDatabaseInit
{
public:
	CDatabaseInit();
	~CDatabaseInit();

public:
	//创建数据库文件
    bool createFile(QString fileName);
	//创建数据库表
	bool createTable();
	//写入初始数据
	bool initData();

protected:
	//解析XML数据
	bool parseXml(QStringList& lst);

private:
    CSqlQuery*  m_sqlQuery;
    QString     m_fileName;
};

#endif//__CDATABASEINIT_H__
