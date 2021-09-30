//<LICENSE>

#ifndef REFERENCEIMAGECORRECTION_H
#define REFERENCEIMAGECORRECTION_H
#include "ImagingAlgorithms_global.h"

#include <map>
#include <string>

#include <base/timage.h>
#include <logging/logger.h>
#include "../include/averageimage.h"
#if !defined(NO_QT)
#include <QTextStream>
#endif
#include <interactors/interactionbase.h>


namespace ImagingAlgorithms {

class IMAGINGALGORITHMSSHARED_EXPORT ReferenceImageCorrection {
protected:
	kipl::logging::Logger logger;
public:
    enum eReferenceMethod {
            ReferenceLogNorm,
            ReferenceNorm
    }; /// Options for Referencing method. not yet used

    enum eBBOptions {
      noBB,
      Interpolate,
      Average,
      OneToOne,
      ExternalBB
    }; /// Options for BB image handling

    enum eInterpOrder{
        SecondOrder,
        FirstOrder,
        ZeroOrder
    }; /// Options for BB mask interpolation

    enum eInterpMethod{
        Polynomial,
        ThinPlateSplines
    }; /// Options for background interpolation

    enum eMaskCreationMethod
    {
        otsuMask,
        manuallyThresholdedMask,
        userDefinedMask,
        referenceFreeMask
    };

    ReferenceImageCorrection(kipl::interactors::InteractionBase *interactor = nullptr);
    ~ReferenceImageCorrection();

//    void LoadReferenceImages(std::string path, std::string obname, size_t firstob, size_t obcnt,
//            std::string dcname, size_t firstdc, size_t dccnt,
//            std::string bbname, size_t firstbb, size_t bbcnt,
//            size_t *roi,
//            size_t *doseroi);

    void SetReferenceImages(kipl::base::TImage<float,2> *ob,
            kipl::base::TImage<float,2> *dc,
            bool  useBB,
            bool  useExtBB,
            bool  useSingleExtBB,
            float dose_OB,
            float dose_DC,
            bool  normBB,
            const std::vector<size_t> &roi,
            const std::vector<size_t> &doseroi);

    /// set interpolation parameters to be used for BB image computation
    void SetInterpParameters(float *ob_parameter,
                             float *sample_parameter,
                             size_t nBBSampleCount,
                             size_t nProj,
                             eBBOptions ebo);

    /// set interpolation parameters to be used for BB image computation in the case of thin plate splines
    void SetSplinesParameters(float *ob_parameter,
                              float *sample_parameter,
                              size_t nBBSampleCount,
                              size_t nProj,
                              eBBOptions ebo,
                              int nBBs);

    /// set roi to be used for computing interpolated values,
    /// it is a roi relative to the projection_roi,
    /// because the interpolation parameters are computed with respect to the projection_roi
    void SetBBInterpRoi(const std::vector<size_t> &roi);

    /// set dose roi to be used for computing interpolated values.
    /// it is the dose roi relative the projection_roi,
    /// because the interpolation parameters are computed with respect to the projection_roi
    void SetBBInterpDoseRoi(const std::vector<size_t> &roi);

    /// set the radius used to define subset of segmented BBs
    void SetRadius(size_t x) {radius=x;}

    /// set value of tau
    void SetTau (float x) {tau=x;}

    /// set diffroi, which is the difference between BBroi and the Projection roi -- it should be now only the BBroi
    void setDiffRoi (const std::vector<int> &roi) { m_diffBBroi = roi;}

    /// set bool value for computation of pierre's variante.
    /// At the moment it is hidden from the Gui and it is intended to be set as dafault true
    void SetPBvariante (bool x) {bPBvariante=x; }

    /// set min area for BB segmentation
    void SetMinArea (size_t x) {min_area=x;}

    void SetInterpolationOrderX(eInterpOrder eim_x);
    void SetInterpolationOrderY(eInterpOrder eim_y);

    /// set map to be used for spline interpolation of ob images
    void SetSplineObValues(std::map<std::pair<int, int>, float> &values) {spline_ob_values = values;}

    /// set map to used for spline interpolation of sample images
    void SetSplineSampleValues(std::map<std::pair<int, int>, float> &values) {spline_sample_values = values; }

    /// set method for background interpolation, polynomial or splines
    void SetInterpolationMethod(eInterpMethod eint_meth) {m_InterpMethod = eint_meth;}

    void SetManualThreshold(bool bThresh, float value) {{bUseManualThresh=bThresh; thresh=value;}}

    /// set angles and number of proj and images with BB, to be used for more general interpolation
    void SetAngles(float *ang, size_t nProj, size_t nBB);

