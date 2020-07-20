#ifndef __CDBECMAIN_H__
#define __CDBECMAIN_H__

#include "CDbManager.h"


//主数据库管理
class CDbMain : public CDbManager
{
    static CDbMain* m_pThis;
public:
    static CDbMain* getInstance(QString filename);

	static void deleteInstance();

public:
    virtual ~CDbMain();

    virtual CSqlQuery::QueryType Type(){ return CSqlQuery::emMain; }

	virtual QSqlDatabase GetDataBase();

    virtual bool ConnectDB(QString fileName);

	virtual void DisConnectDB();

private:
    CDbMain(QString filename);
    Q_DISABLE_COPY(CDbMain)
	
private:
	const QString	m_csConnectionName;
    QString         m_dbFileName;
};
#endif
