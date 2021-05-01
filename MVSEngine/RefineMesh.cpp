#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include <boost/program_options.hpp>
#include "MVSEngine.h"

using namespace MVS;


// D E F I N E S ///////////////////////////////////////////////////

#define APPNAME _T("RefineMesh")


// S T R U C T S ///////////////////////////////////////////////////

namespace OPT_RefineMesh {
	String strInputFileName;
	String strOutputFileName;
	String strMeshFileName;
	unsigned nResolutionLevel;
	unsigned nMinResolution;
	unsigned nMaxViews;
	float fDecimateMesh;
	unsigned nCloseHoles;
	unsigned nEnsureEdgeSize;
	unsigned nScales;
	float fScaleStep;
	unsigned nReduceMemory;
	unsigned nAlternatePair;
	float fRegularityWeight;
	float fRatioRigidityElasticity;
	unsigned nMaxFaceArea;
	float fPlanarVertexRatio;
	float fGradientStep;
#ifdef _USE_CUDA
	bool bUseCUDA;
#endif
	unsigned nArchiveType;
	int nProcessPriority;
	unsigned nMaxThreads;
	String strExportType;
	String strConfigFileName;
	boost::program_options::variables_map vm;
} // namespace OPT_RefineMesh


bool MVSEngine::Initialize_RefineMesh(size_t argc, LPCTSTR* argv)
{
	// Initialize_Dense log and console
	//CLOSE_LOGFILE();
	//CLOSE_LOGCONSOLE();
	//CLOSE_LOG();
	OPEN_LOG();
	OPEN_LOGCONSOLE();

	boost::program_options::options_description generic("Generic options");
	generic.add_options()
		("help,h", "produce this help message")
		("working-folder,w", boost::program_options::value<std::string>(&WORKING_FOLDER), "working directory (default current directory)")
		("config-file,c", boost::program_options::value<std::string>(&OPT_RefineMesh::strConfigFileName)->default_value(APPNAME _T(".cfg")), "file name containing program options")
		("export-type", boost::program_options::value<std::string>(&OPT_RefineMesh::strExportType)->default_value(_T("ply")), "file type used to export the 3D scene (ply or obj)")
		("archive-type", boost::program_options::value<unsigned>(&OPT_RefineMesh::nArchiveType)->default_value(2), "project archive type: 0-text, 1-binary, 2-compressed binary")
		("process-priority", boost::program_options::value<int>(&OPT_RefineMesh::nProcessPriority)->default_value(-1), "process priority (below normal by default)")
		("max-threads", boost::program_options::value<unsigned>(&OPT_RefineMesh::nMaxThreads)->default_value(0), "maximum number of threads (0 for using all available cores)")
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

	boost::program_options::options_description config("Refine options");
	config.add_options()
		("input-file,i", boost::program_options::value<std::string>(&OPT_RefineMesh::strInputFileName), "input filename containing camera poses and image list")
		("output-file,o", boost::program_options::value<std::string>(&OPT_RefineMesh::strOutputFileName), "output filename for storing the mesh")
		("resolution-level", boost::program_options::value<unsigned>(&OPT_RefineMesh::nResolutionLevel)->default_value(0), "how many times to scale down the images before mesh refinement")
		("min-resolution", boost::program_options::value<unsigned>(&OPT_RefineMesh::nMinResolution)->default_value(640), "do not scale images lower than this resolution")
		("max-views", boost::program_options::value<unsigned>(&OPT_RefineMesh::nMaxViews)->default_value(8), "maximum number of neighbor images used to refine the mesh")
		("decimate", boost::program_options::value<float>(&OPT_RefineMesh::fDecimateMesh)->default_value(0.f), "decimation factor in range [0..1] to be applied to the input surface before refinement (0 - auto, 1 - disabled)")
		("close-holes", boost::program_options::value<unsigned>(&OPT_RefineMesh::nCloseHoles)->default_value(30), "try to close small holes in the input surface (0 - disabled)")
		("ensure-edge-size", boost::program_options::value<unsigned>(&OPT_RefineMesh::nEnsureEdgeSize)->default_value(1), "ensure edge size and improve vertex valence of the input surface (0 - disabled, 1 - auto, 2 - force)")
		("max-face-area", boost::program_options::value<unsigned>(&OPT_RefineMesh::nMaxFaceArea)->default_value(64), "maximum face area projected in any pair of images that is not subdivided (0 - disabled)")
		("scales", boost::program_options::value<unsigned>(&OPT_RefineMesh::nScales)->default_value(3), "how many iterations to run mesh optimization on multi-scale images")
		("scale-step", boost::program_options::value<float>(&OPT_RefineMesh::fScaleStep)->default_value(0.5f), "image scale factor used at each mesh optimization step")
		("reduce-memory", boost::program_options::value<unsigned>(&OPT_RefineMesh::nReduceMemory)->default_value(1), "recompute some data in order to reduce memory requirements")
		("alternate-pair", boost::program_options::value<unsigned>(&OPT_RefineMesh::nAlternatePair)->default_value(0), "refine mesh using an image pair alternatively as reference (0 - both, 1 - alternate, 2 - only left, 3 - only right)")
		("regularity-weight", boost::program_options::value<float>(&OPT_RefineMesh::fRegularityWeight)->default_value(0.2f), "scalar regularity weight to balance between photo-consistency and regularization terms during mesh optimization")
		("rigidity-elasticity-ratio", boost::program_options::value<float>(&OPT_RefineMesh::fRatioRigidityElasticity)->default_value(0.9f), "scalar ratio used to compute the regularity gradient as a combination of rigidity and elasticity")
		("gradient-step", boost::program_options::value<float>(&OPT_RefineMesh::fGradientStep)->default_value(45.05f), "gradient step to be used instead (0 - auto)")
		("planar-vertex-ratio", boost::program_options::value<float>(&OPT_RefineMesh::fPlanarVertexRatio)->default_value(0.f), "threshold used to remove vertices on planar patches (0 - disabled)")
#ifdef _USE_CUDA
		("use-cuda", boost::program_options::value<bool>(&OPT_RefineMesh::bUseCUDA)->default_value(true), "refine mesh using CUDA")
#endif
		;

	boost::program_options::options_description hidden("Hidden options");
	hidden.add_options()
		("mesh-file", boost::program_options::value<std::string>(&OPT_RefineMesh::strMeshFileName), "mesh file name to refine (overwrite the existing mesh)")
		;

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(generic).add(config).add(hidden);

	boost::program_options::options_description config_file_options;
	config_file_options.add(config).add(hidden);

	boost::program_options::positional_options_description p;
	p.add("input-file", -1);

	try {
		// parse command line options
		boost::program_options::store(boost::program_options::command_line_parser((int)argc, argv).options(cmdline_options).positional(p).run(), OPT_RefineMesh::vm);
		boost::program_options::notify(OPT_RefineMesh::vm);
		INIT_WORKING_FOLDER;
		// parse configuration file
		std::ifstream ifs(MAKE_PATH_SAFE(OPT_RefineMesh::strConfigFileName));
		if (ifs) {
			boost::program_options::store(parse_config_file(ifs, config_file_options), OPT_RefineMesh::vm);
			boost::program_options::notify(OPT_RefineMesh::vm);
		}
	}
	catch (const std::exception& e) {
		LOG(e.what());
		return false;
	}


	OPEN_LOGFILE(MAKE_PATH(APPNAME _T("-") + Util::getUniqueName(0) + _T(".log")).c_str());

	//Util::LogBuild();
	//LOG(_T("Command line:%s"), Util::CommandLineToString(argc, argv).c_str());


	Util::ensureValidPath(OPT_RefineMesh::strInputFileName);
	Util::ensureUnifySlash(OPT_RefineMesh::strInputFileName);
	if (OPT_RefineMesh::vm.count("help") || OPT_RefineMesh::strInputFileName.IsEmpty()) {
		boost::program_options::options_description visible("Available options");
		visible.add(generic).add(config);
		GET_LOG() << visible;
	}
	if (OPT_RefineMesh::strInputFileName.IsEmpty())
		return false;
	OPT_RefineMesh::strExportType = OPT_RefineMesh::strExportType.ToLower() == _T("obj") ? _T(".obj") : _T(".ply");

	Util::ensureValidPath(OPT_RefineMesh::strOutputFileName);
	Util::ensureUnifySlash(OPT_RefineMesh::strOutputFileName);
	if (OPT_RefineMesh::strOutputFileName.IsEmpty())
		OPT_RefineMesh::strOutputFileName = Util::getFileFullName(OPT_RefineMesh::strInputFileName) + _T("_refine.J3D");

	Process::setCurrentProcessPriority((Process::Priority)OPT_RefineMesh::nProcessPriority);
#ifdef _USE_OPENMP
	if (OPT_RefineMesh::nMaxThreads != 0)
		omp_set_num_threads(OPT_RefineMesh::nMaxThreads);
#endif

#ifdef _USE_BREAKPAD
	// start memory dumper
	MiniDumper::Create(APPNAME, WORKING_FOLDER);
#endif

	Util::Init();
	return true;
}