    /// set dose list for sample images in the BB dose roi,
    /// it has to be called after SetAngles for the right definition of m_nProj
    void SetDoseList(const std::vector<float> &doselist);

    /// Process a single 2D image
    kipl::base::TImage<float,2>  Process(kipl::base::TImage<float,2> &img, float dose);

    /// Process a stack of 2D images packed in a 3D image
    void Process(kipl::base::TImage<float,3> &img, float *dose);

    /// segments normalized image (bb-dark)/(flat-dark) to create mask and then call ComputeInterpolationParameter
    float* PrepareBlackBodyImage(kipl::base::TImage<float,2> &flat,
                                 kipl::base::TImage<float,2> &dark,
                                 kipl::base::TImage<float,2> &bb,
                                 kipl::base::TImage<float,2> &mask);

    /// segments normalized image (bb-dark)/(flat-dark) to create mask and then call ComputeInterpolationParameter, finally computes interpolation error
    float* PrepareBlackBodyImage(kipl::base::TImage<float,2> &flat,
                                 kipl::base::TImage<float,2> &dark,
                                 kipl::base::TImage<float,2> &bb,
                                 kipl::base::TImage<float,2> &mask,
                                 float &error);

    /// segments normalized image to create mask and then call interpolation with splines
    float* PrepareBlackBodyImagewithSplines(kipl::base::TImage<float,2> &flat,
                                            kipl::base::TImage<float,2> &dark,
                                            kipl::base::TImage<float,2> &bb,
                                            kipl::base::TImage<float,2> &mask,
                                            std::map<std::pair<int, int>, float> &values);

    /// uses a predefined mask and then call the thin plates spline interpolation
    float* PrepareBlackBodyImagewithSplinesAndMask(kipl::base::TImage<float,2> &dark,
                                                   kipl::base::TImage<float,2> &bb,
                                                   kipl::base::TImage<float,2>&mask,
                                                   std::map<std::pair<int, int>, float> &values);

    /// uses a predefined mask and then call ComputeInterpolationParameter
    float* PrepareBlackBodyImagewithMask(kipl::base::TImage<float,2> &dark,
                                         kipl::base::TImage<float,2> &bb,
                                         kipl::base::TImage<float,2>&mask);

    /// compute interpolation parameters from img and mask
    float* ComputeInterpolationParameters(kipl::base::TImage<float,2>&mask,
                                          kipl::base::TImage<float,2>&img);

    /// compute interpolation parameters from img and mask and give as output interpolation error
    float* ComputeInterpolationParameters(kipl::base::TImage<float,2>&mask,
                                          kipl::base::TImage<float,2>&img,
                                          float &error);

    /// compute interpolated image from polynomial parameters
    kipl::base::TImage<float,2>  InterpolateBlackBodyImage(float *parameters,
                                                           const std::vector<size_t> &roi);

    /// compute interpolated image from splines parameters
    kipl::base::TImage<float,2> InterpolateBlackBodyImagewithSplines(float *parameters,
                                                                     std::map<std::pair<int,int>,float> &values,
                                                                     const std::vector<size_t> &roi);

    /// compute interpolation error from interpolated image, original image and mask that highlights the pixels to be considered
    float ComputeInterpolationError(kipl::base::TImage<float,2>&interpolated_img,
                                    kipl::base::TImage<float,2>&mask,
                                    kipl::base::TImage<float, 2> &img);

    /// set the BB externally computed images and corresponding doses
    void SetExternalBBimages(kipl::base::TImage<float, 2> &bb_ext,
                             kipl::base::TImage<float, 3> &bb_sample_ext,
                             float &dose,
                             const std::vector<float> &doselist);

    /// set the BB externally computed images and corresponding doses, case of single file also for the sample background
    void SetExternalBBimages(kipl::base::TImage<float, 2> &bb_ext,
                             kipl::base::TImage<float, 2> &bb_sample_ext,
                             float &dose,
                             float &dose_s);

    void SetMaskCreationMethod(ReferenceImageCorrection::eMaskCreationMethod eMaskMethod, kipl::base::TImage<float, 2> &mask);

    void setNormMethod(ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod value) {m_ReferenceMethod = value;}

    void SaveBG(bool value, const std::string &path, const std::string &obname, const std::string &filemask);

protected:
    void PrepareReferences(); /// old version with references image preparation, without BB
    void PrepareReferencesBB(); /// prepare reference images in case of BB
    void PrepareReferencesExtBB(); /// prepare reference images in case of externally created BB

