#include <QCoreApplication>
#include <QDebug>
#include <iostream>


#include "./Src/CDatabaseInit.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

	std::string sCin;
    std::cout << "Initializes the database, whether or not to continue?(y/n)" << std::endl;
	std::cin >> sCin;
	if (sCin != "y" && sCin != "Y")
	{
		return 0;
	}

    int match = 0;
    QString pattern("[\\\\/:*?\"<>|]");
    QRegExp rx(pattern);
    QString filename;
    do {
        std::cout << "Please enter the name of database: ";
        sCin.clear();
        filename.clear();
        std::cin >> sCin;

        filename = QString::fromStdString(sCin);
        match = filename.indexOf(rx);
        if( match >= 0 )
        {
            std::cout << "The filename cannot contain any of the following characters: \\/:*?\"<>|" << std::endl;
        }
    } while(match >= 0);

    filename.append(".db");
/********************************* Begin Init ****************************************/
	std::cout << "Please wait while Setup initializes the database..."<< std::endl;

	CDatabaseInit dbInit;

	do 
	{
		//创建数据库文件
        if (!dbInit.createFile(filename))
        {
            std::cout << "Fail to create the database file!";
            break;
        }

		//创建数据库表
        if (!dbInit.createTable())
        {
            std::cout << "Fail to create the database table!";
            break;
        }

		//导入初始数据
        if (!dbInit.initData())
        {
            std::cout << "Fail to init data!";
            break;
        }

		std::cout << "Database initialization is complete ...";

	} while (0);
	

	std::cout << std::endl 
			<< "Press any key to quit!"<< std::endl;


	return 0;
}
