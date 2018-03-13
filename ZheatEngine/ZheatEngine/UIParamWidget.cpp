#include "ScanNBytes.h"
#include "Log.h"
#include "UIParamWidget.h"

UIParamWidget * UIParamWidget::Factory(Scan::ValueType type, int scanType, bool initial)
{
	switch (type)
	{
	case Scan::ValueType::Scan1Bytes:
		return FactoryNBytes<UINT8>(type, scanType, initial);
	case Scan::ValueType::Scan2Bytes:
		return FactoryNBytes<UINT16>(type, scanType, initial);
	case Scan::ValueType::Scan4Bytes:
		return FactoryNBytes<UINT32>(type, scanType, initial);
	case Scan::ValueType::Scan8Bytes:
		return FactoryNBytes<UINT64>(type, scanType, initial);

	case Scan::ValueType::ScanFloat:
		return FactoryFloatType<float>(type, scanType, initial);
	case Scan::ValueType::ScanDouble:
		return FactoryFloatType<double>(type, scanType, initial);

	default:
		ErrorExit("Unknown Scan::ValueType");
	}
}

bool UIParamWidget::MatchType(Scan::ValueType v, int s, bool i) const
{
	return v == valueType && s == scanType && i == initial;
}

UIParamWidget::UIParamWidget(Scan::ValueType v, int s, bool i)
	:valueType(v), scanType(s), initial(i)
{
}

UIEmptyParamWidget::UIEmptyParamWidget(Scan::ValueType v, int scanType, bool initial)
	: UIParamWidget(v, scanType, initial)
{
}

QList<QVariant> UIEmptyParamWidget::Param() const
{
	return {};
}