    float *SolveLinearEquation(std::map<std::pair<int, int>, float> &values, float &error);
    float *SolveThinPlateSplines(std::map<std::pair<int,int>,float> &values);

    void SegmentBlackBody(kipl::base::TImage<float,2> &img, kipl::base::TImage<float,2> &mask); /// apply Otsu segmentation to img and create mask, it also performs some image cleaning
    void SegmentBlackBody(kipl::base::TImage<float,2> &normimg, kipl::base::TImage<float,2> &img, kipl::base::TImage<float,2> &mask, std::map<std::pair<int, int>, float> &values); /// apply Otsu segmentation to img and create mask, it also performs some image cleaning and creates a list with position of BB centroids
    void ComputeBlackBodyCentroids(kipl::base::TImage<float,2> &img, kipl::base::TImage<float,2> &mask, std::map<std::pair<int, int>, float> &values); /// uses the same mask to create centroids values list

    float *InterpolateParameters(float *param, size_t n, size_t step); /// Interpolate parameters assuming the BB sample image are acquired uniformally with some step over the n Projection images
    float *InterpolateParametersGeneric(float *param); /// Interpolate parameters for generic configuration of number of BB sample images and projection data, it assumes first SetAngle is called
    float *InterpolateSplineGeneric(float *param, int nBBs); /// Interpolate parameters in the case of thin plates splines
    float *ReplicateParameters(float *param, size_t n); /// Replicate interpolation parameter, to be used for the Average method
    float *ReplicateSplineParameters(float *param, size_t n, int nBBs); /// Replicate interpolation parameters, to be used for the Average method and Spline option

    int ComputeLogNorm(kipl::base::TImage<float,2> &img, float dose);
    void ComputeNormOriginal(kipl::base::TImage<float,2> &img, float dose);
    void ComputeNormRevised(kipl::base::TImage<float,2> &img, float dose);
    int* repeat_matrix(int* source, int count, int expand); /// repeat matrix. not used.
    float computedose(kipl::base::TImage<float,2>&img); /// duplicate.. to move in timage probably or something like this
    void saveIntermediateImage(kipl::base::TImage<float,2> &img, const std::string &fname);

	bool m_bHaveOpenBeam;
	bool m_bHaveDarkCurrent;
	bool m_bHaveBlackBody;
    bool m_bHaveExternalBlackBody;
    eReferenceMethod m_ReferenceMethod;
    bool bUseManualThresh;
    bool bSaveBG;
    bool bSaveIntermediate;

    std::string pathBG;      ///< path for saving BGs
    std::string flatname_BG; ///< filename for saving the open beam BG
    std::string filemask_BG; ///< filemask for saving the computed sample BGs


    kipl::base::TImage<float,2> m_OpenBeam;
    kipl::base::TImage<float,2> m_OpenBeamforBB;
	kipl::base::TImage<float,2> m_DarkCurrent;
	kipl::base::TImage<float,2> m_BlackBody;
    kipl::base::TImage<float,2> m_OB_BB_Interpolated;
    kipl::base::TImage<float,2> m_BB_sample_Interpolated;
    kipl::base::TImage<float,2> m_DoseBBsample_image;
    kipl::base::TImage<float,2> m_DoseBBflat_image;

    kipl::base::TImage<float,2> m_OB_BB_ext; /// externally computed OB image with BBs
    kipl::base::TImage<float,3> m_BB_sample_ext; /// externally computed sample image with BBs
    kipl::base::TImage<float,2> m_BB_slice_ext; /// externally computed slice of sample image with BBs



	float m_fOpenBeamDose;
    float m_fDarkDose;
	bool m_bHaveDoseROI;
    bool m_bHaveBBDoseROI;
	bool m_bHaveBlackBodyROI;
    bool bPBvariante;
    bool bExtSingleFile;                /// boolean value on the use of a single file for sample background correction

    float fdoseOB_ext;                  /// dose value in externally computed background for open beam image
    float fdoseS_ext;                   /// dose value in externally computed background for single sample image
    std::vector<float> fdose_ext_list;  /// dose value list in externally computed sample with BB image in dose roi
    float fdose_ext_slice;              /// dose value of current slice from fdose_ext_list

    float *ob_bb_parameters;            /// interpolation parameters for the OB BB image
    float *sample_bb_parameters;        /// interpolation parameters for the sample image with BB
    float *sample_bb_interp_parameters; /// interpolation parameters for the sample image for each projection angle

    ImagingAlgorithms::AverageImage::eAverageMethod m_AverageMethod; /// method used for image averaging (options: sum, mean, max, weightedmean, median and average)

