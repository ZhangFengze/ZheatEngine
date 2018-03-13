#include <thread>

#include <QMessageBox>
#include <QGroupBox>
#include <QComboBox>
#include <QObject>
#include <QTimer>
#include <QInputDialog>

#include "Log.h"
#include "HelperFunctions.h"
#include "ScanNBytes.h"
#include "Settings.h"
#include "UIMain.h"
#include "UIOpenDialog.h"
#include "UIParamWidget.h"
#include "UIResultTreeWidgetItem.h"
#include "UIChangeValueDialog.h"
#include "UIWatchListItem.h"
#include "UIChangeScanValueTypeDialog.h"

UIMain::UIMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.buttonOpen, &QPushButton::clicked, this, &UIMain::OnOpenDialog);
	connect(ui.buttonNextScan, &QPushButton::clicked, this, &UIMain::OnNextScan);

	//
	for (auto valueType : ScanValueTypes)
		ui.comboBoxValueType->addItem(ScanValueTypeToString(valueType), (int)valueType);
	ui.comboBoxValueType->setCurrentIndex(2);// default is 4 Bytes
	void (QComboBox::*currentIndexChanged)(int) = &QComboBox::currentIndexChanged;
	connect(ui.comboBoxValueType, currentIndexChanged, this, &UIMain::OnScanValueTypeChanged);
	OnScanValueTypeChanged();

	//
	connect(ui.comboBoxScanType, currentIndexChanged, this, &UIMain::OnScanTypeChanged);
	OnScanTypeChanged();

	//
	ui.treeWidgetResults->setColumnCount(3);
	ui.treeWidgetResults->setHeaderLabels({ "Address","Now Value","Value At Scan" });
	connect(ui.treeWidgetResults, &QTreeWidget::itemDoubleClicked, this, &UIMain::OnResultItemDoubleClicked);
	UpdateResults();

	//
	QTimer* timerUpdateResultsValues = new QTimer(this);

	timerUpdateResultsValues->setInterval(gUIRefreshResultsInterval);
	connect(timerUpdateResultsValues, &QTimer::timeout, this, &UIMain::UpdateResultsValues);
	//connect(this, &QObject::destroyed,
	//	[timerUpdateResultsValues]()
	//{
	//	timerUpdateResultsValues->stop();
	//});
	timerUpdateResultsValues->start();

	//
	ui.treeWidgetWatchList->setColumnCount(4);
	ui.treeWidgetWatchList->setHeaderLabels({ "Type","Address","Value","Description" });
	connect(ui.treeWidgetWatchList, &QTreeWidget::itemDoubleClicked, this, &UIMain::OnWatchListItemDoubleClicked);

	//
	QTimer* timerUpdateWatchList = new QTimer(this);

	timerUpdateWatchList->setInterval(gUIRefreshWatchListInterval);
	connect(timerUpdateWatchList, &QTimer::timeout, this, &UIMain::UpdateWatchList);
	timerUpdateWatchList->start();
}

void UIMain::OnOpenDialog()
{
	UIOpenDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto pid = dialog.ProcessId();
		if (pid != -1)
		{
			LogInfo(QString("New Session of PID: %1").arg(pid));
			session.reset(Session::NewSession(pid));
			if (session.get() == nullptr)
			{
				QMessageBox::warning(this, "WARNING", "OpenProcess Failed");
				return;
			}
			ui.labelProcessInfo->setText(QString("%1 - %2").arg(pid).arg(dialog.ProcessName()));
			auto& icon = dialog.Icon();
			ui.labelIcon->setPixmap(icon.pixmap(icon.actualSize(ui.labelIcon->size())));

			OnNewScan();
			UpdateResults();
		}
	}
}

