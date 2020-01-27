import xml.etree.ElementTree as ET
from shutil import copyfile
import os

def program_bar(barnum, mass, xx, yy, zz, xy=0, xz=0, yz=0):
    try:
        os.mkdir('autogen_model')
    except:
        pass

    try:
        os.mkdir('autogen_model/bar'+str(barnum))
    except:
        pass

    tree = ET.parse('models/bloodsport_v2_programmable_bar/model.sdf')

    inertial = tree.find('model').find('link').find('inertial')

    inertial.find('mass').text = str(mass)
    inertial.find('inertia').find('ixx').text = str(xx)
    inertial.find('inertia').find('iyy').text = str(yy)
    inertial.find('inertia').find('izz').text = str(zz)
    inertial.find('inertia').find('ixy').text = str(xy)
    inertial.find('inertia').find('ixz').text = str(xz)
    inertial.find('inertia').find('iyz').text = str(yz)

    tree.write('autogen_model/bar'+str(barnum)+'/model.sdf')

    copyfile('models/bloodsport_v2_programmable_bar/model.config',
             'autogen_model/bar'+str(barnum)+'/model.config')

    tree = ET.parse('autogen_model/bar'+str(barnum)+'/model.config')
    tree.find('name').text = 'bloodsport_bar' + str(barnum)
    tree.write('autogen_model/bar'+str(barnum)+'/model.config')

def program_world(worldnum, min_rad_per_sec, max_rad_per_sec, min_force, max_force):

    tree = ET.parse('stability_test.world')

    robot_model = tree.find('world').find('model')

    tree.find('world').find('plugin').find('spinner_min_rad_per_sec').text = str(min_rad_per_sec)
    tree.find('world').find('plugin').find('spinner_max_rad_per_sec').text = str(max_rad_per_sec)
    tree.find('world').find('plugin').find('spinner_min_force_netwon').text = str(min_force)
    tree.find('world').find('plugin').find('spinner_max_force_newton').text = str(max_force)

    robot_model.findall('include')[1].find('uri').text = 'model://bar'+str(worldnum)

    tree.write('autogen_model/world' + str(worldnum) + '.world')

'''
for i in range(0, 10):
    #program_bar(23.5, 0.056, 2.474, 2.42)
    program_bar(i, mass=35, xx=0.056, yy=2.5, zz=(2.5 - i / 5.0))
    program_world(i)
'''

import sys

id = sys.argv[1]
mass = sys.argv[2]
min_rad_per_sec = sys.argv[3]
max_rad_per_sec = sys.argv[4]
ixx = sys.argv[5]
iyy = sys.argv[6]
izz = sys.argv[7]
min_force = sys.argv[8]
max_force = sys.argv[9]

print (" ** Programming bar & world **")

program_bar(id, mass, ixx, iyy, izz)
program_world(id, min_rad_per_sec, max_rad_per_sec, min_force, max_force)
