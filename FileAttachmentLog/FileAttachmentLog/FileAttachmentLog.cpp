// FileAttachmentLog
//----------------------------
// This is a maya standalone application that opens a specified maya file and 
// retreives all the associated dependency files for the currently open
// Maya file. Paths to all the files are written out to a text file.
// This text file is used by another application to package and archive the dependency files.
// This application takes two arguments - mayafile path and textfile path.

#include <maya/MLibrary.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <fstream>
#include <iostream>
#include <string>
#include <maya/MFileIO.h>
#include <sstream>

using namespace std;

int main(int /*argc*/, char* argv[])
{
	ofstream log;
	log.open("C:\\Temp\\Standalone_log.txt", ios_base::app); //log file

	//list of all plugins to be loaded before launching Maya
	const char* plugin_list[] = {"GamePipeline", "MayaMuscle", "redshift4maya", "bifrostvisplugin", "dx11Shader", "Mayatomr", "BifrostMain", "deformerEvaluator",
	"shaderFXPlugin", "bifrostshellnode", "gameFbxExporter", "GPUBuiltInDeformer", "stereoCamera", "lookdevKit", "Unfold3D", "Type", "poseDeformer",
	"fbxmaya", "poseReader", "modelingToolkit"};

	MStringArray Mplugin_list = MStringArray(plugin_list, 20);

	string logPath = argv[2];

	log << "FilePath created: " << logPath << endl;

	ofstream file;
	file.open(logPath, ios_base::app);

	MStatus status;

	status = MLibrary::initialize(false, argv[0], true);
	if (!status){
		status.perror("MLibrary::initialize");
	}

	log << "Loading Plugins" << endl;

	for (int i = 0; i < 20; i++){
		MString pluginLoadcmd = MString("loadPlugin ") + Mplugin_list[i] + ";";
		MGlobal::executeCommand(pluginLoadcmd); //load all the required plugins
	}

	log << "Plugins loaded" << endl;

	const MString filePath = argv[1];

	log << "Trying to open " << argv[1] << endl;

	//open the Maya File
	MFileIO::open(filePath, NULL, true, MFileIO::ReferenceMode::kLoadAllReferences, true);

	log << "File Opened: " << MFileIO::currentFile() << endl;

	//getting all the associated files from the currently open Maya file
	MStringArray files;
	MFileIO::getFiles(files);

	log << "Files retreived" << endl;

	std::string line;
	for (unsigned int i = 0; i < files.length(); i++){
		line = files[i].asChar();
		if (line != argv[1]){
			file << line << endl;
		}
	}
	
	file.close();

	log << "Done" << endl;
	log.close();
	MLibrary::cleanup();
}

