#pragma once

#include <QString>
#include <QDebug>

inline void LogInfo(const QString& description)
{
	qDebug() << "Info: " << description;
}

inline void LogWarning(const QString& description)
{
	qDebug() << "Warning: " << description;
}

inline void LogError(const QString& description)
{
	qDebug() << "Error: " << description;
}

inline void ErrorExit(const QString& description)
{
	LogError(description);
	// for debugger
	throw 1;
	//exit(-1);
}
