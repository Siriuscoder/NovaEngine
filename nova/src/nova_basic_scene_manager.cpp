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
/*
	// Preparing mesh
	// Generating normals to faces and sub mats info
	mMeshBox->GenerateNormalsToFaces();
	if(mMeshBox->GetMeshDefinition().nNormalList.size() == 0)
	//Generating normals to vertexes
		mMeshBox->CalculateNormals();

	// Sorting faces by material id
	// using fast qsort algorithm
	mMeshBox->SortFaceIndexByMaterials();
*/
	// Generating mat changes groups
	mMeshBox->GenerateMatChangesGroups();
	// Generating bouding box
	mBoundingBox = mMeshBox->GenerateBoundingBox();
}

void CBasicSceneNode::BuildNodeImpl(void)
{
	PreparingBatchList();
}

void CBasicSceneNode::ReleaseNodeImpl(void)
{
	mBatchList.clear();
	mVertexChainBuffer.Free();
	mVboIndexBuffer.Free();
}

void CBasicSceneNode::ValidateNodeImpl(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(mpSceneObject->GetMatrix());
}

void CBasicSceneNode::InValidateNodeImpl(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void CBasicSceneNode::RenderNodeImpl(void)
{
	TBatchList::const_iterator it;
	float colorG = 0.4f;
	it = mBatchList.begin();

	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);

	mVertexChainBuffer->BindBuffer();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

	mVboIndexBuffer->BindBuffer();
	for(; it != mBatchList.end(); it++)
	{
		//glDrawRangeElements(GL_TRIANGLES, it->start_index, it->end_index, it->end_index-it->start_index+1, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glColor3f(0.5f, colorG, 0.8f);
		glDrawElements(GL_TRIANGLES, it->count*3, GL_UNSIGNED_INT, BUFFER_OFFSET(it->startOffset * sizeof(TFaceIndex)));

		//glDrawRangeElements(GL_TRIANGLES, 16, 35, 12, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		colorG = 0.8f;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	mVertexChainBuffer->UnbindBuffer();

/*
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < mMeshBox->GetMeshDefinition().nIndexList.size(); i++)
	{
		glVertex3f(mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].a].x, 
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].a].y,
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].a].z);

		glVertex3f(mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].b].x,
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].b].y,
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].b].z);

		glVertex3f(mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].c].x,
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].c].y,
			mMeshBox->GetMeshDefinition().nVertexList[mMeshBox->GetMeshDefinition().nIndexList[i].c].z);
	}
	glEnd();
*/
}

void CBasicSceneNode::PreparingBatchList(void)
{
	CMemoryBuffer vertexes(&(mMeshBox->GetMeshDefinition().nVertexList[0]), 
		sizeof(TVertex3d) * mMeshBox->GetMeshDefinition().nVertexList.size());
	CMemoryBuffer indexes(&(mMeshBox->GetMeshDefinition().nIndexList[0]), 
		sizeof(TFaceIndex) * mMeshBox->GetMeshDefinition().nIndexList.size());

	mVertexChainBuffer.Bind(new CHardwareVertexBuffer(vertexes, HWB_STATIC));
	mVboIndexBuffer.Bind(new CHardwareIndexBuffer(indexes, HWB_STATIC));

	TBatchStruct batch;
	for(nova::nUInt32 i = 0; i < mMeshBox->GetMeshDefinition().nMatChangesGroups.size(); i++)
	{
		batch.startOffset = mMeshBox->GetMeshDefinition().nMatChangesGroups[i];

		if(mMeshBox->GetMeshDefinition().nMatChangesGroups.size() == i+1)
			batch.count = mMeshBox->GetMeshDefinition().nIndexList.size() - mMeshBox->GetMeshDefinition().nMatChangesGroups[i];
		else
			batch.count = mMeshBox->GetMeshDefinition().nMatChangesGroups[i+1] - mMeshBox->GetMeshDefinition().nMatChangesGroups[i];
		//batch.Material = CResourceManager::GetResourceFromHash(mMeshBox->GetMeshDefinition().nMeshInfoList[batch.start_index].nMatName);
		mBatchList.push_back(batch);
	}
}

CBasicSceneNode::CBasicSceneNode(CMovableObject *pObj, CMeshBoxPtr &sourceMesh, CSceneManager *scene) :
	CSceneNode(pObj, scene), mMeshBox(sourceMesh)
{

}

CBasicSceneManager::CBasicSceneManager(const nstring &scene_name, const nstring & factory_name) : CSceneManager(scene_name, factory_name)
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

		xmlTextWriterEndElement(xmlWriter);

		CMeshBoxPtr nodeMesh = dynamic_cast<CBasicSceneNode *>(node->GetData().GetPtr())->GetMeshBox();
		if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "Sub-materials") < 0) // NovaScene doc
			NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
		for(nova::nUInt32 i = 0; i < nodeMesh->GetMeshDefinition().nMatChangesGroups.size(); i++)
		{
			if(xmlTextWriterStartElement(xmlWriter, BAD_CAST "Material") < 0) // NovaScene doc
				NOVA_EXP("CResource::SerializeToXmlFile: xmlTextWriterStartElement fail", BAD_OPERATION);
			if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST "MatName", "%s", 
				nodeMesh->GetMeshDefinition().nMeshInfoList[nodeMesh->GetMeshDefinition().nMatChangesGroups[i]].nMatName.c_str()) < 0)
				NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);
			if(xmlTextWriterWriteFormatAttribute(xmlWriter, BAD_CAST "SubMatID", "%d", 
				nodeMesh->GetMeshDefinition().nMeshInfoList[nodeMesh->GetMeshDefinition().nMatChangesGroups[i]].nMatSubID) < 0)
				NOVA_EXP("CImage::SerializeToXmlFileImpl: xmlTextWriterWriteAttribute fail", BAD_OPERATION);

			xmlTextWriterEndElement(xmlWriter);
		}

		xmlTextWriterEndElement(xmlWriter);	
		
		for(nInt32 i = 0; i < node->GetChildrenLen(); i++)
		{
			SerializeNodeToXml(node->GetNode(i), xmlWriter);
		}
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
					continue;
				}

				if(!xmlStrcmp(subNode->name, (xmlChar *)"Orientation"))
				{
					for(xmlNodePtr orientNode = subNode->children; orientNode; orientNode = orientNode->next)
					{
						if(xmlIsBlankNode(orientNode))
						{
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