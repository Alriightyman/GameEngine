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
#include <maya/MPlug.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
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
	const MString	namesonlyFlag	("FileType");

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

		BuildTextureData();

		for(int i = 0; i < m_meshes.size(); i++)
		{
			m_totalVertexCount += m_meshes[i]->m_vertexIndices.size();
			m_meshes[i]->mat.DiffuseMap = m_diffuseMaps[m_diffuseMaps.size() - i - 1];
			m_meshes[i]->mat.NormalMap = m_normalMaps[m_normalMaps.size() - i - 1];
		}

		WriteRasterTekFormatAscii(ofs);

		ofs.close();
		while(!m_meshes.empty())
		{
			Mesh* m = m_meshes.back();
			if(m){ delete m; m = 0; }
			m_meshes.pop_back();

		}
		// reset containers
		m_meshes.clear();
		m_meshes.resize(0);
		m_diffuseMaps.clear();
		m_diffuseMaps.resize(0);
		m_normalMaps.clear();
		m_normalMaps.resize(0);
		m_totalVertexCount = 0;
	}
	return MS::kSuccess;
}

void MayaFileTranslator::BuildTextureData()
{
// iterate through the mesh nodes in the scene
MItDependencyNodes itDep(MFn::kLambert);

// we have to keep iterating until we get through
// all of the nodes in the scene
//
	while (!itDep.isDone())
	{
  		switch(itDep.item().apiType()) 
		{
  		// if found phong shader
			case MFn::kPhong:
				{
					MFnPhongShader fn( itDep.item() );
					cout<<"Phong "<<fn.name().asChar()<<"\n";
					OutputColor(fn,"ambientColor");
					OutputColor(fn,"color");
					OutputColor(fn,"specularColor");
					OutputColor(fn,"incandescence");
					OutputColor(fn,"transparency");
					cout<<"\tcos "<<fn.cosPower()<< endl;
					OutputBumpMap(itDep.item());
					//OutputEnvMap(itDep.item());
				}
				break;
			// if found blinn shader
			case MFn::kBlinn:
				{
					MFnBlinnShader fnBlinn( itDep.item() );
					cout<<"Blinn "<<fnBlinn.name().asChar()<<"\n";
					OutputColor(fnBlinn,"ambientColor");
					OutputColor(fnBlinn,"color");
					OutputColor(fnBlinn,"specularColor");
					OutputColor(fnBlinn,"incandescence");
					OutputColor(fnBlinn,"transparency");
					cout 	<< "\teccentricity " << fnBlinn.eccentricity() << endl;
					cout 	<< "\tspecularRollOff " << fnBlinn.specularRollOff() << endl;
					OutputBumpMap(itDep.item());
					//OutputEnvMap(itDep.item());
				}
				break;
			default:
				break;

		}
		itDep.next();
	}

} 
void MayaFileTranslator::OutputColor(MFnDependencyNode& fn,const char* name)
{
	MPlug p;

	MString r = name;
	r += "R";
	MString g = name;
	g += "G";
	MString b = name;
	b += "B";
	MString a = name;
	a += "A";

	// get the color value
	MColor color;

	// get a plug to the attribute
	p = fn.findPlug(r);
	p.getValue(color.r);
	p = fn.findPlug(g);
	p.getValue(color.g);
	p = fn.findPlug(b);
	p.getValue(color.b);
	p = fn.findPlug(a);
	p.getValue(color.a);
	p = fn.findPlug(name);

	// will hold the txture node name
	MString texname;

	// get plugs connected to colour attribute
	MPlugArray plugs;
	p.connectedTo(plugs,true,false);

	// see if any file textures are present
	for(int i=0;i!=plugs.length();++i)
	{
  		// if file texture found
		if(plugs[i].node().apiType() == MFn::kFileTexture)
		{
  			// bind a function set to it ....
			MFnDependencyNode fnDep(plugs[i].node());

			// to get the node name
			texname = fnDep.name();
			MPlug ftn = fnDep.findPlug("ftn");
			MString filename;
			ftn.getValue(filename);
			MStringArray tex;
			filename.split('/',tex);
			m_diffuseMaps.push_back(tex[tex.length()-1].asChar());
			// stop looping
			break;

		}

	}

	if( name == "color" && color.r <0.01 &&
		color.g < 0.01 && color.b < 0.01)

	{
  		color.r=color.g=color.b=0.6f;

	}
	
}

