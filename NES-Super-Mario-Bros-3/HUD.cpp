#include "HUD.h"
#include <string>

CHud::CHud()
{
	aniID = ID_ANI_HUD;

	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		scoreDigits[i] = new CDigit(false, 0);
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		coinDigits[i] = new CDigit(false, 0);
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		timeDigits[i] = new CDigit(false, 0);
	}
	for (int i = 0; i < MOMENTUM_COUNT; i++)
	{
		int pType = i == MOMENTUM_COUNT - 1;
		pMeter[i] = new CPMeter(pType, false);
	}
}

void CHud::UpdateTime(int time)
{
	int currentTime = (int)(time / 1000);
	string str_currentTime = to_string(currentTime);

	for (int i = 0; i < DIGIT_COUNT_TIME - str_currentTime.size(); i++)
		str_currentTime = "0" + str_currentTime;

	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		string digit_str;
		digit_str = digit_str + str_currentTime[i];
		int digit = atoi(digit_str.c_str());
		timeDigits[i]->SetDigit(digit);
	}
}

void CHud::UpdateCoin(int coin)
{
	string str_coin = to_string(coin);
	int str_length = str_coin.size();

	if (str_length == 1)
	{
		coinDigits[0]->SetEmpty(true);
		coinDigits[1]->SetDigit(coin);
	}
	else
	{
		string digit_0_str, digit_1_str;

		digit_0_str = digit_0_str + str_coin[str_length - 2];
		digit_1_str = digit_1_str + str_coin[str_length - 1];

		int digit_0 = atoi(digit_0_str.c_str());
		int digit_1 = atoi(digit_1_str.c_str());

		if (digit_0 == 0)
		{
			coinDigits[0]->SetEmpty(true);
		}
		else {
			coinDigits[0]->SetEmpty(false);
			coinDigits[0]->SetDigit(digit_0);
		}
		coinDigits[1]->SetDigit(digit_1);
	}
}

void CHud::UpdateScore(int score)
{
	string str_score = to_string(score);
	int str_length = str_score.size();

	for (int i = 0; i < DIGIT_COUNT_SCORE - str_length; i++)
		str_score = "0" + str_score;

	str_length = str_score.size();
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		string digit_str;
		digit_str = digit_str + str_score[str_length - (DIGIT_COUNT_SCORE - i)];
		int digit = atoi(digit_str.c_str());
		scoreDigits[i]->SetDigit(digit);
	}
}

void CHud::UpdaeMomentum(int momentum)
{
	for (int i = 0; i < MOMENTUM_COUNT; i++)
		pMeter[i]->SetToggle(false);
	for (int i = 0; i < momentum; i++)
		pMeter[i]->SetToggle(true);
}

void CHud::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		scoreDigits[i]->SetPosition((int)(x + SCORE_OFFSET + i * DIGIT_NEAR_SPACING), (int)(y + OFFSET_Y_LINE2));
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		timeDigits[i]->SetPosition((int)(x + TIME_OFFSET + i * DIGIT_NEAR_SPACING), (int)(y + OFFSET_Y_LINE2));
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		coinDigits[i]->SetPosition((int)(x + CURRENCY_OFFSET + i * DIGIT_NEAR_SPACING), (int)(y + OFFSET_Y_LINE1));
	}
	for (int i = 0; i < MOMENTUM_COUNT; i++)
	{
		int pOffset = (i == MOMENTUM_COUNT - 1) ? P_SWITCH_WIDTH / 4 : 0;
		pMeter[i]->SetPosition((int)(x + pOffset + P_METER_OFFSET + i * DIGIT_NEAR_SPACING), (int)(y + OFFSET_Y_LINE1));
	}
}

void CHud::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);

	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		scoreDigits[i]->Render();
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		coinDigits[i]->Render();
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		timeDigits[i]->Render();
	}
	for (int i = 0; i < MOMENTUM_COUNT; i++)
	{
		pMeter[i]->Render();
	}
}

CHud::~CHud()
{
	for (int i = 0; i < DIGIT_COUNT_SCORE; i++)
	{
		delete scoreDigits[i];
	}
	for (int i = 0; i < DIGIT_COUNT_CURRENCY; i++)
	{
		delete coinDigits[i];
	}
	for (int i = 0; i < DIGIT_COUNT_TIME; i++)
	{
		delete timeDigits[i];
	}
	for (int i = 0; i < MOMENTUM_COUNT; i++)
	{
		delete pMeter[i];
	}
}
