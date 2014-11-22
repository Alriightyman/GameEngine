//-------------------------------------------------------------------
///	\file    	MayaMain.cpp
///	\date   	24/4/2004   20:10
///	\author		Rob Bateman
///	\brief      This file registers and unregisters the mel commands
///				and the file translator when the plugin in loaded
///				and un-loaded. 
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

#include "MayaFileTranslator.h"
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

#include <iostream>


// link to some fairly funky maya libs. NOTE: The Image library only became part of maya
// from version 4.01 onwards. It is not available in version 4.0
//
#ifdef WIN32
	#pragma comment(lib,"Foundation.lib")
	#pragma comment(lib,"OpenMaya.lib")
	#pragma comment(lib,"OpenMayaFx.lib")
	#pragma comment(lib,"Image.lib")
	#pragma comment(lib,"OpenMayaAnim.lib")
#endif


//-------------------------------------------------------------------
#ifdef WIN32
	#define MLL_EXPORT extern __declspec(dllexport) 
#else
	#define MLL_EXPORT
#endif

//-------------------------------------------------------------------

/// specifies a script to use for the user interface options box
char* g_OptionScript = "MayaFileExportScript";

/// a set of default options for the exporter
char* g_DefaultOptions = "-namesonly=0;";

//-------------------------------------------------------------------	initializePlugin
///	\brief	initializePlugin( MObject obj )
///	\param	obj		-	the plugin handle
///	\return	MS::kSuccess if ok
///	\note	Registers all of the new commands, file translators and new 
///			node types.
///
MLL_EXPORT MStatus initializePlugin( MObject obj )
{
	MStatus status;

	std::cout << "A Simple File translator plugin example. Rob Bateman 2004 [robthebloke@hotmail.com]" << std::endl;
	MFnPlugin plugin( obj, "Rob Bateman", "3.0", "Any");

	// Register the translator with the system
	status =  plugin.registerFileTranslator( "SimpleExport", "none",
		MayaFileTranslator::creator,
		(char*)g_OptionScript,
		(char*)g_DefaultOptions );  

	if (status != MS::kSuccess) {
		status.perror("MayaExportCommand::registerFileTranslator");
	}
	return status;
                                     
}


//-------------------------------------------------------------------	uninitializePlugin
///	\brief	uninitializePlugin( MObject obj )
///	\param	obj	-	the plugin handle to un-register
///	\return	MS::kSuccess if ok
///	\note	un-registers the plugin and destroys itself
///
MLL_EXPORT MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	MStatus status =  plugin.deregisterFileTranslator( "SimpleExport" );
	if (status != MS::kSuccess) {
		status.perror("MayaExportCommand::deregisterFileTranslator");
	}

	return status;
}

