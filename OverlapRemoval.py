import ROOT
import numpy as np
from array import array
from datetime import datetime

import argparse
parser = argparse.ArgumentParser(description='output file')
parser.add_argument('-o', '--output', type=str)
args = parser.parse_args()

isDib = False
isDib2bchannel = False

f_txt = open('overlapList/DijetOverlaps.txt', 'r')
lines = f_txt.readlines()

if isDib:
	overlapList = np.empty([len(lines), 2], dtype=np.int64)
else:
	f_dib_txt = open('', 'r')
	diblines = f_dib_txt.readlines()
	overlapList = np.empty([len(lines)+len(diblines), 2], dtype=np.int64)

i_line = 0
for line in lines:
	tmplist = line.rstrip().split(',')
	tmplist = [int(tmplist[0]), int(tmplist[1])]
	overlapList[i_line] = tmplist
	i_line = i_line + 1

if not isDib:
	for line in diblines:
		tmplist = line.rstrip().split(',')
		tmplist = [int(tmplist[0]), int(tmplist[1])]
		overlapList[i_line] = tmplist
		i_line = i_line + 1
overlapList = overlapList.tolist()

f_output = ROOT.TFile.Open(args.output, 'RECREATE')
tree = ROOT.TTree('overlap','overlap')
run_number = array('i', [0]) 
region = array('i',[0])
event_number = array('q', [0])
m_jj = array('f', [0])
tree.Branch('run_number', run_number, 'run_number/I')
tree.Branch('event_number', event_number, 'event_number/L')
tree.Branch('m_jj', m_jj, 'm_jj/F')
#tree.Branch('region', region, 'region/F')

for year in range(15,19):
	if not isDib:
		f_dijet = ROOT.TFile.Open('inputData/dijet_Data{}_removeddib.root'.format(year),'r')
	else:
		f_dijet = ROOT.TFile.Open('inputData/dibjet_Data{}.root'.format(year),'r')
	dijetTree = f_dijet.Get('overlap')

	for i in range(0, dijetTree.GetEntries()):
		dijetTree.GetEntry(i)
		tmplist =  [getattr(dijetTree, 'run_number'), getattr(dijetTree, 'event_number')]
		if i % 10000 == 0:
			print('{}: processed {} / {} events'.format(datetime.now().strftime('%H:%M:%S'),i, dijetTree.GetEntries()))
		if isDib2bchannel and len(getattr(dijetTree, 'region')) == 1:
			continue
		tmplist =  [getattr(dijetTree, 'run_number'), getattr(dijetTree, 'event_number')]
		if tmplist in overlapList:
			continue
		#run_number[0] = getattr(dijetTree, 'run_number')
		#event_number[0] =  getattr(dijetTree, 'event_number')
		m_jj[0] = getattr(dijetTree, 'm_jj')
		tree.Fill()
	f_dijet.Close()

f_output.cd()
tree.Write()
f_output.Close()

