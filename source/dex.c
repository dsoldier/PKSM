/*  This file is part of PKSM
>	Copyright (C) 2016/2017 Bernardo Giordano
>
>   This program is free software: you can redistribute it and/or modify
>   it under the terms of the GNU General Public License as published by
>   the Free Software Foundation, either version 3 of the License, or
>   (at your option) any later version.
>
>   This program is distributed in the hope that it will be useful,
>   but WITHOUT ANY WARRANTY; without even the implied warranty of
>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>   GNU General Public License for more details.
>
>   You should have received a copy of the GNU General Public License
>   along with this program.  If not, see <http://www.gnu.org/licenses/>.
>   See LICENSE for information.
*/

#include "dex.h"

static const u16 formtable_SM[] = // u16 species, u16 formcount
{
	0x0003, 0x0002, 0x0006, 0x0003, 0x0009, 0x0002, 0x000F, 0x0002,
	0x0012, 0x0002, 0x0013, 0x0002, 0x0014, 0x0003, 0x0019, 0x0007,
	0x001A, 0x0002, 0x001B, 0x0002, 0x001C, 0x0002, 0x0025, 0x0002,
	0x0026, 0x0002, 0x0032, 0x0002, 0x0033, 0x0002, 0x0034, 0x0002,
	0x0035, 0x0002, 0x0041, 0x0002, 0x004A, 0x0002, 0x004B, 0x0002,
	0x004C, 0x0002, 0x0050, 0x0002, 0x0058, 0x0002, 0x0059, 0x0002,
	0x005E, 0x0002, 0x0067, 0x0002, 0x0069, 0x0002, 0x0073, 0x0002,
	0x007F, 0x0002, 0x0082, 0x0002, 0x008E, 0x0002, 0x0096, 0x0003,
	0x00B5, 0x0002, 0x00C9, 0x001C, 0x00D0, 0x0002, 0x00D4, 0x0002,
	0x00D6, 0x0002, 0x00E5, 0x0002, 0x00F8, 0x0002, 0x00FE, 0x0002,
	0x0101, 0x0002, 0x0104, 0x0002, 0x011A, 0x0002, 0x012E, 0x0002,
	0x012F, 0x0002, 0x0132, 0x0002, 0x0134, 0x0002, 0x0136, 0x0002,
	0x013F, 0x0002, 0x0143, 0x0002, 0x014E, 0x0002, 0x015F, 0x0004,
	0x0162, 0x0002, 0x0167, 0x0002, 0x016A, 0x0002, 0x0175, 0x0002,
	0x0178, 0x0002, 0x017C, 0x0002, 0x017D, 0x0002, 0x017E, 0x0002,
	0x017F, 0x0002, 0x0180, 0x0002, 0x0182, 0x0004, 0x019C, 0x0003,
	0x019D, 0x0003, 0x01A5, 0x0002, 0x01A6, 0x0002, 0x01A7, 0x0002,
	0x01AC, 0x0002, 0x01BD, 0x0002, 0x01C0, 0x0002, 0x01CC, 0x0002,
	0x01DB, 0x0002, 0x01DF, 0x0006, 0x01E7, 0x0002, 0x01EC, 0x0002,
	0x01ED, 0x0012, 0x0213, 0x0002, 0x0226, 0x0002, 0x022B, 0x0002,
	0x0249, 0x0004, 0x024A, 0x0004, 0x0281, 0x0002, 0x0282, 0x0002,
	0x0285, 0x0002, 0x0286, 0x0003, 0x0287, 0x0002, 0x0288, 0x0002,
	0x0289, 0x0005, 0x0292, 0x0003, 0x029A, 0x0014, 0x029D, 0x0005,
	0x029E, 0x0006, 0x029F, 0x0005, 0x02A4, 0x000A, 0x02A6, 0x0002,
	0x02A9, 0x0002, 0x02C6, 0x0004, 0x02C7, 0x0004, 0x02CC, 0x0002,
	0x02CE, 0x0005, 0x02CF, 0x0002, 0x02D0, 0x0002, 0x02DF, 0x0002,
	0x02E2, 0x0002, 0x02E5, 0x0004, 0x02E9, 0x0002, 0x02EA, 0x0002,
	0x02F2, 0x0002, 0x02F6, 0x0002, 0x0305, 0x0012, 0x0306, 0x000E,
	0x030A, 0x0004, 0x0310, 0x0002, 0x0321, 0x0002
}; // 230