void UIMain::OnFirstScan()
{
	session->NewScan(ValueType(), Option());

	ui.progressBar->setMaximum(0);
	std::atomic_bool finished = false;
	std::thread t([this, &finished]()
	{
		auto scan = session->Scan();
		scan->InitialScan(InitialScanType(), Param());
		finished = true;
	});
	while (!finished)
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	t.join();
	ui.progressBar->setMaximum(1);

	UpdateResults();
	UpdateResultsValues();

	SwitchNewScanFirstScanButton(false);
	SetScanOptionsEnable(false);
	ui.comboBoxValueType->setEnabled(false);
	OnScanValueTypeChanged();
}

void UIMain::OnNewScan()
{
	session->AbortScan();

	SwitchNewScanFirstScanButton(true);
	SetScanOptionsEnable(true);
	ui.comboBoxValueType->setEnabled(true);
	OnScanValueTypeChanged();

	UpdateResults();
	UpdateResultsValues();
}

void UIMain::OnNextScan()
{
	ui.progressBar->setMaximum(0);
	std::atomic_bool finished = false;
	std::thread t([this, &finished]()
	{
		auto scan = session->Scan();
		scan->NextScan(ScanType(), Param());
		finished = true;
	});
	while (!finished)
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	t.join();
	ui.progressBar->setMaximum(1);

	UpdateResults();
	UpdateResultsValues();
}

void UIMain::OnScanValueTypeChanged()
{
	bool initial = !HasScan();

	void (QComboBox::*currentIndexChanged)(int) = &QComboBox::currentIndexChanged;
	disconnect(ui.comboBoxScanType, currentIndexChanged, this, &UIMain::OnScanTypeChanged);
	switch (ValueType())
	{
	case Scan::ValueType::Scan1Bytes:
	case Scan::ValueType::Scan2Bytes:
	case Scan::ValueType::Scan4Bytes:
	case Scan::ValueType::Scan8Bytes:
	{
		if (initial)
		{
			ui.comboBoxScanType->clear();

			// same InitialScanType, just use Scan4Bytes:InitialScanType here
#define addScanType(what) ui.comboBoxScanType->addItem(#what,(int)Scan4Bytes::InitialScanType::what);
			addScanType(Equal);
			addScanType(NotEqual);
			addScanType(GreaterThan);
			addScanType(LessThan);
			addScanType(GreaterThanOrEqualTo);
			addScanType(LessThanOrEqualTo);
			addScanType(Between);
			addScanType(NotBetween);
			addScanType(Unknown);
#undef addScanType
		}
		else
		{
			ui.comboBoxScanType->clear();
#define addScanType(what) ui.comboBoxScanType->addItem(#what,(int)Scan4Bytes::ScanType::what);
			addScanType(Equal);
			addScanType(NotEqual);
			addScanType(GreaterThan);
			addScanType(LessThan);
			addScanType(GreaterThanOrEqualTo);
			addScanType(LessThanOrEqualTo);
			addScanType(Between);
			addScanType(NotBetween);
			addScanType(IncreasedValue);
			addScanType(IncreasedBy);
			addScanType(NotIncreasedBy);
			addScanType(DecreasedValue);
			addScanType(DecreasedBy);
			addScanType(NotDecreasedBy);
			addScanType(ChangedValue);
			addScanType(UnChangedValue);
#undef addScanType
		}
	}
	break;

	case Scan::ValueType::ScanFloat:
	case Scan::ValueType::ScanDouble:
	{
		if (initial)
		{
			ui.comboBoxScanType->clear();

			// same InitialScanType, just use ScanFloat:InitialScanType here
#define addScanType(what) ui.comboBoxScanType->addItem(#what,(int)ScanFloat::InitialScanType::what);
			addScanType(Equal);
			addScanType(NotEqual);
			addScanType(GreaterThan);
			addScanType(LessThan);
			addScanType(GreaterThanOrEqualTo);
			addScanType(LessThanOrEqualTo);
			addScanType(Between);
			addScanType(NotBetween);
			addScanType(Unknown);
#undef addScanType
		}
		else
		{
			ui.comboBoxScanType->clear();
#define addScanType(what) ui.comboBoxScanType->addItem(#what,(int)ScanFloat::ScanType::what);
			addScanType(Equal);
			addScanType(NotEqual);
			addScanType(GreaterThan);
			addScanType(LessThan);
			addScanType(GreaterThanOrEqualTo);
			addScanType(LessThanOrEqualTo);
			addScanType(Between);
			addScanType(NotBetween);
			addScanType(IncreasedValue);
			addScanType(IncreasedBy);
			addScanType(NotIncreasedBy);
			addScanType(DecreasedValue);
			addScanType(DecreasedBy);
			addScanType(NotDecreasedBy);
			addScanType(ChangedValue);
			addScanType(UnChangedValue);
#undef addScanType
		}
	}
	break;

	default:
		ErrorExit("Unknown Scan Value Type");
	}
	connect(ui.comboBoxScanType, currentIndexChanged, this, &UIMain::OnScanTypeChanged);
	OnScanTypeChanged();

}

