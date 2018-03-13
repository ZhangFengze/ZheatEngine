#pragma once

#include <QVariant>
#include <QString>

#include <Windows.h>

void* ToAddress(const QString& s);

QString ToString(void*);

void RemoveIfExist(const QString& path);

template <typename T>
T ToType(const QString& s, bool* ok = nullptr);

template <>
UINT8 ToType<UINT8>(const QString& s, bool* ok);

template <>
UINT16 ToType<UINT16>(const QString& s, bool* ok);

template <>
UINT32 ToType<UINT32>(const QString& s, bool* ok);

template <>
UINT64 ToType<UINT64>(const QString& s, bool* ok);

template <>
float ToType<float>(const QString& s, bool* ok);

template <>
double ToType<double>(const QString& s, bool* ok);

template <typename T>
T ToType(const QVariant& v);

template <>
UINT8 ToType<UINT8>(const QVariant& v);

template <>
UINT16 ToType<UINT16>(const QVariant& v);

template <>
UINT32 ToType<UINT32>(const QVariant& v);

template <>
UINT64 ToType<UINT64>(const QVariant& v);

template <>
float ToType<float>(const QVariant& v);

template <>
double ToType<double>(const QVariant& v);
