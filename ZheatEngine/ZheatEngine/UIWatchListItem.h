#pragma once

#include <QTreeWidgetItem>
#include <QWidget>
#include <QString>

#include <Windows.h>

#include "Scan.h"

class UIWatchListItem : public QTreeWidgetItem
{
public:
	UIWatchListItem(Scan::ValueType, void* baseAddress, const QList<ULONGLONG>& offsets, const QString& description);

	void Update(HANDLE process);

	Scan::ValueType ValueType() const;
	void SetValueType(Scan::ValueType);

	void* NowAddress(HANDLE process);

	QString Description() const;
	void SetDescription(const QString&);

private:
	Scan::ValueType valueType;
	void* baseAddress;
	QList<ULONGLONG> offsets;
};
