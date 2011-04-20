/***************************************************************************
 *   Copyright (C) 2011 by Sirius										   *
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

#include "nova_basic_scene_manager.h"
#include "nova_string_utils.h"

namespace nova
{


CBasicSceneNode::CBasicSceneNode(CMovableObject *pObj, CSceneManager *scene) :
	CSceneNode(pObj, scene)
{

}

void CBasicSceneNode::SetMeshBox(CMeshBoxPtr &mesh)
{
	mMeshBox = mesh;
}

nInt32 CBasicSceneManager::RenderSceneImpl(void)
{
	return RenderNode(GetRootElement());
}

void CBasicSceneManager::PrepareNode(CTreeNode<CSceneNode*> *node)
{
	if(node)
	{
		CBasicSceneNode *curNode = dynamic_cast<CBasicSceneNode *>(node->GetData());
		// Preparing mesh
		// Generating normals to faces and sub mats info
		curNode->GetMeshBox()->GenerateNormalsToFaces();
		if(curNode->GetMeshBox()->GetMeshDefinition().nNormalList.size() == 0)
		//Generating normals to vertexes
			curNode->GetMeshBox()->CalculateNormals();

		// Sorting faces by material id
		// using fast qsort algorithm
		curNode->GetMeshBox()->SortFaceIndexByMaterials();
		// Generating mat changes groups
		curNode->GetMeshBox()->GenerateMatChangesGroups();

		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			// next layer
			PrepareNode(node->GetNode(i));
		}
	}
}


void CBasicSceneManager::PrepareSceneImpl(void)
{
	PrepareNode(GetRootElement());
}

nInt32 CBasicSceneManager::PrepareRenderQueueImpl(void)
{

	return 0;
}

void CBasicSceneManager::PrepareSceneFrameImpl(void)
{

}

void CBasicSceneManager::BuildSceneImpl(void)
{

}

void CBasicSceneManager::ReleaseObjectsImpl(void)
{
	ReleaseNodeRenderableObject(GetRootElement());
}

void CBasicSceneManager::ReleaseNodeRenderableObject(CTreeNode<CSceneNode*> *node)
{
	if(node)
	{
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			ReleaseNodeRenderableObject(node->GetNode(i));
		}

		delete node->GetData()->GetObjectInterface();
	}
}

nInt32 CBasicSceneManager::RenderNode(CTreeNode<CSceneNode*> *node)
{
	nova::nUInt32 nodesRenderedCount = 0;
	if(node)
	{
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			node->GetData()->ValidateNode();
			node->GetData()->RenderNode();
			nodesRenderedCount++;

			RenderNode(node->GetNode(i));
			node->GetData()->InValidateNode();
		}
	}

	return nodesRenderedCount;
}

CSceneNode *CBasicSceneManager::AddRenderableResourceToScene(const nstring &resource_name)
{
	CMeshBoxPtr mesh = CMeshManager::GetSingelton().GetResourceFromHash(resource_name);
	if(!mesh.IsNull())
	{
		CMovableObject *obj = new CMovableObject(mesh->GetResName(), GetSceneSlavesGroup());
		CBasicSceneNode *target = new CBasicSceneNode(obj, this);
		target->SetMeshBox(mesh);

		return target;
	}

	return NULL;
}

void CBasicSceneManager::SerializeNodeToXml(CTreeNode<CSceneNode*> *node, xmlTextWriterPtr xmlWriter)
{
	if(!node)
		return;

	CBasicSceneNode *curNode = dynamic_cast<CBasicSceneNode *>(node->GetData());
	if(curNode)
	{
		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "MeshObject") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
		if(xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "ResourceName", BAD_CAST curNode->GetMeshBox()->GetMeshDefinition().nName.c_str()) < 0)
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
				if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST colName.c_str(), "%f", curNode->GetMeshBox()->GetMeshDefinition().nTMatrix[i][j]) < 0)
					NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);
			}
			xmlTextWriterEndElement(xmlWriter);
		}
	}

	for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
	{
		SerializeNodeToXml(node->GetNode(i), xmlWriter);
	}
	
	if(curNode)
		xmlTextWriterEndElement(xmlWriter);
}

void CBasicSceneManager::SerializeSceneToXmlImpl(xmlTextWriterPtr xmlWriter)
{
	SerializeNodeToXml(GetRootElement(), xmlWriter);
}

void CBasicSceneManager::DeSerializeNodeFromXml(xmlNodePtr node, CTreeNode<CSceneNode*> *sceneNode)
{
	while(node != NULL)
	{
		if(xmlIsBlankNode(node))
		{
			node = node->next;
			continue;
		}

		if(!xmlStrcmp(node->name, (xmlChar *)"MeshObject"))
		{
			xmlChar * resourceName = xmlGetProp(node, (xmlChar *) "ResourceName");
			CBasicSceneNode *newNode = dynamic_cast<CBasicSceneNode *>(AddRenderableResourceToScene((char *)resourceName));	
			CTreeNode<CSceneNode*> *newSceneNode = new CTreeNode<CSceneNode*>(newNode);

			sceneNode->AddCurrentNone(newSceneNode);
			for(xmlNodePtr subNode = node->children; subNode; subNode = subNode->next)
			{
				if(xmlIsBlankNode(subNode))
				{
					subNode = subNode->next;
					continue;
				}

				if(!xmlStrcmp(subNode->name, (xmlChar *)"Orientation"))
				{
					for(xmlNodePtr orientNode = subNode->children; orientNode; orientNode = orientNode->next)
					{
						if(xmlIsBlankNode(subNode))
						{
							subNode = subNode->next;
							continue;
						}
						 
						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row0"))
						{
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[0][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[0][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[0][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[0][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row1"))
						{
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[1][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[1][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[1][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[1][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row2"))
						{
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[2][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[2][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[2][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[2][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row3"))
						{
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[3][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[3][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[3][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newNode->GetMeshBox()->GetMeshDefinition().nTMatrix[3][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}
					}
				}

				if(!xmlStrcmp(subNode->name, (xmlChar *)"MeshObject"))
				{
					DeSerializeNodeFromXml(subNode, newSceneNode);
				}
			}
		}

		node = node->next;
	}
}

void CBasicSceneManager::DeSerializeSceneFromXmlImpl(xmlNodePtr node)
{

}

}