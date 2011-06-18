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

void CBasicSceneNode::PrepareNodeImpl(void)
{
	if(mMeshBox.IsNull())
		return;
	// Preparing mesh
	// Generating normals to faces and sub mats info
	mMeshBox->GenerateNormalsToFaces();
	if(mMeshBox->GetMeshDefinition().nNormalList.size() == 0)
	//Generating normals to vertexes
		mMeshBox->CalculateNormals();

	// Sorting faces by material id
	// using fast qsort algorithm
	mMeshBox->SortFaceIndexByMaterials();
	// Generating mat changes groups
	mMeshBox->GenerateMatChangesGroups();
	// Generating bouding box
	mBoundingBox = mMeshBox->GenerateBoundingBox();
}

void CBasicSceneNode::BuildNodeImpl(void)
{

}

void CBasicSceneNode::PreparingBatchList(void)
{

}

CBasicSceneNode::CBasicSceneNode(CMovableObject *pObj, CMeshBoxPtr &sourceMesh, CSceneManager *scene) :
	CSceneNode(pObj, scene), mMeshBox(sourceMesh)
{

}

nInt32 CBasicSceneManager::RenderSceneImpl(void)
{
	return RenderNode(GetRootElement());
}

void CBasicSceneManager::PrepareNode(CTreeNode<CSceneManager::TNodeType> *node)
{
	if(node)
	{
		if(!node->GetData().IsNull())
			node->GetData()->PrepareNode();

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
	BuildNode(GetRootElement());
}

void CBasicSceneManager::BuildNode(CTreeNode<CSceneManager::TNodeType> *node)
{
	if(node)
	{
		if(!node->GetData().IsNull())
			node->GetData()->BuildNode();
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			// next layer
			BuildNode(node->GetNode(i));
		}
	}
}

nInt32 CBasicSceneManager::RenderNode(CTreeNode<CSceneManager::TNodeType> *node)
{
	nova::nUInt32 nodesRenderedCount = 0;
	if(node)
	{
		node->GetData()->ValidateNode();
		if(node->GetData()->IsVisible())
		{
			node->GetData()->RenderNode();
			nodesRenderedCount++;
			mRenderedFaces += node->GetData()->GetRenderedFacesCount();
			mRenderedBatches += node->GetData()->GetRenderedBatchesCount();
		}

		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			nodesRenderedCount += RenderNode(node->GetNode(i));
		}

		node->GetData()->InValidateNode();
	}

	return nodesRenderedCount;
}

CSceneManager::TNodeType CBasicSceneManager::CreateRenderableNodeFromResource(const nstring &resource_name)
{
	CMeshBoxPtr mesh = CMeshManager::GetSingelton().GetResourceFromHash(resource_name);
	if(!mesh.IsNull())
	{
		CSceneManager::TNodeType target(new CBasicSceneNode(new CMovableObject(mesh->GetResName(), GetSceneSlavesGroup()), mesh, this));

		if(target.IsNull())
			throw NOVA_EXP("CBasicSceneManager::AddRenderableResourceToScene: target is bad pointer", MEM_ERROR);

		return target;
	}

	return NULL;
}

void CBasicSceneManager::SerializeNodeToXml(CTreeNode<CSceneManager::TNodeType> *node, xmlTextWriterPtr xmlWriter)
{
	if(!node)
		return;

	//CBasicSceneNode *curNode = dynamic_cast<CBasicSceneNode *>(node->GetData().GetPtr());
	if(!node->GetData().IsNull())
	{
		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "MeshObject") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
		if(xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "ResourceName", BAD_CAST node->GetData()->GetObjectInterface()->GetObjectName().c_str()) < 0)
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
				if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST colName.c_str(), "%f", node->GetData()->GetObjectInterface()->GetMatrix()[i][j]) < 0)
					NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);
			}
			xmlTextWriterEndElement(xmlWriter);
		}
	}

	for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
	{
		SerializeNodeToXml(node->GetNode(i), xmlWriter);
	}
	
	if(!node->GetData().IsNull())
		xmlTextWriterEndElement(xmlWriter);
}

void CBasicSceneManager::SerializeSceneToXmlImpl(xmlTextWriterPtr xmlWriter)
{
	SerializeNodeToXml(GetRootElement(), xmlWriter);
}

void CBasicSceneManager::DeSerializeNodeFromXml(xmlNodePtr node, CTreeNode<CSceneManager::TNodeType> *sceneNode)
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
			CTreeNode<CSceneManager::TNodeType> *newSceneNode = NULL;

			if(!sceneNode)
			{
				GetSceneTree().CreateRoot(CreateRenderableNodeFromResource((char *)resourceName));
				newSceneNode = GetSceneTree().GetRootElement();
			}
			else
			{
				newSceneNode = sceneNode->GetNode(sceneNode->AddNone(CreateRenderableNodeFromResource((char *)resourceName)));
			}

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
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[0][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[0][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[0][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[0][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row1"))
						{
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[1][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[1][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[1][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[1][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row2"))
						{
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[2][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[2][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[2][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[2][3] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C3"));
						}

						if(!xmlStrcmp(orientNode->name, (xmlChar *)"Row3"))
						{
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[3][0] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C0"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[3][1] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C1"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[3][2] = 
								CStringUtils::StringToFloat((char *)xmlGetProp(orientNode, (xmlChar *) "C2"));
							newSceneNode->GetData()->GetObjectInterface()->GetMatrix()[3][3] = 
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
	DeSerializeNodeFromXml(node, GetRootElement());
}

}