// Finalize_RefineMesh application instance
void MVSEngine::Finalize_RefineMesh()
{
#if TD_VERBOSE != TD_VERBOSE_OFF
	// print memory statistics
	Util::LogMemoryInfo();
#endif

}

int MVSEngine::RefineMesh(int num, char* cmd[])
{
#ifdef _DEBUGINFO
	// set _crtBreakAlloc index to stop in <dbgheap.c> at allocation
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// | _CRTDBG_CHECK_ALWAYS_DF);
#endif
	int argc = num;
	LPCTSTR* argv = (LPCTSTR*)cmd;

	if (!Initialize_RefineMesh(argc, argv))
		return EXIT_FAILURE;

	Scene scene(OPT_RefineMesh::nMaxThreads);
	// load and refine the coarse mesh
	if (!scene.Load(MAKE_PATH_SAFE(OPT_RefineMesh::strInputFileName)))
		return EXIT_FAILURE;
	if (!OPT_RefineMesh::strMeshFileName.IsEmpty()) {
		// load given coarse mesh
		scene.mesh.Load(MAKE_PATH_SAFE(OPT_RefineMesh::strMeshFileName));
	}
	if (scene.mesh.IsEmpty()) {
		VERBOSE("error: empty initial mesh");
		return EXIT_FAILURE;
	}
	TD_TIMER_START();
#ifdef _USE_CUDA
	if (!OPT_RefineMesh::bUseCUDA ||
		!scene.RefineMeshCUDA(OPT_RefineMesh::nResolutionLevel, OPT_RefineMesh::nMinResolution, OPT_RefineMesh::nMaxViews,
			OPT_RefineMesh::fDecimateMesh, OPT_RefineMesh::nCloseHoles, OPT_RefineMesh::nEnsureEdgeSize,
			OPT_RefineMesh::nMaxFaceArea,
			OPT_RefineMesh::nScales, OPT_RefineMesh::fScaleStep,
			OPT_RefineMesh::nAlternatePair > 10 ? OPT_RefineMesh::nAlternatePair % 10 : 0,
			OPT_RefineMesh::fRegularityWeight,
			OPT_RefineMesh::fRatioRigidityElasticity,
			OPT_RefineMesh::fGradientStep))
#endif
		if (!scene.RefineMesh(OPT_RefineMesh::nResolutionLevel, OPT_RefineMesh::nMinResolution, OPT_RefineMesh::nMaxViews,
			OPT_RefineMesh::fDecimateMesh, OPT_RefineMesh::nCloseHoles, OPT_RefineMesh::nEnsureEdgeSize,
			OPT_RefineMesh::nMaxFaceArea,
			OPT_RefineMesh::nScales, OPT_RefineMesh::fScaleStep,
			OPT_RefineMesh::nReduceMemory, OPT_RefineMesh::nAlternatePair,
			OPT_RefineMesh::fRegularityWeight,
			OPT_RefineMesh::fRatioRigidityElasticity,
			OPT_RefineMesh::fPlanarVertexRatio,
			OPT_RefineMesh::fGradientStep))
			return EXIT_FAILURE;
	VERBOSE("Mesh refinement completed: %u vertices, %u faces (%s)", scene.mesh.vertices.GetSize(), scene.mesh.faces.GetSize(), TD_TIMER_GET_FMT().c_str());

	// save the final mesh
	const String baseFileName(MAKE_PATH_SAFE(Util::getFileFullName(OPT_RefineMesh::strOutputFileName)));
	scene.Save(baseFileName + _T(".J3D"), (ARCHIVE_TYPE)OPT_RefineMesh::nArchiveType);
	scene.mesh.Save(baseFileName + OPT_RefineMesh::strExportType);
#if TD_VERBOSE != TD_VERBOSE_OFF
	if (VERBOSITY_LEVEL > 2)
		scene.ExportCamerasMLP(baseFileName + _T(".mlp"), baseFileName + OPT_RefineMesh::strExportType);
#endif

	Finalize_RefineMesh();
	return EXIT_SUCCESS;
}
/*----------------------------------------------------------------*/
