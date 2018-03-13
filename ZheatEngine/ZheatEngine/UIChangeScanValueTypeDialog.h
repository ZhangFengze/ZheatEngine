#pragma once

#include <QDialog>

#include "Scan.h"

class UIChangeScanValueTypeDialog : public QDialog
{
	Q_OBJECT

public:
	UIChangeScanValueTypeDialog(Scan::ValueType default,QWidget *parent=Q_NULLPTR);

	Scan::ValueType ValueType() const { return valueType; }

private:
	Scan::ValueType valueType;
};
