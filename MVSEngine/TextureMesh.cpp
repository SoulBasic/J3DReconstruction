#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include <boost/program_options.hpp>
#include "MVSEngine.h"
using namespace MVS;
// D E F I N E S ///////////////////////////////////////////////////
#define APPNAME _T("TextureMesh")

// S T R U C T S ///////////////////////////////////////////////////

namespace OPT_TextureMesh {
	String strInputFileName;
	String strOutputFileName;
	String strMeshFileName;
	float fDecimateMesh;
	unsigned nCloseHoles;
	unsigned nResolutionLevel;
	unsigned nMinResolution;
	float fOutlierThreshold;
	float fRatioDataSmoothness;
	bool bGlobalSeamLeveling;
	bool bLocalSeamLeveling;
	unsigned nTextureSizeMultiple;
	unsigned nRectPackingHeuristic;
	uint32_t nColEmpty;
	unsigned nOrthoMapResolution;
	unsigned nArchiveType;
	int nProcessPriority;
	unsigned nMaxThreads;
	String strExportType;
	String strConfigFileName;
	boost::program_options::variables_map vm;
} // namespace OPT_TextureMesh

// Initialize_TextureMesh and parse the command line parameters
bool MVSEngine::Initialize_TextureMesh(size_t argc, LPCTSTR* argv)
{
	// Initialize_Dense log and console
	CLOSE_LOGFILE();
	CLOSE_LOGCONSOLE();
	CLOSE_LOG();
	OPEN_LOG();
	OPEN_LOGCONSOLE();

	// group of options allowed only on command line
	boost::program_options::options_description generic("Generic options");
	generic.add_options()
		("help,h", "produce this help message")
		("working-folder,w", boost::program_options::value<std::string>(&WORKING_FOLDER), "working directory (default current directory)")
		("config-file,c", boost::program_options::value<std::string>(&OPT_TextureMesh::strConfigFileName)->default_value(APPNAME _T(".cfg")), "file name containing program options")
		("export-type", boost::program_options::value<std::string>(&OPT_TextureMesh::strExportType)->default_value(_T("ply")), "file type used to export the 3D scene (ply or obj)")
		("archive-type", boost::program_options::value<unsigned>(&OPT_TextureMesh::nArchiveType)->default_value(2), "project archive type: 0-text, 1-binary, 2-compressed binary")
		("process-priority", boost::program_options::value<int>(&OPT_TextureMesh::nProcessPriority)->default_value(-1), "process priority (below normal by default)")
		("max-threads", boost::program_options::value<unsigned>(&OPT_TextureMesh::nMaxThreads)->default_value(0), "maximum number of threads (0 for using all available cores)")
#if TD_VERBOSE != TD_VERBOSE_OFF
		("verbosity,v", boost::program_options::value<int>(&g_nVerbosityLevel)->default_value(
#if TD_VERBOSE == TD_VERBOSE_DEBUG
			3
#else
			2
#endif
		), "verbosity level")
#endif
		;

	// group of options allowed both on command line and in config file
	boost::program_options::options_description config("Texture options");
	config.add_options()
		("input-file,i", boost::program_options::value<std::string>(&OPT_TextureMesh::strInputFileName), "input filename containing camera poses and image list")
		("output-file,o", boost::program_options::value<std::string>(&OPT_TextureMesh::strOutputFileName), "output filename for storing the mesh")
		("decimate", boost::program_options::value<float>(&OPT_TextureMesh::fDecimateMesh)->default_value(1.f), "decimation factor in range [0..1] to be applied to the input surface before refinement (0 - auto, 1 - disabled)")
		("close-holes", boost::program_options::value<unsigned>(&OPT_TextureMesh::nCloseHoles)->default_value(30), "try to close small holes in the input surface (0 - disabled)")
		("resolution-level", boost::program_options::value<unsigned>(&OPT_TextureMesh::nResolutionLevel)->default_value(0), "how many times to scale down the images before mesh refinement")
		("min-resolution", boost::program_options::value<unsigned>(&OPT_TextureMesh::nMinResolution)->default_value(640), "do not scale images lower than this resolution")
		("outlier-threshold", boost::program_options::value<float>(&OPT_TextureMesh::fOutlierThreshold)->default_value(6e-2f), "threshold used to find and remove outlier face textures (0 - disabled)")
		("cost-smoothness-ratio", boost::program_options::value<float>(&OPT_TextureMesh::fRatioDataSmoothness)->default_value(0.1f), "ratio used to adjust the preference for more compact patches (1 - best quality/worst compactness, ~0 - worst quality/best compactness)")
		("global-seam-leveling", boost::program_options::value<bool>(&OPT_TextureMesh::bGlobalSeamLeveling)->default_value(true), "generate uniform texture patches using global seam leveling")
		("local-seam-leveling", boost::program_options::value<bool>(&OPT_TextureMesh::bLocalSeamLeveling)->default_value(true), "generate uniform texture patch borders using local seam leveling")
		("texture-size-multiple", boost::program_options::value<unsigned>(&OPT_TextureMesh::nTextureSizeMultiple)->default_value(0), "texture size should be a multiple of this value (0 - power of two)")
		("patch-packing-heuristic", boost::program_options::value<unsigned>(&OPT_TextureMesh::nRectPackingHeuristic)->default_value(3), "specify the heuristic used when deciding where to place a new patch (0 - best fit, 3 - good speed, 100 - best speed)")
		("empty-color", boost::program_options::value<uint32_t>(&OPT_TextureMesh::nColEmpty)->default_value(0x00696969), "color used for faces not covered by any image")
		("orthographic-image-resolution", boost::program_options::value<unsigned>(&OPT_TextureMesh::nOrthoMapResolution)->default_value(0), "orthographic image resolution to be generated from the textured mesh - the mesh is expected to be already geo-referenced or at least properly oriented (0 - disabled)")
		;

	// hidden options, allowed both on command line and
	// in config file, but will not be shown to the user
	boost::program_options::options_description hidden("Hidden options");
	hidden.add_options()
		("mesh-file", boost::program_options::value<std::string>(&OPT_TextureMesh::strMeshFileName), "mesh file name to texture (overwrite the existing mesh)")
		;

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(generic).add(config).add(hidden);

	boost::program_options::options_description config_file_options;
	config_file_options.add(config).add(hidden);

	boost::program_options::positional_options_description p;
	p.add("input-file", -1);

	try {

		boost::program_options::store(boost::program_options::command_line_parser((int)argc, argv).options(cmdline_options).positional(p).run(), OPT_TextureMesh::vm);
		boost::program_options::notify(OPT_TextureMesh::vm);
		INIT_WORKING_FOLDER;
		// parse configuration file
		std::ifstream ifs(MAKE_PATH_SAFE(OPT_TextureMesh::strConfigFileName));
		if (ifs) {
			boost::program_options::store(parse_config_file(ifs, config_file_options), OPT_TextureMesh::vm);
			boost::program_options::notify(OPT_TextureMesh::vm);
		}
	}
	catch (const std::exception& e) {
		LOG(e.what());
		return false;
	}


	OPEN_LOGFILE(MAKE_PATH(APPNAME _T("-") + Util::getUniqueName(0) + _T(".log")).c_str());


	//Util::LogBuild();
	//LOG(_T("Command line:%s"), Util::CommandLineToString(argc, argv).c_str());

	// validate input
	Util::ensureValidPath(OPT_TextureMesh::strInputFileName);
	Util::ensureUnifySlash(OPT_TextureMesh::strInputFileName);
	if (OPT_TextureMesh::vm.count("help") || OPT_TextureMesh::strInputFileName.IsEmpty()) {
		boost::program_options::options_description visible("Available options");
		visible.add(generic).add(config);
		GET_LOG() << visible;
	}
	if (OPT_TextureMesh::strInputFileName.IsEmpty())
		return false;
	OPT_TextureMesh::strExportType = OPT_TextureMesh::strExportType.ToLower() == _T("obj") ? _T(".obj") : _T(".ply");


	Util::ensureValidPath(OPT_TextureMesh::strOutputFileName);
	Util::ensureUnifySlash(OPT_TextureMesh::strOutputFileName);
	if (OPT_TextureMesh::strOutputFileName.IsEmpty())
		OPT_TextureMesh::strOutputFileName = Util::getFileFullName(OPT_TextureMesh::strInputFileName) + _T("_texture.J3D");

	Process::setCurrentProcessPriority((Process::Priority)OPT_TextureMesh::nProcessPriority);
#ifdef _USE_OPENMP
	if (OPT_TextureMesh::nMaxThreads != 0)
		omp_set_num_threads(OPT_TextureMesh::nMaxThreads);
#endif

#ifdef _USE_BREAKPAD
	// start memory dumper
	MiniDumper::Create(APPNAME, WORKING_FOLDER);
#endif

	Util::Init();
	return true;
}

