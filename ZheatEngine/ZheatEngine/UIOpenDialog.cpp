#include <QTreeWidgetItem>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QtWin>

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include "UIOpenDialog.h"
#include "Log.h"

class UIProcessTreeWidgetItem : public QTreeWidgetItem
{
private:
	bool operator<(const QTreeWidgetItem& other) const
	{
		auto tree = treeWidget();
		auto sortColumn = tree->sortColumn();
		if (sortColumn == 1) // sort by PID
			return text(1).toInt() < other.text(1).toInt();
		else
			return QTreeWidgetItem::operator<(other);
	}
};

UIOpenDialog::UIOpenDialog(QWidget *parent)
	: QDialog(parent)
{
	layout = new QVBoxLayout(this);

	tree = new QTreeWidget();
	tree->setColumnCount(4);
	tree->setHeaderLabels({ "","PID","Name","Full Path" });
	tree->setColumnWidth(0, 50);
	tree->setColumnWidth(1, 50);
	tree->setSortingEnabled(true);
	connect(tree, &QTreeWidget::itemSelectionChanged, [this]()
	{
		auto selected = tree->selectedItems().front();
		pid = selected->text(1).toInt();
		processName = selected->text(2);
		icon = selected->icon(0);
	});

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &UIOpenDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &UIOpenDialog::reject);


	layout->addWidget(tree);
	layout->addWidget(buttonBox);

	resize(360, 400);


	auto hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		ErrorExit("CreateToolhelp32Snapshot (of processes)");
	}

	PROCESSENTRY32 pe32{};
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		ErrorExit("Process32First");
	}

	do
	{
		int pid = pe32.th32ProcessID;
		QString exeName(pe32.szExeFile);
		QString fullPath;
		QIcon icon;

		auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, pid);
		if (hProcess == NULL)
		{
			LogWarning(QString("OpenProcess %1 failed, error code %2").arg(pid).arg(GetLastError()));
		}
		else
		{
			char buffer[MAX_PATH];
			DWORD n = MAX_PATH;
			if (0 == QueryFullProcessImageName(hProcess, NULL, buffer, &n))
			{
				ErrorExit("QueryFullProcessImageName");
			}
			fullPath = buffer;

			auto hIcon = ExtractIcon(NULL, buffer, 0);
			if (NULL == hIcon)
			{
				LogWarning(QString("ExtractIcon %1 failed, error code %2").arg(pid).arg(GetLastError()));
			}
			else
			{
				icon = QIcon(QtWin::fromHICON(hIcon));
				if (!DestroyIcon(hIcon))
					ErrorExit("DestroyIcon");
			}

			CloseHandle(hProcess);
		}

		auto treeItem = new UIProcessTreeWidgetItem();
		treeItem->setIcon(0, icon);
		treeItem->setText(1, QString::number(pid));
		treeItem->setText(2, exeName);
		treeItem->setText(3, fullPath);

		tree->addTopLevelItem(treeItem);

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
}

UIOpenDialog::~UIOpenDialog()
{
}

int UIOpenDialog::ProcessId()
{
	return pid;
}

QString UIOpenDialog::ProcessName()
{
	return processName;
}

QIcon UIOpenDialog::Icon()
{
	return icon;
}
