import ROOT
import sys
import numpy as np
from array import array

import argparse

parser = argparse.ArgumentParser(description = "input dijet and dibijet root file")
parser.add_argument('-j', '--dijet', help='dijet input file')
parser.add_argument('-b', '--dibjet', help='dibjet input file')
parser.add_argument('-o', '--output', help='output file')
args = parser.parse_args()

f_dijet = ROOT.TFile.Open(args.dijet, 'READ')
f_dibjet = ROOT.TFile.Open(args.dibjet, 'READ')

f_output = ROOT.TFile.Open(args.output, 'RECREATE')
tree = ROOT.TTree('overlap','overlap')
f_output.cd()
run_number = array('i', [0]) 
event_number = array('i', [0])
m_jj = array('f', [0])
tree.Branch('run_number', run_number, 'run_number/I')
tree.Branch('event_number', event_number, 'event_number/I')
tree.Branch('m_jj', m_jj, 'm_jj/F')

dijetTree = f_dijet.Get('overlap')
dibjetTree = f_dibjet.Get('overlap')

diblist = np.array([])
for i in range(0,dibjetTree.GetEntries()):
	dibjetTree.GetEntry(i)
	tmplist =  [getattr(dibjetTree, 'run_number'), getattr(dibjetTree, 'event_number')]
	diblist = np.append(diblist, tmplist)
	if i % 5000 == 0:
		print('processed {} events'.format(i))

for i in range(0, dijetTree.GetEntries()):
	dijetTree.GetEntry(i)
	tmplist =  [getattr(dijetTree, 'run_number'), getattr(dijetTree, 'event_number')]
	if tmplist in diblist:
		print("kicking out an event!")
		continue
	run_number[0] = getattr(dijetTree, 'run_number')
	event_number[0] =  getattr(dijetTree, 'event_number')
	m_jj[0] = getattr(dijetTree, 'm_jj')
	tree.Fill()
	if i % 5000 == 0:
		print('processed {} events'.format(i))



tree.Write("", ROOT.TObject.kOverwrite);
f_output.Close()
