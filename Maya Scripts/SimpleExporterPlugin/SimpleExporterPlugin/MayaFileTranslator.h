//-----------------------------------------------------------------------
///	\file    	MayaFileTranslator.h
///	\date   	24/4/2004   14:55
///	\author		Rob Bateman
///	\brief      Defines the main File translator class. This contains 
///				the read and write methods for the file translator.
/// \note       
/// 
/// 		This sourcefile is provided as-is and no guarentee or warrenty 
///			is provided for any damage that may arise from using it. This
///			code is free for your own use, the only thing i ask is a small
///			credit somewhere for my work. An e-mail saying you found it 
///			useful would also be much appreciated by myself ;)
///
///			If you find any bugs arising to maya version changes or you
///			have improvements to the source code, then feel free to mail
///			them to me and i'll update the source-code on the website.
///			Before you e-mail bug reports, make sure that it is my code 
///			causing the problem, and not some wierd oddity in the maya 
///			scene. This code is designed to handle most common model set-ups,
///			it is not designed to replicate perfectly everything that maya
///			does. 
/// 
///			DO NOT send me feature requests, i'm not likely to add to this
///			code myself unless something really new and groovy comes along.
///			This is mainly due to my current time restrictions. If you do
///			have code written that adds something fairly groovy to the 
///			exporter, then i will happily update the exporter code and give
///			full credit where due.....
/// 
///			Robert Bateman
/// 
///				email code to:    rbateman@bmth.ac.uk
///				contact:          robthebloke@hotmail.com
/// 
//--------------------------------------------------------------------------

#ifndef __MayaFileTranslator__H__
#define __MayaFileTranslator__H__

#ifdef WIN32
	#pragma once
#endif

#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MFloatVector.h>
#include <maya/MPoint.h>
#include <vector>
//-----------------------------------------------------------------------
/// \brief	This class defines the main file translator class.
///
class MayaFileTranslator :
	public MPxFileTranslator
{
public:

	//-------------------------------------------------------------------
	///	\brief	ctor  
	///	
	MayaFileTranslator(void);

	//-------------------------------------------------------------------
	///	\brief	dtor  
	///	
	~MayaFileTranslator(void);


	//-------------------------------------------------------------------
	///	\brief  This function reads the requested file into maya (import)
	///	\param  file			-	info about the file, ie paths, filename etc
	///	\param	optionsString	-	mel passes options read from the user
	///			interface through to the read function
	/// \param	mode	-	ignore for reading
	///
	MStatus		reader( const MFileObject& file,
						const MString& optionsString,
						FileAccessMode mode);

	//-------------------------------------------------------------------
	///	\brief  
	///	\param  
	///	\return	
	///
	MStatus		writer( const MFileObject& file,
						const MString& optionsString,
						FileAccessMode mode );

	//-------------------------------------------------------------------
	///	\brief  returns true if the file translator supports importing 
	///	\return	true
	///
	bool        haveReadMethod	 () const;

	//-------------------------------------------------------------------
	///	\brief  returns true if the file translator supports exporting 
	///	\return	true
	///
	bool        haveWriteMethod	 () const;

	//-------------------------------------------------------------------
	///	\brief  returns the file extension of this format
	///	\return	"rob"
	///
	MString     defaultExtension () const;

	//-------------------------------------------------------------------
	///	\brief  This method is used to check to see if we support the 
	///			specified file format. Usually it is sensible to have a 
	///			standard identifier at the start of the file that you can
	///			check, ie 'MYFILE01'. i don't have that luxury, so instead
	///			I am using only a check against the files extension.
	///	\param  fileName	-	the name of the file
	///	\param	buffer		-	the first few bytes of the file
	/// \param	size		-	number of bytes in buffer
	///	\return	returns is my file type or is not my file type
	///
	MFileKind   identifyFile(	const MFileObject& fileName,
								const char* buffer,
								short size) const;

	//-------------------------------------------------------------------
	///	\brief  Maya requires you to provide a static function that 
	///			returns the address of a new instance of your node.
	///	\return  new file translator node
	///
	static void* creator();

	void LoadMesh(MObject& item);
private:
	MMatrix BuildTransform(MObject& obj);
	void WriteRasterTekFormatAscii(std::ofstream& ofs);
	void WriteRasterTekFormatBinary(std::ofstream& ofs);
	void CreateSubsetTable(std::ofstream& ofs);
private:

	int m_totalVertexCount;

	struct Mesh
	{
		int m_numVertices;
		int m_numNormals;
		int m_numUVs;
		int m_numPolygons;
		std::vector<MPoint> m_vertices;
		std::vector<MFloatVector> m_normals;
		std::vector<MFloatVector> m_uvs;
		std::vector<int> m_vertexIndices;
		std::vector<int> m_normalIndices;
		std::vector<int> m_uvIndices;
	};

	bool		m_bShort;

	std::vector<Mesh*> m_meshes;
};

#endif