void UIMain::OnScanTypeChanged()
{
	bool initial = !HasScan();

	if (!paramWidget)
	{
		paramWidget = UIParamWidget::Factory(ValueType(), ScanType(), initial);
		ui.groupParam->layout()->addWidget(paramWidget);
	}
	else
	{
		if (!paramWidget->MatchType(ValueType(), ScanType(), initial))
		{
			ui.groupParam->layout()->removeWidget(paramWidget);
			delete paramWidget;
			paramWidget = UIParamWidget::Factory(ValueType(), ScanType(), initial);
			ui.groupParam->layout()->addWidget(paramWidget);
		}
	}
}

void UIMain::OnResultItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	if (column == 1)
		ChangeResultItemValue(item);
	else
		AddToWatchList(item);
}

void UIMain::OnWatchListItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	auto watchListItem = (UIWatchListItem*)item;

	if (column == 0)
		ChangeWatchListItemValueType(watchListItem);
	// TODO
	//else if (column == 1)
	//	ChangeWatchListItemAddress(item);
	else if (column == 2)
		ChangeWatchListItemValue(watchListItem);
	else if (column = 3)
		ChangeWatchListItemDescription(watchListItem);
}

void UIMain::SwitchNewScanFirstScanButton(bool toFirstScan)
{
	if (toFirstScan)
	{
		ui.buttonNewScan->setText("First Scan");
		ui.buttonNewScan->setEnabled(true);
		connect(ui.buttonNewScan, &QPushButton::clicked, this, &UIMain::OnFirstScan);
		disconnect(ui.buttonNewScan, &QPushButton::clicked, this, &UIMain::OnNewScan);

		ui.buttonNextScan->setEnabled(false);
	}
	else
	{
		ui.buttonNewScan->setText("New Scan");
		ui.buttonNewScan->setEnabled(true);
		disconnect(ui.buttonNewScan, &QPushButton::clicked, this, &UIMain::OnFirstScan);
		connect(ui.buttonNewScan, &QPushButton::clicked, this, &UIMain::OnNewScan);

		ui.buttonNextScan->setEnabled(true);
	}
}

void UIMain::SetScanOptionsEnable(bool b)
{
	ui.lineEditStart->setEnabled(b);
	ui.lineEditStop->setEnabled(b);
	ui.checkBoxAlignment->setEnabled(b);
	ui.checkBoxPause->setEnabled(b);
}

