п»ї/***************************************************************************/
/*                                                                         */
/*  ft2build.h                                                             */
/*                                                                         */
/*    Build macros of the FreeType 2 library.                              */
/*                                                                         */
/*  Copyright 1996-2001, 2003, 2006 by                                     */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************
 *   Copyright (C) 2008 by SIRIUS										   *
 *   SiriusStarNick@yandex.ru											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program;					                           *
 ***************************************************************************/
#include "nova_stable_precompiled_headers.h"

#include "nova_fonts.h"
#include "nova_string_utils.h"

namespace nova {

wchar_t CNormalUnicode::Convert(char inch)
{
	//size_t in = strlen(in_buff);
	//return iconv(cd, &in_buff, &in, &out_buff, &outmaxlen);
	nova::nstring instr(&inch, 1);

	nova::nwstring outstr;
	outstr = unicode::utf_converter(instr).to_wstring();

	return outstr[0];
}

CLetter::CLetter(FT_Pos BearingX,
		FT_Pos BearingY,
		FT_Pos Advance,
		FT_Pos width,
		FT_Pos height,
		int bitmapw,
		int bitmapr,
		wchar_t letter,
		CTexturePtr texid) : CBase("CLetter")
{
	this->BearingX = BearingX;
	this->BearingY = BearingY;
	this->Advance = Advance;
	this->width = width;
	this->height = height;

	this->letter = letter;
	this->tex = texid;
	this->bitmapw = bitmapw;
	this->bitmapr = bitmapr;
}

CFreeFont::CFreeFont(CResourceManager * rm,
	const nstring & name,
	const nstring & group,
	TAttach state) : height(0), face(NULL), slot(NULL),
	library(NULL), CResource(rm, name, group, state)
{

}

void CFreeFont::PrepareResource(void)
{
	CResource::PrepareResource();
}

void CFreeFont::BuildResource(void)
{
	if(isReady)
		return;

	BuildFont();

	CResource::PrepareResource();
}

void CFreeFont::SetFontParam(FT_Library library,
		nova::uint height,
		const nstring & ttffile,
		nova::uint coord_grid)
{
	this->height = height;
	this->ttffile = ttffile;
	this->coord_grid = coord_grid;

	if(!library)
		throw NOVA_EXP("CFreeFont::CreateFont - FreeFont library object is bad", MEM_ERROR);
	this->library = library;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CFontListener * lis =
			dynamic_cast<CFontListener *>(GetListener(i));
		lis->CreateFontListener(this);
	}
}

void CFreeFont::BuildFont()
{
	// Р—Р°РіСЂСѓР·РёРј С€СЂРёС„С‚ РёР· С„Р°Р№Р»Р°. Р•СЃР»Рё С„Р°Р№Р»Р° С€СЂРёС„С‚Р° РЅРµ СЃСѓС‰РµСЃС‚РІСѓРµС‚ РёР»Рё С€СЂРёС„С‚ Р±РёС‚С‹Р№,
	// С‚Рѕ РїСЂРѕРіСЂР°РјРјР° РјРѕР¶РµС‚ СѓРјРµСЂРµС‚СЊ.
	if (FT_New_Face(library, ttffile.c_str(), 0, &face))
		throw NOVA_EXP("CFreeFont::BuildFont - Loading free font failed \
			(there is probably a problem with your font file)", BAD_OPERATION);
	// РџРѕ РЅРµРєРѕС‚РѕСЂС‹Рј РїСЂРёС‡РёРЅР°Рј FreeType РёР·РјРµСЂСЏРµС‚ СЂР°Р·РјРµСЂ С€СЂРёС„С‚Р° РІ С‚РµСЂРјРёРЅР°С… 1/64 РїРёРєСЃРµР»СЏ.
	// РўР°РєРёРј РѕР±СЂР°Р·РѕРј, РґР»СЏ С‚РѕРіРѕ С‡С‚РѕР±С‹ СЃРґРµР»Р°С‚СЊ С€СЂРёС„С‚ РІС‹СЃС‚РѕР№ h РїРёРєСЃРµР»РµР№, РјС‹ Р·Р°РїСЂР°С€РёРІР°РµРј СЂР°Р·РјРµСЂ h*64.
	// (h << 6 С‚РѕР¶Рµ СЃР°РјРѕРµ С‡С‚Рѕ Рё h*64)
	FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
	slot = face->glyph;

	letters_map.clear();

	for(word ch = base_plane_begin; ch <= base_plane_end; ++ch)
		MakeLetter(ch);
	for(word ch = rus_begin; ch <= rus_end; ++ch)
		MakeLetter(ch);

	// РЈРЅРёС‡С‚РѕР¶РёРј С€СЂРёС„С‚.
	FT_Done_Face(face);
	isReady = true;

	for(nova::uint i = 0; i < GetListenersCount(); i++)
	{
		CFontListener * lis =
			dynamic_cast<CFontListener *>(GetListener(i));
		lis->BuildFontListener(this);
	}
}

int NextP2(int a)
{
	int rval = 1;
    while(rval < a) rval <<= 1;

	return rval;
}

void CFreeFont::MakeLetter(wchar_t code)
{
	// РџРµСЂРІР°СЏ РІРµС‰СЊ, РєРѕС‚РѕСЂСѓСЋ РЅР°Рј РЅР°РґРѕ СЃРґРµР»Р°С‚СЊ, СЌС‚Рѕ РІС‹РІРµСЃС‚Рё РЅР°С€ СЃРёРјРІРѕР»
	// РІ СЂР°СЃС‚СЂ. Р­С‚Рѕ РґРµР»Р°РµС‚СЃСЏ РЅР°Р±РѕСЂРѕРј РєРѕРјР°РЅРґ FreeType

	// Р—Р°РіСЂСѓР·РёС‚СЊ РіР»РёС„С‹ РґР»СЏ РєР°Р¶РґРѕРіРѕ СЃРёРјРІРѕР»Р°.
	if(FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_DEFAULT))
		throw NOVA_EXP("CFreeFont::MakeLetter - FT_Load_Glyph failed", BAD_OPERATION);

	// РџРѕРјРµСЃС‚РёС‚СЊ РіР»РёС„ РІ РѕР±СЉРµРєС‚.
	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph))
		throw NOVA_EXP("CFreeFont::MakeLetter - FT_Get_Glyph failed", BAD_OPERATION);

	// РљРѕРЅРІРµСЂС‚РёСЂРѕРІР°С‚СЊ РіР»РёС„ РІ СЂР°СЃС‚СЂ.
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	// РЎ РїРѕРјРѕС‰СЊСЋ СЌС‚РѕР№ СЃСЃС‹Р»РєРё, РїРѕР»СѓС‡Р°РµРј Р»РµРіРєРёР№ РґРѕСЃС‚СѓРї РґРѕ СЂР°СЃС‚СЂР°.
	FT_Bitmap & bitmap = bitmap_glyph->bitmap;

    // Р