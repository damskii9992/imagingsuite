//<LICENSE>
#ifndef IMAGENORMALIZATION_H_
#define IMAGENORMALIZATION_H_

#include "ScatteringCorrection_global.h"

#include <map>
#include <vector>
#include <string>

#include <base/timage.h>
#include <logging/logger.h>

class SCATTERINGCORRECTIONSHARED_EXPORT ImageNormalization
{
    kipl::logging::Logger logger;

    public:
        enum eNormalizationMethod 
        {
            basicNormalization,
            scatterNormalization,
            interpolateScatterNormalization,
            one2oneScatterNormalization
        };

        ImageNormalization();

        void setDarkCurrent(const kipl::base::TImage<float,2> &img);
        // void setDarkCurrent(const kipl::base::TImage<float,3> &img);

        void setOpenBeam(const kipl::base::TImage<float,2> &img, float dose, bool subtractDC=true);
        // void setOpenBeam(const kipl::base::TImage<float,3> &img, const std::vector<float> &dose);

        void setBBOpenBeam(const kipl::base::TImage<float,2> &img, float dose);
        
        void setBBSample(  const kipl::base::TImage<float,2> &img, float dose);
        // void setBBSample(  const kipl::base::TImage<float,3> &img, const std::vector<float> &dose);

        void useLogarithm(bool useLog);

        kipl::base::TImage<float,2> process(kipl::base::TImage<float,2> &img, 
                                                  float                        dose, 
                                                  eNormalizationMethod         normMethod=basicNormalization,
                                                  float                        tau=0.0f);

        void process(   kipl::base::TImage<float,3> &img, 
                        const std::vector<float>          &dose, 
                        eNormalizationMethod               normMethod=basicNormalization,
                        float                              tau=0.0f);

    private: 
        bool useLog;
        kipl::base::TImage<float,2> dc;
        kipl::base::TImage<float,2> ob;
        kipl::base::TImage<float,2> bbob;
        kipl::base::TImage<float,2> bbs;
        float obDose;
        float bbobDose;
        float bbsDose;

        void checkImageDims(kipl::base::TImage<float,2> &imgA, kipl::base::TImage<float,2> &imgB);
        void applyLogarithm(kipl::base::TImage<float,2> &img);

        void processBasicNormalization(kipl::base::TImage<float,2> &img, float dose);
        void processScatterNormalization(kipl::base::TImage<float,2> &img, float dose, float tau);


};

#endif