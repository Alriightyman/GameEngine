//-------------------------------------------------------------------
///	\file    	MayaFileTranslator.cpp
///	\date   	24/4/2004   16:39
///	\author		Rob Bateman
///	\brief      
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

#include <fstream>
#include <iostream>

#include "MayaFileTranslator.h"

#include <maya/MArgList.h>
#include <maya/MFileObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MFnMesh.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVector.h>
#include <maya/MItMeshPolygon.h>

//	maya base class includes for your own defined gubbins
#include <maya/MPxFileTranslator.h>


MMatrix MayaFileTranslator::BuildTransform(MObject& obj)
{
	MFnTransform fn(obj);

	// get matrix transform for this bone
	MMatrix M = fn.transformationMatrix();

	if (fn.parentCount()>0) {
		MObject _obj =fn.parent(0);
		return M* BuildTransform(_obj) ;
	}
	
	return M;
}
//-------------------------------------------------------------------	MayaFileTranslator
///	\brief  ctor
///
MayaFileTranslator::MayaFileTranslator(void) 
	: m_totalVertexCount(0)
{
	m_bShort = false;
}

//-------------------------------------------------------------------	~MayaFileTranslator
///	\brief  dtor	
///
MayaFileTranslator::~MayaFileTranslator(void)
{
}


//-------------------------------------------------------------------	reader
///	\brief  This function reads the requested file into maya (import)
///	\param  file	-	info about the file, ie paths, filename etc
///	\param	options	-	mel passes options read from the user
///			interface through to the read function
/// \param	mode	-	ignore for reading
///
MStatus	MayaFileTranslator::reader( const MFileObject& file,
				   const MString& options,
				   FileAccessMode mode)
{
	std::cerr << "Read method as yet unimplimented\n" << std::endl;
	return MS::kFailure;
}

//-------------------------------------------------------------------	writer
///	\brief  Exports the scene data to a text file
///	\param  file	-	info about what to output
/// \param	options	-	a set of output options
/// \param	mode	-	all or selected
///	\return	status code
///
MStatus	MayaFileTranslator::writer( const MFileObject& file,
				   const MString& options,
				   FileAccessMode mode )
{
	MStatus     	stat;

	//	flags that are created by the option script
	//
	const MString	namesonlyFlag	("File type");

	//	set initial default flags incase something has 
	//	gone wrong on the maya side
	//
	m_bShort		= true;

	//	process the option string passed in from the option script
	//
	if (options.length() > 0) 
	{
        int i, length;
        // Start parsing.
        MStringArray optionList;
        MStringArray theOption;

		//	each option is in the form -
		//	[Option] = [Value];
		//
		//	the options are therefore split by the semi-colons
		//
        options.split(';', optionList);

		//	process each option in the string
		//
		length = optionList.length();
		for( i = 0; i < length; ++i )
		{
            theOption.clear();

			//	split the option using the '=' character. This should make 
			//	theOption[0] hold the option name, and theOption[1] hold the
			//	value of the current option
			//
            optionList[i].split( '=', theOption );

            if( theOption[0] == namesonlyFlag && theOption.length() > 1 ) 
			{
                if(theOption[1].asInt()>0) 
					m_bShort = true;
				else 
					m_bShort = false;
            }
        }
	}
	
	// if this is true, then "Export Selected" was chosen in stead of "Export All"
	//
	if( mode == MPxFileTranslator::kExportActiveAccessMode )
	{
		std::cerr << "As yet this does not support export selected\nExporting all instead\n";
	}

	// open the output file
	//
	std::ofstream ofs(file.fullName().asChar());


	// make sure it opened ok
	if(!ofs) 
	{
		std::cerr << "[ERROR] Could not open file for writing " << file.fullName().asChar() << std::endl;
	}
	else 
	{
		// use an iterator to iterate through all nodes in the scene
		MItDependencyNodes it(MFn::kInvalid);

		// loop through all nodes
		for( ; !it.isDone(); it.next() )
		{
			// attach a function set to this node so we can access some data.
			// the dependency node is just one of many function set types we
			// can use, it forms the base class of all function sets used within the
			// maya API.
			//MFnDependencyNode fn(it.item());
			MObject item = it.item();

			switch (item.apiType())
			{
			case MFn::kMesh:
				LoadMesh(item);
				break;
			}
		}

		for(int i = 0; i < m_meshes.size(); i++)
			m_totalVertexCount += m_meshes[i].m_vertexIndices.size();

		WriteRasterTekFormatAscii(ofs);

		ofs.close();
		
		m_meshes.clear();
		m_meshes.resize(0);
	}
	return MS::kSuccess;
}

