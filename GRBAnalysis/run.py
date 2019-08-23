#!/usr/bin/python

import os
import datetime
import  time
from subprocess import Popen
from multiprocessing import Pool

now=datetime.datetime.now()

datadir='/RAID/simulations'

NPhi=90
GRBs=['GRB170206A','GRB170101A', 'GRB170114A']
cmds=[]

def mkdir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def addRuns(GRB):
    mac='%s_TG2.mac'%GRB
    outputdir='%s/%s/database_thr20keV'%(datadir,GRB)
    mkdir(outputdir)

    logfile="%s/sim_phi0_pol0.log"%outputdir

    cmd='./g4POLAR -m %s -ppol 0  -pphi  0  -o  %s/phi_0_pol_0.root >>%s 2>&1'%(mac,outputdir, logfile)
    cmds.append(cmd)
    for nphi in range(0,NPhi):
        phi=nphi*180./NPhi 
        logfile="%s/sim_p1_phi_%d.log" %(outputdir,phi)
        cmd='./g4POLAR -m %s -ppol 1  -pphi  %f  -o  %s/phi_%d_pol_1.root >>%s 2>&1'%( mac,phi,outputdir, phi,logfile)
        cmds.append(cmd)



def run(command):
    print command
    os.system(command)

if __name__=='__main__':
    for grb in GRBs:
        addRuns(grb)

    p=Pool(14)
    p.map(run,cmds)
