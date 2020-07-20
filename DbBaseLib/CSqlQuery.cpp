#include "CSqlQuery.h"
#include "src/CDbMain.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <assert.h>
#include <QDebug>

QMutex CSqlQuery::m_gMutexMain(QMutex::Recursive);

CSqlQuery::CSqlQuery(QString fileName, QueryType type)
    : QSqlQuery(getDbManager(fileName, type)->GetDataBase()), m_queryType(type), m_fileName(fileName)
{
    m_bIsMutexLock = false;
    m_pMutex = &m_gMutexMain;
	m_bAllDbTransaction = false;
}

CSqlQuery::~CSqlQuery()
{
	if (m_bIsMutexLock)
		assert(0 && "Check your process, whether or not call endTransaction() after beginTransaction().");
}

void CSqlQuery::closeDbConnection()
{
    CDbMain::deleteInstance();
}

CDbManager* CSqlQuery::getDbManager( QString filename, QueryType type )
{
	switch (type)
	{
    case emMain:
        return CDbMain::getInstance(filename);
	}

	assert (0);
	return NULL;
}

QString CSqlQuery::GetSqlErrCodeStr()
{
	m_pMutex->lock();
    QSqlError qsqlErr = getDbManager(m_fileName, m_queryType)->GetDataBase().lastError();
	m_pMutex->unlock();

	switch(qsqlErr.type())
	{
	case QSqlError::NoError:
		return "No Error!";

	case QSqlError::ConnectionError:
		return "Failed to connect!";

	case QSqlError::StatementError:
		return "Statement error!";

	case QSqlError::TransactionError:
		return "Transaction error!";

	case QSqlError::UnknownError:
		return "Unknown error!";
	}

	return QString();
}

bool CSqlQuery::InsertQuery( const QString& sTableName, const QString& sValues, const QString& sFields )
{
	QString sQuery;
	if (!sFields.isEmpty())
	{
		sQuery = QString("insert into %1(%2) values(%3);").arg(sTableName).arg(sFields).arg(sValues);
	}
	else
	{
		sQuery = QString("insert into %1 values(%2);").arg(sTableName).arg(sValues);
	}

	m_pMutex->lock();
	bool bRet = exec(sQuery);
	m_pMutex->unlock();

	return bRet;
}

bool CSqlQuery::DeleteQuery( const QString& sTableName, const QString& sWheCondition )
{
	QString sQuery;
	if (!sWheCondition.isEmpty())
	{
		sQuery = QString("delete from %1 where %2;").arg(sTableName).arg(sWheCondition);
	}
	else
	{
		sQuery = QString("delete from %1;").arg(sTableName);
	}

	m_pMutex->lock();
	bool bRet = exec(sQuery);
	m_pMutex->unlock();

	return bRet;
}

bool CSqlQuery::UpdateQuery( const QString& sTableName, const QString& sSetFields, const QString& sWheCondition )
{
	if (sSetFields.isEmpty())
	{
		return true;
	}

	QString sQuery;
	if (!sWheCondition.isEmpty())
	{
		sQuery = QString("update %1 set %2 where %3;").arg(sTableName).arg(sSetFields).arg(sWheCondition);
	}
	else
	{
		sQuery = QString("update %1 set %2;").arg(sTableName).arg(sSetFields);
	}

	m_pMutex->lock();
	bool bRet = exec(sQuery);
	m_pMutex->unlock();

	return bRet;
}

bool CSqlQuery::SelectQuery( const QString& sFields, const QString& sTableName, const QString& sWheCondition 
							 , const QString& sOrderCondition, int nLimNum, int nOffNum)
{
	QString sQuery;
	if (!sWheCondition.isEmpty())
	{
		sQuery = QString("select %1 from %2 where %3").arg(sFields).arg(sTableName).arg(sWheCondition);
	}
	else
	{
		sQuery = QString("select %1 from %2").arg(sFields).arg(sTableName);
	}

	if (!sOrderCondition.isEmpty())
	{
		sQuery.append(" ").append(QString("order by %3").arg(sOrderCondition));
	}

	if (nLimNum!=-1 && nOffNum != -1)
	{
		sQuery.append(" ").append(QString("limit %1 offset %2").arg(nLimNum).arg(nOffNum));
	}

	m_pMutex->lock();
	bool bRet = exec(sQuery);
	m_pMutex->unlock();

	return bRet;
}

bool CSqlQuery::SelectDistinct(const QString &sFields, const QString &sTableName, const QString &sCondition)
{
    QString sQuery;
    if(sCondition.isEmpty())
    {
        sQuery = QString("select distinct %1 from %2").arg(sFields).arg(sTableName);
    }
    else
    {
        sQuery = QString("select distinct %1 from %2 where %3").arg(sFields).arg(sTableName).arg(sCondition);
    }

    m_pMutex->lock();
    bool bRet = exec(sQuery);
    m_pMutex->unlock();

    return bRet;
}

bool CSqlQuery::beginTransaction()
{
    if (m_bIsMutexLock)
        assert (0 && "Check your process, whether already called beginTransaction() before there.");

	m_pMutex->lock();
    bool bRet = getDbManager(m_fileName, m_queryType)->GetDataBase().transaction();
	m_bIsMutexLock = true;

// 	if (m_bAllDbTransaction)
// 	{
// 		assert (CDbManager::emECMain == m_queryType);
// 
// 		m_gMutexMainAd.lock();
// 		bRet &= getDbManager(CDbManager::emAdData)->GetDataBase().transaction();
// 	}

	return bRet;
}

bool CSqlQuery::commit()
{
	if (!m_bIsMutexLock)
		assert (0 && "Check your process, whether or not call beginTransaction() before there.");

    bool bRet = getDbManager(m_fileName, m_queryType)->GetDataBase().commit();
// 	if (m_bAllDbTransaction)
// 	{
// 		bRet &= getDbManager(CDbManager::emAdData)->GetDataBase().commit();
// 	}

	return bRet;
}

bool CSqlQuery::rollback()
{
	if (!m_bIsMutexLock)
		assert (0 && "Check your process, whether or not call beginTransaction() before there.");

    bool bRet = getDbManager(m_fileName, m_queryType)->GetDataBase().rollback();
// 	if (m_bAllDbTransaction)
// 	{
// 		bRet &= getDbManager(CDbManager::emAdData)->GetDataBase().rollback();
// 	}

	return bRet;
}

void CSqlQuery::endTransaction()
{
	m_pMutex->unlock();
// 	if (m_bAllDbTransaction)
// 	{
// 		m_gMutexMainAd.unlock();
// 	}
    m_bIsMutexLock = false;
}

//thread lock
void CSqlQuery::lockQuery()
{
     m_pMutex->lock();
}

void CSqlQuery::unlockQuery()
{
    m_pMutex->unlock();
}

QSharedPointer<CSqlQuery> CSqlQuery::transactionQuery(QString fileName, bool allDatabase )
{
    QSharedPointer<CSqlQuery> query = QSharedPointer<CSqlQuery>(new CSqlQuery(fileName, CSqlQuery::emMain));
	query->m_bAllDbTransaction = allDatabase;

	return query;
}
