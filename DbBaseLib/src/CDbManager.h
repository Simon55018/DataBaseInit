#ifndef __CDbManager_H__
#define __CDbManager_H__

#include <QSqlDatabase>
#include "../CSqlQuery.h"

class CDbManager
{
public:

    virtual CSqlQuery::QueryType Type() = 0;

    virtual bool ConnectDB(QString fileName) = 0;
	
	virtual QSqlDatabase GetDataBase() = 0;

	virtual void DisConnectDB() = 0;
};
#endif
