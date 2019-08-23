
#ifndef backgrounds_h
#define backgrounds_h 1

#include <math.h>

class G4ParticleGun;
class G4ParticleTable;

class TF1;


class backgrounds
{
	public: 
		backgrounds();
		~backgrounds();
	public:
		void GetEvent(G4ParticleGun *p);
		void SetDuration(double d){duration=d;}
		void InitParticleGun();
		double GetNumberOfParticles(){return n_phot;}
		static backgrounds* GetInstance();


	private:
		double duration;
		double n_phot;
		TF1 *cxbb99;
		TF1 *eleb;
		TF1 *posb;
		double dt;
		double pos_frac;
		double cxb_frac;
		double ele_frac;

		double emin;
		double emax;
		double SphrRad;
		double CircRad;
		G4ParticleTable  *fPartiTable;

};

#endif
