#include <QFile>

#include "Log.h"
#include "HelperFunctions.h"

void * ToAddress(const QString & s)
{
	bool ok = false;
	auto ret = s.toULongLong(&ok, 16);
	if (!ok)
		ErrorExit("QString.toULongLong()");
	return (void*)ret;
}

QString ToString(void * p)
{
	auto str = QString::number((ULONGLONG)p, 16);
	str = str.toUpper();

	int betterSize;
	if (str.size() > 12)
		betterSize = 16;
	else if (str.size() > 8)
		betterSize = 12;
	else
		betterSize = 8;

	str = QString(betterSize - str.size(), '0') + str;
	return str;
}

void RemoveIfExist(const QString & path)
{
	if (QFile::exists(path))
		if (!QFile::remove(path))
			ErrorExit("can't remove " + path);
}

template <>
UINT8 ToType<UINT8>(const QString& s, bool* ok)
{
	auto ret = s.toUShort(ok);
	// if s.toUShort failed, ret will be 0 

	if (ret<std::numeric_limits<UINT8>::min() || ret> std::numeric_limits<UINT8>::max())
	{
		if (ok != nullptr)
			*ok = false;
		return 0;
	}
	return ret;
}

template <>
UINT16 ToType<UINT16>(const QString& s, bool* ok)
{
	static_assert(std::is_same_v< unsigned short, UINT16>, "");
	return s.toUShort(ok);
}

template <>
UINT32 ToType<UINT32>(const QString& s, bool* ok)
{
	static_assert(std::is_same_v< unsigned int, UINT32>, "");
	return s.toUInt(ok);
}

template <>
UINT64 ToType<UINT64>(const QString& s, bool* ok)
{
	static_assert(std::is_same_v< unsigned long long, UINT64>, "");
	return s.toULongLong(ok);
}

template <>
float ToType<float>(const QString& s, bool* ok)
{
	return s.toFloat(ok);
}

template <>
double ToType<double>(const QString& s, bool* ok)
{
	return s.toDouble(ok);
}

template <typename T>
T ToType(const QVariant& v) {}

template <>
UINT8 ToType<UINT8>(const QVariant& v)
{
	return v.toULongLong();
}

template <>
UINT16 ToType<UINT16>(const QVariant& v)
{
	return v.toULongLong();
}

template <>
UINT32 ToType<UINT32>(const QVariant& v)
{
	return v.toULongLong();
}

template <>
UINT64 ToType<UINT64>(const QVariant& v)
{
	return v.toULongLong();
}

template <>
float ToType<float>(const QVariant& v)
{
	return v.toFloat();
}

template <>
double ToType<double>(const QVariant& v)
{
	return v.toDouble();
}
