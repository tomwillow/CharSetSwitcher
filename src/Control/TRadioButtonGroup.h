#pragma once
#include "TControl.h"
#include "TCheckBox.h"
#include <assert.h>

#include <vector>
#include <unordered_set>
#include <unordered_map>

/*
ʹ�÷�����
	radioAs.LinkControl(m_hWnd, { IDC_RADIO_AS_PLAIN,IDC_RADIO_AS_UNICODE,IDC_RADIO_AS_UTF8 }, { IDC_RADIO_AS_PLAIN, IDC_RADIO_AS_UNICODE, IDC_RADIO_AS_UTF8 });

	Ȼ���ڸ����ڵ�WM_COMMAND����Ӧ�¼��У�����ReceiveCommand��
*/

class TRadioButtonGroup :
	public TControl
{
private:
	struct TRadioButton
	{
		std::unique_ptr<TCheckBox> rb;
		int value;
	};
	int nowId, nowValue;
	std::unordered_set<int> uSet;
	std::unordered_map<int, TRadioButton> uMap;
public:
	void LinkControl(HWND hDlg, std::vector<int> id_group, std::vector<int> values);

	bool ReceiveCommand(int id);

	void SetChecked(int id);

	int GetValue();

	virtual void SetEnable(bool bEnable) override;
	virtual bool GetEnable() override;
};

