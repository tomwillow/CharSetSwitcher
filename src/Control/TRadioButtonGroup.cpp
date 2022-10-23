#include "TRadioButtonGroup.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace std;

void TRadioButtonGroup::LinkControl(HWND hDlg, std::vector<int> id_group, std::vector<int> values)
{
	assert(uSet.empty());
	assert(id_group.size() == values.size());

	for (size_t i = 0; i < id_group.size(); ++i)
	{
		TCheckBox rb;
		uMap[id_group[i]].rb.reset(new TCheckBox);
		uMap[id_group[i]].rb->LinkControl(hDlg, id_group[i]);
		uMap[id_group[i]].value=values[i];

		// BS_AUTORADIOBUTTON 会让系统自动设置选中状态。具体规则为，按照TAB顺序，第一个有WS_GROUP状态的按钮为一组的第一个。
		// BS_RADIOBUTTON 则系统不处理任何选中状态。仅由上级父窗口的WM_COMMAND手动处理。
		uMap[id_group[i]].rb->ClearStyleMark(BS_AUTORADIOBUTTON);
		uMap[id_group[i]].rb->SetStyleMark(BS_RADIOBUTTON);
		uSet.insert(id_group[i]);
	}
}

bool TRadioButtonGroup::ReceiveCommand(int id)
{
	if (uSet.find(id) != uSet.end())
	{
		for (auto& pr : uMap)
		{
			pr.second.rb->SetChecked(pr.first == id);
		}
		nowId = id;
		nowValue = uMap[id].value;
		return true;
	}

	return false;
}

void TRadioButtonGroup::SetChecked(int id)
{
	assert(uSet.find(id) != uSet.end());
	ReceiveCommand(id);
}

int TRadioButtonGroup::GetValue()
{
	return nowValue;
}

void TRadioButtonGroup::SetEnable(bool bEnable)
{
	for (auto& pr : uMap)
		pr.second.rb->TControl::SetEnable(bEnable);
}
bool TRadioButtonGroup::GetEnable()
{\
	return (*uMap.begin()).second.rb->TControl::GetEnable();
}