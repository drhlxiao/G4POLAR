#!/usr/bin/python

import os
import datetime
import  time
from subprocess import Popen
from multiprocessing import Pool

now=datetime.datetime.now()
logfile="sim_%s.log" % now.strftime("%Y%m%d")



NPhi=90

def run(command):
    print(command)
    os.system(command)

SolarFlares=['solar_flare_M1','solar_flare_X1','solar_flare_X10']


cmds=[]
def run_all(SolarFlare):
    Mac='%s.mac'%SolarFlare
    cmd='./g4POLAR -m %s -ppol 0  -pphi  0  -o  %s_Simout_10M_events/phi_0_pol_0.root >>%s 2>&1'%(Mac,SolarFlare,  logfile)
    #cmd='./g4POLAR -m %s -ppol 0.1  -pphi  0  -o  %s_Simout_10M_events/%s_phi_0_pol_0_1.root >>%s 2>&1'%(Mac,SolarFlare, SolarFlare, logfile)
    #cmd='./g4POLAR -m %s -ppol 0.2  -pphi  0  -o  %s_Simout_10M_events/%s_phi_0_pol_0_2.root >>%s 2>&1'%(Mac,SolarFlare, SolarFlare, logfile)
    #cmd='./g4POLAR -m %s -ppol 0.5  -pphi  0  -o  %s_Simout_10M_events/%s_phi_0_pol_0_5.root >>%s 2>&1'%(Mac,SolarFlare, SolarFlare, logfile)
    cmds.append(cmd)
    for nphi in range(0,NPhi):
        phi=nphi*180./NPhi 
        cmd='./g4POLAR -m %s -ppol 1  -pphi  %f  -o  %s_Simout_10M_events/phi_%d_pol_1.root >>%s 2>&1'%( Mac,phi,SolarFlare,  nphi,logfile)
        cmds.append(cmd)

run_all(SolarFlares[1])
#run_all(SolarFlares[2])
p=Pool(14)
p.map(run,cmds)

