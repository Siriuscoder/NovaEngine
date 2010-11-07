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
	nInt32 mat_count = 0;
	nInt32 submat_count = 0;

	strcpy ( word, " " );
	strcpy ( wordm1, " " );
//
//  Read a line of text from the file.
//
	input_buf = mpStream->ReadLine((size_t)LINE_MAX_LEN);
	count = sscanf ( input_buf.c_str(), "%s%n", word, &width );
//
//  *3DSMAX_ASCIIEXPORT  200
//
	if ( strcmp ( word, "*3DSMAX_ASCIIEXPORT" ) != 0 )
	{
		throw NOVA_EXP("CASELoader::LoadAseInternal: fatal error, bad ase file format...", BAD_OPERATION);
	}

	for ( ;; )
	{
		if(mpStream->Eof())
			break;

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
			if ( iword == 1 )
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
//  *COMMENT
//
			if ( strcmp ( word1, "*COMMENT" ) == 0 )
			{
				LOG_MESSAGE(nstring("void CASELoader::LoadAseInternal(void) ase comment: ") + next);
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
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width );
		    		next = next + width;
// Добавляем новый каркас в кеш объектов, сохраняем на него ссылку
					nstring nObjName(word2);
					TMeshContainer mesh;
					memset(&mesh, 0, sizeof(TMeshContainer));

					mesh.nName = nObjName;
					mMeshesMap.insert(std::pair<nstring, TMeshContainer>(nObjName, mesh));
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
					count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

					if(LastGeomObject)
						LastGeomObject->MatID = i;
					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in GEOMOBJECT part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
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
					continue;
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
					continue;
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
		    		continue;
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
					TFaceIndex index;
					TMatGroupInfo mat_alias;

					count = sscanf ( next, "%d%n", &mat_alias.nFace, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.a, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.b, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%s%n", word2, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.c, &width );
		    		next = next + width;


					/////////////////// Sub mat read //////////////////////
					while(true)
					{
		    			count = sscanf ( next, "%s%n", word2, &width );
		    			next = next + width;
		
						if(	strcmp ( word2, "*MESH_MTLID" ) == 0 )
						{
							count = sscanf ( next, "%d%n", &i, &width );
		    				next = next + width;
							mat_alias.nMatSubID = i;
							break;
						}
					}

					if(LastGeomObject)
					{
						LastGeomObject->nIndexList.push_back(index);
						LastGeomObject->nMatGroupsList.push_back(mat_alias);
					}
					

		    		break;
		    	}
		    	else
		    	{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_FACE_LIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
		    		continue;
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
					continue;
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
					TFaceIndex index;

					count = sscanf ( next, "%d%n", &i, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.a, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.b, &width );
		    		next = next + width;

		    		count = sscanf ( next, "%d%n", &index.c, &width );
		    		next = next + width;

					if(LastGeomObject)
					{
						LastGeomObject->nTVIndexList.push_back(index);
					}

					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_TFACELIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
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
					TUVMapping uv;

					count = sscanf ( next, "%d%n", &i, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &uv.s, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &uv.t, &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &uv.w, &width );
					next = next + width;

					if(LastGeomObject)
					{
						LastGeomObject->nTVMappingList.push_back(uv);
					}

					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_TVERTLIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
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

					if(LastGeomObject)
					{
						LastGeomObject->nVertexList.push_back(vertex);
					}

					break;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MESH_VERTEX_LIST part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
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
					Vector4f row;
					count = sscanf ( next, "%f%n", &(row[0]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[1]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[2]), &width );
					next = next + width;

					row[3] = 0;
					if(LastGeomObject)
					{
						LastGeomObject->nTMatrix.SetRow(0, row);
					}

					break;
				}
				else if ( strcmp ( word, "*TM_ROW1" ) == 0 )
				{
					Vector4f row;
					count = sscanf ( next, "%f%n", &(row[0]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[1]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[2]), &width );
					next = next + width;

					row[3] = 0;
					if(LastGeomObject)
					{
						LastGeomObject->nTMatrix.SetRow(1, row);
					}

					break;
				}
				else if ( strcmp ( word, "*TM_ROW2" ) == 0 )
				{
					Vector4f row;
					count = sscanf ( next, "%f%n", &(row[0]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[1]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[2]), &width );
					next = next + width;

					row[3] = 0;
					if(LastGeomObject)
					{
						LastGeomObject->nTMatrix.SetRow(2, row);
					}

					break;
				}
				else if ( strcmp ( word, "*TM_ROW3" ) == 0 )
				{
					Vector4f row;
					count = sscanf ( next, "%f%n", &(row[0]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[1]), &width );
					next = next + width;

					count = sscanf ( next, "%f%n", &(row[2]), &width );
					next = next + width;

					row[3] = 1;
					if(LastGeomObject)
					{
						LastGeomObject->nTMatrix.SetRow(3, row);
					}

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
					continue;
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
					continue;
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
					continue;
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
					LastMaterial = NULL;
					LastSubMaterial = NULL;
					submat_count = 0;
					continue;
				}
				else if ( strcmp ( word, "*MATERIAL_NAME" ) == 0 )
				{
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width);
		    		next = next + width;
// Добавляем новый материал в кеш объектов, сохраняем на него ссылку
					nstring nMatName(word2);
					TMaterialContainer mat;
					memset(&mat, 0, sizeof(TMaterialContainer));

					mat.nName = nMatName;
					mat.nID = mat_count;
					mat_count++;

					mMaterialsMap.insert(std::pair<nstring, TMaterialContainer>(nMatName, mat));
					LastMaterial = &(mMaterialsMap[nMatName]);

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_CLASS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_AMBIENT" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nAmbientColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nAmbientColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nAmbientColor.B()), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_DIFFUSE" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nDiffuseColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nDiffuseColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nDiffuseColor.B()), &width );
						next = next + width;
					}
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SPECULAR" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nSpecularColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nSpecularColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastMaterial->nSpecularColor.B()), &width );
						next = next + width;
					}
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINE" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nShininess), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINESTRENGTH" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nShinStrength), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_TRANSPARENCY" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nTransparency), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_WIRESIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHADING" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nShading), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SELFILLUM" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nSelfIllum), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_FALLOFF" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastMaterial->nFalloff), &width );
						next = next + width;
					}

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
				else if ( strcmp ( word, "*MAP_BUMP" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in MATERIAL part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
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
					LastSubMaterial = NULL;
					continue;
				}
				else if ( strcmp ( word, "*MATERIAL_NAME" ) == 0 )
				{
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width );
		    		next = next + width;
// Добавляем новый материал в кеш объектов, сохраняем на него ссылку
					nstring nMatName(word2);
					TMaterialContainer mat;
					memset(&mat, 0, sizeof(TMaterialContainer));

					mat.nName = nMatName;
					mat.nID = submat_count;
					submat_count++;
					mMaterialsMap.insert(std::pair<nstring, TMaterialContainer>(nMatName, mat));

					if(LastMaterial)
						LastMaterial->nSubMats.push_back(nMatName);

					LastSubMaterial = &(mMaterialsMap[nMatName]);
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_CLASS" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_AMBIENT" ) == 0 )
				{
					if(LastSubMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nAmbientColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nAmbientColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nAmbientColor.B()), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_DIFFUSE" ) == 0 )
				{
					if(LastSubMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nDiffuseColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nDiffuseColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nDiffuseColor.B()), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SPECULAR" ) == 0 )
				{
					if(LastSubMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nSpecularColor.R()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nSpecularColor.G()), &width );
						next = next + width;

						count = sscanf ( next, "%f%n", &(LastSubMaterial->nSpecularColor.B()), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINE" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nShininess), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHINESTRENGTH" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nShinStrength), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_TRANSPARENCY" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nTransparency), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_WIRESIZE" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SHADING" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nShading), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_SELFILLUM" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nSelfIllum), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*MATERIAL_XP_FALLOFF" ) == 0 )
				{
					if(LastMaterial)
					{
						count = sscanf ( next, "%f%n", &(LastSubMaterial->nFalloff), &width );
						next = next + width;
					}

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
				else if ( strcmp ( word, "*MAP_BUMP" ) == 0 )
				{
					continue;
				}
				else
				{
					CLog::GetInstance().PrintMessage(nstring("void CASELoader::LoadAseInternal(void) warning: find unknown section in SUBMATERIAL part, line: ") + 
						CStringUtils::IntToString(text_num), CLog::LG_WITH_TIME_WARNING);
					continue;
				}
			}
//
//  *MAP_AMBIENT && MAP_OPACITY && MAP_DIFFUSE && MAP_BUMP
//
			else if ( strcmp ( level_name[level], "*MAP_AMBIENT" ) == 0 || 
				strcmp ( level_name[level], "*MAP_OPACITY" ) == 0 || 
				strcmp ( level_name[level], "*MAP_DIFFUSE" ) == 0 || 
				strcmp ( level_name[level], "*MAP_BUMP" ) == 0)
			{
				if ( strcmp ( word, "{" ) == 0 )
				{
					continue;
				}
				else if ( strcmp ( word, "}" ) == 0 )
				{
					level = nlbrack - nrbrack;
					LastTexture = NULL;
					continue;
				}
				else if ( strcmp ( word, "*MAP_NAME" ) == 0 )
				{
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width );
		    		next = next + width;
// Добавляем новую текстуру в кеш объектов, сохраняем на него ссылку
					nstring nMapName(word2);
					TTextureContainer tmap;
					memset(&tmap, 0, sizeof(TTextureContainer));

					tmap.nName = nMapName;

					stl<nstring, TTextureContainer>::map::iterator it = mTexturesMap.find(nMapName);
					if(it != mTexturesMap.end())
						LastTexture = &(it->second);
					else
					{
						mTexturesMap.insert(std::pair<nstring, TTextureContainer>(nMapName, tmap));
						LastTexture = &(mTexturesMap[nMapName]);
					}

					if(LastSubMaterial)
					{
						if(strcmp ( level_name[level], "*MAP_AMBIENT" ) == 0)
							LastSubMaterial->nAmbientMap = nMapName;
						else if(strcmp ( level_name[level], "*MAP_OPACITY" ) == 0)
							LastSubMaterial->nOpacMap = nMapName;
						else if(strcmp ( level_name[level], "*MAP_DIFFUSE" ) == 0)
							LastSubMaterial->nDiffuseMap1 = nMapName;
						else if(strcmp ( level_name[level], "*MAP_BUMP" ) == 0)
							LastSubMaterial->nBumpMap = nMapName;
					}
					else
					{
						if(LastMaterial)
						{
							if(strcmp ( level_name[level], "*MAP_AMBIENT" ) == 0)
								LastMaterial->nAmbientMap = nMapName;
							else if(strcmp ( level_name[level], "*MAP_OPACITY" ) == 0)
								LastMaterial->nOpacMap = nMapName;
							else if(strcmp ( level_name[level], "*MAP_DIFFUSE" ) == 0)
								LastMaterial->nDiffuseMap1 = nMapName;
							else if(strcmp ( level_name[level], "*MAP_BUMP" ) == 0)
								LastMaterial->nBumpMap = nMapName;
						}
					}

					
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
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width );
		    		next = next + width;

					if(LastTexture)
					{
						LastTexture->nBitMap = nstring(word2);
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_U_OFFSET" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nUOffset), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_V_OFFSET" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nVOffset), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_U_TILING" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nUScale), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_V_TILING" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nVScale), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_ANGLE" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nRotation), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nBlur), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_SIZE" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nNoiseSize), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_LEVEL" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nNoiseLevel), &width );
						next = next + width;
					}

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
					continue;
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
					count = sscanf ( next, "%*[^\"]%*c%[^\"]%n", word2, &width );
		    		next = next + width;

					if(LastTexture)
					{
						LastTexture->nBitMap = nstring(word2);
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_U_OFFSET" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nUOffset), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_V_OFFSET" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nVOffset), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_U_TILING" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nUScale), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_V_TILING" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nVScale), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_ANGLE" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nRotation), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nBlur), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_BLUR_OFFSET" ) == 0 )
				{
					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_SIZE" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nNoiseSize), &width );
						next = next + width;
					}

					break;
				}
				else if ( strcmp ( word, "*UVW_NOISE_LEVEL" ) == 0 )
				{
					if(LastTexture)
					{
						count = sscanf ( next, "%f%n", &(LastTexture->nNoiseLevel), &width );
						next = next + width;
					}

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
					continue;
				}
			}
		}
	}

	return 0;
}

