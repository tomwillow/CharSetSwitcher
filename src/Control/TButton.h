#pragma once
#include "TControl.h"

//TButton* btn = new TButton;
//btn->Create(this, 0, TEXT("test"));
//btn->SetRect(50, 50, 100, 100);
//btn->SetDefaultGuiFont();

class TButton :
	public TControl
{
public:
	void Create(TWindowBase *pParent, UINT uId, std::tstring text);
};

