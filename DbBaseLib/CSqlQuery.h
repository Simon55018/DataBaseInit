#ifndef _CSQL_QUERY_H
#define _CSQL_QUERY_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QMutex>
#include <QSharedPointer>

#define REPL(X) QString(X).replace("\'","\'\'")

class CDbManager;
class CSqlQuery : public QSqlQuery
{
public:
    enum QueryType
    {
        emMain  //主数据库

    };

public:
    CSqlQuery(QString fileName, QueryType type = emMain);
	virtual ~CSqlQuery();

    static QSharedPointer<CSqlQuery> transactionQuery(QString fileName, bool allDatabase);

	//关闭数据库，之后再次构造查询时自动重新连接
	static void closeDbConnection();

public:
	QString GetSqlErrCodeStr();

	bool InsertQuery(const QString& sTableName, const QString& sValues,  const QString& sFields = QString());
	bool DeleteQuery(const QString& sTableName, const QString& sWheCondition = QString());
	bool UpdateQuery(const QString& sTableName, const QString& sSetFields, const QString& sWheCondition = QString());
	bool SelectQuery(const QString& sFields, const QString& sTableName, const QString& sWheCondition = QString(), 
					 const QString& sOrderCondition = QString(), int nLimNum = -1, int nOffNum = -1);
    //筛选某个字段不重复数据
    bool SelectDistinct(const QString &sFields, const QString &sTableName, const QString &sCondition);
	//beginTransaction 与 endTransaction 必须成对调用， comit，rollback位于beginTransaction与endTransaction之间
	bool beginTransaction();
	bool commit();
	bool rollback();
	void endTransaction();

    //thread lock
    void lockQuery();
    void unlockQuery();

private:
    CDbManager* getDbManager(QString filename, QueryType type );

    static QMutex	m_gMutexMain;

    QMutex*			m_pMutex;
    bool			m_bIsMutexLock;	//是否曾调用过beginTransaction为互斥上锁

    QueryType		m_queryType;
    bool			m_bAllDbTransaction;	//多数据库时此标志有效

    QString         m_fileName;
};

#endif
