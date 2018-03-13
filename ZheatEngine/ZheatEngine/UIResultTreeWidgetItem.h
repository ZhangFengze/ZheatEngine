#pragma once

#include <QTreeWidgetItem>

#include <Windows.h>

#include "ScanNBytes.h"
#include "ScanFloatType.h"
#include "HelperFunctions.h"

template<typename ScanType>
class UISimpleResultTreeWidgetItem : public QTreeWidgetItem
{
public:
	using ResultType = typename ScanType::Result;

	UISimpleResultTreeWidgetItem(const ResultType& r)
		:result(r)
	{
		UpdateAddressAndPrev();
		setText(1, "???");
	}

	void UpdateAddressAndPrev()
	{
		setText(0, ToString(result.remote));
		setText(2, QString::number(result.value));
	}
	void UpdateNowValue(HANDLE hProcess)
	{
		decltype(result.value) v;
		if (!result.NowValue(hProcess, v))
			setText(1, "???");
		else
			setText(1, QString::number(v));
	}

	ResultType Result() const
	{
		return result;
	}

private:
	ResultType result;
};

using UIResult1BytesTreeWidgetItem = UISimpleResultTreeWidgetItem<Scan1Bytes>;
using UIResult2BytesTreeWidgetItem = UISimpleResultTreeWidgetItem<Scan2Bytes>;
using UIResult4BytesTreeWidgetItem = UISimpleResultTreeWidgetItem<Scan4Bytes>;
using UIResult8BytesTreeWidgetItem = UISimpleResultTreeWidgetItem<Scan8Bytes>;

using UIResultFloatTreeWidgetItem = UISimpleResultTreeWidgetItem<ScanFloat>;
using UIResultDoubleTreeWidgetItem = UISimpleResultTreeWidgetItem<ScanDouble>;
