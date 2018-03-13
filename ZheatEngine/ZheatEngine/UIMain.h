#pragma once

#include <memory>

#include <QMainWindow>
#include <QDebug>
#include <QTreeWidgetItem>

#include "Session.h"
#include "UIParamWidget.h"
#include "UIResultTreeWidgetItem.h"
#include "UIWatchListItem.h"
#include "ui_UIMain.h"

class UIMain : public QMainWindow
{
	Q_OBJECT

public:
	UIMain(QWidget *parent = Q_NULLPTR);

private:
	void OnOpenDialog();

	void OnFirstScan();

	void OnNewScan();

	void OnNextScan();

	void OnScanValueTypeChanged();

	void OnScanTypeChanged();

	void OnResultItemDoubleClicked(QTreeWidgetItem *item,int column);

	void OnWatchListItemDoubleClicked(QTreeWidgetItem* item, int column);

private:

	void SwitchNewScanFirstScanButton(bool toFirstScan);

	void SetScanOptionsEnable(bool);

	void UpdateResults();

	void UpdateResultsValues();

	void ChangeResultItemValue(QTreeWidgetItem* item);

	void UpdateWatchList();

	void AddToWatchList(QTreeWidgetItem* item);

	void ChangeWatchListItemValueType(UIWatchListItem* item);

	void ChangeWatchListItemValue(UIWatchListItem* item);

	void ChangeWatchListItemDescription(UIWatchListItem* item);

	Scan::ValueType ValueType();

	int InitialScanType();

	int ScanType();

	ScanOption Option();

	QList<QVariant> Param();

	bool HasSession() const;

	bool HasScan() const;

private:
	Ui::UIMainClass ui;
	std::unique_ptr<Session> session;

	// without ownership
	UIParamWidget* paramWidget = nullptr;
};
