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
#include "nova_example_application.h"

using namespace nova;

class ImageExample : public CExampleApplication
{
protected:

class MyImageExampleListener : public CResourceListener
{
public:

	void FreeResourceListener(CResource * object)
	{
		nstringstream str;

		str << "Image: " << object->GetResName() << " destroyed successfully\n";

		LOG_MESSAGE(str.str());
	}

	void PostLoadResourceListener(CResource * object)
	{
		nstringstream str;

		str << "Image: " << object->GetResName() << " load successfully, ";
		str << "Total resource hash size: " << CResourceManager::GetHashSize() << " bytes";

		LOG_MESSAGE(str.str());
	}

	void PostBuildResourceListener(CResource * object)
	{
		nstringstream str;

		str << "Image: " << object->GetResName() << " successfully builded!\n";
		str << "Resolution: " << ((CImage*)object)->GetWidth() << 
			"x" << ((CImage*)object)->GetHeight();
		str << "\nTotal resource hash size: " << CResourceManager::GetHashSize();

		LOG_MESSAGE(str.str());
	}

	void PreUnloadingActionListener(CResource * object)
	{
		nstringstream str;

		str << "Image: " << object->GetResName() << " successfully unloaded!\n";

		LOG_MESSAGE(str.str());
	}

} mListener;

public:

	ImageExample()
	{

	}

	~ImageExample()
	{
		ShutDown();
	}

	CImagePtr LoadImageFromFile(const nstring & file)
	{
		CImagePtr image = CImageManager::GetSingelton().CreateNewImage(file, nstring("manual"),
			file, "DevIL", CImageFormats::NF_DEFAULT);

		image->LoadResource();
		return image;
	}

	CImagePtr LoadImageFromMemory(const nstring &name, const CMemoryBuffer & buffer, ESaveFormats comp)
	{
		CImagePtr image = CImageManager::GetSingelton().CreateNewImage(name, nstring("manual"),
			buffer, comp, "DevIL", CImageFormats::NF_DEFAULT);

		return image;
	}

	CImagePtr CopyImage(const CImagePtr &image_ptr)
	{
		nstring name("copy_");
		name.append(image_ptr->GetResName());

		CImagePtr image = CImageManager::GetSingelton().CreateNewImage(name, nstring("manual"),
			image_ptr->GetBits(), image_ptr->GetWidth(), image_ptr->GetHeight(), 
			image_ptr->GetDepth(), image_ptr->GetPixelFormat());

		image->LoadResource();

		return image;
	}

	void SaveImage(const nstring & file, const CImagePtr &image_ptr)
	{
		CDevILCodec::GetSingelton().CodeToFile(file, *(image_ptr.GetPtr()));
	}

	void Start()
	{
		StartUpEngine(ST_INIT_YOURSELF);

		CImageManager::GetSingelton().AddDefaultListener(&mListener);

		CImagePtr MainImage = LoadImageFromFile("Test.jpg");

		CImagePtr Image1 = CopyImage(MainImage);
		SaveImage(nstring("Image1.png"), MainImage);

		Image1->Negativity();
		SaveImage(nstring("Image2.png"), Image1);
		SaveImage(nstring("Image2.jpg"), Image1);
		SaveImage(nstring("Image2.bmp"), Image1);
		SaveImage(nstring("Image2.tif"), Image1);
		SaveImage(nstring("Image2.tga"), Image1);

		MainImage->SerializeToXmlFile("image.xml");
		MainImage->FreeResource();
		CResourceManager::UnloadResourceFromHash(MainImage);

		CImagePtr Image2 = CResourceManager::LoadResourceFromXml("image.xml");
		Image2->BackHeigth();
		Image2->Pixelization(10);

		SaveImage(nstring("Image3.png"), Image2);
		Image2->FreeResource();
		CResourceManager::UnloadResourceFromHash(Image2);

		nova::CFilesPackage mPack;
		mPack.OpenPackage("test.package", false);
		CImagePtr Image3 = CResourceManager::LoadResourceFromXml("image.xml", mPack);

		mPack.ClosePackage();

		SaveImage(nstring("Image5.png"), Image3);

/*

		CImagePtr Image2 = CopyImage(Image1);
		Image2->Blurring(3);

		SaveImage(nstring("Image2.png"), Image2);
		SaveImage(nstring("Image2.jpg"), Image2);
		SaveImage(nstring("Image2.bmp"), Image2);
		SaveImage(nstring("Image2.tif"), Image2);
		SaveImage(nstring("Image2.tga"), Image2);

		CImagePtr Image3 = CopyImage(Image2);
		Image3->Negativity();
		Image3->Gamma(0.5f);

		SaveImage(nstring("Image3.png"), Image3);
		SaveImage(nstring("Image3.jpg"), Image3);
		SaveImage(nstring("Image3.bmp"), Image3);
		SaveImage(nstring("Image3.tif"), Image3);
		SaveImage(nstring("Image3.tga"), Image3);

		MainImage->Alienifying();

		SaveImage(nstring("MainImage.png"), MainImage);
		SaveImage(nstring("MainImage.jpg"), MainImage);
		SaveImage(nstring("MainImage.bmp"), MainImage);
		SaveImage(nstring("MainImage.tif"), MainImage);
		SaveImage(nstring("MainImage.tga"), MainImage);
*/
	}
};


ENTRY_POINT	
{
	ImageExample example;

	try
	{
		example.Start();
	}
	catch(NovaExp & exp)
	{
		cerr << exp.Message() << endl;
		exp.FixError();
	}
	catch(exception & exp)
	{
		cerr << exp.what() << endl;
	}
	cout << "ShutDoun Parser Example... " << endl;

	return 0;
}
