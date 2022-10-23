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

		// BS_AUTORADIOBUTTON ����ϵͳ�Զ�����ѡ��״̬���������Ϊ������TAB˳�򣬵�һ����WS_GROUP״̬�İ�ťΪһ��ĵ�һ����
		// BS_RADIOBUTTON ��ϵͳ�������κ�ѡ��״̬�������ϼ������ڵ�WM_COMMAND�ֶ�����
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