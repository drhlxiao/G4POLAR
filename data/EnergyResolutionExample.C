{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Wed Feb 10 15:45:12 2016) by ROOT version5.34/30
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",723,204,363,327);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   Canvas_1->Range(-88,-0.08297299,565.3333,1.273092);
   Canvas_1->SetFillColor(10);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetTickx(1);
   Canvas_1->SetTicky(1);
   Canvas_1->SetLeftMargin(0.15);
   Canvas_1->SetBottomMargin(0.15);
   Canvas_1->SetFrameFillColor(0);
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameLineWidth(3);
   
   TF1 *f306 = new TF1("f306","sqrt(0.002896+12.411181/x)",10,500);
   f306->SetNpx(500);
   f306->SetFillColor(19);
   f306->SetFillStyle(0);
   f306->SetLineWidth(3);
   f306->GetXaxis()->SetTitle("Energy (keV)");
   f306->GetXaxis()->CenterTitle(true);
   f306->GetXaxis()->SetLabelSize(0.05);
   f306->GetXaxis()->SetTitleSize(0.06);
   f306->GetYaxis()->SetTitle("#sigma/E");
   f306->GetYaxis()->CenterTitle(true);
   f306->GetYaxis()->SetLabelSize(0.06);
   f306->GetYaxis()->SetTitleSize(0.06);
   f306->Draw("");
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}
