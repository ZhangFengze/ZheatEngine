#include <QVBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>

#include "UIChangeScanValueTypeDialog.h"

UIChangeScanValueTypeDialog::UIChangeScanValueTypeDialog(Scan::ValueType default,QWidget *parent)
	: QDialog(parent)
{
		auto layout = new QVBoxLayout(this);
		auto combo = new QComboBox();
		for (auto valueType : ScanValueTypes)
		{
			combo->addItem(ScanValueTypeToString(valueType), (int)valueType);
			if (valueType == default)
				combo->setCurrentIndex((int)default);
		}
		layout->addWidget(combo);

		auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		connect(buttonBox, &QDialogButtonBox::accepted, [this,combo]()
		{
			valueType = (Scan::ValueType)combo->currentData().toInt();
		});
		connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
		layout->addWidget(buttonBox);
}
