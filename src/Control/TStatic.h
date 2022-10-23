#pragma once
#include "TControl.h"

class TStatic :
	public TControl
{
protected:
public:
	void Create(TWindow *pParent,RECT rect);
	void SetTransparent();
};

