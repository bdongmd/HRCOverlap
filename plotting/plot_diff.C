#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include "/afs/cern.ch/work/b/bdong/macros/AtlasLabels.C"
#include "/afs/cern.ch/work/b/bdong/macros/AtlasStyle.C"
#include "/afs/cern.ch/work/b/bdong/macros/AtlasUtils.C"

bool runDib = true

void plot_diff(){

	if (runDib){
		const double mjj_bins[] = {1133, 1166, 1200, 1234, 1269, 1305, 1341, 1378, 1416, 1454, 1493, 1533, 1573, 1614, 1656, 1698, 1741, 1785, 1830, 1875, 1921, 1968, 2016, 2065, 2114, 2164, 2215, 2267, 2320, 2374, 2429, 2485, 2542, 2600, 2659, 2719, 2780, 2842, 2905, 2969, 3034, 3100, 3167, 3235, 3305, 3376, 3448, 3521, 3596, 3672, 3749, 3827, 3907, 3988, 4070, 4154, 4239, 4326, 4414, 4504, 4595, 4688, 4782, 4878, 4975, 5074, 5175, 5277, 5381, 5487, 5595, 5705, 5817, 5931, 6047, 6165, 6285};
	} else{
		const double mjj_bins[] = {1100, 1133, 1166, 1200, 1234, 1269, 1305, 1341, 1378, 1416, 1454, 1493, 1533, 1573, 1614, 1656, 1698, 1741, 1785, 1830, 1875, 1921, 1968, 2016, 2065, 2114, 2164, 2215, 2267, 2320, 2374, 2429, 2485, 2542, 2600, 2659, 2719, 2780, 2842, 2905, 2969, 3034, 3100, 3167, 3235, 3305, 3376, 3448, 3521, 3596, 3672, 3749, 3827, 3907, 3988, 4070, 4154, 4239, 4326, 4414, 4504, 4595, 4688, 4782, 4878, 4975, 5074, 5175, 5277, 5381, 5487, 5595, 5705, 5817, 5931, 6047, 6165, 6285, 6407, 6531, 6658, 6787, 6918, 7052, 7188, 7326, 7467, 7610, 7756, 7904, 8055, 8208, 8364, 8523, 8685, 8850, 9019};
	}

	const int mjj_nbins = sizeof(mjj_bins)/sizeof(mjj_bins[0]);

	TH1D *h_mjj_removed = new TH1D("mjj_removed", "mjj_removed", mjj_nbins-1, &mjj_bins[0]);
	TH1D *h_mjj_original = new TH1D("mjj_original", "mjj_original", mjj_nbins-1, &mjj_bins[0]);

	TFile *f_original = new TFile("../inputData/FullRun2_dib_original.root", "READ");
	TFile *f_removed = new TFile("../output/FullRun2_dibjet_2btag.root", "READ");

	TTree *t_original = (TTree*)f_original->Get("overlap");
	TTree *t_removed = (TTree*)f_removed->Get("overlap");

	double original_mjj = 0;
	float removed_mjj = 0;
	vector<int> *region=0;
	t_original->SetBranchAddress("m_jj", &original_mjj);
	t_original->SetBranchAddress("region", &region);
	t_removed->SetBranchAddress("m_jj", &removed_mjj);

	for (int i_entry =0; i_entry<t_original->GetEntries(); i_entry++){
		t_original->GetEntry(i_entry);
		//h_mjj_original->Fill(original_mjj);
		if (region->size()==2){
			h_mjj_original->Fill(original_mjj);
		}
	}
	for (int i_entry =0; i_entry<t_removed->GetEntries();  i_entry++){
		t_removed->GetEntry(i_entry);
		h_mjj_removed->Fill(removed_mjj);
	}

	// plotting
	SetAtlasStyle();

	TCanvas *c1 = new TCanvas("c1","c1", 800, 600);
	c1->SetFillColor(1);
	c1->SetFrameBorderSize(1);

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
	pad1->SetTopMargin(0.05);
	pad1->SetBottomMargin(0.);
	pad1->SetLogy(1);
	pad1->Draw("same");
	pad1->cd();

	TLegend *leg1 = new TLegend(0.65, 0.7, 0.92, 0.85, NULL, "brNDC");
	float maxi = 500.*h_mjj_original->GetMaximum();
	h_mjj_original->SetLineColor(kBlack);
	h_mjj_original->GetYaxis()->SetTitle("Events");
	h_mjj_original->GetYaxis()->SetRangeUser(0.8, maxi);
	h_mjj_original->SetMarkerStyle(20);
	h_mjj_original->SetMarkerColor(kBlack);
	h_mjj_original->SetMarkerSize(1);

	h_mjj_removed->SetLineColor(kRed);
	h_mjj_removed->SetMarkerStyle(24);
	h_mjj_removed->SetMarkerColor(kRed);
	h_mjj_removed->SetMarkerSize(1);

	leg1->AddEntry(h_mjj_original, "Original", "lp");
	leg1->AddEntry(h_mjj_removed, "Overlap Removed", "lp");
	h_mjj_original->Draw("samelP");
	h_mjj_removed->Draw("samelP");
	leg1->Draw("same");

	ATLASLabel(0.2, 0.87, "Internal", kBlack);
	myText(0.2,0.80, kBlack, "#sqrt{s} = 13 TeV, 139fb^{-1}");
	//myText(0.2,0.73, kBlack, "di-b-jet channel (#geq 1 b-tag)");
	myText(0.2,0.73, kBlack, "di-b-jet channel (2 b-tag)");

	TH1D *h_removed_ratio = (TH1D*)h_mjj_removed->Clone();

	c1->cd();
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
	pad2->SetGrid(1);
	pad2->SetPad(0, 0, 1, 0.3);
	pad2->SetTopMargin(0);
	pad2->SetBottomMargin(0.3);
	pad2->Draw("same");
	pad2->cd();

	h_removed_ratio->Divide(h_mjj_original);
	h_removed_ratio->SetMarkerStyle(20);
	h_removed_ratio->SetMarkerSize(1);
	h_removed_ratio->SetStats(0);
	h_removed_ratio->GetXaxis()->SetTitle("M_{jj} [GeV]");
	h_removed_ratio->GetYaxis()->SetTitle("ratio");
	h_removed_ratio->GetYaxis()->SetNdivisions(5);
	h_removed_ratio->GetYaxis()->SetRangeUser(0.95, 1.05);
	h_removed_ratio->GetXaxis()->SetLabelSize(0.1);	 
	h_removed_ratio->GetXaxis()->SetTitleSize(0.13);
	h_removed_ratio->GetXaxis()->SetTitleOffset(1);
	h_removed_ratio->GetYaxis()->SetTitleOffset(0.45);
	h_removed_ratio->GetYaxis()->SetLabelSize(0.1);
	h_removed_ratio->GetYaxis()->SetTitleSize(0.1);
	h_removed_ratio->Draw("sameLP");

	c1->SaveAs("dibCompare_2btag.pdf");

}
