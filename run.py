#!/usr/bin/python

import os
import datetime
import  time
from subprocess import Popen
from multiprocessing import Pool

now=datetime.datetime.now()
logfile="sim_%s.log" % now.strftime("%Y%m%d")

NPhi=40
GRB='GRB170206A'
Mac='%s.mac'%GRB
cmds=[]
cmd='./g4POLAR -m %s -ppol 0  -pphi  0  -o  %s_Simout/%s_phi_0_pol_0.root >>%s 2>&1'%(Mac,GRB, GRB, logfile)
cmds.append(cmd)
for nphi in range(0,NPhi):
    
    phi=nphi*180./NPhi 
    cmd='./g4POLAR -m %s -ppol 1  -pphi  %f  -o  %s_Simout/%s_phi_%d_pol_1.root >>%s 2>&1'%( Mac,phi,GRB, GRB, nphi,logfile)
    cmds.append(cmd)
def run(command):
    print command
    os.system(command)

p=Pool(8)
p.map(run,cmds)