void MayaFileTranslator::OutputBumpMap(MObject& obj)
{
	MFnDependencyNode fn(obj);

	// get a plug to the normalCamera attribute on the material
	MPlug bump_plug = fn.findPlug("normalCamera");
	MPlugArray connections;

	// get connections to the attribute
	bump_plug.connectedTo(connections,true,false);

	// loop through each one to find a bump2d node
	for(unsigned int i=0;i<connections.length();++i)
	{
  	

		if (connections[i].node().apiType() == MFn::kBump)
		{
			// attach a function set to the 2d bump node
			MFnDependencyNode fnBump(connections[i].node());

			float bump_depth;

			// get the bump depth value from the node
			MPlug bumpDepth = fnBump.findPlug("bumpDepth");
			bumpDepth.getValue(bump_depth);

			// we now have the fun and joy of actually finding
			// the file node that is connected to the bump map
			// node itself. This is going to involve checking
			// the attribute connections to the bumpValue attribute.
			MPlug bump_value = fnBump.findPlug("bumpValue");
			MPlugArray bv_connections;

			bump_value.connectedTo(bv_connections,true,false);
			for(unsigned int j=0;j<bv_connections.length();++j)
			{
  	

				if(bv_connections[i].node().apiType() == MFn::kFileTexture)
				{
  	

					// we have a texture. determine
					// which texture it is.
					MFnDependencyNode fnTex(bv_connections[i].node());

					MPlug ftn = fnTex.findPlug("ftn");

					MString filename;
					ftn.getValue(filename);
					MStringArray tex;
					filename.split('/',tex);
					m_normalMaps.push_back(tex[tex.length()-1].asChar());

					return;
				}

			}

		}

	}
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
		for (int i = 0; i < m_meshes[m]->m_vertexIndices.size(); i++)
		{
			float vIndex = m_meshes[m]->m_vertexIndices[i];
			float tIndex = m_meshes[m]->m_uvIndices[i];
			float nIndex = m_meshes[m]->m_normalIndices[i];

			fout << m_meshes[m]->m_vertices[vIndex].x << ' ' << m_meshes[m]->m_vertices[vIndex].y << ' ' << -m_meshes[m]->m_vertices[vIndex].z << ' '
				 << m_meshes[m]->m_uvs[tIndex].x << ' ' << 1.0f - m_meshes[m]->m_uvs[tIndex].y << ' '
				 << m_meshes[m]->m_normals[nIndex].x << ' ' << m_meshes[m]->m_normals[nIndex].y << ' ' << -m_meshes[m]->m_normals[nIndex].z << endl;
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
		startCount += m_meshes[m]->m_vertexIndices.size();
		fout << "Size: " << (startCount - oldCount) << endl;
		std::string DiffuseMap = m_meshes[m]->mat.DiffuseMap;;
		std::string NormalMap = m_meshes[m]->mat.NormalMap;
		std::string SpecularMap = m_meshes[m]->mat.SpecularMap;
		if (NormalMap.size() < 2)
			NormalMap = "n";
		if (SpecularMap.size() < 2)
			SpecularMap = "n";

		fout << "DiffuseMap: " << DiffuseMap.c_str() << std::endl;
		fout << "NormalMap: " << NormalMap.c_str() << std::endl;
		fout << "SpecularMap: " << SpecularMap.c_str() << std::endl << std::endl;
		//startCount++;
	}
}
void MayaFileTranslator::LoadMesh(MObject& item)
{
	MFnMesh fn(item);
	

	if(!fn.isIntermediateObject() )
	{
		Mesh* mesh = new Mesh();

		for (int k = 0; k < fn.parentCount(); k++)
		{
			// get the counts
			mesh->m_numVertices = fn.numVertices();
			mesh->m_numNormals = fn.numNormals();
			mesh->m_numUVs = fn.numUVs();
			mesh->m_numPolygons = fn.numPolygons();

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
				mesh->m_vertices.push_back(point);
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

				mesh->m_normals.push_back(normal);
			}


			MFloatArray u_coords;
			MFloatArray v_coords;

			fn.getUVs(u_coords,v_coords);

			for (int i =0; i != u_coords.length(); i++)
			{				 
				mesh->m_uvs.push_back(MFloatVector(u_coords[i],v_coords[i]));
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
					mesh->m_vertexIndices.push_back(itPoly.vertexIndex(i));
					mesh->m_normalIndices.push_back(itPoly.normalIndex(i));
					mesh->m_uvIndices.push_back(uv_index);
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
	if (str[len-3] == 'r' &&
		str[len-2] == 't' &&
		str[len-1] == 'x')
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