static const u16 formtable_USUM[] = // u16 species, u16 formcount
{
	0x0003, 0x0002, 0x0006, 0x0003, 0x0009, 0x0002, 0x000F, 0x0002,
	0x0012, 0x0002, 0x0013, 0x0002, 0x0014, 0x0003, 0x0019, 0x0008,
	0x001A, 0x0002, 0x001B, 0x0002, 0x001C, 0x0002, 0x0025, 0x0002,
	0x0026, 0x0002, 0x0032, 0x0002, 0x0033, 0x0002, 0x0034, 0x0002,
	0x0035, 0x0002, 0x0041, 0x0002, 0x004A, 0x0002, 0x004B, 0x0002,
	0x004C, 0x0002, 0x0050, 0x0002, 0x0058, 0x0002, 0x0059, 0x0002,
	0x005E, 0x0002, 0x0067, 0x0002, 0x0069, 0x0003, 0x0073, 0x0002,
	0x007F, 0x0002, 0x0082, 0x0002, 0x008E, 0x0002, 0x0096, 0x0003,
	0x00B5, 0x0002, 0x00C9, 0x001C, 0x00D0, 0x0002, 0x00D4, 0x0002,
	0x00D6, 0x0002, 0x00E5, 0x0002, 0x00F8, 0x0002, 0x00FE, 0x0002,
	0x0101, 0x0002, 0x0104, 0x0002, 0x011A, 0x0002, 0x012E, 0x0002,
	0x012F, 0x0002, 0x0132, 0x0002, 0x0134, 0x0002, 0x0136, 0x0002,
	0x013F, 0x0002, 0x0143, 0x0002, 0x014E, 0x0002, 0x015F, 0x0004,
	0x0162, 0x0002, 0x0167, 0x0002, 0x016A, 0x0002, 0x0175, 0x0002,
	0x0178, 0x0002, 0x017C, 0x0002, 0x017D, 0x0002, 0x017E, 0x0002,
	0x017F, 0x0002, 0x0180, 0x0002, 0x0182, 0x0004, 0x019C, 0x0003,
	0x019D, 0x0003, 0x019E, 0x0003, 0x01A5, 0x0002, 0x01A6, 0x0002,
	0x01A7, 0x0002, 0x01AC, 0x0002, 0x01BD, 0x0002, 0x01C0, 0x0002,
	0x01CC, 0x0002, 0x01DB, 0x0002, 0x01DF, 0x0006, 0x01E7, 0x0002,
	0x01EC, 0x0002, 0x01ED, 0x0012, 0x0213, 0x0002, 0x0226, 0x0002,
	0x022B, 0x0002, 0x0249, 0x0004, 0x024A, 0x0004, 0x0281, 0x0002,
	0x0282, 0x0002, 0x0285, 0x0002, 0x0286, 0x0003, 0x0287, 0x0002,
	0x0288, 0x0002, 0x0289, 0x0005, 0x0292, 0x0003, 0x0298, 0x0014,
	0x0299, 0x0014, 0x029A, 0x0014, 0x029D, 0x0005, 0x029E, 0x0006,
	0x029F, 0x0005, 0x02A4, 0x000A, 0x02A6, 0x0002, 0x02A9, 0x0002,
	0x02C6, 0x0004, 0x02C7, 0x0004, 0x02CC, 0x0002, 0x02CE, 0x0005,
	0x02CF, 0x0002, 0x02D0, 0x0002, 0x02DF, 0x0002, 0x02E2, 0x0002,
	0x02E5, 0x0004, 0x02E7, 0x0002, 0x02E8, 0x0002, 0x02E9, 0x0003,
	0x02EA, 0x0002, 0x02F0, 0x0002, 0x02F2, 0x0002, 0x02F6, 0x0002,
	0x0305, 0x0012, 0x0306, 0x000E, 0x0309, 0x0002, 0x030A, 0x0004,
	0x0310, 0x0002, 0x0320, 0x0004, 0x0321, 0x0002
}; // 246

