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

		str << "Image: " << object->GetResName() << " successfully free!";

		LOG_MESSAGE(str.str());
	}

	void PostLoadResourceListener(CResource * object)
	{
		nstringstream str;

		str << "Image: " << object->GetResName() << " successfully prepared!";

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
		str << "Total resource hash size: " << CResourceManager::GetHashSize();

		LOG_MESSAGE(str.str());
	}

} *mListener;

public:

	ImageExample()
	{
		mListener = new MyImageExampleListener();
	}

	~ImageExample()
	{
		ShutDown();
		delete mListener;
	}

	CImagePtr LoadImageFromFile(const nstring & file)
	{
		CImagePtr image = CImageManager::GetSingelton().CreateNewImage(file, nstring("manual"),
			file, "DevIL", CImageFormats::NF_DEFAULT);

		image->BackHeigth();
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

		return image;
	}

	void SaveImage(const nstring & file, const CImagePtr &image_ptr)
	{
		CDevILCodec::GetSingelton().CodeToFile(nstring("image_test\\") + file, *(image_ptr.GetPtr()));
	}

	void Start()
	{
		StartUpEngine(ST_INIT_YOURSELF);
		MyImageExampleListener *listener = new MyImageExampleListener();

		CImageManager::GetSingelton().AddDefaultListener(listener);

		CImagePtr MainImage = LoadImageFromFile("Test.jpg");

		CImagePtr Image1 = CopyImage(MainImage);

		Image1->Negativity();
		SaveImage(nstring("Image1.png"), Image1);
		SaveImage(nstring("Image1.jpg"), Image1);
		SaveImage(nstring("Image1.bmp"), Image1);
		SaveImage(nstring("Image1.tif"), Image1);
		SaveImage(nstring("Image1.tga"), Image1);

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

		CImageManager::GetSingelton().UnloadResourceFromHash(MainImage);
		CImageManager::GetSingelton().UnloadResourceFromHash(Image1);
		CImageManager::GetSingelton().UnloadResourceFromHash(Image2);
		CImageManager::GetSingelton().UnloadResourceFromHash(Image3);
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
