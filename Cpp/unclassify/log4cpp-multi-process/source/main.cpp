
#include <log4cpp/Category.hh>

#include <log4cpp/Appender.hh>

#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Appender.hh>



#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include"log4cpp/Category.hh"
#include"log4cpp/OstreamAppender.hh"
#include"log4cpp/BasicLayout.hh"
#include"log4cpp/Priority.hh"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
void child_process();
void paraent_process();
using namespace std;
int main()
{

	

	pid_t ret = fork();
	if (ret < 0)
	{
	
	}
	else if (ret == 0)
	{
		child_process();
	}
	else 
	{
		printf("paraent.");
		// return 0;	
	}

	paraent_process();

	return 0;
}

void child_process()
{

	log4cpp::PatternLayout* pLayout1 = new log4cpp::PatternLayout();
	pLayout1->setConversionPattern("child %d: %p %c%x: %m%n");


	log4cpp::PatternLayout* pLayout2 = new log4cpp::PatternLayout();
	pLayout2->setConversionPattern("child %d: %p %c%x: %m%n");
	log4cpp::Appender* fileAppender = new log4cpp::FileAppender("fileAppender","wxb.log");

	fileAppender->setLayout(pLayout1);

	log4cpp::RollingFileAppender* rollfileAppender = new log4cpp::RollingFileAppender( "rollfileAppender","rollwxb.log", 10*1024,1);

	rollfileAppender->setLayout(pLayout2);

	 

	log4cpp::Category& root =log4cpp::Category::getRoot().getInstance("RootName");

	root.addAppender(fileAppender);

	root.addAppender(rollfileAppender);

	root.setPriority(log4cpp::Priority::DEBUG);


	for (int i = 0; i < 100; i++)

	{

		 string strError;

		 ostringstream oss;

		 oss<<i<<":RootError Message!";

		 strError = oss.str();

		 root.error(strError);

	}

	while (1) ;

}

void paraent_process()
{
	printf("%s:%d", __FUNCTION__, __LINE__);

	log4cpp::PatternLayout* pLayout1 = new log4cpp::PatternLayout();

	pLayout1->setConversionPattern("main %d: %p %c%x: %m%n");

	 

	log4cpp::PatternLayout* pLayout2 = new log4cpp::PatternLayout();

	pLayout2->setConversionPattern("main %d: %p %c%x: %m%n");

	 

	log4cpp::Appender* fileAppender = new log4cpp::FileAppender("fileAppender","wxb.log");

	fileAppender->setLayout(pLayout1);

	 

	log4cpp::RollingFileAppender* rollfileAppender = new log4cpp::RollingFileAppender( "rollfileAppender","rollwxb.log", 10*1024,1);

	rollfileAppender->setLayout(pLayout2);

	 

	log4cpp::Category& root =log4cpp::Category::getRoot().getInstance("RootName");

	root.addAppender(fileAppender);

	root.addAppender(rollfileAppender);

	root.setPriority(log4cpp::Priority::DEBUG);

	for (int i = 0; i < 100; i++)

	{

		 string strError;

		 ostringstream oss;

		 oss<<i<<":RootError Message!";

		 strError = oss.str();

		 root.error(strError);

	}

	while(1) ;
}



