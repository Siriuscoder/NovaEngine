/***************************************************************************
 *   Copyright (C) 2010 by Sirius										   *
 *	 Vdov Nikita Sergeevich	(c)											   *
 *	 siriusnick@gmail.com												   *
 *																		   *
 *	 This source file is part of NovaEngine	tool-box					   *
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
#include "nova_example_application.h"
#include "nova_string_utils.h"

using namespace nova;

class SceneBuilderTool : public CExampleApplication
{
protected:

	nova::CCPUTimer mTimer;

	nova::nstring mDest;
	nova::nstring mSource;
	nova::CASELoader mLoader;
	nova::stl<nova::nstring>::vector mFiles;

public:

	SceneBuilderTool()
	{

	}

	~SceneBuilderTool()
	{

	}

	void WriteMeshObject(xmlTextWriterPtr xmlWriter)
	{
		nova::stl<nstring>::vector meshes = mLoader.GetMeshList();
		for(nova::nUInt32 i = 0; i < meshes.size(); i++)
		{
			nova::CMesh::TMeshContainer *meshInfo = mLoader.GetMesh(meshes[i]);

			if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "MeshObject") < 0) // NovaScene doc
				NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
			if(xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "ResourceName", BAD_CAST meshInfo->nName.c_str()) < 0)
				NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);

			if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "Orientation") < 0) // NovaScene doc
				NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

			for(nova::nUInt32 i = 0; i < 4; i++)
			{
				nstring rowName = "Row" + CStringUtils::IntToString(i);
				if(xmlTextWriterStartElement(xmlWriter, BAD_CAST rowName.c_str()) < 0) // NovaScene doc
					NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

				for(nova::nUInt32 j = 0; j < 4; j++)
				{
					nstring colName = "C" + CStringUtils::IntToString(j);
					if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST colName.c_str(), "%f", meshInfo->nTMatrix[i][j]) < 0)
						NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);
				}
				xmlTextWriterEndElement(xmlWriter);
			}

			xmlTextWriterEndElement(xmlWriter);

			// Sorting faces by material id
			// using fast qsort algorithm
			nova::CMeshBoxPtr meshPtr = nova::CMeshManager::GetSingelton().CreateMesh(meshInfo, "default");
			if(meshPtr.IsNull())
				continue;

			meshPtr->SortFaceIndexByMaterials();
			meshPtr->GenerateMatChangesGroups();

			if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "Sub-materials") < 0) // NovaScene doc
				NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
			for(nova::nUInt32 i = 0; i < meshPtr->GetMeshDefinition().nMatChangesGroups.size(); i++)
			{
				if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "Material") < 0) // NovaScene doc
					NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
				if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST "MatName", "%s", 
					meshPtr->GetMeshDefinition().nMeshInfoList[meshPtr->GetMeshDefinition().nMatChangesGroups[i]].nMatName.c_str()) < 0)
					NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);
				if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST "SubMatID", "%d", 
					meshPtr->GetMeshDefinition().nMeshInfoList[meshPtr->GetMeshDefinition().nMatChangesGroups[i]].nMatSubID) < 0)
					NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);

				xmlTextWriterEndElement(xmlWriter);
			}

			xmlTextWriterEndElement(xmlWriter);
			nova::CResourceManager::UnloadResourceFromHash(meshInfo->nName);

			xmlTextWriterEndElement(xmlWriter);
		}
	}

	void SerializeSceneToXmlFile(const nova::nstring &destFolder)
	{
	    xmlTextWriterPtr xmlWriter = NULL;
		nova::nstring xmlSceneFileName =  destFolder + "scene.xml";

	    /* Create a new XmlWriter for uri, with no compression. */
		if((xmlWriter = xmlNewTextWriterFilename(xmlSceneFileName.c_str(), 0)) == NULL)
			NOVA_EXP("CResource::SerializeToXmlFile: Error creating the xml writer", BAD_OPERATION);

		// automatic indentation for readability
		xmlTextWriterSetIndent(xmlWriter, 1);
		if(xmlTextWriterStartDocument(xmlWriter, NULL, "UTF-8", NULL) < 0) // start doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartDocument fail", BAD_OPERATION);

		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "NovaScene") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

		xmlTextWriterWriteComment(xmlWriter, BAD_CAST "Scene resource lister");
		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "SceneResources") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);

		for(nova::nUInt32 i = 0; i < mFiles.size(); i++)
		{	
			if(mFiles[i].find(".xml") != nstring::npos)
			{
				if(xmlTextWriterWriteElement(xmlWriter, BAD_CAST "ResourceFile", BAD_CAST mFiles[i].c_str()) < 0)
					NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterWriteElement fail", BAD_OPERATION);
			}
		}

		xmlTextWriterEndElement(xmlWriter);

		xmlTextWriterWriteComment(xmlWriter, BAD_CAST "Scene objects and scene managers");
		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "SceneContent") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
		if(xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "SceneManager", BAD_CAST "BasicSceneManager") < 0)
			NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);

		WriteMeshObject(xmlWriter);

		xmlTextWriterEndDocument(xmlWriter);
		xmlFreeTextWriter(xmlWriter);
	}

	void Launch(const nova::nstring &aseFileName, const nova::nstring &destFolder, bool pack, bool copyLocal)
	{
		nova::CFileStream aseFile;
		aseFile.Open(aseFileName, false, false);

		std::cout << std::endl << "Parsing ASE file " << aseFileName << "...";std::cout.flush();
		mTimer.Reset();
		mLoader.LoadImmediately(&aseFile);
		double msec = mTimer.GetKernelMilliseconds();

		aseFile.Close();

		std::cout << "Done" << std::endl; std::cout.flush();
		std::cout << "File parsed with " << msec << " ms" << std::endl << std::endl;std::cout.flush();

		CreateAndSaveResources(destFolder, copyLocal);
		SerializeSceneToXmlFile(destFolder);
	}

	void CreateAndSaveResources(const nstring &destFolder, bool copyLocal)
	{
		nova::stl<nstring>::vector meshes = mLoader.GetMeshList();
		std::cout << "============= Serializing Objects: " << std::endl; std::cout.flush();
		for(nova::nUInt32 i = 0; i < meshes.size(); i++)
		{
			std::cout << "Trying to serialize mesh object " << meshes[i] << std::endl;std::cout.flush();
			nova::CMesh::TMeshContainer *meshInfo = mLoader.GetMesh(meshes[i]);

			std::cout << "-- Vertex count: " << nova::CStringUtils::IntToString(meshInfo->nVertexList.size()) << std::endl;std::cout.flush();
			std::cout << "-- Face count: " << nova::CStringUtils::IntToString(meshInfo->nIndexList.size()) << std::endl;std::cout.flush();
			std::cout << "-- Mat ID: " << nova::CStringUtils::IntToString(meshInfo->MatID) << std::endl << std::endl;std::cout.flush();
			SerializeMeshObject(meshes[i], *meshInfo, destFolder);

			std::cout << "Mesh object " << meshes[i] << " serialized successfully" << std::endl;std::cout.flush();
		}
	
		nova::stl<nstring>::vector materials = mLoader.GetMaterialList();
		std::cout << std::endl << "============= Serializing Materials: " << std::endl; std::cout.flush();
		for(nova::nUInt32 i = 0; i < materials.size(); i++)
		{
			std::cout << "Trying to serialize material " << materials[i] << std::endl;std::cout.flush();
			SerializeMaterial(materials[i], *mLoader.GetMaterial(materials[i]), destFolder);

			std::cout << "Material object " << materials[i] << " serialized successfully" << std::endl;std::cout.flush();
		}

		nova::stl<nstring>::vector maps = mLoader.GetTextureList();
		std::cout << std::endl << "============= Serializing Textures: " << std::endl; std::cout.flush();
		for(nova::nUInt32 i = 0; i < maps.size(); i++)
		{
			std::cout << "Trying to serialize texture " << maps[i] << std::endl;std::cout.flush();
			CTexture::TTextureContainer *textureInfo = mLoader.GetTexture(maps[i]);

			CopyAndSerializeImage("i" + maps[i], textureInfo->nBitMap, destFolder, copyLocal); 
			SerializeTexture(maps[i], *textureInfo, destFolder);

			std::cout << "Texture object " << maps[i] << " serialized successfully" << std::endl;std::cout.flush();
		}
	}

	void SerializeMeshObject(const nstring &meshName, nova::CMesh::TMeshContainer &meshInfo, const nstring &destFolder)
	{
		meshInfo.nMeshfile = destFolder + meshName + ".msh";
		nova::CMeshBoxPtr meshPtr = nova::CMeshManager::GetSingelton().CreateMesh(&meshInfo, "default");

		if(!meshPtr.IsNull())
		{
			nstring xmlMeshFileName = destFolder + meshName + ".xml";

			std::cout << "Optimizing mesh.."; std::cout.flush();
			// Preparing mesh
			// Generating normals to faces and sub mats info
			meshPtr->GenerateNormalsToFaces();
			if(meshPtr->GetMeshDefinition().nNormalList.size() == 0)
			//Generating normals to vertexes
				meshPtr->CalculateNormals();

			// Sorting faces by material id
			// using fast qsort algorithm
			meshPtr->SortFaceIndexByMaterials();

			std::cout << "Saving mesh object to file " << meshInfo.nMeshfile << "..."; std::cout.flush();
			CGlobalMshLoader::SaveMeshToFile(meshPtr->GetMeshDefinition(), meshPtr->GetMeshDefinition().nMeshfile);
			mFiles.push_back(meshInfo.nMeshfile);
			std::cout << "Done" << std::endl; std::cout.flush();

			meshPtr->SerializeToXmlFile(xmlMeshFileName);
			mFiles.push_back(xmlMeshFileName);
			nova::CResourceManager::UnloadResourceFromHash(meshPtr->GetMeshDefinition().nName);
		}
	}



	void SerializeMaterial(const nstring &materialName, CMaterial::TMaterialContainer materialInfo, const nstring &destFolder)
	{
		nova::CMaterialPtr materialPtr = nova::CMaterialManager::GetSingelton().CreateMaterial(materialName, "default", materialInfo);
			
		if(!materialPtr.IsNull())
		{
			nstring xmlMaterialName = destFolder + materialName + ".xml";
			materialPtr->SerializeToXmlFile(xmlMaterialName);
			mFiles.push_back(xmlMaterialName);

			nova::CResourceManager::UnloadResourceFromHash(materialName);
		}
	}

	void CopyAndSerializeImage(const nstring &imageName, const nstring &imageFile, const nstring &destFolder, bool copyLocal)
	{
		nova::CImagePtr imagePtr = nova::CImageManager::GetSingelton().CreateNewImage(imageName, 
			"default", imageFile, nova::SF_UNKNOWN, "DevIL", nova::CImageFormats::NF_DEFAULT);

		if(!imagePtr.IsNull())
		{
			nova::nstring xmlfileName = destFolder + imageName + ".xml";
			imagePtr->LoadResource();
			imagePtr->BackHeigth();

			if(copyLocal)
			{
				// use devil by default
				// use png file format by default
				nova::nstring imageFileLocalName = destFolder + imageName + ".png";
				std::cout << "Encoding image " <<  imageFile << " to " << imageFileLocalName << "..."; std::cout.flush();

				CDevILCodec::GetSingelton().CodeToFile(imageFileLocalName, *(imagePtr.GetPtr()));

				std::cout << "Done" << std::endl; std::cout.flush();
				imagePtr->GetImageSource().mFilename = imageFileLocalName;
				imagePtr->GetImageSource().mCompressor = SF_PNG;
			}

			imagePtr->SerializeToXmlFile(xmlfileName);
			mFiles.push_back(xmlfileName);

			imagePtr->FreeResource();
			nova::CResourceManager::UnloadResourceFromHash(imageName);
		}
	}

	

	void SerializeTexture(const nstring &textureName, CTexture::TTextureContainer textureInfo, const nstring &destFolder)
	{
		textureInfo.nBitMap = "i" + textureName;
		nova::CTexturePtr texturePtr = nova::CTextureManager::GetSingelton().CreateNewTexture(textureName, "default", textureInfo);

		if(!texturePtr.IsNull())
		{
			nova::nstring xmlfileName = destFolder + textureName + ".xml";
			texturePtr->SerializeToXmlFile(xmlfileName);
			mFiles.push_back(xmlfileName);

			// unload resource
			nova::CResourceManager::UnloadResourceFromHash(textureName);
		}
	}

	void PrintHelp(void)
	{
		cout << "Usage: [-i, -pack, -copylocal] -s Source.ase -folder DestinationFolder/" << endl;
	}
};


