#include "SevenSegment.h"
#include "Util/IM.h"
#include "../Reg.h"
//-----------------------------------------------------------------------------------------------------------
SevenSegment::SevenSegment() 
{
	memset((char*)&oldVessel, 0, sizeof(VesselPacket));
}
//-----------------------------------------------------------------------------------------------------------
//XYPos, label, FigureToDisplay,grad,iNumDigits,bSigned
void SevenSegment::FloatPM(XY pos, std::string label, float* fl, int dig, bool sign, Colour clrLedColour, int iNumDecimalPlace)
{
	draw->SwitchShader(SHADER_TEXT);
	draw->SetTextColor(1, 1, 1);
	draw->DrawString(f, label, pos.x + lastPos.x - f->GetTextWidth(label) / 2, pos.y + lastPos.y);

	lastPos.y += 18;

	int width = (dig + sign) * 20;
	//char buff[64];
	//sprintf_s(buff, "%.4f", *fl);

	IM::SegFloatDecimal(pos + lastPos + XY{ -width / 2,-12 }, *fl, dig, sign, clrLedColour, iNumDecimalPlace);

	lastPos.y += 40;
}
//-----------------------------------------------------------------------------------------------------------
//#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
//-----------------------------------------------------------------------------------------------------------
std::string SevenSegment::GetTitle() 
{
	return "Seven Segment";
}
//-----------------------------------------------------------------------------------------------------------
void SevenSegment::Draw(XY pos, XY size)
{
	VesselPacket vp = client.Vessel;

	//Draw Background (Grey Rectangle size of window)
	draw->SwitchShader(SHADER_2D);
	draw->SetDrawColor2D(62.f / 256.f, 62.f / 256.f, 61.f / 256.f);
	draw->DrawRect2D(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	OrbitData* o = &client.Vessel.CurrentOrbit;
	//char buff[64];
	//int sy = pos.y + 15;
	float apoapsis = o->AP; //in meters
	float periapsis = o->PE / 1000; //in meter convert to km to match game

	char buff[64];
	sprintf_s(buff, "%.3f", periapsis);

	lastPos = XY{ 16, 32 };

	lastPos.x += 60;
	lastPos.y -= 10;

	FloatPM(pos, "APOAPSIS", &apoapsis, 8, true, yellow, 4); //XYPos, label, FigureToDisplay,grad,iNumDigits,bSigned, LEDColour, iNumDecPlaces
	lastPos.y += 200;
	FloatPM(pos, "PERIAPSIS", &periapsis, 8, true, green, 3); //XYPos, label, FigureToDisplay,grad,iNumDigits,bSigned, LEDColour, iNumDecPlaces


//	printf("set: %d\n", client.Control.ControlerMode);

	memcpy(&oldVessel, &vp, sizeof(VesselPacket));
}
//-----------------------------------------------------------------------------------------------------------