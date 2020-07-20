#include "CDbMain.h"

#include <QCoreApplication>
#include <QString>
#include <assert.h>
#include "zsqlitecipherdriver.h"

CDbMain* CDbMain::m_pThis = NULL;

CDbMain* CDbMain::getInstance(QString filename)
{
	if (!m_pThis)
        m_pThis = new CDbMain(filename);

	return m_pThis;
}

void CDbMain::deleteInstance()
{
	delete m_pThis;
	m_pThis = NULL;
}

CDbMain::CDbMain(QString filename)
    : m_csConnectionName("DB_Main"), m_dbFileName(filename)
{
    ConnectDB(filename);
}

CDbMain::~CDbMain()
{
	DisConnectDB();
}

QSqlDatabase CDbMain::GetDataBase()
{
	QSqlDatabase db = QSqlDatabase::database(m_csConnectionName);

	return db;
}

bool CDbMain::ConnectDB(QString fileName)
{
	if (QSqlDatabase::database(m_csConnectionName).isOpen())
		return true;

	//添加数据库驱动、设置数据库名称、数据库登录用户名、密码
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_csConnectionName);
    ZSQLiteCipherDriver *driver = new ZSQLiteCipherDriver();
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, m_csConnectionName);

    QString sDbFile = QCoreApplication::applicationDirPath() + "/database/" + fileName;
	db.setDatabaseName(sDbFile);
    db.setPassword("test");

	//打开数据库
	if(!db.open())
	{
		assert(0);
		return false;
	}

	return true;
}

void CDbMain::DisConnectDB()
{
    QString sDbName;
    {
        sDbName = QSqlDatabase::database().connectionName();
        QSqlDatabase::database(sDbName).close();
    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
    QSqlDatabase::removeDatabase(sDbName);
}


