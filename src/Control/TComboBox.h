#pragma once
#include "TControl.h"
#include <vector>
#include <unordered_map>

class TComboBox :
	public TControl
{
public:
	void AddItem(const std::tstring s, void *value);
	void AddItem(std::vector<std::tstring> vecNames,std::vector<void*> vecValues);

	std::tstring GetCurSelString();

	int GetCount();

	void *GetCurSel();
	void SetCurSel(void *value);
	void SetCurSel(int i);

	void DeleteItem(int index);
	void DeleteAllItems();
private:
	std::vector<void *> data;
};

