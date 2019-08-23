#!/usr/bin/python

import os
import datetime
import  time
from subprocess import Popen
from multiprocessing import Pool

now=datetime.datetime.now()
logfile="sim_%s.log" % now.strftime("%Y%m%d")
import numpy as np

NPhi=40
GRB='GRB170206A'
Mac='%s.mac'%GRB
cmds=[]
cmd='./g4POLAR -m %s -ppol 0  -pphi  0  -o  %s_Simout/validate/phi_0_pol_0.root >>%s 2>&1'%(Mac,GRB,  logfile)
cmds.append(cmd)
for nphi in range(0,NPhi):
    for pol in np.arange(0,1, 0.1):
        phi=nphi*180./NPhi 
        cmd='./g4POLAR -m %s -ppol %.2f  -pphi  %f  -o  %s_Simout/validate/phi_%.2f_pol_%.2f.root >>%s 2>&1'%( Mac,pol,phi, GRB, phi,pol,logfile)
        cmds.append(cmd)
def run(command):
    print command
    os.system(command)

p=Pool(8)
p.map(run,cmds)
