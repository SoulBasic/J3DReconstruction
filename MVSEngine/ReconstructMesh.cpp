
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include <boost/program_options.hpp>
#include "MVSEngine.h"

using namespace MVS;



#define APPNAME _T("ReconstructMesh")

#ifdef _USE_OPENMP
#define RECMESH_USE_OPENMP
#endif



namespace OPT_ReconstructMesh {
	String strInputFileName;
	String strOutputFileName;
	String strMeshFileName;
	bool bMeshExport;
	float fDistInsert;
	bool bUseConstantWeight;
	bool bUseFreeSpaceSupport;
	float fThicknessFactor;
	float fQualityFactor;
	float fDecimateMesh;
	float fRemoveSpurious;
	bool bRemoveSpikes;
	unsigned nCloseHoles;
	unsigned nSmoothMesh;
	unsigned nArchiveType;
	int nProcessPriority;
	unsigned nMaxThreads;
	String strExportType;
	String strConfigFileName;
	boost::program_options::variables_map vm;
}

bool MVSEngine::Initialize_ReconstructMesh(size_t argc, LPCTSTR* argv)
{
	// Initialize_Dense log and console
	CLOSE_LOGFILE();
	CLOSE_LOGCONSOLE();
	CLOSE_LOG();
	OPEN_LOG();
	OPEN_LOGCONSOLE();

	boost::program_options::options_description generic("Generic options");
	generic.add_options()
		("help,h", "produce this help message")
		("working-folder,w", boost::program_options::value<std::string>(&WORKING_FOLDER), "working directory (default current directory)")
		("config-file,c", boost::program_options::value<std::string>(&OPT_ReconstructMesh::strConfigFileName)->default_value(APPNAME _T(".cfg")), "file name containing program options")
		("export-type", boost::program_options::value<std::string>(&OPT_ReconstructMesh::strExportType)->default_value(_T("ply")), "file type used to export the 3D scene (ply or obj)")
		("archive-type", boost::program_options::value<unsigned>(&OPT_ReconstructMesh::nArchiveType)->default_value(2), "project archive type: 0-text, 1-binary, 2-compressed binary")
		("process-priority", boost::program_options::value<int>(&OPT_ReconstructMesh::nProcessPriority)->default_value(-1), "process priority (below normal by default)")
		("max-threads", boost::program_options::value<unsigned>(&OPT_ReconstructMesh::nMaxThreads)->default_value(0), "maximum number of threads (0 for using all available cores)")
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

	boost::program_options::options_description config_main("Reconstruct options");
	config_main.add_options()
		("input-file,i", boost::program_options::value<std::string>(&OPT_ReconstructMesh::strInputFileName), "input filename containing camera poses and image list")
		("output-file,o", boost::program_options::value<std::string>(&OPT_ReconstructMesh::strOutputFileName), "output filename for storing the mesh")
		("min-point-distance,d", boost::program_options::value<float>(&OPT_ReconstructMesh::fDistInsert)->default_value(2.5f), "minimum distance in pixels between the projection of two 3D points to consider them different while triangulating (0 - disabled)")
		("constant-weight", boost::program_options::value<bool>(&OPT_ReconstructMesh::bUseConstantWeight)->default_value(true), "considers all view weights 1 instead of the available weight")
		("free-space-support,f", boost::program_options::value<bool>(&OPT_ReconstructMesh::bUseFreeSpaceSupport)->default_value(false), "exploits the free-space support in order to reconstruct weakly-represented surfaces")
		("thickness-factor", boost::program_options::value<float>(&OPT_ReconstructMesh::fThicknessFactor)->default_value(1.f), "multiplier adjusting the minimum thickness considered during visibility weighting")
		("quality-factor", boost::program_options::value<float>(&OPT_ReconstructMesh::fQualityFactor)->default_value(1.f), "multiplier adjusting the quality weight considered during graph-cut")
		;
	boost::program_options::options_description config_clean("Clean options");
	config_clean.add_options()
		("decimate", boost::program_options::value<float>(&OPT_ReconstructMesh::fDecimateMesh)->default_value(1.f), "decimation factor in range (0..1] to be applied to the reconstructed surface (1 - disabled)")
		("remove-spurious", boost::program_options::value<float>(&OPT_ReconstructMesh::fRemoveSpurious)->default_value(20.f), "spurious factor for removing faces with too long edges or isolated components (0 - disabled)")
		("remove-spikes", boost::program_options::value<bool>(&OPT_ReconstructMesh::bRemoveSpikes)->default_value(true), "flag controlling the removal of spike faces")
		("close-holes", boost::program_options::value<unsigned>(&OPT_ReconstructMesh::nCloseHoles)->default_value(30), "try to close small holes in the reconstructed surface (0 - disabled)")
		("smooth", boost::program_options::value<unsigned>(&OPT_ReconstructMesh::nSmoothMesh)->default_value(2), "number of iterations to smooth the reconstructed surface (0 - disabled)")
		;

	boost::program_options::options_description hidden("Hidden options");
	hidden.add_options()
		("mesh-file", boost::program_options::value<std::string>(&OPT_ReconstructMesh::strMeshFileName), "mesh file name to clean (skips the reconstruction step)")
		("mesh-export", boost::program_options::value<bool>(&OPT_ReconstructMesh::bMeshExport)->default_value(false), "just export the mesh contained in loaded project")
		;

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(generic).add(config_main).add(config_clean).add(hidden);

	boost::program_options::options_description config_file_options;
	config_file_options.add(config_main).add(config_clean).add(hidden);

	boost::program_options::positional_options_description p;
	p.add("input-file", -1);

	try {

		boost::program_options::store(boost::program_options::command_line_parser((int)argc, argv).options(cmdline_options).positional(p).run(), OPT_ReconstructMesh::vm);
		boost::program_options::notify(OPT_ReconstructMesh::vm);
		INIT_WORKING_FOLDER;

		std::ifstream ifs(MAKE_PATH_SAFE(OPT_ReconstructMesh::strConfigFileName));
		if (ifs) {
			boost::program_options::store(parse_config_file(ifs, config_file_options), OPT_ReconstructMesh::vm);
			boost::program_options::notify(OPT_ReconstructMesh::vm);
		}
	}
	catch (const std::exception& e) {
		LOG(e.what());
		return false;
	}

	// Initialize_ReconstructMesh the log file
	OPEN_LOGFILE(MAKE_PATH(APPNAME _T("-") + Util::getUniqueName(0) + _T(".log")).c_str());

	// print application details: version and command line
	//Util::LogBuild();
	//LOG(_T("Command line:%s"), Util::CommandLineToString(argc, argv).c_str());

	// validate input
	Util::ensureValidPath(OPT_ReconstructMesh::strInputFileName);
	Util::ensureUnifySlash(OPT_ReconstructMesh::strInputFileName);
	if (OPT_ReconstructMesh::vm.count("help") || OPT_ReconstructMesh::strInputFileName.IsEmpty()) {
		boost::program_options::options_description visible("Available options");
		visible.add(generic).add(config_main).add(config_clean);
		GET_LOG() << visible;
	}
	if (OPT_ReconstructMesh::strInputFileName.IsEmpty())
		return false;
	OPT_ReconstructMesh::strExportType = OPT_ReconstructMesh::strExportType.ToLower() == _T("obj") ? _T(".obj") : _T(".ply");

	// Initialize_ReconstructMesh optional options
	Util::ensureValidPath(OPT_ReconstructMesh::strOutputFileName);
	Util::ensureUnifySlash(OPT_ReconstructMesh::strOutputFileName);
	if (OPT_ReconstructMesh::strOutputFileName.IsEmpty())
		OPT_ReconstructMesh::strOutputFileName = Util::getFileFullName(OPT_ReconstructMesh::strInputFileName) + _T("_mesh.J3D");

	// Initialize_ReconstructMesh global options
	Process::setCurrentProcessPriority((Process::Priority)OPT_ReconstructMesh::nProcessPriority);
#ifdef _USE_OPENMP
	if (OPT_ReconstructMesh::nMaxThreads != 0)
		omp_set_num_threads(OPT_ReconstructMesh::nMaxThreads);
#endif

#ifdef _USE_BREAKPAD
	// start memory dumper
	MiniDumper::Create(APPNAME, WORKING_FOLDER);
#endif

	Util::Init();
	return true;
}

