# -*- coding: utf-8 -*-
import sys
#print __file__
home_dir="/media/psf/Home"
#home_dir="/Users/laurent"
sys.path.append("/opt/salome/GDML_GEOMPY/python_modules")

import gdml
from gdml.GDMLContentHandler import MYGDMLContentHandler





gdml_handler= MYGDMLContentHandler()

#file_name="%s/NGRM/geo_models/GDML/Rossini_HAB_Innovative_test.gdml"  %(home_dir)
file_name="/opt/geant4/g4work/geant4POLAR/gdml_polar/Module/logicESRTop.gdml"
gdml_handler.read_gdml_file(file_name)

gdml_handler.declare_in_geompy()


