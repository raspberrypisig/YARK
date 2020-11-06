#include "IM.h"
//-----------------------------------------------------------------------------------------------------------
GLuint IM::rad0, IM::rad1, IM::T_UP, IM::T_DOWN, IM::led0, IM::led1, IM::seg_v, IM::seg_h, IM::seg_dot, IM::seg_v_128, IM::seg_h_128, IM::seg_dot_128, IM::push;
//Font* IM::f;
extern Draw* draw;
extern Window* win;
//-----------------------------------------------------------------------------------------------------------
void IM::TextInput(XY pos, int width, Font* f, TextBox* tb, std::string prompt) {
	draw->SwitchShader(SHADER_TEXT);
	draw->SetTextColor(1, 1, 1);
	draw->DrawString(f, prompt, pos.x, pos.y + 15);
	pos.x += f->GetTextWidth(prompt);

	XY size = XY{ width + 15,20 };
	bool in = win->MouseX() > pos.x && win->MouseY() > pos.y && win->MouseX() < pos.x + size.x && win->MouseY() < pos.y + size.y;
	draw->SwitchShader(SHADER_2D);
	if (tb->type) {
		draw->SetDrawColor2D(33.f / 256.f, 66.f / 256.f, 103.f / 256.f);
	}
	else {
		if (in) {
			if (win->MouseDown(SDL_BUTTON_LEFT)) draw->SetDrawColor2D(33.f / 256.f, 66.f / 256.f, 103.f / 256.f);
			else draw->SetDrawColor2D(53.f / 256.f, 86.f / 256.f, 123.f / 256.f);
		}
		else {
			draw->SetDrawColor2D(43.f / 256.f, 76.f / 256.f, 113.f / 256.f);
		}
	}
	draw->BindTex2D(0);

	draw->DrawRect2D(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	draw->SwitchShader(SHADER_TEXT);
	draw->DrawString(f, *tb->text, pos.x + 10, pos.y + 15);
	if (tb->type) {
		if (win->KeyRepeating(SDL_SCANCODE_LEFT) && tb->pos != 0) {
			tb->pos--;
		}
		if (win->KeyRepeating(SDL_SCANCODE_RIGHT) && tb->pos < tb->text->size()) {
			tb->pos++;
		}
		if (tb->flag && TEXT_ALLOW_CHAR) {
			for (int i = SDL_SCANCODE_A; i <= SDL_SCANCODE_Z; i++) {
				if (win->KeyRepeating(i)) {
					int add = (win->KeyDown(SDL_SCANCODE_LSHIFT) || win->KeyDown(SDL_SCANCODE_RSHIFT)) ? 'A' : 'a';
					tb->text->insert(tb->text->begin() + tb->pos, (i - SDL_SCANCODE_A + add));
					tb->pos++;
				}
			}
		}
		if (tb->flag && TEXT_ALLOW_NUM) {
			for (int i = SDL_SCANCODE_1; i <= SDL_SCANCODE_9; i++) {
				if (win->KeyRepeating(i)) {
					tb->text->insert(tb->text->begin() + tb->pos, (i - SDL_SCANCODE_1 + '1'));
					tb->pos++;
				}
			}
			if (win->KeyRepeating(SDL_SCANCODE_0)) {
				tb->text->insert(tb->text->begin() + tb->pos, '0');
				tb->pos++;
			}
		}
		if (tb->flag && TEXT_ALLOW_OTHER) {
			if (win->KeyRepeating(SDL_SCANCODE_PERIOD)) {
				tb->text->insert(tb->text->begin() + tb->pos, '.');
				tb->pos++;
			}
			if (win->KeyRepeating(SDL_SCANCODE_SEMICOLON) && (win->KeyDown(SDL_SCANCODE_LSHIFT) || win->KeyDown(SDL_SCANCODE_RSHIFT))) {
				tb->text->insert(tb->text->begin() + tb->pos, ':');
				tb->pos++;
			}
		}
		if (win->KeyRepeating(SDL_SCANCODE_BACKSPACE) && tb->pos != 0) {
			tb->text->erase(tb->pos - 1, 1);
			tb->pos--;
		}
		if (win->KeyRepeating(SDL_SCANCODE_DELETE) && tb->pos < tb->text->size()) {
			tb->text->erase(tb->pos, 1);
		}
		draw->DrawChar(f, '_', pos.x + 10 + f->GetTextWidth(*tb->text, tb->pos) + 2, pos.y + 15 + 2);
	}
	if (win->MouseClicked(SDL_BUTTON_LEFT)) {
		if (in) {
			tb->type = true;
		}
		else {
			tb->type = false;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
void IM::Load() {
	if (!rad0) {
		rad0 = loadTexture("Tex/radio-off.png", false);
	}
	if (!rad1) {
		rad1 = loadTexture("Tex/radio-on.png", false);
	}
	if (!T_UP) {
		T_UP = loadTexture("Tex/t-up.png", false);
	}
	if (!T_DOWN) {
		T_DOWN = loadTexture("Tex/t-down.png", false);
	}
	if (!led0) {
		led0 = loadTexture("Tex/led0.png", false);
	}
	if (!led1) {
		led1 = loadTexture("Tex/led1.png", false);
	}
	if (!seg_v) {
		seg_v = loadTexture("Tex/7-seg-v.png", false);
	}
	if (!seg_h) {
		seg_h = loadTexture("Tex/7-seg-h.png", false);
	}
	if (!seg_dot) {
		seg_dot = loadTexture("Tex/7-seg-dot.png", false);
	}
	if (!seg_v_128) {
		seg_v_128 = loadTexture("Tex/7-seg-v128.png", false);
	}
	if (!seg_h_128) {
		seg_h_128 = loadTexture("Tex/7-seg-h128.png", false);
	}
	if (!seg_dot_128) {
		seg_dot_128 = loadTexture("Tex/7-seg-dot128.png", false);
	}
	if (!push) {
		push = loadTexture("Tex/push.png", false);
	}
}
//-----------------------------------------------------------------------------------------------------------
#define SEG_STATE(x) (x) ? draw->SetDrawColor2D(0.98f,0.f,0.f) : draw->SetDrawColor2D(0.25f,0.16f,0.16f); //red
//#define SEG_STATE(x) (x) ? draw->SetDrawColor2D(0.f,0.98f,0.f) : draw->SetDrawColor2D(0.16f,0.25f,0.16f); //green
//#define SEG_STATE(x) (x) ? draw->SetDrawColor2D(0.98f,0.27f,0.f) : draw->SetDrawColor2D(0.25f,0.16f,0.16f); //orange
//#define SEG_STATE(x) (x) ? draw->SetDrawColor2D(0.98f,0.84f,0.f) : draw->SetDrawColor2D(0.25f,0.25f,0.16f); //yellow



void IM::LEDColour(Colour r, bool bSegState)
{
	switch (r)
	{
		case red: 
			if (bSegState)
			{
				draw->SetDrawColor2D(0.98f, 0.f, 0.f);
			}
			else
			{
				draw->SetDrawColor2D(0.25f, 0.16f, 0.16f);
			}
			break;
		case green: 
			if (bSegState)
			{
				draw->SetDrawColor2D(0.f, 0.98f, 0.f);
			}
			else
			{
				draw->SetDrawColor2D(0.16f, 0.25f, 0.16f);
			}
			break;
		case orange: 
			if (bSegState)
			{
				draw->SetDrawColor2D(0.98f, 0.27f, 0.f);
			}
			else
			{
				draw->SetDrawColor2D(0.25f, 0.16f, 0.16f);
			}
			break;
		case yellow: 
			if (bSegState)
			{
				draw->SetDrawColor2D(0.98f, 0.84f, 0.f);
			}
			else
			{
				draw->SetDrawColor2D(0.25f, 0.25f, 0.16f);
			}
			break;
		//default:

	}
}
//-----------------------------------------------------------------------------------------------------------
#define v_space  102 //distance between horizontal bars
#define h_space  100 //

#define h_move  15
#define v_move  15

#define width 100
#define height 100 //height of the bar (

#define x_offset 140 //how close together to draw each 7-seg digit tile
//x_offset is equal to width = (2 * h_move)
//-----------------------------------------------------------------------------------------------------------
void IM::SegDigit(XY pos, int dig) {
	draw->SwitchShader(SHADER_2D);

	draw->BindTex2D(seg_h);
	SEG_STATE(dig == 2 || dig == 3 || dig == 5 || dig == 6 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 0, pos.x + width + h_move, pos.y + height + v_space * 0);
	SEG_STATE(dig == 2 || dig == 3 || dig == 4 || dig == 5 || dig == 6 || dig == 8 || dig == 9);
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 1, pos.x + width + h_move, pos.y + height + v_space * 1);
	SEG_STATE(dig == 2 || dig == 3 || dig == 5 || dig == 6 || dig == 8 || dig == 0);
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 2, pos.x + width + h_move, pos.y + height + v_space * 2);

	draw->BindTex2D(seg_v);
	SEG_STATE(dig == 4 || dig == 5 || dig == 6 || dig == 8 || dig == 9 || dig == 0);
	draw->DrawRect2D(pos.x, pos.y + v_move, pos.x + width, pos.y + height + v_move);
	SEG_STATE(dig == 2 || dig == 6 || dig == 8 || dig == 0);
	draw->DrawRect2D(pos.x, pos.y + v_space + v_move, pos.x + width, pos.y + height + v_space + v_move);

	SEG_STATE(dig == 1 || dig == 2 || dig == 3 || dig == 4 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	draw->DrawRect2D(pos.x + h_space, pos.y + v_move, pos.x + width + h_space, pos.y + height + v_move);
	SEG_STATE(dig == 1 || dig == 3 || dig == 4 || dig == 5 || dig == 6 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	draw->DrawRect2D(pos.x + h_space, pos.y + v_space + v_move, pos.x + width + h_space, pos.y + height + v_space + v_move);
}
//-----------------------------------------------------------------------------------------------------------
void IM::SegInt(XY pos, int dig, int fig, bool negS) 
{
	draw->SwitchShader(SHADER_2D);
	int xOffset = (fig - 1) * 20;
	if (negS) {
		if (dig < 0) {
			dig = -dig;
			SEG_STATE(1);
		}
		else {
			SEG_STATE(0);
		}
		draw->BindTex2D(seg_h);
		draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 1, pos.x + width + h_move, pos.y + height + v_space * 1);
		xOffset += 20;
	}
	int div = 10;
	for (int i = 0; i < fig; i++) {
		int mod = dig % div;
		SegDigit(pos + XY{ xOffset, 0 }, dig % div * 10 / div);
		xOffset -= 20;
		div *= 10;
		dig -= mod;
	}
}
//-----------------------------------------------------------------------------------------------------------
void IM::SegDigitDecimal(XY pos, int dig, bool bDecimalPoint, Colour clrLedColour)
{
	bool bSegState;
	draw->SwitchShader(SHADER_2D);
	draw->BindTex2D(seg_h_128);

	bSegState = (dig == 2 || dig == 3 || dig == 5 || dig == 6 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);	
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 0, pos.x + width + h_move, pos.y + height + v_space * 0);
	
	bSegState = (dig == 2 || dig == 3 || dig == 4 || dig == 5 || dig == 6 || dig == 8 || dig == 9);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 1, pos.x + width + h_move, pos.y + height + v_space * 1);
	
	bSegState = (dig == 2 || dig == 3 || dig == 5 || dig == 6 || dig == 8 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 2, pos.x + width + h_move, pos.y + height + v_space * 2);
	
	draw->BindTex2D(seg_v_128);
	
	bSegState = (dig == 4 || dig == 5 || dig == 6 || dig == 8 || dig == 9 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x, pos.y + v_move, pos.x + width, pos.y + height + v_move);
	
	bSegState = (dig == 2 || dig == 6 || dig == 8 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x, pos.y + v_space + v_move, pos.x + width, pos.y + height + v_space + v_move);
	
	bSegState = (dig == 1 || dig == 2 || dig == 3 || dig == 4 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x + h_space, pos.y + v_move, pos.x + width + h_space, pos.y + height + v_move);
	
	bSegState = (dig == 1 || dig == 3 || dig == 4 || dig == 5 || dig == 6 || dig == 7 || dig == 8 || dig == 9 || dig == 0);
	IM::LEDColour(clrLedColour, bSegState);
	draw->DrawRect2D(pos.x + h_space, pos.y + v_space + v_move, pos.x + width + h_space, pos.y + height + v_space + v_move);

	//Decimal Point
	bSegState = false;
	IM::LEDColour(clrLedColour, bSegState);
	if (bDecimalPoint)
	{
		bSegState = true;
		IM::LEDColour(clrLedColour, bSegState);
	}
	draw->BindTex2D(seg_dot_128);
	//draw->DrawRect2D(pos.x + width + h_move, pos.y + v_space + v_move, pos.x + width + h_space + h_move, pos.y + height + v_space + v_move);//x1,y1,x2,y2
	draw->DrawRect2D(pos.x + width + h_move + 5, pos.y + height + v_move + 15, pos.x + width + h_space + h_move + 5, pos.y + height + v_space + v_move + 15);//x1,y1,x2,y2
}
//-----------------------------------------------------------------------------------------------------------
//XYPos, FigureToDisplay,iNumDigits,bSigned
void IM::SegFloatDecimal(XY pos, float dig, int fig, bool negS, Colour clrLedColour, int iNumDecimalPlace)
{
	draw->SwitchShader(SHADER_2D);
	int iNumFigure = fig;
	int xOffset = iNumFigure * x_offset;
	bool bSegState = false;
	char buff[64];
	IM::LEDColour(clrLedColour, bSegState);
	if (negS) 
	{
		iNumFigure = iNumFigure - 1; //Sign takes up one digit tile
		if (dig < 0) 
		{
			dig = -dig;				//change to positive number
			bSegState = true;
			IM::LEDColour(clrLedColour, bSegState);
		}
		draw->BindTex2D(seg_h_128);
		draw->DrawRect2D(pos.x + h_move, pos.y + v_space * 1, pos.x + width + h_move, pos.y + height + v_space * 1);
		xOffset -= x_offset;
	}
	
	float param, fractpart, intpart;
	param = dig;
	
	fractpart = modf(param, &intpart);
	int wholenumber = static_cast<int>(intpart);
	int i = 0;
	while (i < iNumDecimalPlace)
	{
		fractpart = fractpart * 10;
		i++;
	}
	int decimalnumber = static_cast<int>(fractpart);
	
	sprintf_s(buff, "%f = %d + %d\n", param, wholenumber, decimalnumber);
	printf(buff);
	int numbertowrite = (wholenumber * pow(10, iNumDecimalPlace)) + decimalnumber;
	sprintf_s(buff, "%d\n", numbertowrite);
	printf(buff);
	int div = 10;
	for (int i = 0; i < iNumFigure; i++)
	{
		int mod = numbertowrite % div;
		bool bInsertDecimal = false;
		if((i == (iNumDecimalPlace)) && (i != 0))
			bInsertDecimal = true;
		SegDigitDecimal(pos + XY{ xOffset, 0 }, numbertowrite % div * 10 / div, bInsertDecimal, clrLedColour);
		xOffset -= x_offset;
		div *= 10;
		numbertowrite -= mod;
	}
}
//-----------------------------------------------------------------------------------------------------------
bool IM::PushButton(XY renderAt) {
	draw->SwitchShader(SHADER_2D);
	draw->BindTex2D(push);
	draw->DrawRect2D(renderAt.x, renderAt.y, renderAt.x + 64, renderAt.y + 64);

	renderAt += XY{ 32,32 };
	return ((win->MouseX() - renderAt.x) * (win->MouseX() - renderAt.x) + (win->MouseY() - renderAt.y) * (win->MouseY() - renderAt.y) < 32 * 32) && win->MouseClicked(SDL_BUTTON_LEFT);
}
//-----------------------------------------------------------------------------------------------------------
bool IM::Button(XY pos, Font* f, std::string text) {
	int textWidth = f->GetTextWidth(text);
	XY size = XY{ textWidth + 15,20 };
	bool in = win->MouseX() > pos.x && win->MouseY() > pos.y && win->MouseX() < pos.x + size.x && win->MouseY() < pos.y + size.y;
	draw->SwitchShader(SHADER_2D);
	if (in) {
		if (win->MouseDown(SDL_BUTTON_LEFT)) draw->SetDrawColor2D(33.f / 256.f, 66.f / 256.f, 103.f / 256.f);
		else draw->SetDrawColor2D(53.f / 256.f, 86.f / 256.f, 123.f / 256.f);
	}
	else {
		draw->SetDrawColor2D(43.f / 256.f, 76.f / 256.f, 113.f / 256.f);
	}
	draw->BindTex2D(0);

	draw->DrawRect2D(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	draw->SwitchShader(SHADER_TEXT);
	draw->DrawString(f, text, pos.x + (size.x / 2) - (textWidth / 2), pos.y + 15);
	return in && win->MouseClicked(SDL_BUTTON_LEFT);
}
//-----------------------------------------------------------------------------------------------------------
bool IM::Button(XY pos, XY size, GLuint tex) {
	bool in = win->MouseX() > pos.x && win->MouseY() > pos.y && win->MouseX() < pos.x + size.x && win->MouseY() < pos.y + size.y;
	draw->SwitchShader(SHADER_2D);
	if (in) {
		if (win->MouseDown(SDL_BUTTON_LEFT)) draw->SetDrawColor2D(0.8, 0.8, 0.8);
		else draw->SetDrawColor2D(1, 1, 1);
	}
	else {
		draw->SetDrawColor2D(0.9, 0.9, 0.9);
	}
	draw->BindTex2D(tex);
	draw->DrawRect2D(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	draw->BindTex2D();
	return in && win->MouseClicked(SDL_BUTTON_LEFT);
}
//-----------------------------------------------------------------------------------------------------------
bool IM::Radio(XY pos, bool* ptr, Font* f, std::string text) {
	draw->SwitchShader(SHADER_2D);
	draw->SetDrawColor2D(1, 1, 1);
	draw->BindTex2D(*ptr ? IM::rad1 : IM::rad0);
	draw->DrawRect2D(pos.x, pos.y, pos.x + 16, pos.y + 16);
	draw->BindTex2D();
	draw->SwitchShader(SHADER_TEXT);
	draw->SetTextColor(1, 1, 1);
	draw->DrawString(f, text, pos.x + 20, pos.y + 14);
	if (win->MouseX() > pos.x && win->MouseY() > pos.y && win->MouseX() < pos.x + 16 && win->MouseY() < pos.y + 16) {
		if (win->MouseClicked(SDL_BUTTON_LEFT)) {
			*ptr = !*ptr;
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------
#define YOFFSET 32
#define XOFFSET 2

extern Font* f;
//-----------------------------------------------------------------------------------------------------------
void IM::LED(XY pos, bool state) {
	draw->SwitchShader(SHADER_2D);
	draw->SetDrawColor2D(1, 1, 1);
	draw->BindTex2D(state ? led1 : led0);
	draw->DrawRect2D(pos.x, pos.y, pos.x + 32, pos.y + 32);
}
//-----------------------------------------------------------------------------------------------------------
bool IM::ToggleSwitch(XY pos, std::string text, bool* ptr) {
	LED(XY{ pos.x + 16, pos.y }, *ptr);
	draw->BindTex2D(*ptr ? IM::T_UP : IM::T_DOWN);
	int offset = *ptr ? YOFFSET : YOFFSET + 6;
	draw->DrawRect2D(pos.x + XOFFSET, pos.y + offset, pos.x + TOGGLE_SIZE + XOFFSET, pos.y + TOGGLE_SIZE + offset);
	draw->BindTex2D();
	draw->SwitchShader(SHADER_TEXT);
	draw->SetTextColor(1, 1, 1);
	draw->DrawString(f, text, pos.x + TOGGLE_SIZE / 2 - f->GetTextWidth(text) / 2, pos.y + TOGGLE_SIZE + 54);
	if (win->MouseX() > pos.x + XOFFSET && win->MouseY() > pos.y + offset && win->MouseX() < pos.x + XOFFSET + TOGGLE_SIZE && win->MouseY() < pos.y + offset + TOGGLE_SIZE) {
		if (win->MouseClicked(SDL_BUTTON_LEFT)) {
			*ptr = !*ptr;
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------