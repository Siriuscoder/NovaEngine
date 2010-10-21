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
	// Загрузим шрифт из файла. Если файла шрифта не существует или шрифт битый,
	// то программа может умереть.
	if (FT_New_Face(library, ttffile.c_str(), 0, &face))
		throw NOVA_EXP("CFreeFont::BuildFont - Loading free font failed \
			(there is probably a problem with your font file)", BAD_OPERATION);
	// По некоторым причинам FreeType измеряет размер шрифта в терминах 1/64 пикселя.
	// Таким образом, для того чтобы сделать шрифт выстой h пикселей, мы запрашиваем размер h*64.
	// (h << 6 тоже самое что и h*64)
	FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
	slot = face->glyph;

	letters_map.clear();

	for(word ch = base_plane_begin; ch <= base_plane_end; ++ch)
		MakeLetter(ch);
	for(word ch = rus_begin; ch <= rus_end; ++ch)
		MakeLetter(ch);

	// Уничтожим шрифт.
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
	// Первая вещь, которую нам надо сделать, это вывести наш символ
	// в растр. Это делается набором команд FreeType

	// Загрузить глифы для каждого символа.
	if(FT_Load_Glyph(face, FT_Get_Char_Index(face, code), FT_LOAD_DEFAULT))
		throw NOVA_EXP("CFreeFont::MakeLetter - FT_Load_Glyph failed", BAD_OPERATION);

	// Поместить глиф в объект.
	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph))
		throw NOVA_EXP("CFreeFont::MakeLetter - FT_Get_Glyph failed", BAD_OPERATION);

	// Конвертировать глиф в растр.
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	// С помощью этой ссылки, получаем легкий доступ до растра.
	FT_Bitmap & bitmap = bitmap_glyph->bitmap;

    // Используем нашу вспомогательную функцию для вычисления ширины и высоты
	// текстуры для нашего растра.
	int bwidth = NextP2(bitmap.width);
	int bheight = NextP2(bitmap.rows);

	// Выделим память для данных текстуры.
	//byte * expanded_data = NULL;
	//expanded_data = getmem<byte>(expanded_data, 2 * bwidth * bheight);
	CMemoryBuffer mem;
	mem.AllocBuffer(2 * bwidth * bheight);
	byte * expanded_data = (nova::byte *)mem.GetBegin();

	// Поместим данные в расширенный растр.
	// Отмечу, что использован двухканальный растр (Один для
	// канала яркости и один для альфа), но мы будем назначать
	// обоим каналам одно и тоже значение, которое мы
	// получим из растра FreeType.
	// Мы используем оператор ?: для того чтобы поместить 0 в зону вне растра FreeType.
	for(int j = 0; j < bheight; ++j)
		for(int i = 0; i < bwidth; ++i)
			expanded_data[2*(i + j * bwidth)] = expanded_data[2*(i + j * bwidth)+1] =
				(i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];

/*	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture( GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Здесь мы создаем текстуру
	// Помните, что используем GL_LUMINANCE_ALPHA, чтобы было два альфа канала данных
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bwidth, bheight, 0,
		GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
*/
	//int tid = CTextureManager::GetSingeltonPtr()->AddTexture(GL_TEXTURE_2D,
	//	expanded_data, bwidth, bheight, CImageFormats::NF_LUMINANCE_ALPHA);
/*	CTexturePtr ptex = CTextureManager::GetSingeltonPtr()->AddTexture(new CTexture(fname.c_str(), GL_TEXTURE_2D));
	ptex->SetEnvType(GL_MODULATE);
	ptex->CreateTexture(expanded_data, bwidth, bheight, CImageFormats::NF_LUMINANCE_ALPHA, GL_CLAMP);

	// После создания текстуры, мы больше не нуждаемся в промежуточных данных.
	freemems(expanded_data);

	CLetter letter(slot->metrics.horiBearingX >> 6, slot->metrics.horiBearingY >> 6,
		slot->metrics.horiAdvance >> 6, bwidth, bheight, bitmap.width, bitmap.rows,
		code, ptex);

*/
	nstring resnamet;
	nstring resnamei("TempFontImage");
	resnamet = mName + "_" + CStringUtils::IntTo16xString(static_cast<int>(code));
// Создаем промежуточное изображение в памяти
	CImagePtr pImage = CImageManager::GetSingelton().CreateNewImage(resnamei, mName, mem,
		bwidth, bheight, 1, CImageFormats::NF_LUMINANCE_ALPHA, CResource::NV_FREE);
// На базе изображения создаем текстуру
	CTexturePtr texp = CTextureManager::GetSingelton().CreateNewTexture(resnamet, mName, pImage);

	// После создания текстуры, мы больше не нуждаемся в промежуточных данных.
	mem.FreeBuffer();

	CLetter letter(slot->metrics.horiBearingX >> 6, slot->metrics.horiBearingY >> 6,
		slot->metrics.horiAdvance >> 6, bwidth, bheight, bitmap.width, bitmap.rows,
		code, texp);
	mSize += texp->GetSize();

/// Создаем дисплейный список на букву ///////////////////////////////////////////////
	letter.GetDispList().CreateList();
	letter.GetDispList().BeginList();
/// //////////////////////////////////////////////////////////////////////////////////
	if(!texp.IsNull())
		texp->ApplyTexture();