void CASELoader::LoadImpl(void)
{
	LoadAseInternal();
}

void CASELoader::InitLoader(void)
{
	if(!mpStream)
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is bad ptr..", MEM_ERROR);

	if(!mpStream->IsOpened())
		throw NOVA_EXP("void CASELoader::LoadAseInternal(void): mpStream is not opened..", MEM_ERROR);
}

CSceneContentLoaderBase::TMaterialContainer CASELoader::FindMatByID(nInt32 id)
{
	stl<nstring, TMaterialContainer>::map::iterator it;
	for(it = mMaterialsMap.begin(); it != mMaterialsMap.end(); it++)
	{
		if(it->second.nID == id)
			return it->second;
	}

	return TMaterialContainer();
}

void CASELoader::CloseLoader(void)
{
	stl<nstring, TMeshContainer>::map::iterator it;
	for(it = mMeshesMap.begin(); it != mMeshesMap.end(); it++)
	{
		TMeshContainer *mesh_def = &((*it).second);
		TMaterialContainer matref = FindMatByID(mesh_def->MatID);
// Preparing sub mat indexes
		for(nUInt32 i = 0; i < mesh_def->nMatGroupsList.size(); i++)
		{
			if(mesh_def->nMatGroupsList[i].nMatSubID >= matref.nSubMats.size())
			{
				mesh_def->nMatGroupsList[i].nMatSubID = 0;
				mesh_def->nMatGroupsList[i].nMatName = matref.nName;
			}
			else
			{
				mesh_def->nMatGroupsList[i].nMatName = matref.nSubMats[mesh_def->nMatGroupsList[i].nMatSubID];
			}
		}
// Calculating real uv from texture faces
		mesh_def->nMappingFacesList.resize(mesh_def->nVertexList.size());
		for(nUInt32 i = 0; i < mesh_def->nTVIndexList.size(); i++)
		{
			TFaceIndex face = mesh_def->nIndexList[i];
			TFaceIndex tex_face =  mesh_def->nTVIndexList[i];

			mesh_def->nMappingFacesList[face.a] = mesh_def->nTVMappingList[tex_face.a];
			mesh_def->nMappingFacesList[face.b] = mesh_def->nTVMappingList[tex_face.b];
			mesh_def->nMappingFacesList[face.c] = mesh_def->nTVMappingList[tex_face.c];
		}

		mesh_def->nTVMappingList.clear();
		mesh_def->nTVIndexList.clear();
	}
}

}