void MayaFileTranslator::WriteRasterTekFormatAscii(std::ofstream& fout)
{
	using namespace std;
	fout << "Vertex Count: " << m_totalVertexCount << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	CreateSubsetTable(fout);
	fout << endl;

	for(int m = 0; m < m_meshes.size(); m++)
	{
		for (int i = 0; i < m_meshes[m].m_vertexIndices.size(); i++)
		{
			float vIndex = m_meshes[m].m_vertexIndices[i];
			float tIndex = m_meshes[m].m_uvIndices[i];
			float nIndex = m_meshes[m].m_normalIndices[i];

			fout << m_meshes[m].m_vertices[vIndex].x << ' ' << m_meshes[m].m_vertices[vIndex].y << ' ' << -m_meshes[m].m_vertices[vIndex].z << ' '
				 << m_meshes[m].m_uvs[tIndex].x << ' ' << 1.0f - m_meshes[m].m_uvs[tIndex].y << ' '
				 << m_meshes[m].m_normals[nIndex].x << ' ' << m_meshes[m].m_normals[nIndex].y << ' ' << -m_meshes[m].m_normals[nIndex].z << endl;
		}
	}
}

void MayaFileTranslator::CreateSubsetTable(std::ofstream& fout)
{
	using namespace std;
	int startCount = 0, oldCount = 0;

	fout << "MeshCount: " << m_meshes.size() << endl;

	for (int m = 0; m < m_meshes.size(); m++)
	{
		fout << "Start: " << startCount << endl;
		oldCount = startCount;
		startCount += m_meshes[m].m_vertexIndices.size();
		fout << "Size: " << (startCount - oldCount) << endl;
	}
}
void MayaFileTranslator::LoadMesh(MObject& item)
{
	MFnMesh fn(item);
	

	if(!fn.isIntermediateObject() )
	{
		Mesh mesh;

		for (int k = 0; k < fn.parentCount(); k++)
		{
			// get the counts
			mesh.m_numVertices = fn.numVertices();
			mesh.m_numNormals = fn.numNormals();
			mesh.m_numUVs = fn.numUVs();
			mesh.m_numPolygons = fn.numPolygons();

			// ------------------------------------------
			// get position vertices
			// ------------------------------------------
			MPointArray vertices; 
			fn.getPoints(vertices);

			MObject obj = fn.parent(k);
			MMatrix mat = BuildTransform(obj);

			for(int i = 0; i != vertices.length(); i++)
			{
				MPoint point = vertices[i] * mat;
				mesh.m_vertices.push_back(point);
			}
			// ------------------------------------------
			// get normal vertices
			// ------------------------------------------
			MFloatVectorArray normals;

			fn.getNormals(normals);

			MFloatMatrix fmat;

			for(int i=0;i!=4;++i)
			{
				for(int j=0;j!=4;++j)
				{
					fmat.matrix[i][j] = static_cast<float>(mat.matrix[i][j]);
				}
			}

			fmat = fmat.inverse();
			fmat = fmat.transpose();

			for (int i = 0; i != normals.length(); i++)
			{
				MFloatVector normal = normals[i];

				normal.normalize();

				mesh.m_normals.push_back(normal);
			}


			MFloatArray u_coords;
			MFloatArray v_coords;

			fn.getUVs(u_coords,v_coords);

			for (int i =0; i != u_coords.length(); i++)
			{				 
				mesh.m_uvs.push_back(MFloatVector(u_coords[i],v_coords[i]));
			}


			//--------------------------------------
			// face data
			// --------------------------------------
			MItMeshPolygon itPoly(fn.object());

			while(!itPoly.isDone() )
			{
				unsigned int vcount = itPoly.polygonVertexCount();

				for(int i = vcount-1; i != -1; i--)
				{
					int uv_index;
					itPoly.getUVIndex(i,uv_index);
					mesh.m_vertexIndices.push_back(itPoly.vertexIndex(i));
					mesh.m_normalIndices.push_back(itPoly.normalIndex(i));
					mesh.m_uvIndices.push_back(uv_index);
				}

				itPoly.next();
			}

		}

		m_meshes.push_back(mesh);
	}
}

//-------------------------------------------------------------------	haveReadMethod
///	\brief  returns true if the file translator supports importing 
///	\return	false
///
bool  MayaFileTranslator::haveReadMethod	 () const {
	return false;
}


//-------------------------------------------------------------------	haveWriteMethod
///	\brief  returns true if the file translator supports exporting 
///	\return	true
///
bool MayaFileTranslator::haveWriteMethod	 () const {
	return true;
}


//-------------------------------------------------------------------	defaultExtension
///	\brief  returns the file extension of this format
///	\return	"rob"
///
MString MayaFileTranslator::defaultExtension () const {
	return "rtx";
}


//-------------------------------------------------------------------	identifyFile
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
MPxFileTranslator::MFileKind MayaFileTranslator::identifyFile(	const MFileObject& fileName,
						 const char* buffer,
						 short size) const {
	const char* str = fileName.name().asChar();
	unsigned int len = fileName.name().length();
	if (str[len-3] == 'm' &&
		str[len-2] == 'o' &&
		str[len-1] == 'd')
	{
		return kCouldBeMyFileType;
	}
	return kNotMyFileType;
}

//-------------------------------------------------------------------	creator
///	\brief  Maya requires you to provide a static function that 
///			returns the address of a new instance of your node.
///	\return  new file translator node
///
void* MayaFileTranslator::creator() {
	return new MayaFileTranslator;
}





