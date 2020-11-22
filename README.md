## Geant4 simulation package developed for POLAR
README for g4POLAR
------------------------------------


### 1.Features:
- Geometry defined in gdml
- Particle generation uses General Particle Source
- Optical photon simulation enabled by default
- Energy resolution model: sigma E=sqrt(a+b*E+c*E^2)
- Birks quenching effect  simulated
- Support input particle information in a root file: see Generator/*.C
- Simulation output see src/AnalysisManager.cc and src/t2polar.C
- Modulation curves generated directly
- Visualization using Qt 


### 2. Dependencies:
  Geant4.10, ROOT, cmake, make,g++, Linux

### 3. How to make the program 
 - (1) Set geant4.10 environment
   Example:  source <G4INSTALL_DIRECTORY>/bin/geant4.sh
 -  (2) cmake CMakeLists.txt
 - (3) make

### 4. How to Run:

Usage:
```sh
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
```
### 5. Output ROOT file structure

  ```cpp
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

### 6. TO list
(1) Uses real data to optimize parameters of the energy resolution model 
(2) Adds AnalysisManagerMessger.C to control the output with macros

 



### Change Log
see README.txt
