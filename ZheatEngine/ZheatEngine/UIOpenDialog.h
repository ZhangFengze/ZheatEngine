#pragma once

#include <QString>
#include <QDialog>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QIcon>
#include <QDialogButtonBox>

class UIOpenDialog : public QDialog
{
	Q_OBJECT

public:
	UIOpenDialog(QWidget *parent = Q_NULLPTR);
	~UIOpenDialog();

public:
	int ProcessId();

	QString ProcessName();

	QIcon Icon();

private:
	int pid = -1;
	QString processName;
	QIcon icon;

	// without ownership
	QVBoxLayout* layout;
	QTreeWidget* tree;
	QDialogButtonBox* buttonBox;

};