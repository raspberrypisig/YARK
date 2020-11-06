#pragma once
#include "../../Wrap/Window.h"
#include <string>
#include <math.h>

#define TEXT_ALLOW_NUM 1
#define TEXT_ALLOW_CHAR 2
#define TEXT_ALLOW_OTHER 4
#define TEXT_ALLOW_ALL 7

struct TextBox {
	std::string* text;
	bool type = 0;
	int pos = 0;
	int flag;
	TextBox(std::string* t, int f){
		text = t;
		flag = f;
	}
};

enum Colour { red, green, orange, yellow };

namespace IM {
	extern GLuint rad0, rad1, T_UP, T_DOWN, led0, led1, seg_v, seg_h, seg_dot, seg_v_128, seg_h_128, seg_dot_128, push;
	//, push;
	void TextInput(XY pos, int width, Font* f, TextBox* tb, std::string prompt = "");
	void SegDigit(XY pos, int dig);
	void SegInt(XY pos, int dig, int fig, bool negS);
	void SegDigitDecimal(XY pos, int dig, bool bDecimalPoint, Colour clrLedColour);
	void SegFloatDecimal(XY pos, float dig, int fig, bool negS, Colour clrLedColour, int iNumDecimalPlace);
	void LEDColour(Colour clrLedColour, bool bSegState);
	bool PushButton(XY renderAt);
	bool Button(XY pos, Font* f, std::string text);
	bool Button(XY pos, XY size, GLuint tex);
	bool Radio(XY pos, bool* ptr, Font* f, std::string text);
	void LED(XY pos, bool state);
	bool ToggleSwitch(XY pos, std::string text, bool* ptr);

	void Load();
}

#define TOGGLE_SIZE 64
