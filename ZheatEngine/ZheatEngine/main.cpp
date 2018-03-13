#include <QApplication>

#include "UIMain.h"
#include "Log.h"
#include "LoadDebugPrivilege.h"

int main(int argc, char *argv[])
{
	if (!LoadDebugPrivilege())
		ErrorExit("LoadDebugPrivilege");

	QApplication a(argc, argv);
	UIMain m;
	m.show();
	return a.exec();
}
