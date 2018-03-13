#pragma once

#include <limits>

#include <QWidget>
#include <QList>
#include <QVariant>
#include <QLineEdit>
#include <QLayout>
#include <QString>
#include <QValidator>

#include "Scan.h"
#include "ScanNBytes.h"
#include "ScanFloatType.h"
#include "HelperFunctions.h"

class UIParamWidget : public QWidget
{
	Q_OBJECT

public:
	static UIParamWidget* Factory(Scan::ValueType, int scanType, bool initial);

	virtual ~UIParamWidget() {};

	virtual QList<QVariant> Param() const = 0;

	bool MatchType(Scan::ValueType, int scanType, bool initial) const;

protected:
	UIParamWidget(Scan::ValueType v, int scanType, bool initial);

private:
	template<typename T>
	static UIParamWidget* FactoryNBytes(Scan::ValueType valueType, int scanType, bool initial);

	template<typename T>
	static UIParamWidget* FactoryFloatType(Scan::ValueType valueType, int scanType, bool initial);

protected:
	Scan::ValueType valueType;
	int scanType;
	bool initial;
};

class UIEmptyParamWidget : public UIParamWidget
{
public:
	UIEmptyParamWidget(Scan::ValueType v, int scanType, bool initial);
	QList<QVariant> Param() const override;
};

template<typename T>
class SimpleValidator : public QValidator
{
public:
	State validate(QString &input, int &pos) const override;

	void fixup(QString &input) const override;
};

template<typename T>
class UIParamNBytes : public QWidget
{
public:
	UIParamNBytes(QWidget* parent = Q_NULLPTR);
	T Get() const;

private:
	QLineEdit * lineEdit;
};

template<typename T>
class UISingleParamNBytes : public UIParamWidget
{
public:
	UISingleParamNBytes(Scan::ValueType v, int scanType, bool initial);
	QList<QVariant> Param() const override;

private:
	UIParamNBytes<T>* param;
};

template<typename T>
class UIDoubleParamNBytes : public UIParamWidget
{
public:
	UIDoubleParamNBytes(Scan::ValueType v, int scanType, bool initial);
	QList<QVariant> Param() const override;

private:
	UIParamNBytes<T>* paramLeft;
	UIParamNBytes<T>* paramRight;
};

template<typename T>
class UIParamFloatType :public QWidget
{
public:
	UIParamFloatType(QWidget* parent = Q_NULLPTR);
	T Get() const;

private:
	QLineEdit * lineEdit;
};

template<typename T>
class UISingleParamFloatType : public UIParamWidget
{
public:
	UISingleParamFloatType(Scan::ValueType v, int scanType, bool initial);
	QList<QVariant> Param() const override;

private:
	UIParamFloatType<T>* param;
};

template<typename T>
class UIDoubleParamFloatType : public UIParamWidget
{
public:
	UIDoubleParamFloatType(Scan::ValueType v, int scanType, bool initial);
	QList<QVariant> Param() const override;

private:
	UIParamFloatType<T>* paramLeft;
	UIParamFloatType<T>* paramRight;
};

template<typename T>
QValidator::State SimpleValidator<T>::validate(QString & input, int & pos) const
{
	bool ok = false;
	auto ret = ToType<T>(input, &ok);
	if (!ok)
		return State::Intermediate;

	if (ret > std::numeric_limits<T>::max() || ret < std::numeric_limits<T>::min())
		return State::Intermediate;

	return State::Acceptable;
}

template<typename T>
void SimpleValidator<T>::fixup(QString & input) const
{
	bool ok = false;
	auto ret = ToType<T>(input, &ok);
	if (!ok)
	{
		input = "0";
		return;
	}

	if (ret > std::numeric_limits<T>::max())
	{
		input = QString::number(std::numeric_limits<T>::max());
	}
	else if (ret < std::numeric_limits<T>::min())
	{
		input = QString::number(std::numeric_limits<T>::min());
	}
}

