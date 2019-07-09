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

    tree = ET.parse('models/bloodsport_programmable_bar/model.sdf')

    inertial = tree.find('model').find('link').find('inertial')

    inertial.find('mass').text = str(mass)
    inertial.find('inertia').find('ixx').text = str(xx)
    inertial.find('inertia').find('iyy').text = str(yy)
    inertial.find('inertia').find('izz').text = str(zz)
    inertial.find('inertia').find('ixy').text = str(xy)
    inertial.find('inertia').find('ixz').text = str(xz)
    inertial.find('inertia').find('iyz').text = str(yz)

    tree.write('autogen_model/bar'+str(barnum)+'/model.sdf')

    copyfile('models/bloodsport_programmable_bar/model.config',
             'autogen_model/bar'+str(barnum)+'/model.config')

    tree = ET.parse('autogen_model/bar'+str(barnum)+'/model.config')
    tree.find('name').text = 'bloodsport_bar' + str(barnum)
    tree.write('autogen_model/bar'+str(barnum)+'/model.config')

def program_world(worldnum):

    tree = ET.parse('stability_test.world')

    robot_model = tree.find('world').find('model')

    robot_model.findall('include')[1].find('uri').text = 'model://bar'+str(worldnum)

    tree.write('autogen_model/world' + str(worldnum) + '.world')


for i in range(0, 10):
    #program_bar(23.5, 0.056, 2.474, 2.42)
    program_bar(i, mass=35, xx=0.056, yy=2.5, zz=(2.5 - i / 5.0))
    program_world(i)
