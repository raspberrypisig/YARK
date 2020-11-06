#pragma once
#include "Widget.h"

class Stage : public Widget {
	bool bmcstage;
//	bool mc[7];  //Main Control(Brakes,Lights,Gear,[0]SAS,RCS,STAGE,ABORT)
//	bool ag[10]; //Action Group 1 - 10
//	GLuint  sliderKnob;
//	bool armAbort;
	bool armStage;
	VesselPacket oldVessel;
//	bool drag;
protected:
	XY lastPos;
	void Toggle(std::string option, bool* ptr, XY pos);
//	void SASOption(uint8_t thisMode, XY pos);
//	void FloatPM(XY pos,std::string label, float* fl, float grad, int dig, bool sign);
public:
	virtual std::string GetTitle();
	Stage();
	void Draw(XY pos, XY size);
};