ENTRY_POINT	
{
	SceneBuilderTool SceneBuilder;
	SceneBuilder.StartUpEngine(ST_INIT_YOURSELF);

	try
	{
/////// Parsing unput args //////////////
		CParser rParser;
		stl<nstring>::vector args;
		bool pack = false;
		bool copyLocal = false;
		nstring aseFile;
		nstring destFolder;

		cout << "Hello, this is Scene Builder tool for Nova engine.." << endl;
		cout << "Converting ASE file to html Nova engine scene files, and after packing using gz(zlib)" << endl;

		{
#if defined(__WIN32__)
			nstring input;
			input.append(cmdParam);

			rParser.ParseStringRecurse(input, args);
#else
			for(int i = 1; i < argc, i++)
				args.push_back(nstring(arg[i]));
#endif
		}

		for(nova::nUInt32 i = 0; i < args.size(); i++)
		{
			if(args[i] == "-i")
			{

			}
			else if(args[i] == "-pack")
				pack = true;
			else if(args[i] == "-copylocal")
				copyLocal = true;
			else if(args[i] == "-s")
			{
				aseFile = args[i+1];
				i++;
			}
			else if(args[i] == "-folder")
			{
				destFolder = args[i+1];
				i++;
			}
			else
			{
				SceneBuilder.PrintHelp();
				cin.get();

				SceneBuilder.ShutDown();
				return -1;
			}
		}

		//SceneBuilder.Launch(args[0], args[1], pack);
		SceneBuilder.Launch(aseFile, destFolder, pack, copyLocal);
	}
	catch(NovaExp & exp)
	{
		cerr << exp.Message() << endl;
		//exp.FixError();
	}
	catch(exception & exp)
	{
		cerr << exp.what() << endl;
	}

	cin.get();
	SceneBuilder.ShutDown();

	return 0;
}

