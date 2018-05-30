// Pi1541 - A Commodore 1541 disk drive emulator
// Copyright(C) 2018 Stephen White
//
// This file is part of Pi1541.
// 
// Pi1541 is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Pi1541 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Pi1541. If not, see <http://www.gnu.org/licenses/>.

#include "options.h"
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define INVALID_VALUE	((unsigned) -1)

char* TextParser::GetToken(bool includeSpace)
{
	bool isSpace;

	do
	{
	} while (ParseComment());

	while (*data != '\0')
	{
		isSpace = isspace(*data);

		if (!isSpace || (includeSpace && isSpace))
			break;

		data++;
	}

	if (*data == '\0')
		return 0;

	char* pToken = data;

	while (*data != '\0')
	{
		isSpace = isspace(*data);

		if ((!includeSpace && isSpace) || (*data == '\n') || (*data == '\r'))
		{
			*data++ = '\0';
			break;
		}
		data++;
	}

	return pToken;
}

void TextParser::SkipWhiteSpace()
{
	while (*data != '\0')
	{
		if (!isspace(*data))
			break;

		data++;
	}
}

bool TextParser::ParseComment()
{
	SkipWhiteSpace();

	if (*data == '\0')
		return 0;

	if (data[0] != '/')
		return false;

	if (data[1] == '/')
	{
		// One line comment
		data += 2;

		while (*data)
		{
			if (*data == '\n')
				break;

			data++;
		}
		SkipWhiteSpace();
		return true;
	}
	else if (data[1] == '*')
	{
		// Multiline comment
		data += 2;

		while (*data)
		{
			if (*data++ == '*' && *data && *data == '/')
			{
				data++;
				break;
			}
		}
		SkipWhiteSpace();
		return true;
	}
	return false;
}


Options::Options(void)
	: TextParser()
	, deviceID(8)
	, onResetChangeToStartingFolder(0)
	, extraRAM(0)
	, enableRAMBOard(0)
	, disableSD2IECCommands(0)
	, supportUARTInput(0)
	, graphIEC(0)
	, quickBoot(0)
	, displayPNGIcons(0)
	, soundOnGPIO(0)
	, invertIECInputs(0)
	, invertIECOutputs(1)
	, splitIECLines(0)
	, ignoreReset(0)
	, screenWidth(1024)
	, screenHeight(768)
{
	strcpy(ROMFontName, "chargen");
	starFileName[0] = 0;
	ROMName[0] = 0;
	ROMNameSlot2[0] = 0;
	ROMNameSlot3[0] = 0;
	ROMNameSlot4[0] = 0;
	ROMNameSlot5[0] = 0;
	ROMNameSlot6[0] = 0;
	ROMNameSlot7[0] = 0;
	ROMNameSlot8[0] = 0;
}

void Options::Process(char* buffer)
{
	SetData(buffer);

	char* pOption;
	while ((pOption = GetToken()) != 0)
	{
		/*char* equals = */GetToken();
		char* pValue = GetToken();

		if (strcasecmp(pOption, "deviceID") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				deviceID = nValue;
		}
		else if (strcasecmp(pOption, "OnResetChangeToStartingFolder") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				onResetChangeToStartingFolder = nValue;
		}
		else if (strcasecmp(pOption, "ExtraRAM") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				extraRAM = nValue;
		}
		else if (strcasecmp(pOption, "RAMBOard") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				enableRAMBOard = nValue;
		}
		else if (strcasecmp(pOption, "DisableSD2IECCommands") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				disableSD2IECCommands = nValue;
		}
		else if (strcasecmp(pOption, "SupportUARTInput") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				supportUARTInput = nValue;
		}
		else if (strcasecmp(pOption, "GraphIEC") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				graphIEC = nValue;
		}
		else if (strcasecmp(pOption, "QuickBoot") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				quickBoot = nValue;
		}
		else if (strcasecmp(pOption, "DisplayPNGIcons") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				displayPNGIcons = nValue;
		}
		else if (strcasecmp(pOption, "soundOnGPIO") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				soundOnGPIO = nValue;
		}
		else if (strcasecmp(pOption, "invertIECInputs") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				invertIECInputs = nValue;
		}
		else if (strcasecmp(pOption, "invertIECOutputs") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				invertIECOutputs = nValue;
		}
		else if (strcasecmp(pOption, "splitIECLines") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				splitIECLines = nValue;
		}
		else if (strcasecmp(pOption, "ignoreReset") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				ignoreReset = nValue;
		}
		else if (strcasecmp(pOption, "screenWidth") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				screenWidth = nValue;
		}
		else if (strcasecmp(pOption, "screenHeight") == 0)
		{
			unsigned nValue = 0;
			if ((nValue = GetDecimal(pValue)) != INVALID_VALUE)
				screenHeight = nValue;
		}
		else if ((strcasecmp(pOption, "Font") == 0))
		{
			strncpy(ROMFontName, pValue, 255);
		}
		else if ((strcasecmp(pOption, "StarFileName") == 0))
		{
			strncpy(starFileName, pValue, 255);
		}
		else if ((strcasecmp(pOption, "ROM") == 0) || (strcasecmp(pOption, "ROM1") == 0))
		{
			strncpy(ROMName, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM2") == 0)
		{
			strncpy(ROMNameSlot2, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM3") == 0)
		{
			strncpy(ROMNameSlot3, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM4") == 0)
		{
			strncpy(ROMNameSlot4, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM5") == 0)
		{
			strncpy(ROMNameSlot5, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM6") == 0)
		{
			strncpy(ROMNameSlot6, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM7") == 0)
		{
			strncpy(ROMNameSlot7, pValue, 255);
		}
		else if (strcasecmp(pOption, "ROM8") == 0)
		{
			strncpy(ROMNameSlot8, pValue, 255);
		}
	}
}

unsigned Options::GetDecimal(char* pString)
{
	if (pString == 0 || *pString == '\0')
		return INVALID_VALUE;

	unsigned nResult = 0;

	char chChar = *pString++;
	while (chChar != '\0' && chChar != 13)
	{
		if (!('0' <= chChar && chChar <= '9'))
			return INVALID_VALUE;

		unsigned nPrevResult = nResult;

		nResult = nResult * 10 + (chChar - '0');
		if (nResult < nPrevResult || nResult == INVALID_VALUE)
			return INVALID_VALUE;

		chChar = *pString++;
	}

	return nResult;
}

const char* Options::GetRomName(int index) const
{
	switch (index)
	{
	case 1:
		return ROMNameSlot2;
	case 2:
		return ROMNameSlot3;
	case 3:
		return ROMNameSlot4;
	case 4:
		return ROMNameSlot5;
	case 5:
		return ROMNameSlot6;
	case 6:
		return ROMNameSlot7;
	case 7:
		return ROMNameSlot8;
	}
	return ROMName;
}