// Вычислим какая часть нашей текстуры будет заполнена пустым пространством.
// Мы рисуем только ту часть текстуры, в которой находится символ, и сохраняем
// информацию в переменных x и y, затем, когда мы рисуем четырехугольник,
// мы будем только ссылаться на ту часть текстуры, в которой непосредственно
// содержится символ.
	nReal x = static_cast<nReal>(letter.GetBitmapw()) / static_cast<nReal>(letter.GetWidth()),
		y = static_cast<nReal>(letter.GetBitmapr()) / static_cast<nReal>(letter.GetHeight());

	//glBindTexture(GL_TEXTURE_2D, let.GetTex());

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2i(0,0);
		glTexCoord2f(0,y);
		glVertex2i(0,letter.GetBitmapr());
		glTexCoord2f(x,y);
		glVertex2i(letter.GetBitmapw(), letter.GetBitmapr());
		glTexCoord2f(x,0);
		glVertex2i(letter.GetBitmapw(), 0);
	glEnd();

/// Завершаем дисплейный список //////////////////////////////////////////////////
	letter.GetDispList().EndList();
/// //////////////////////////////////////////////////////////////////////////////

	letters_map[code] = letter;
}

void CFreeFont::Draw(POINT & pos, const CColorRGB & pc, const wchar_t code)
{
	stl<wchar_t, CLetter>::map::iterator it;
	it = letters_map.find(code);
	if(it == letters_map.end())
		return;

	CLetter let = (*it).second;
	//glEnable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3fv(pc);

	//CLetter let(letters_map[code]);
//	if(!let.GetTex().IsNull())
//		let.GetTex()->ApplyTexture();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	if(coord_grid == FT_COORD_NORMAL)
	{
		glTranslatef(static_cast<nReal>(pos.x), static_cast<nReal>(pos.y), 0);
		glTranslatef(static_cast<nReal>(let.GetBearingX()), static_cast<nReal>(let.GetBearingY() - let.GetBitmapr()), 0);
	}
	else if(coord_grid == FT_COORD_LEFT_TOP)
	{
		glTranslatef(static_cast<nReal>(pos.x), static_cast<nReal>(pos.y-let.GetBitmapr()), 0);
		glTranslatef(static_cast<nReal>(let.GetBearingX()), static_cast<nReal>(let.GetBitmapr() - let.GetBearingY()), 0);
	}

	// вызываем созданный список ----
	let.GetDispList().SetList();

	glPopMatrix();
	pos.x += let.GetAdvance();
}

CFreeFont::~CFreeFont()
{
	nova::nstringstream str;
	str << "CFreeFont::~CFreeFont() " << "name: " << mName << " group: " << mGroup;
	LOG_MESSAGE(str.str().c_str());

	FreeResource();
}

void CFreeFont::FreeResource()
{
/*
	nova::stl<wchar_t, CLetter>::map::iterator it;
	for(it = letters_map.begin(); it != letters_map.end(); it++)
		CTextureManager::GetSingeltonPtr()->DeleteTexture((*it).second.GetTex().GetPtr());
*/

	CResource::FreeResource();
	nova::stl<wchar_t, CLetter>::map::iterator it;
	for(it = letters_map.begin(); it != letters_map.end(); it++)
		(*it).second.GetDispList().DestroyList();

	//letters_map[code]
	CTextureManager::GetSingelton().UnloadResourceGroupFromHash(mName);
	letters_map.clear();
}


template<> CFontManager * CSingelton<CFontManager>::SingeltonObject = NULL;

CFontManager::CFontManager()
{
	// Инициализация библиотеки FreeType.
	if(FT_Init_FreeType(&library))
		throw NOVA_EXP("CFontManager::CFontManager() - Error Init freetype fonts..", BAD_OPERATION);

	LOG_MESSAGE("Font manager created..");
}

CFontManager::~CFontManager()
{
	UnloadAllManagerResources();
	if(FT_Done_FreeType(library))
		throw NOVA_EXP("CFontManager::CFontManager() - Error done freetype lib..", BAD_OPERATION);

	LOG_MESSAGE("Font manager destroyed..");
}

CResourcePtr CFontManager::CreateInstance(const nstring & name,
	const nstring & group, CResource::TAttach state)
{
	CFontPtr fontp(new CFreeFont(this, name, group, state));
	return fontp;
}

CFontPtr CFontManager::CreateNewFont(const nstring & name, const nstring & group,
	nova::uint height, const nstring & ttffile, nova::uint coord_grid,
	CResource::TAttach state)
{
	CFontPtr fontp = CResourceManager::AddNewResource(name, group, state);
	if(fontp.IsNull())
		throw NOVA_EXP("CFontManager::CreateNewFont - resource factory return \
							Null pointer...", MEM_ERROR);

	fontp->SetFontParam(library, height, ttffile, coord_grid);
	CResourceManager::BuildNextResource(name);

	nova::nstringstream str;
	str << "Font Factory: font object name: " << name << " group: " << group << " created...";
	LOG_MESSAGE(str.str().c_str());

	return fontp;
}

CFontPtr CFontManager::CreateNewFontAsync(const nstring & name, const nstring & group,
	nova::uint height, const nstring & ttffile, nova::uint coord_grid,
	CResource::TAttach state)
{
	CFontPtr fontp = CResourceManager::AddNewResource(name, group, state);
	if(fontp.IsNull())
		throw NOVA_EXP("CFontManager::CreateNewFont - resource factory return \
							Null pointer...", MEM_ERROR);

	fontp->SetFontParam(library, height, ttffile, coord_grid);
	mResourceBuildQueue.AddToQueue(fontp.GetPtr());
	nova::nstringstream str;
	str << "Font Factory: font object name: " << name << " group: " << group << " async created...";
	LOG_MESSAGE(str.str().c_str());

	return fontp;
}

void CFontManager::UnloadAllManagerResources()
{
	UnloadResourceFromHash(this);
}


}
