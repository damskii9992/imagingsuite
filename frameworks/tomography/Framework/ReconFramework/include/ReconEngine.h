//<LICENSE>

#ifndef RECONENGINE_H
#define RECONENGINE_H
#include "ReconFramework_global.h"

#include <list>
#include <string>

#include <interactors/interactionbase.h>
#include <logging/logger.h>
#include <base/kiplenums.h>

#include "PreprocModuleBase.h"
#include "BackProjectorModuleBase.h"
#include "ProjectionReader.h"
#include "ReconHelpers.h"
#include "ModuleItem.h"

class RECONFRAMEWORKSHARED_EXPORT ProjectionBlock
{
public:
    ProjectionBlock();
    ProjectionBlock(kipl::base::TImage<float,3> & projections,
                    const std::vector<size_t> &roi,
                    std::map<std::string,
                    std::string> parameters);

    ProjectionBlock(const ProjectionBlock &b);
    ProjectionBlock & operator=(const ProjectionBlock &b);
    ~ProjectionBlock();

    kipl::base::TImage<float,3> projections;
    std::vector<size_t> roi;
    std::map<std::string, std::string> parameters;
};


/// \brief A processing engine that performs the reconstruction task using a set of preprocessing modules and a back projector module
///
/// The engine is usually configured using the reconstructor factory that takes a config struct as input.
class RECONFRAMEWORKSHARED_EXPORT ReconEngine
{
protected:
    kipl::logging::Logger logger; ///< The logger instance for messages generated by the engine

public:
    /// \brief Initializes an empty reconstrution engine
    /// \param name The instance name of the engin, will be assigned to the logger.
    /// \param interactor reference to an interactor object. Here, a base class is used since the derived interactor is unknown at this point.
    ReconEngine(std::string name="ReconEngine",
                kipl::interactors::InteractionBase *interactor=nullptr);

    /// \brief Adds a preprocessing module during the configuration phase
    /// \param module A reference to a preprocessing module. The instance will be destroyed when the engine is destroyed.
    /// \returns The number of preprocessing modules in the list
	size_t AddPreProcModule(ModuleItem *module);

    /// \brief Sets the back projector module
    /// \param module A reference to a backprojector module
	void SetBackProjector(BackProjItem *module);

    /// \brief Starts a reconstruction process based on single projections.
    ///
    /// This function is rarely used and only kept for historical reasons
    int Run();

    /// \brief Starts a reconstruction process based on blocks of projections.
    /// \param bRerunBackproj This switch selects if the entire reconstruction process it to be performed (false) or if it is sufficient to run the backprojector only using stored projections.
    int Run3D(bool bRerunBackproj=false);

    /// \brief Starts the preprocessing chain including loading the projection data. This function is called by the user interface to provide data to the configuration dialogs.
    /// \param roi The region of interest to process
    /// \param sLastModule The chain shall only process until this module is reached
    kipl::base::TImage<float,3> RunPreproc(const std::vector<size_t> &roi, std::string sLastModule);

    /// \brief Set reconstruction configuration as preparation of geometry and number of files
    /// \param config The configuration data
	void SetConfig(ReconConfig & config) ;

    /// \brief Writes the reconstructed image to the location specified by the matrix config
    /// \param matrixconfig Configuration struct for the matrix
	bool Serialize(ReconConfig::cMatrix *matrixconfig);

    /// \brief Writes the reconstructed image to disk. If the filename contains any # a sequence of slices will be written, otherwise the data will be written as a single matlab mat file (outdated format).
    /// \param dims The stored image dimensions will be copied to this argument if it is non-nullptr.
    bool Serialize(std::vector<size_t> &dims);

	size_t GetHistogram(float *axis, size_t *hist,size_t nBins);
    const std::vector<size_t> & GetMatrixDims() {return m_Volume.dims();}
    kipl::base::TImage<float,2> GetSlice(size_t index, kipl::base::eImagePlanes plane=kipl::base::ImagePlaneXY);
    std::string citations();
    std::vector<Publication> publicationList();
    void writePublicationList(const std::string &fname = "");

	virtual ~ReconEngine(void);
protected:
    void ConfigSanityCheck(ReconConfig &config);
    int Run3DFull();
    int Run3DBackProjOnly();
    int Process(const std::vector<size_t> &roi);
    int Process3D(const std::vector<size_t> &roi);
    int ProcessExistingProjections3D(const std::vector<size_t> &roi);
    int BackProject3D(kipl::base::TImage<float,3> & projections, const std::vector<size_t> &roi, std::map<std::string, std::string> parameters);
	bool UpdateProgress(float val, std::string msg);
    float CurrentOverallProgress();
    size_t validateImage(float *data, size_t N, const string &description);
	void Done();
    void resetTimers();

    bool TransferMatrix(const std::vector<size_t> &dims);

    void MakeExtendedROI(size_t *roi, size_t margin, size_t *extroi, size_t *margins);
    void UnpadProjections(kipl::base::TImage<float,3> &projections, size_t *roi, size_t *margins);
	ReconConfig m_Config;
    std::vector<Publication> publications;

	size_t m_FirstSlice;
    size_t m_ProjectionMargin;
	ProjectionReader m_ProjectionReader;             //!< Instance of the projection reader
	
    std::vector<ModuleItem *> m_PreprocList;
	BackProjItem * m_BackProjector;

	kipl::base::TImage<float,3> m_Volume;
	std::map<float,ProjectionInfo> m_ProjectionList;
	std::map<std::string, float> m_PreprocCoefficients;

    std::list<ProjectionBlock> m_ProjectionBlocks;

	size_t nProcessedBlocks;						//!< Counts the number of processed blocks for the progress monitor
	size_t nProcessedProjections;					//!< Counts the number of processed projections for the progress monitor
	size_t nTotalProcessedProjections;				//!< Counts the total number of processed projections for the progress monitor
	size_t nTotalBlocks;							//!< The total number of blocks to process
	bool m_bCancel;									//!< Cancel flag if true the reconstruction process will terminate
    std::vector<size_t> CBroi;                                //!< Additional ROI to be used for the cone beam case
	//eReconstructorStatus status;
    kipl::interactors::InteractionBase *m_Interactor;
};

#endif
