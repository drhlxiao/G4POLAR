# Geant4 simulation package developed for POLAR
README for g4POLAR
------------------------------------


##1.Features:

(1) The package use gdml, the mass model was mainly developed by SiweiKong
(2) Use General Particle Source
(3) Merlin's optical simulation results have been adopted.
(4) Energy resolution model: sigma E=sqrt(a+b*E+c*E^2)
(5) Birks quenching effect taken into account
(6) Support input particle information in a root file: see Generator/*.C
(7) Simulation output see src/AnalysisManager.cc and src/t2polar.C
(8) Outputs contain modulation curves
(9) Visualization using Qt 
(10) support geant4.10


## 2. Dependencies:
  Geant4.10, ROOT, cmake, make,g++, Linux

## 3. How to make the program 
  (1) Set geant4.10 environment
   Example:  source <G4INSTALL_DIRECTORY>/bin/geant4.sh
  (2) cmake CMakeLists.txt
  (3) make

## 4. How to Run:

Usage:
./g4POLAR [OPTIONS] -m run.mac  -o OUTPUT
Options:
 -i                  <input.root> 
                     Read incident particle information from input.root.
                     The structure of the root file is defined in t2sim.h .
 -h                  print help information
Examples: 
./g4POLAR -m test.mac -o test.root
see test.mac
./g4POLAR -m vis.mac -o testvis.root
./g4POLAR  

## 5. Output root file structure
	see: src/t2polar.C
  ```c++
		TTree *tree = new TTree("polar","polar simulation data");	
		tree->Branch("pattern",pattern,Form("pattern[%d]/I",1600));      ///over threshold from VA
		tree->Branch("pm",pm,Form("pm[%d]/F",1600));                     ///primary energy deposition
		tree->Branch("pm1",pm1,Form("pm1[%d]/F",1600));                  ///energy deposition after quenching
		tree->Branch("pm2",pm2,Form("pm2[%d]/F",1600));                  ///collected energy deposition, Merlin's simulation results will be used
		tree->Branch("adc",adc,Form("adc[%d]/F",1600));                  ///energy deposition after considering energy resolution smearing
		tree->Branch("evnt",&evnt,"evnt/L");                             ///event number
		tree->Branch("n",&n,"n/I");                                      ///number of hit bars per photon (bars above software threshold)
		tree->Branch("time",&time,"time/D");                             /// event time
		tree->Branch("xideg",&xideg,"xideg/D");                          /// quick look azimuthal angle
        	tree->Branch("particle_position",particle_position,"particle_position[3]/D");    ///primary particle position 
		tree->Branch("particle_direction",particle_direction,"particle_direction[3]/D");  ///primary particle direction 
		tree->Branch("particle_pol",particle_pol,"particle_pol[3]/D");    ///primary particle polarization
		tree->Branch("particle_energy",&particle_energy,"particle_energy/D"); ///primary particle energy 
```

## 6. todo list
(1) to use real data to optimize parameters of the energy resolution model 
(2) to add AnalysisManagerMessger.C to control the output with macros:

 



## History

*****
Dec 16 2015:
(1)PrimaryGeneratorAction:
add a new particle source: na22,
(2)g4POLAR.cc
one can not specify input root file in command line any more.
One should define it in the mac file 
see macros/na22Source.mac

(3)Add PrimaryGeneratorMessenger
(4) Add DetResponse
 
move detector readout sim from analysisManager to this class
(5)Implementation of crosstalk simulation in DetResponse

   

*******************************************************
Dec 18 2015

Implementation of realistic simulation

(1) Added a new class to manage realistic simulation: DetRespose;
(2) Thresholds:
*use the data taken from ESRF2015 test. threshold data is in polar4/ESRF2015/threshold_MC
*data source is in polar4/ESRF2015/L2/
*thresholds are in units of adc channel
*half maximum as the threshold value.
*Error of position:  the position having maximum adc channel - the position at half maximum

*script to fit threshold L2/dothresholdFit.C
(3) Response matrix
*use the data taken from ESRF2015 test. response data is in polar4/ESRF2015/responseMatrix_MC
(4) Response matrix and threshold histograms are merged into one file g4POLAR/realisticSim/response.root

(5) simulation:
*ADC channel
   **energy deposition-->quenching simulation->collection efficiency simulation -->energy smearing -->response matrix
* trigger
 **thresholds are randomized 
 **use threshold information to generate tout1,tout2, tmth, accepted

 (6) validation:
 *macros: g4POLAR/realisticSim/realisticSim.mac, output: g4POLAR/validation/

Feb. 10 2016
Updates:
Use energy resolutions obtained by Xiaofeng, from the calibration with sources
Energy resolution function:

R^2=a+b/E
