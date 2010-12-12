/***************************************************************************/
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


  /*************************************************************************/
  /*                                                                       */
  /* This is a Unix-specific version of <ft2build.h> that should be used   */
  /* exclusively *after* installation of the library.                      */
  /*                                                                       */
  /* It assumes that `/usr/local/include/freetype2' (or whatever is        */
  /* returned by the `freetype-config --cflags' or `pkg-config --cflags'   */
  /* command) is in your compilation include path.                         */
  /*                                                                       */
  /* We don't need to do anything special in this release.  However, for   */
  /* a future FreeType 2 release, the following installation changes will  */
  /* be performed:                                                         */
  /*                                                                       */
  /*   - The contents of `freetype-2.x/include/freetype' will be installed */
  /*     to `/usr/local/include/freetype2' instead of                      */
  /*     `/usr/local/include/freetype2/freetype'.                          */
  /*                                                                       */
  /*   - This file will #include <freetype2/config/ftheader.h>, instead    */
  /*     of <freetype/config/ftheader.h>.                                  */
  /*                                                                       */
  /*   - The contents of `ftheader.h' will be processed with `sed' to      */
  /*     replace all `<freetype/xxx>' with `<freetype2/xxx>'.              */
  /*                                                                       */
  /*   - Adding `/usr/local/include/freetype2' to your compilation include */
  /*     path will not be necessary anymore.                               */
  /*                                                                       */
  /* These changes will be transparent to client applications which use    */
  /* freetype-config (or pkg-config).  No modifications will be necessary  */
  /* to compile with the new scheme.                                       */
  /*                                                                       */
  /*************************************************************************/
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
#pragma once

/// \file nova_fonts.h
/// \brief Файл с описаниеим всего что нужно для построения шрифтов
///
/// Тут у нас легкий ковертор кодировок (интерфейс iconv) и расширенная реализация
/// шрифтов TrueType, я решил убрать вин шрифты, они совсем не гибкие, и их
/// все равно никто не будет использовать. Остается толь один тип шрифтов
/// FreeType2. Думаю все поймут почему я сделал выбор в пользу этой библиотеки)
#include "nova_unicode.h"
#include "nova_singelton.h"
#include "nova_texture_manager.h"
#include "nova_color.h"
#include "nova_display_list.h"

//#include <iconv.h>
//#include <localcharset.h>


namespace nova
{

#define FT_COORD_NORMAL		0x01
#define FT_COORD_LEFT_TOP	0x02

const nova::nUInt16 base_plane_begin = 0x0020;
const nova::nUInt16 base_plane_end = 0x007F;
const nova::nUInt16 rus_begin = 0x0410;
const nova::nUInt16 rus_end = 0x044F;

class CFreeFont;

class NOVA_EXPORT CNormalUnicode
{
private:
	//iconv_t cdlocale;
	//iconv_t cd;
public:

	static wchar_t Convert(char inch);
};

class NOVA_EXPORT CLetter : public CBase, public CListenerInterface
{
private:
	wchar_t letter;
	FT_Pos BearingX;
	FT_Pos BearingY;
	FT_Pos Advance;
	FT_Pos width;
    FT_Pos height;
	nInt32 bitmapw;
	nInt32 bitmapr;
	CTexturePtr tex;
	CDisplayList mDList;
public:
	CLetter() : letter(0), BearingX(0), BearingY(0), Advance(0), width(0), height(0),
		bitmapw(0), bitmapr(0), tex(NULL), CBase("CFreeFont") {}

	CLetter(const CLetter & s) : CBase("CFreeFont")
	{
		this->letter = s.letter;
		this->BearingX = s.BearingX;
		this->BearingY = s.BearingY;
		this->Advance = s.Advance;
		this->width = s.width;
		this->height = s.height;
		this->tex = s.tex;
		this->bitmapw = s.bitmapw;
		this->bitmapr = s.bitmapr;
		this->mDList = s.mDList;
	}

	CLetter(FT_Pos BearingX,
		FT_Pos BearingY,
		FT_Pos Advance,
		FT_Pos width,
		FT_Pos height,
		nInt32 bitmapw,
		nInt32 bitmapr,
		wchar_t letter,
		CTexturePtr texid);

	~CLetter() {}

	inline wchar_t GetLetter()
	{
		return letter;
	}

	inline FT_Pos GetBearingX()
	{
		return BearingX;
	}

	inline FT_Pos GetBearingY()
	{
		return BearingY;
	}

	inline FT_Pos GetAdvance()
	{
		return Advance;
	}

	inline FT_Pos GetWidth()
	{
		return width;
	}

    inline FT_Pos GetHeight()
	{
		return height;
	}

	inline nInt32 GetBitmapw()
	{
		return bitmapw;
	}

	inline nInt32 GetBitmapr()
	{
		return bitmapr;
	}

	inline CTexturePtr GetTex()
	{
		return tex;
	}

	inline CDisplayList & GetDispList()
	{
		return mDList;
	}
};


class NOVA_EXPORT CFreeFont : public CResource
{
private:
	nova::stl<wchar_t, CLetter>::map letters_map;
	FT_Face face;
	nova::nUInt32 height;
	nstring ttffile;
	nova::nUInt32 coord_grid;
	FT_Library library;
	FT_GlyphSlot slot;

	void MakeLetter(wchar_t code);
	void BuildFont();

protected:

	void LoadResourceImpl(void);

	void FreeResourceImpl(void);

	void BuildResourceImpl(void); 

public:
	CFreeFont(CResourceManager * rm, const nstring & name, const nstring & group, TAttach state);

	void SetFontParam(FT_Library library,
		nova::nUInt32 height,
		const nstring & ttffile,
		nova::nUInt32 coord_grid);

	~CFreeFont();

	void Draw(POINT & pos, const CColorRGB & pc, const wchar_t code);

};

typedef CSmartPtr<CFreeFont> CFontPtr;

class NOVA_EXPORT CFontManager : public CSingelton<CFontManager>, public CResourceManager
{
protected:

	FT_Library library;
public:

	CFontManager(/* texture manager */);
	~CFontManager();

	virtual void UnloadAllManagerResources();

	virtual CResourcePtr CreateInstance(const nstring & name,
		const nstring & group, CResource::TAttach state);

	CFontPtr CreateNewFont(const nstring & name, const nstring & group,
		nova::nUInt32 height, const nstring & ttffile, nova::nUInt32 coord_grid,
		CResource::TAttach state = CResource::NV_ATTACHED);

	CFontPtr CreateNewFont(const nstring & name, const nstring & group,
		nova::nUInt32 height, const CMemoryBuffer & ttf, nova::nUInt32 coord_grid,
		CResource::TAttach state = CResource::NV_ATTACHED);

	CResourcePtr LoadResourceFromXml(const nstring &filename, const CFilesPackage &package);

	CResourcePtr LoadResourceFromXml(const nstring &filename);
};

}