void MVSEngine::Finalize_TextureMesh()
{
#if TD_VERBOSE != TD_VERBOSE_OFF
	// print memory statistics
	Util::LogMemoryInfo();
#endif

}

int MVSEngine::TextureMesh(int num, char* cmd[])
{
#ifdef _DEBUGINFO
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// | _CRTDBG_CHECK_ALWAYS_DF);
#endif
	int argc = num;
	LPCTSTR* argv = (LPCTSTR*)cmd;
	if (!Initialize_TextureMesh(argc, argv))
		return EXIT_FAILURE;

	Scene scene(OPT_TextureMesh::nMaxThreads);
	// load and texture the mesh
	if (!scene.Load(MAKE_PATH_SAFE(OPT_TextureMesh::strInputFileName)))
		return EXIT_FAILURE;
	if (!OPT_TextureMesh::strMeshFileName.IsEmpty()) {
		// load given mesh
		scene.mesh.Load(MAKE_PATH_SAFE(OPT_TextureMesh::strMeshFileName));
	}
	if (scene.mesh.IsEmpty()) {
		VERBOSE("error: empty initial mesh");
		return EXIT_FAILURE;
	}
	const String baseFileName(MAKE_PATH_SAFE(Util::getFileFullName(OPT_TextureMesh::strOutputFileName)));
	if (OPT_TextureMesh::nOrthoMapResolution && !scene.mesh.textureDiffuse.empty()) {
		goto ProjectOrtho;
	}

	{

		if (OPT_TextureMesh::fDecimateMesh < 1.f) {
			ASSERT(OPT_TextureMesh::fDecimateMesh > 0.f);
			scene.mesh.Clean(OPT_TextureMesh::fDecimateMesh, 0.f, false, OPT_TextureMesh::nCloseHoles, 0u, false);
			scene.mesh.Clean(1.f, 0.f, false, 0, 0u, true); // extra cleaning to remove non-manifold problems created by closing holes
#if TD_VERBOSE != TD_VERBOSE_OFF
			if (VERBOSITY_LEVEL > 3)
				scene.mesh.Save(baseFileName + _T("_decim") + OPT_TextureMesh::strExportType);
#endif
		}


		TD_TIMER_START();
		if (!scene.TextureMesh(OPT_TextureMesh::nResolutionLevel, OPT_TextureMesh::nMinResolution, OPT_TextureMesh::fOutlierThreshold, OPT_TextureMesh::fRatioDataSmoothness, OPT_TextureMesh::bGlobalSeamLeveling, OPT_TextureMesh::bLocalSeamLeveling, OPT_TextureMesh::nTextureSizeMultiple, OPT_TextureMesh::nRectPackingHeuristic, Pixel8U(OPT_TextureMesh::nColEmpty)))
			return EXIT_FAILURE;
		VERBOSE("Mesh texturing completed: %u vertices, %u faces (%s)", scene.mesh.vertices.GetSize(), scene.mesh.faces.GetSize(), TD_TIMER_GET_FMT().c_str());

		scene.Save(baseFileName + _T(".J3D"), (ARCHIVE_TYPE)OPT_TextureMesh::nArchiveType);
		scene.mesh.Save(baseFileName + OPT_TextureMesh::strExportType);
#if TD_VERBOSE != TD_VERBOSE_OFF
		if (VERBOSITY_LEVEL > 2)
			scene.ExportCamerasMLP(baseFileName + _T(".mlp"), baseFileName + OPT_TextureMesh::strExportType);
#endif
	}

	if (OPT_TextureMesh::nOrthoMapResolution) {
	ProjectOrtho:
		Image8U3 imageRGB;
		Image8U imageRGBA[4];
		Point3 center;
		scene.mesh.ProjectOrthoTopDown(OPT_TextureMesh::nOrthoMapResolution, imageRGB, imageRGBA[3], center);
		Image8U4 image;
		cv::split(imageRGB, imageRGBA);
		cv::merge(imageRGBA, 4, image);
		image.Save(baseFileName + _T("_orthomap.png"));
		SML sml(_T("OrthoMap"));
		sml[_T("Center")].val = String::FormatString(_T("%g %g %g"), center.x, center.y, center.z);
		sml.Save(baseFileName + _T("_orthomap.txt"));
	}

	Finalize_TextureMesh();
	return EXIT_SUCCESS;
}