void UIMain::UpdateResults()
{
	if (!HasScan())
	{
		ui.labelFound->setText("Found: 0");
		ui.treeWidgetResults->clear();
		return;
	}

	auto numResults = session->Scan()->NumResults();
	auto shownResults = 0;

	switch (ValueType())
	{
#define caseNBytes(N) \
	case Scan::ValueType::Scan ## N ## Bytes: \
	{\
		auto scan = (Scan ## N ## Bytes*)session->Scan();\
		auto results = scan->Results(gMaxShownResults);\
		shownResults = results.size();\
		ui.treeWidgetResults->clear();\
		for (const auto& r : results)\
			ui.treeWidgetResults->addTopLevelItem(new UIResult ## N ## BytesTreeWidgetItem(r));\
	}\
	break;

		caseNBytes(1);
		caseNBytes(2);
		caseNBytes(4);
		caseNBytes(8);

#undef caseNBytes

#define caseFloatType(type) \
	case Scan::ValueType::Scan ## type ## : \
	{\
		auto scan = (Scan ## type ## *)session->Scan();\
		auto results = scan->Results(gMaxShownResults);\
		shownResults = results.size();\
		ui.treeWidgetResults->clear();\
		for (const auto& r : results)\
			ui.treeWidgetResults->addTopLevelItem(new UIResult ## type ## TreeWidgetItem(r));\
	}\
	break;

		caseFloatType(Float);
		caseFloatType(Double);

#undef caseFloatType

	default:
		ErrorExit("Unknown Scan::ValueType");
	}

	if (shownResults < numResults)
		ui.labelFound->setText(QString("Found: %1 (Shown %2 )").arg(numResults).arg(shownResults));
	else
		ui.labelFound->setText(QString("Found: %1 ").arg(numResults));

}

void UIMain::UpdateResultsValues()
{
	if (!HasScan() || ui.treeWidgetResults->topLevelItemCount() == 0)
		return;

	switch (ValueType())
	{
#define caseNBytes(N) \
	case Scan::ValueType::Scan ## N ## Bytes:\
	{\
		auto visibleRect = ui.treeWidgetResults->visibleRegion().boundingRect();\
		if (visibleRect.isEmpty())\
			break;\
		auto item = (UIResult ## N ## BytesTreeWidgetItem*)ui.treeWidgetResults->itemAt(visibleRect.topLeft());\
		auto end = (UIResult ## N ## BytesTreeWidgetItem*)ui.treeWidgetResults->itemAt(visibleRect.bottomLeft());\
		while (item != end)\
		{\
			item->UpdateNowValue(session->Handle());\
			item = (UIResult ## N ## BytesTreeWidgetItem*)ui.treeWidgetResults->itemBelow(item);\
		}\
	}\
	break;

		caseNBytes(1);
		caseNBytes(2);
		caseNBytes(4);
		caseNBytes(8);

#undef caseNBytes

#define caseFloatType(type) \
	case Scan::ValueType::Scan ## type ## :\
	{\
		auto visibleRect = ui.treeWidgetResults->visibleRegion().boundingRect();\
		if (visibleRect.isEmpty())\
			break;\
		auto item = (UIResult ## type ## TreeWidgetItem*)ui.treeWidgetResults->itemAt(visibleRect.topLeft());\
		auto end = (UIResult ## type ## TreeWidgetItem*)ui.treeWidgetResults->itemAt(visibleRect.bottomLeft());\
		while (item != end)\
		{\
			item->UpdateNowValue(session->Handle());\
			item = (UIResult ## type ## TreeWidgetItem*)ui.treeWidgetResults->itemBelow(item);\
		}\
	}\
	break;

		caseFloatType(Float);
		caseFloatType(Double);

#undef caseFloatType

	default:
		ErrorExit("Unknown Scan::ValueType");
	}
}

void UIMain::ChangeResultItemValue(QTreeWidgetItem * item)
{
	switch (ValueType())
	{
#define caseNBytes(N) \
	case Scan::ValueType::Scan ## N ## Bytes: \
	{\
		UIChangeValue ## N ## BytesDialog dialog(this);\
		if (dialog.exec() == QDialog::Accepted)\
		{\
			auto result = ((UIResult ## N ## BytesTreeWidgetItem*)item)->Result();\
			result.SetValue(session->Handle(), dialog.Value());\
		}\
	}\
	break;

		caseNBytes(1);
		caseNBytes(2);
		caseNBytes(4);
		caseNBytes(8);

#undef caseNBytes

#define caseFloatType(type) \
	case Scan::ValueType::Scan ## type ## : \
	{\
		UIChangeValue ## type ## Dialog dialog(this);\
		if (dialog.exec() == QDialog::Accepted)\
		{\
			auto result = ((UIResult ## type ## TreeWidgetItem*)item)->Result();\
			result.SetValue(session->Handle(), dialog.Value());\
		}\
	}\
	break;

		caseFloatType(Float);
		caseFloatType(Double);

#undef caseFloatType

	default:
		ErrorExit("Unknown Scan::ValueType");
	}
}

void UIMain::UpdateWatchList()
{
	if (!HasSession())
		return;

	for (int i = 0; i < ui.treeWidgetWatchList->topLevelItemCount(); ++i)
	{
		auto item = (UIWatchListItem*)ui.treeWidgetWatchList->topLevelItem(i);
		item->Update(session->Handle());
	}
}

void UIMain::AddToWatchList(QTreeWidgetItem * item)
{
	auto watchListItem=new UIWatchListItem(Scan::ValueType(ValueType()),
		ToAddress(item->text(0)), {}, "");
	ui.treeWidgetWatchList->addTopLevelItem(watchListItem);
}

void UIMain::ChangeWatchListItemValueType(UIWatchListItem * item)
{
	UIChangeScanValueTypeDialog dialog(item->ValueType(), this);
	if (dialog.exec() == QDialog::Accepted)
	{
		item->SetValueType(dialog.ValueType());
	}
}

void UIMain::ChangeWatchListItemValue(UIWatchListItem * item)
{
	switch (item->ValueType())
	{
#define caseNBytes(N) \
	case Scan::ValueType::Scan ## N ## Bytes: \
	{\
		UIChangeValue ## N ## BytesDialog dialog(this);\
		if (dialog.exec() == QDialog::Accepted)\
		{\
			Scan ## N ## Bytes::Result result{(decltype(result.remote))ToAddress(item->text(1)),0};\
			result.SetValue(session->Handle(), dialog.Value());\
		}\
	}\
	break;

		caseNBytes(1);
		caseNBytes(2);
		caseNBytes(4);
		caseNBytes(8);

#undef caseNBytes

#define caseFloatType(type) \
	case Scan::ValueType::Scan ## type ## : \
	{\
		UIChangeValue ## type ## Dialog dialog(this);\
		if (dialog.exec() == QDialog::Accepted)\
		{\
			Scan ## type ## ::Result result{(decltype(result.remote))ToAddress(item->text(1)),0};\
			result.SetValue(session->Handle(), dialog.Value());\
		}\
	}\
	break;

		caseFloatType(Float);
		caseFloatType(Double);

#undef caseFloatType

	default:
		ErrorExit("Unknown Scan::ValueType");
	}
}

void UIMain::ChangeWatchListItemDescription(UIWatchListItem * item)
{
	bool ok;
	QString text = QInputDialog::getText(this, "", "New Description:",
		QLineEdit::Normal, item->text(3), &ok);
	if (ok && !text.isEmpty())
		item->setText(3, text);
}

Scan::ValueType UIMain::ValueType()
{
	return (Scan::ValueType)(ui.comboBoxValueType->
		itemData(ui.comboBoxValueType->currentIndex()).toInt());
}

int UIMain::InitialScanType()
{
	return ui.comboBoxScanType->itemData(ui.comboBoxScanType->currentIndex()).toInt();
}

int UIMain::ScanType()
{
	return ui.comboBoxScanType->itemData(ui.comboBoxScanType->currentIndex()).toInt();
}

ScanOption UIMain::Option()
{
	ScanOption option;
	option.start = ToAddress(ui.lineEditStart->text());
	option.stop = ToAddress(ui.lineEditStop->text());
	option.pause = ui.checkBoxPause->isChecked();
	option.alignment = ui.checkBoxAlignment->isChecked() ? ui.spinBoxAlignment->value() : 1;
	return option;
}

QList<QVariant> UIMain::Param()
{
	return paramWidget->Param();
}

bool UIMain::HasSession() const
{
	return session.get() != nullptr;
}

bool UIMain::HasScan() const
{
	return HasSession() && session->Scan() != nullptr;
}