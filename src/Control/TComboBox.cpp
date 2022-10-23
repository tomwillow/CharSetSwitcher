#include "TComboBox.h"

#include <assert.h>
#include <windowsx.h>

using namespace std;


void TComboBox::AddItem(const std::tstring s,void *value)
{
	assert(m_hWnd);
	ComboBox_AddString(m_hWnd, s.c_str());
	data.push_back(value);
}


void TComboBox::AddItem(std::vector<std::tstring> vecNames, std::vector<void *> vecValues)
{
	assert(m_hWnd);
	assert(vecNames.size() != vecValues.size());

	size_t sz = vecNames.size();
	for (size_t i = 0; i < sz; ++i)
	{
		ComboBox_AddString(m_hWnd, vecNames[i].c_str());
		data.push_back(vecValues[i]);
	}
}


std::tstring TComboBox::GetCurSelString()
{
	assert(m_hWnd);
	int cursel = ComboBox_GetCurSel(m_hWnd);
	assert(cursel >= 0);

	int len=ComboBox_GetLBTextLen(m_hWnd, cursel)+1;
	TCHAR* buf = new TCHAR[len];
	ComboBox_GetLBText(m_hWnd, cursel, buf);
	tstring s = buf;
	delete[] buf;
	return s;
}

int TComboBox::GetCount()
{
	assert(m_hWnd);
	return ComboBox_GetCount(m_hWnd);
}


void *TComboBox::GetCurSel()
{
	assert(m_hWnd);
	int sel=ComboBox_GetCurSel(m_hWnd);
	return data[sel];
}

void TComboBox::SetCurSel(void *value)
{
	assert(m_hWnd);
	auto it = find(data.begin(), data.end(), value);
	if (it == data.end())
	{
		assert("no this item value");
		return;
	}

	int i = it - data.begin();
	ComboBox_SetCurSel(m_hWnd, i);
}

void TComboBox::SetCurSel(int i)
{
	assert(m_hWnd);
	assert(i >= 0 && i < data.size());
	ComboBox_SetCurSel(m_hWnd, i);
}


void TComboBox::DeleteItem(int index)
{
	assert(m_hWnd);
	ComboBox_DeleteString(m_hWnd, index);
}

void TComboBox::DeleteAllItems()
{
	assert(m_hWnd);
	ComboBox_ResetContent(m_hWnd);
}
