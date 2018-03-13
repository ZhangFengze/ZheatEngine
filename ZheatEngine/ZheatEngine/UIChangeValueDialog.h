#pragma once

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include <Windows.h>

#include "UIParamWidget.h"

template <typename T>
class UIChangeValueNBytesDialog : public QDialog
{
public:
	UIChangeValueNBytesDialog(QWidget* parent = Q_NULLPTR)
		:QDialog(parent)
	{
		auto layout = new QVBoxLayout(this);
		auto param = new UIParamNBytes<T>(this);
		layout->addWidget(param);

		auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		connect(buttonBox, &QDialogButtonBox::accepted, [this, param]()
		{
			value = param->Get();
		});
		connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
		layout->addWidget(buttonBox);
	}

	T Value() const
	{
		return value;
	}

private:
	T value;
};

using UIChangeValue1BytesDialog = UIChangeValueNBytesDialog<UINT8>;
using UIChangeValue2BytesDialog = UIChangeValueNBytesDialog<UINT16>;
using UIChangeValue4BytesDialog = UIChangeValueNBytesDialog<UINT32>;
using UIChangeValue8BytesDialog = UIChangeValueNBytesDialog<UINT64>;

template <typename T>
class UIChangeValueFloatTypeDialog : public QDialog
{
public:
	UIChangeValueFloatTypeDialog(QWidget* parent = Q_NULLPTR)
		:QDialog(parent)
	{
		auto layout = new QVBoxLayout(this);
		auto param = new UIParamFloatType<T>(this);
		layout->addWidget(param);

		auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		connect(buttonBox, &QDialogButtonBox::accepted, [this, param]()
		{
			value = param->Get();
		});
		connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
		connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
		layout->addWidget(buttonBox);
	}

	T Value()
	{
		return value;
	}

private:
	T value;
};

using UIChangeValueFloatDialog = UIChangeValueFloatTypeDialog<float>;
using UIChangeValueDoubleDialog = UIChangeValueFloatTypeDialog<double>;