    eInterpOrder m_IntMeth_x;
    eInterpOrder m_IntMeth_y;

    ImagingAlgorithms::ReferenceImageCorrection::eMaskCreationMethod m_MaskMethod;
    kipl::base::TImage<float,2> m_MaskImage;

    eInterpMethod m_InterpMethod;


    int a,b,c,d,e,f;                     /// weights for interpolation scheme, used to set different combined order

    std::vector<size_t> m_nDoseROI;      /// roi to be used for dose computation on BB images ("big roi" I would say..)
    std::vector<size_t> m_nROI;          /// actual roi onto compute the background images
    std::vector<size_t> m_nBlackBodyROI; /// roi to be used for computing interpolated BB images, roi position is relative to the image projection roi, on which interpolation parameters are computed
    std::vector<int>    m_diffBBroi;     /// difference between BB roi and projection roi, important when computing interpolation parameters
    std::vector<size_t> m_nDoseBBRoi;    /// "big roi" to be used for dose computation on BB images, this is now relative to projection roi coordinates, due to the interpolation scheme
    size_t m_nBBimages;                  /// number of images with BB sample that are available
    size_t m_nProj;                      /// number of images that are needed after interpolation
    std::vector<float> angles;           /// first and last angles of nProj and nBBImages, respectively
    size_t radius;                       /// radius value for BB mask creation
    float tau;                           /// mean pattern transmission
    std::vector<float> dosesamplelist;   /// list of doses for sample images computed at dose BB roi, to be used in the Interpolate BBOption
    size_t min_area;                     /// min area for BB segmentation
    float thresh;                        /// manual threshold

    std::map<std::pair<int, int>, float> spline_ob_values;     /// map to be used for interpolation with splines and ob image. should be in principle the same number of images with BB, i start now with 1
    std::map<std::pair<int, int>, float> spline_sample_values; /// map to be used for interpolation with splines and sample image



    kipl::interactors::InteractionBase *m_Interactor;
    bool updateStatus(float val, std::string msg);
};

}

void         IMAGINGALGORITHMSSHARED_EXPORT   string2enum(std::string str, ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod &erm);
std::string  IMAGINGALGORITHMSSHARED_EXPORT   enum2string(const ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod &erm);
std::ostream IMAGINGALGORITHMSSHARED_EXPORT & operator<<(ostream & s, ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod erm);

void         IMAGINGALGORITHMSSHARED_EXPORT   string2enum(std::string str, ImagingAlgorithms::ReferenceImageCorrection::eBBOptions &ebo);
std::string  IMAGINGALGORITHMSSHARED_EXPORT   enum2string(const ImagingAlgorithms::ReferenceImageCorrection::eBBOptions &ebo);
std::ostream IMAGINGALGORITHMSSHARED_EXPORT & operator<<(ostream & s, ImagingAlgorithms::ReferenceImageCorrection::eBBOptions ebo);

void         IMAGINGALGORITHMSSHARED_EXPORT   string2enum(std::string str, ImagingAlgorithms::ReferenceImageCorrection::eInterpOrder &eim_x);
std::string  IMAGINGALGORITHMSSHARED_EXPORT   enum2string(const ImagingAlgorithms::ReferenceImageCorrection::eInterpOrder &eim_x);
std::ostream IMAGINGALGORITHMSSHARED_EXPORT & operator<<(ostream & s, ImagingAlgorithms::ReferenceImageCorrection::eInterpOrder eim_x);

void         IMAGINGALGORITHMSSHARED_EXPORT   string2enum(std::string str, ImagingAlgorithms::ReferenceImageCorrection::eInterpMethod &eint);
std::string  IMAGINGALGORITHMSSHARED_EXPORT   enum2string(const ImagingAlgorithms::ReferenceImageCorrection::eInterpMethod &eint);
std::ostream IMAGINGALGORITHMSSHARED_EXPORT & operator<<(ostream & s, ImagingAlgorithms::ReferenceImageCorrection::eInterpMethod eint);

void         IMAGINGALGORITHMSSHARED_EXPORT   string2enum(const std::string &str, ImagingAlgorithms::ReferenceImageCorrection::eMaskCreationMethod &emask);
std::string  IMAGINGALGORITHMSSHARED_EXPORT   enum2string(const ImagingAlgorithms::ReferenceImageCorrection::eMaskCreationMethod &emask);
std::ostream IMAGINGALGORITHMSSHARED_EXPORT & operator<<(ostream & s, ImagingAlgorithms::ReferenceImageCorrection::eMaskCreationMethod emask);


#endif /* REFERENCEIMAGECORRECTION_H_ */
