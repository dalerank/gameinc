#pragma once

#include "nrpChartObject.h"

namespace irr
{

namespace gui
{

class CChartAxis;

class CChartGrid : public CChartObject  
{
	friend CChartAxis;
public:
	CChartGrid( IGUIEnvironment* env, CChartCtrl* pParent, bool horizontal );
	virtual ~CChartGrid();
	void Draw();

private:
	
	void AddTick(int position);
	void ClearTicks();

	core::list<int> tickPos_;
	bool           horizontal_;
};

}//namespace gui

}//namespace irr