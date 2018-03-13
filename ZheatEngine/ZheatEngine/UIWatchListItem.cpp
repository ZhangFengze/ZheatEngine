#include "UIWatchListItem.h"
#include "ScanNBytes.h"
#include "ScanFloatType.h"

UIWatchListItem::UIWatchListItem(Scan::ValueType v,
	void * b, const QList<ULONGLONG>& o, const QString & description)
	:valueType(v),baseAddress(b),offsets(o)
{
	setText(3, description);
}

void UIWatchListItem::Update(HANDLE process)
{
	// type
	setText(0, ScanValueTypeToString(valueType));

	void* now = NowAddress(process);

	// address
	if (offsets.empty())
		setText(1, ToString(baseAddress));
	else if (now)
		setText(1, "P->" + ToString(NowAddress(process)));
	else
		setText(1, "P->???");

	// value
	switch (valueType)
	{
#define caseScanType(type) \
case Scan::ValueType::type:\
{\
		type::Result r{(decltype(type::Result::remote))now,0 };\
		if (!r.NowValue(process, r.value))\
			setText(2, "???");\
		else\
			setText(2, QString::number(r.value));\
}\
break;
	caseScanType(Scan1Bytes);
	caseScanType(Scan2Bytes);
	caseScanType(Scan4Bytes);
	caseScanType(Scan8Bytes);
	caseScanType(ScanFloat);
	caseScanType(ScanDouble);
#undef caseScanType

	default:
		ErrorExit("unknown scan value type");
		break;
	}
}

Scan::ValueType UIWatchListItem::ValueType() const
{
	return valueType;
}

void UIWatchListItem::SetValueType(Scan::ValueType t)
{
	valueType = t;
}

void * UIWatchListItem::NowAddress(HANDLE process)
{
	void* now = baseAddress;
	for (auto offset : offsets)
	{
		void* p;
		if (!ReadProcessMemory(process, now, &p, sizeof p, nullptr))
			return nullptr;

		now = (char*)p+offset;
	}
	return now;
}

QString UIWatchListItem::Description() const
{
	return text(3);
}

void UIWatchListItem::SetDescription(const QString &d)
{
	setText(3, d);
}
