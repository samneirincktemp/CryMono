/////////////////////////////////////////////////////////////////////////*
//Ink Studios Source File.
//Copyright (C), Ink Studios, 2011.
//////////////////////////////////////////////////////////////////////////
// Wrapper for the MonoArray for less intensively ugly code and
// better workflow.
//////////////////////////////////////////////////////////////////////////
// 17/12/2011 : Created by Filip 'i59' Lundgren
////////////////////////////////////////////////////////////////////////*/
#ifndef __MONO_ARRAY_H__
#define __MONO_ARRAY_H__

#include "MonoObject.h"

#include <IMonoArray.h>

class CMonoArray : public IMonoArray
{
public:
	// Used on MonoArray's returned from C#.
	CMonoArray(mono::array monoArray) : m_pArray(monoArray) { m_arrayHandle = mono_gchandle_new((MonoObject *)m_pArray, false); }

	// Used to send arrays to C#.
	CMonoArray(int size);
	virtual ~CMonoArray() { mono_gchandle_free(m_arrayHandle); }

	// IMonoArray
	virtual void Clear() override { for(int i = 0; i < GetSize(); i++) mono_array_set((MonoArray *)m_pArray, void *, i, NULL);  }
	virtual int GetSize() const override { return (int)mono_array_length((MonoArray *)m_pArray); }

	virtual IMonoObject *GetItem(int index) override;
	virtual const char *GetItemString(int index) override { return ToCryString(mono_array_get((MonoArray *)m_pArray, mono::string , index)); }
	virtual IMonoArray *GetItemArray(int index) override { return new CMonoArray((mono::array)mono_array_get((MonoArray *)m_pArray, MonoArray *, index)); }

	virtual void Insert(IMonoObject *pObject) override;
	virtual void Insert(IMonoArray *pArray) override { pArray ? InsertArray(*pArray) : InsertArray(0); }

	virtual void InsertString(mono::string string) override;

	virtual void Insert(MonoAnyValue value) override;
	// ~IMonoArray

	void InsertObject(mono::object object);
	virtual void InsertArray(mono::array arr);

	virtual mono::array GetMonoArray() override { return m_pArray; }

private:
	mono::array m_pArray;
	int curIndex;

	int m_arrayHandle;
};

#endif //__MONO_ARRAY_H__