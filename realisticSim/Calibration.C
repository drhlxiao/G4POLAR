///
///Author  hualin.xiao@psi.ch
//Dec. 19, 2015
#ifndef _calibration__h
#define _calibration_h 


class TObject;
class TH2F;
class TH1F;

struct EnergyResolution
{
    float energy;
    float resolution;
}

class CalibrationData:TObject
{
    public:

        CalibrationData();
        ~CalibrationData();
    public:
        float crosstalk_matrix[25][64][64];
        float response_matrix[25][64][64];
        float energy_conversion[25][64];
        float threshold_adc[25][64];
        float thresohld_sigma[25][64];
        std::vector<struct EnergyResolution> energy_resolution;

        
        float GetResponseMatrix(int module, int bar, int bar2);


}

