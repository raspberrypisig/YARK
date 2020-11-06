#pragma once
#include "Widget.h"

class SevenSegment : public Widget 
{
	VesselPacket oldVessel;
protected:
	XY lastPos;
	void FloatPM(XY pos,std::string label, float* fl, int dig, bool sign, Colour clrLedColour, int iNumDecimalPlace);
public:
	virtual std::string GetTitle();
	SevenSegment();
	void Draw(XY pos, XY size);
};