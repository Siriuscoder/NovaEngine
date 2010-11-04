/***************************************************************************
 *   Copyright (C) 2010 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine								   *
 *	 (Object-oriented Game Development Engine)							   *
 *																		   *
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
//
//  Purpose:
//
//    IVCON converts various 3D graphics files.
//
//  Acknowledgements:
//
//    Coding, comments, and advice were supplied by a number of collaborators.
//
//    Jean-Cristophe Hoelt (hoeltj AT tcd.ie) pointed out that the program was
//    not compiling under GNU C++, and I made the necessary corrections.
//
//    John F Flanagan made some corrections to the 3D Studio Max routines.
//
//    Zik Saleeba (zik AT zikzak.net) enhanced the DXF routines, and added the 
//    Golgotha GMOD routines.
//
//    Thanks to Susan M. Fisher, University of North Carolina,
//    Department of Computer Science, for pointing out a coding error
//    in FACE_NULL_DELETE that was overwriting all the data!
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    19 January 2004
//
//  Author:
//
//    John Burkardt
//

#include "nova_ase_loader.h"
#include "nova_formats.h"
#include "nova_string_utils.h"

#define COLOR_MAX		1000
#define COR3_MAX		200000
#define FACE_MAX		200000
#define LINE_MAX_LEN	256
#define LEVEL_MAX		10
#define LINES_MAX		100000
#define MATERIAL_MAX	100
#define ORDER_MAX		10
#define TEXTURE_MAX		100
#define G1_SECTION_MODEL_QUADS				18
#define G1_SECTION_MODEL_TEXTURE_NAMES		19
#define G1_SECTION_MODEL_VERT_ANIMATION		20
#define GMOD_MAX_SECTIONS					32
#define GMOD_UNUSED_VERTEX					0xFFFF

namespace nova
{

nInt32 CASELoader::LoadAseInternal(void)
//****************************************************************************80
//
//  Purpose:
//   
//    ASE_READ reads an AutoCAD ASE file.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    22 May 1999
//
//  Author:
//
//    John Burkardt
//
{
	float	bval;
	nInt32  count;
	float	gval;
	nInt32  i;
	nInt32  iface;
	nInt32  iword;
	nInt32  level;
	const char	*next;
	nInt32  nlbrack;
	nInt32  nrbrack;
	float	rval;
	float	temp;
	nInt32  width;
	char	word[LINE_MAX_LEN];
	char	word1[LINE_MAX_LEN];
	char	word2[LINE_MAX_LEN];
	char	wordm1[LINE_MAX_LEN];
	float	x;
	float	y;
	float	z;
	const char	*input;
	char	level_name[LEVEL_MAX][LINE_MAX_LEN];
	nInt32  text_num = 0;
	nstring input_buf;

	level = 0;
	strcpy ( level_name[0], "Top" );
	nlbrack = 0;
	nrbrack = 0;

	TMeshContainer *LastGeomObject = NULL;
	TMaterialContainer *LastMaterial = NULL;
	TMaterialContainer *LastSubMaterial = NULL;
	TTextureContainer *LastTexture = NULL;

	strcpy ( word, " " );
	strcpy ( wordm1, " " );
//
//  Read a line of text from the file.
//

	for ( ;; )
	{
		input_buf = mpStream->ReadLine((size_t)LINE_MAX_LEN);
		input = input_buf.c_str();

		text_num++;
		next = input;
		iword = 0;
//
//  Read the next word from the line.
//
	    for ( ;; )
	    {
			strcpy ( wordm1, word );
			strcpy ( word, " " );

	    	count = sscanf ( next, "%s%n", word, &width );
	    	next = next + width;

	    	if ( count <= 0 )
	    	{
	    		break;
	    	}

	    	iword = iword + 1;

	    	if ( iword == 1 )
	    	{
	    		strcpy ( word1, word );
	    	}
//
//  In case the new word is a bracket, update the bracket count.
//
	    	if ( strcmp ( word, "{" ) == 0 )
	    	{
				nlbrack = nlbrack + 1;
				level = nlbrack - nrbrack;
				strcpy ( level_name[level], wordm1 );
	    	}
	    	else if ( strcmp ( word, "}" ) == 0 )
	    	{
	    		nrbrack = nrbrack + 1;

	    		if ( nlbrack < nrbrack )
	    		{
					nstring err;
					err.append("void CASELoader::LoadAseInternal(void): ase file read fatal: Extraneous right bracket on line: ");
					err.append(CStringUtils::IntToString(text_num));
					err.append("\nCurrently processing field: ");
					err.append(level_name[level]);
					throw NOVA_EXP(err.c_str(), BAD_OPERATION);
	    		}
	    	}
//
//  *3DSMAX_ASCIIEXPORT  200
//
			if ( strcmp ( word1, "*3DSMAX_ASCIIEXPORT" ) == 0 )
			{
				break;
			}
//
//  *COMMENT
//
			else if ( strcmp ( word1, "*COMMENT" ) == 0 )
			{
				char comment[LINE_MAX_LEN];
				sscanf(next, "\"%s\"", comment);
				LOG_MESSAGE(nstring("void CASELoader::LoadAseInternal(void) ase comment: ") + comment);
				break;
			}
//
//  *GEOMOBJECT
//
			else if ( strcmp ( level_name[level], "*GEOMOBJECT" ) == 0 )
			{
       			if ( strcmp ( word, "{" ) == 0 )
       			{
       				continue;
       			}
       			else if ( strcmp ( word, "}" ) == 0 )
       			{
       				level = nlbrack - nrbrack;
					LastGeomObject = NULL;
       				continue;
       			}
//
//  Why don't you read and save this name?
//
				else if ( strcmp ( word, "*NODE_NAME" ) == 0 )
				{
					count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;
// Добавляем новый каркас в кеш объектов, сохраняем на него ссылку
					nstring nObjName(word2);
					mMeshesMap.insert(std::pair<nstring, TMeshContainer>(nObjName, TMeshContainer()));
					LastGeomObject = &(mMeshesMap[nObjName]);
					break;
				}
				else if ( strcmp ( word, "*NODE_TM" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MESH" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*PROP_CASTSHADOW" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*PROP_MOTIONBLUR" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*PROP_RECVSHADOW" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_REF" ) == 0 )
				{
					count = sscanf ( next, "%d%n", i, &width );
		    		next = next + width;

					if(!LastGeomObject)
						LastGeomObject->MatID = i;
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in GEOMOBJECT part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MESH
//
			else if ( strcmp ( level_name[level], "*MESH" ) == 0 )
			{
		        if ( strcmp ( word, "{" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "}" ) == 0 )
		        {
					level = nlbrack - nrbrack;
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_CFACELIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_CVERTLIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_FACE_LIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_NORMALS" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_NUMCVERTEX" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_NUMCVFACES" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_NUMFACES" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_NUMTVERTEX" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_NUMTVFACES" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_NUMVERTEX" ) == 0 )
		        {
					break;
		        }
		        else if ( strcmp ( word, "*MESH_TFACELIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_TVERTLIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*MESH_VERTEX_LIST" ) == 0 )
		        {
					continue;
		        }
		        else if ( strcmp ( word, "*TIMEVALUE" ) == 0 )
		        {
					break;
		        }
		        else
		        {
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
		        }
			}
//
//  *MESH_CFACELIST
//
			else if ( strcmp ( level_name[level], "*MESH_CFACELIST" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MESH_CFACE" ) == 0 )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_CFACELIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MESH_CVERTLIST
//
//  Mesh vertex indices must be incremented by COR3_NUM_OLD before being stored
//  in the internal array.
//
		    else if ( strcmp ( level_name[level], "*MESH_CVERTLIST" ) == 0 )
		    {
		    	if ( strcmp ( word, "{" ) == 0 )
		    	{
		    		continue;
		    	}
		    	else if ( strcmp ( word, "}" ) == 0 )
		    	{
		    		level = nlbrack - nrbrack;
		    		continue;
		    	}
		    	else if ( strcmp ( word, "*MESH_VERTCOL" ) == 0 )
		    	{
		    		count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%f%n", &rval, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%f%n", &gval, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%f%n", &bval, &width );
		    		next = next + width;
		    	}
		    	else
		    	{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_CVERTLIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
		    		break;
		    	}
		    }
//
//  *MESH_FACE_LIST
//  This coding assumes a face is always triangular or quadrilateral.
//
		    else if ( strcmp ( level_name[level], "*MESH_FACE_LIST" ) == 0 )
		    {
		    	if ( strcmp ( word, "{" ) == 0 )
		    	{
		    		continue;
		    	}
		    	else if ( strcmp ( word, "}" ) == 0 )
		    	{
		    		level = nlbrack - nrbrack;
		    		continue;
		    	}
		    	else if ( strcmp ( word, "*MESH_FACE" ) == 0 )
		    	{
		    		count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		if ( strcmp ( word2, "D:" ) == 0 )
		    		{
		    			count = sscanf ( next, "%d%n", &i, &width );
		    			next = next + width;
		    		}

		    		break;
		    	}
		    	else
		    	{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_FACE_LIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
		    		break;
		    	}
		    }
//
//  *MESH_NORMALS
//
			else if ( strcmp ( level_name[level], "*MESH_NORMALS" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MESH_FACENORMAL" ) == 0 )
				{
					count = sscanf ( next, "%d%n", &iface, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &x, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &y, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &z, &width );
					next = next + width;

					break;

				}
				else if ( strcmp ( word, "*MESH_VERTEXNORMAL" ) == 0 )
				{
					count = sscanf ( next, "%d%n", &i, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &x, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &y, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &z, &width );
					next = next + width;

					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_NORMALS part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MESH_TFACELIST
//
			else if ( strcmp ( level_name[level], "*MESH_TFACELIST" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word1, "*MESH_TFACE" ) == 0 )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_TFACELIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MESH_TVERTLIST
//
			else if ( strcmp ( level_name[level], "*MESH_TVERTLIST" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word1, "*MESH_TVERT" ) == 0  )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_TVERTLIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
				  break;
				}
			}
//
//  *MESH_VERTEX_LIST
//
			else if ( strcmp ( level_name[level], "*MESH_VERTEX_LIST" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word1, "*MESH_VERTEX" ) == 0 )
				{
					TVertex3d vertex;

					count = sscanf ( next, "%d%n", &i, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &vertex.x, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &vertex.y, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &vertex.z, &width );
					next = next + width;

					if(!LastGeomObject)
					{
						LastGeomObject->nVertexList.push_back(vertex);
					}

					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_VERTEX_LIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *NODE_TM
//
//  Each node should start out with a default transformation matrix.
//
			else if ( strcmp ( level_name[level], "*NODE_TM" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*INHERIT_POS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*INHERIT_ROT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*INHERIT_SCL" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*NODE_NAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_POS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_ROTANGLE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_ROTAXIS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_ROW0" ) == 0 )
				{
					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[0][0] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[1][0] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[2][0] = temp;

					break;
				}
				else if ( strcmp ( word, "*TM_ROW1" ) == 0 )
				{
					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[0][1] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[1][1] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[2][1] = temp;

					break;
				}
				else if ( strcmp ( word, "*TM_ROW2" ) == 0 )
				{
					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[0][2] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[1][2] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[2][2] = temp;

					break;
				}
				else if ( strcmp ( word, "*TM_ROW3" ) == 0 )
				{
					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[0][3] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[1][3] = temp;

					count = sscanf ( next, "%f%n", &temp, &width );
					next = next + width;
					//transform_matrix[2][3] = temp;

					break;
				}
				else if ( strcmp ( word, "*TM_SCALE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_SCALEAXIS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*TM_SCALEAXISANG" ) == 0 )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in NODE_TM part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *SCENE
//
			else if ( strcmp ( level_name[level], "*SCENE" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*SCENE_AMBIENT_STATIC" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_BACKGROUND_STATIC" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_FILENAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_FIRSTFRAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_FRAMESPEED" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_LASTFRAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SCENE_TICKSPERFRAME" ) == 0 )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in SCENE part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MATERIAL_LIST
//
			else if ( strcmp ( level_name[level], "*MATERIAL_LIST" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MATERIAL_COUNT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MATERIAL_LIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MATERIAL
//
			else if ( strcmp ( level_name[level], "*MATERIAL" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MATERIAL_NAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_CLASS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_AMBIENT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_DIFFUSE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SPECULAR" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINESTRENGTH" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_TRANSPARENCY" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_WIRESIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*NUMSUBMTLS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*SUBMATERIAL" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MAP_DIFFUSE" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MAP_AMBIENT" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MAP_OPACITY" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MATERIAL part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *SUBMATERIAL
//
			else if ( strcmp ( level_name[level], "*SUBMATERIAL" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MATERIAL_NAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_CLASS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_AMBIENT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_DIFFUSE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SPECULAR" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINESTRENGTH" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_TRANSPARENCY" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_WIRESIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHADING" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_FALLOFF" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SELFILLUM" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_FALLOFF" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_TYPE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_DIFFUSE" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MAP_AMBIENT" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "*MAP_OPACITY" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in SUBMATERIAL part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MAP_AMBIENT && MAP_OPACITY && MAP_DIFFUSE
//
			else if ( strcmp ( level_name[level], "*MAP_AMBIENT" ) == 0 || 
				strcmp ( level_name[level], "*MAP_OPACITY" ) == 0 || 
				strcmp ( level_name[level], "*MAP_DIFFUSE" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MAP_NAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_SUBNO" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_AMOUNT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*BITMAP" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_U_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_V_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_U_TILING" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_V_TILING" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_ANGLE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_SIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_LEVEL" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_TYPE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_GENERIC" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in ") + level_name[level] + " part, line: " + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
//
//  *MAP_GENERIC
//
			else if ( strcmp ( level_name[level], "*MAP_GENERIC" ) == 0 )
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					continue;
				}
				else if ( strcmp ( word, "*MAP_NAME" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_SUBNO" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MAP_AMOUNT" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*BITMAP" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_U_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_V_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_U_TILING" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_V_TILING" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_ANGLE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_SIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_LEVEL" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_TYPE" ) == 0 )
				{
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in ") + level_name[level] + " part, line: " + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					break;
				}
			}
		}
	}

	return 0;
}

void CASELoader::LoadImpl(void)
{
	InitLoader();
	LoadAseInternal();
	CloseLoader();
}

void CASELoader::InitLoader(void)
{
	if(!mpStream)
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is bad ptr..", MEM_ERROR);

	if(!mpStream->IsOpened())
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is not opened..", MEM_ERROR);
}

void CASELoader::CloseLoader(void)
{

}

}