int getDexFormIndex(int species, int formct, int start) {
	bool usum = game_getisUSUM();
	int count = usum ? 246 : 230;
	
	int formindex = start;
	int f = 0;
	for (int i = 0; i < count; i += 2)
	{
		int s = usum ? formtable_USUM[i] : formtable_SM[i];
		f = usum ? formtable_USUM[i + 1] : formtable_SM[i + 1];
		if (s == species)
		{
			break;
		}
		formindex += f - 1;
	}

	return f > formct ? -1 : formindex;
}

int getDexFormCount(int species)
{
	bool usum = game_getisUSUM();
	int count = usum ? 246 : 230;
	
	for (int i = 0; i < count; i+= 2)
	{
		if (usum && formtable_USUM[i] == species)
		{
			return formtable_USUM[i + 1];
		}
		else if (formtable_SM[i] == species)
		{
			return formtable_SM[i + 1];
		}
	}
	
	return 0;
}

void setDexFlags(u8 mainbuf[], int index, int gender, int shiny, int baseSpecies) {
	const int brSize = 0x8C;
	int shift = gender | (shiny << 1);
	int off = ofs.pokedex + 0x08 + 0x80 + 0x68;
	int bd = index >> 3; 
	int bm = index & 7;
	int bd1 = baseSpecies >> 3;
	int bm1 = baseSpecies & 7;

	int brSeen = shift * brSize;
	mainbuf[off + brSeen + bd] |= (u8)(1 << bm);

	bool displayed = false;
	for (int i = 0; i < 4; i++) {
		int brDisplayed = (4 + i) * brSize;
		displayed |= (mainbuf[off + brDisplayed + bd1] & (u8)(1 << bm1)) != 0;
	}

	if (!displayed && baseSpecies != index) {
		for (int i = 0; i < 4; i++) {
			int brDisplayed = (4 + i) * brSize;
			displayed |= (mainbuf[off + brDisplayed + bd] & (u8)(1 << bm)) != 0;
		}
	}
	if (displayed)
		return;

	mainbuf[off + (4 + shift) * brSize + bd] |= (u8)(1 << bm);
}

bool sanitizeFormsToIterate(int species, int fsfe[], int formIn) {
	switch (species) {
		case 351 : // Castform
			fsfe[0] = 0;
			fsfe[1] = 3;
			return true;
		case 20 : // Raticate
		case 105 : // Marowak
			fsfe[0] = 0;
			fsfe[1] = 1;
			return true;
		case 735: // Gumshoos
		case 758: // Salazzle
		case 754: // Lurantis
		case 738: // Vikavolt
		case 784: // Kommo-o
		case 752: // Araquanid
		case 777: // Togedemaru
		case 743: // Ribombee
		case 744: // Rockruff
			break;
		case 421: // Cherrim
		case 555: // Darmanitan
		case 648: // Meloetta
		case 746: // Wishiwashi
		case 778: // Mimikyu
			fsfe[0] = 0;
			fsfe[1] = 1;
			return true;
		case 774 : // Minior
			if (formIn <= 6) break;
			else {
				int count = getDexFormCount(species);
				fsfe[0] = 0;
				fsfe[1] = 0;
				return count < formIn;
			} break;
		case 718 :
			if (formIn > 1) break;
			else {
				int count = getDexFormCount(species);
				fsfe[0] = 0;
				fsfe[1] = 0;
				return count < formIn;
			} break;
	}
	
	fsfe[0] = 0;
	fsfe[1] = 0;
	return true;
}