void MVSEngine::Finalize_ReconstructMesh()
{
#if TD_VERBOSE != TD_VERBOSE_OFF
	// print memory statistics
	Util::LogMemoryInfo();
#endif

}


int MVSEngine::ReconstructMesh(int num, char * cmd[])
{
#ifdef _DEBUGINFO
	// set _crtBreakAlloc index to stop in <dbgheap.c> at allocation
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// | _CRTDBG_CHECK_ALWAYS_DF);
#endif
	int argc = num;
	LPCTSTR* argv = (LPCTSTR*)cmd;

	if (!Initialize_ReconstructMesh(argc, argv))
		return EXIT_FAILURE;

	Scene scene(OPT_ReconstructMesh::nMaxThreads);
	// load project
	if (!scene.Load(MAKE_PATH_SAFE(OPT_ReconstructMesh::strInputFileName)))
		return EXIT_FAILURE;

	if (OPT_ReconstructMesh::bMeshExport)
	{
		// check there is a mesh to export
		if (scene.mesh.IsEmpty())
			return EXIT_FAILURE;
		// save mesh
		const String fileName(MAKE_PATH_SAFE(OPT_ReconstructMesh::strOutputFileName));
		scene.mesh.Save(fileName);
#if TD_VERBOSE != TD_VERBOSE_OFF
		if (VERBOSITY_LEVEL > 2)
			scene.ExportCamerasMLP(Util::getFileFullName(OPT_ReconstructMesh::strOutputFileName) + _T(".mlp"), fileName);
#endif
	}
	else
	{
		if (OPT_ReconstructMesh::strMeshFileName.IsEmpty()) {
			// reset image resolution to the original size and
			// make sure the image neighbors are Initialize_ReconstructMeshd before deleting the point-cloud
#ifdef RECMESH_USE_OPENMP
			bool bAbort(false);
#pragma omp parallel for
			for (int_t idx = 0; idx < (int_t)scene.images.GetSize(); ++idx) {
#pragma omp flush (bAbort)
				if (bAbort)
					continue;
				const uint32_t idxImage((uint32_t)idx);
#else
			FOREACH(idxImage, scene.images) {
#endif
				Image& imageData = scene.images[idxImage];
				if (!imageData.IsValid())
					continue;
				// reset image resolution
				if (!imageData.ReloadImage(0, false)) {
#ifdef RECMESH_USE_OPENMP
					bAbort = true;
#pragma omp flush (bAbort)
					continue;
#else
					return EXIT_FAILURE;
#endif
				}
				imageData.UpdateCamera(scene.platforms);
				// select neighbor views
				if (imageData.neighbors.IsEmpty()) {
					IndexArr points;
					scene.SelectNeighborViews(idxImage, points);
				}
			}
#ifdef RECMESH_USE_OPENMP
			if (bAbort)
				return EXIT_FAILURE;
#endif
			// reconstruct a coarse mesh from the given point-cloud
			TD_TIMER_START();
			if (OPT_ReconstructMesh::bUseConstantWeight)
				scene.pointcloud.pointWeights.Release();
			if (!scene.ReconstructMesh(OPT_ReconstructMesh::fDistInsert, OPT_ReconstructMesh::bUseFreeSpaceSupport, 4, OPT_ReconstructMesh::fThicknessFactor, OPT_ReconstructMesh::fQualityFactor))
				return EXIT_FAILURE;
			VERBOSE("三角网重建完成: %u vertices, %u faces (%s)", scene.mesh.vertices.GetSize(), scene.mesh.faces.GetSize(), TD_TIMER_GET_FMT().c_str());
#if TD_VERBOSE != TD_VERBOSE_OFF
			if (VERBOSITY_LEVEL > 2) {
				// dump raw mesh
				scene.mesh.Save(MAKE_PATH_SAFE(Util::getFileFullName(OPT_ReconstructMesh::strOutputFileName)) + _T("_raw") + OPT_ReconstructMesh::strExportType);
			}
#endif
			}
		else {
			// load existing mesh to clean
			scene.mesh.Load(MAKE_PATH_SAFE(OPT_ReconstructMesh::strMeshFileName));
		}

		// clean the mesh
		scene.mesh.Clean(OPT_ReconstructMesh::fDecimateMesh, OPT_ReconstructMesh::fRemoveSpurious, OPT_ReconstructMesh::bRemoveSpikes, OPT_ReconstructMesh::nCloseHoles, OPT_ReconstructMesh::nSmoothMesh, false);
		scene.mesh.Clean(1.f, 0.f, OPT_ReconstructMesh::bRemoveSpikes, OPT_ReconstructMesh::nCloseHoles, 0, false); // extra cleaning trying to close more holes
		scene.mesh.Clean(1.f, 0.f, false, 0, 0, true); // extra cleaning to remove non-manifold problems created by closing holes

		// save the final mesh
		const String baseFileName(MAKE_PATH_SAFE(Util::getFileFullName(OPT_ReconstructMesh::strOutputFileName)));
		scene.Save(baseFileName + _T(".J3D"), (ARCHIVE_TYPE)OPT_ReconstructMesh::nArchiveType);
		scene.mesh.Save(baseFileName + OPT_ReconstructMesh::strExportType);
#if TD_VERBOSE != TD_VERBOSE_OFF
		if (VERBOSITY_LEVEL > 2)
			scene.ExportCamerasMLP(baseFileName + _T(".mlp"), baseFileName + OPT_ReconstructMesh::strExportType);
#endif
		}

	Finalize_ReconstructMesh();
	return EXIT_SUCCESS;
	}
/*----------------------------------------------------------------*/
