#include "G4ThreeVector.hh"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "globallib.hh"

void CopyThreeVector(G4ThreeVector &from, float to[3])
{
	to[0]=(float)from.x(); to[1]=(float)from.y();to[2]=(float)from.z();
}
void CanRebin(TH1* h){
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	h->SetBit(TH1::kCanRebin);
#else
	h->SetCanExtend(TH1::kXaxis);
#endif
}
void SetPolarMapBinLabels(TH2F* h2)
{
	for(int i=1;i<=40;i++)
	{
		if(i%3==0)	h2->GetYaxis()->SetBinLabel(i,Form("%d",(i-1)*40));
	}
}