template<typename T>
UIParamNBytes<T>::UIParamNBytes(QWidget * parent)
	:QWidget(parent), lineEdit(new QLineEdit(this))
{
	auto layout = new QVBoxLayout(this);
	layout->addWidget(lineEdit);
	lineEdit->setValidator(new SimpleValidator<T>());
	lineEdit->setText("0");
}

template<typename T>
T UIParamNBytes<T>::Get() const
{
	return ToType<T>(lineEdit->text());
}

template<typename T>
UISingleParamNBytes<T>::UISingleParamNBytes(Scan::ValueType v, int scanType, bool initial)
	:UIParamWidget(v, scanType, initial), param(new UIParamNBytes<T>(this))
{
	auto layout = new QHBoxLayout(this);
	layout->addWidget(param);
}

template<typename T>
QList<QVariant> UISingleParamNBytes<T>::Param() const
{
	return { param->Get() };
}

template<typename T>
UIDoubleParamNBytes<T>::UIDoubleParamNBytes(Scan::ValueType v, int scanType, bool initial)
	:UIParamWidget(v, scanType, initial),
	paramLeft(new UIParamNBytes<T>(this)),
	paramRight(new UIParamNBytes<T>(this))
{
	auto layout = new QHBoxLayout(this);
	layout->addWidget(paramLeft);
	layout->addWidget(paramRight);
}

template<typename T>
QList<QVariant> UIDoubleParamNBytes<T>::Param() const
{
	return { paramLeft->Get(),paramRight->Get() };
}

template<typename T>
UIParamWidget * UIParamWidget::FactoryNBytes(Scan::ValueType valueType, int scanType, bool initial)
{
	if (initial)
	{
		// same InitialScanType, just use Scan4Bytes::InitialScanType here
		auto type = Scan4Bytes::InitialScanType(scanType);
		switch (type)
		{
		case Scan4Bytes::InitialScanType::Equal:
		case Scan4Bytes::InitialScanType::NotEqual:
		case Scan4Bytes::InitialScanType::GreaterThan:
		case Scan4Bytes::InitialScanType::LessThan:
		case Scan4Bytes::InitialScanType::GreaterThanOrEqualTo:
		case Scan4Bytes::InitialScanType::LessThanOrEqualTo:
			return new UISingleParamNBytes<T>(valueType, scanType, initial);

		case Scan4Bytes::InitialScanType::Between:
		case Scan4Bytes::InitialScanType::NotBetween:
			return new UIDoubleParamNBytes<T>(valueType, scanType, initial);
		case Scan4Bytes::InitialScanType::Unknown:
			return new UIEmptyParamWidget(valueType, scanType, initial);

		default:
			ErrorExit("Unknown ScanNBytes::InitialScanType");
		}
	}
	else
	{
		// same InitialScanType, just use Scan4Bytes::InitialScanType here
		auto type = Scan4Bytes::ScanType(scanType);
		switch (type)
		{
		case Scan4Bytes::ScanType::Equal:
		case Scan4Bytes::ScanType::NotEqual:
		case Scan4Bytes::ScanType::GreaterThan:
		case Scan4Bytes::ScanType::LessThan:
		case Scan4Bytes::ScanType::GreaterThanOrEqualTo:
		case Scan4Bytes::ScanType::LessThanOrEqualTo:
		case Scan4Bytes::ScanType::IncreasedBy:
		case Scan4Bytes::ScanType::NotIncreasedBy:
		case Scan4Bytes::ScanType::DecreasedBy:
		case Scan4Bytes::ScanType::NotDecreasedBy:
			return new UISingleParamNBytes<T>(valueType, scanType, initial);

		case Scan4Bytes::ScanType::Between:
		case Scan4Bytes::ScanType::NotBetween:
			return new UIDoubleParamNBytes<T>(valueType, scanType, initial);

		case Scan4Bytes::ScanType::IncreasedValue:
		case Scan4Bytes::ScanType::DecreasedValue:
		case Scan4Bytes::ScanType::ChangedValue:
		case Scan4Bytes::ScanType::UnChangedValue:
			return new UIEmptyParamWidget(valueType, scanType, initial);
		default:
			ErrorExit("Unknown ScanNBytes::ScanType");
		}

	}
}