void setDex(u8 mainbuf[], u8* pkmn) {
	if (!game_isgen7())
		return;
	
	int n = pkx_get_species(pkmn);
	int MaxSpeciesID = ofs.maxSpecies;
	int PokeDex = ofs.pokedex;
	int PokeDexLanguageFlags = ofs.pokedex + 0x550;
	
	if (n == 0 || n > MaxSpeciesID)
		return;
	if (pkx_is_egg(pkmn))
		return;

	int bit = n - 1;
	int bd = bit >> 3;
	int bm = bit & 7;
	int gender = pkx_get_gender(pkmn) % 2;
	int shiny = pkx_is_shiny(pkmn) ? 1 : 0;
	if (n == 351)
		shiny = 0;
	int shift = gender | (shiny << 1);
	if (n == 327) { // Spinda
		if ((mainbuf[PokeDex + 0x84] & (1 << (shift + 4))) != 0) { // Already 2
			u32 encryptionkey;
			memcpy(&encryptionkey, &pkmn[0x0], 4);
			memcpy(&mainbuf[PokeDex + 0x8E8 + shift * 4], &encryptionkey, 4);
			mainbuf[PokeDex + 0x84] |= (u8)(1 << shift);
		} else if ((mainbuf[PokeDex + 0x84] & (1 << shift)) == 0) // Not yet 1
			mainbuf[PokeDex + 0x84] |= (u8)(1 << shift); // 1
	}
	
	int off = PokeDex + 0x08 + 0x80;
	mainbuf[off + bd] |= (u8)(1 << bm);

	int formstart = pkx_get_form(pkmn);
	int formend = formstart;
	
	int fsfe[2] = {0, 0};
	if (sanitizeFormsToIterate(pkx_get_species(pkmn), fsfe, formstart)) {
		formstart = fsfe[0];
		formend = fsfe[1];
	}

	for (int form = formstart; form <= formend; form++) {
		int bitIndex = bit;
		if (form > 0) {
			u8 fc;
			memcpy(&fc, &personal.pkmData[pkx_get_species(pkmn)][0x0C], 1);
			if ((int)fc > 1) { // actually has forms
				int f = getDexFormIndex(pkx_get_species(pkmn), (int)fc, MaxSpeciesID - 1);
				if (f >= 0) // bit index valid
					bitIndex = f + form;
			}
		}
		setDexFlags(mainbuf, bitIndex, gender, shiny, n - 1);
	}

	int lang = pkx_get_language(pkmn);
	const int langCount = 9;
	if (lang <= 10 && lang != 6 && lang != 0) {
		if (lang >= 7)
			lang--;
		lang--;
		if (lang < 0) lang = 1;
		int lbit = bit * langCount + lang;
		if (lbit >> 3 < 920)
			mainbuf[PokeDexLanguageFlags + (lbit >> 3)] |= (u8)(1 << (lbit & 7));
	}
}

bool getCaught(u8* mainbuf, int species) {
	int PokeDex = ofs.pokedex;
	int miscdata = 0;
	if (game_isgen7()) {
		miscdata = 0x80;
	}
	
	int bit = species - 1;
	int bd = bit >> 3;
	int bm = bit & 7; 
	int off = PokeDex + 0x08 + miscdata;
			  
	if (game_isgen6()) {
		if ((1 << bm & mainbuf[off + bd]) != 0)
			return true;

		if (game_getisORAS() || bit >= 649)
			return false;
		
		return (1 << bm & mainbuf[off + bd + 0x644]) != 0;		
	}

	return (1 << bm & mainbuf[off + bd]) != 0;
}

bool getSeen(u8* mainbuf, int species) {
	int PokeDex = ofs.pokedex;
	int miscdata = 0;
	int brSize = 0;
	
	if (game_isgen7()) {
		miscdata = 0x80;
		brSize = 0x8C;
	} else if (game_getisORAS()) {
		brSize = 0x60;
	} else if (game_getisXY()) {
		brSize = 0x60;
	}

	int bit = species - 1;
	int bd = bit >> 3;
	int bm = bit & 7;
	u8 mask = (u8)(1 << bm);
	int off = PokeDex + 0x08 + miscdata; 

	for (int i = 1; i <= 4; i++)
		if ((mainbuf[off + bd + i * brSize] & mask) != 0)
			return true;
	return false;
}