template<typename T>
UIParamFloatType<T>::UIParamFloatType(QWidget * parent)
	:QWidget(parent), lineEdit(new QLineEdit(this))
{
	auto layout = new QVBoxLayout(this);
	layout->addWidget(lineEdit);
	lineEdit->setValidator(new SimpleValidator<T>());
	lineEdit->setText("0");
}

template<typename T>
T UIParamFloatType<T>::Get() const
{
	return ToType<T>(lineEdit->text());
}

template<typename T>
UISingleParamFloatType<T>::UISingleParamFloatType(Scan::ValueType v, int scanType, bool initial)
	:UIParamWidget(v, scanType, initial), param(new UIParamFloatType<T>(this))
{
	auto layout = new QHBoxLayout(this);
	layout->addWidget(param);
}

template<typename T>
QList<QVariant> UISingleParamFloatType<T>::Param() const
{
	return { param->Get() };
}

template<typename T>
UIDoubleParamFloatType<T>::UIDoubleParamFloatType(Scan::ValueType v, int scanType, bool initial)
	:UIParamWidget(v, scanType, initial),
	paramLeft(new UIParamFloatType<T>(this)),
	paramRight(new UIParamFloatType<T>(this))
{
	auto layout = new QHBoxLayout(this);
	layout->addWidget(paramLeft);
	layout->addWidget(paramRight);
}

template<typename T>
QList<QVariant> UIDoubleParamFloatType<T>::Param() const
{
	return { paramLeft->Get(),paramRight->Get() };
}

template<typename T>
UIParamWidget * UIParamWidget::FactoryFloatType(Scan::ValueType valueType, int scanType, bool initial)
{
	if (initial)
	{
		// same InitialScanType, just use ScanFloat::InitialScanType here
		auto type = ScanFloat::InitialScanType(scanType);
		switch (type)
		{
		case ScanFloat::InitialScanType::Equal:
		case ScanFloat::InitialScanType::NotEqual:
		case ScanFloat::InitialScanType::GreaterThan:
		case ScanFloat::InitialScanType::LessThan:
		case ScanFloat::InitialScanType::GreaterThanOrEqualTo:
		case ScanFloat::InitialScanType::LessThanOrEqualTo:
			return new UISingleParamFloatType<T>(valueType, scanType, initial);

		case ScanFloat::InitialScanType::Between:
		case ScanFloat::InitialScanType::NotBetween:
			return new UIDoubleParamFloatType<T>(valueType, scanType, initial);
		case ScanFloat::InitialScanType::Unknown:
			return new UIEmptyParamWidget(valueType, scanType, initial);

		default:
			ErrorExit("Unknown ScanFloat::InitialScanType");
		}
	}
	else
	{
		// same InitialScanType, just use ScanFloat::InitialScanType here
		auto type = ScanFloat::ScanType(scanType);
		switch (type)
		{
		case ScanFloat::ScanType::Equal:
		case ScanFloat::ScanType::NotEqual:
		case ScanFloat::ScanType::GreaterThan:
		case ScanFloat::ScanType::LessThan:
		case ScanFloat::ScanType::GreaterThanOrEqualTo:
		case ScanFloat::ScanType::LessThanOrEqualTo:
		case ScanFloat::ScanType::IncreasedBy:
		case ScanFloat::ScanType::NotIncreasedBy:
		case ScanFloat::ScanType::DecreasedBy:
		case ScanFloat::ScanType::NotDecreasedBy:
			return new UISingleParamFloatType<T>(valueType, scanType, initial);

		case ScanFloat::ScanType::Between:
		case ScanFloat::ScanType::NotBetween:
			return new UIDoubleParamFloatType<T>(valueType, scanType, initial);

		case ScanFloat::ScanType::IncreasedValue:
		case ScanFloat::ScanType::DecreasedValue:
		case ScanFloat::ScanType::ChangedValue:
		case ScanFloat::ScanType::UnChangedValue:
			return new UIEmptyParamWidget(valueType, scanType, initial);
		default:
			ErrorExit("Unknown ScanFloat::ScanType");
		}
	}
}
