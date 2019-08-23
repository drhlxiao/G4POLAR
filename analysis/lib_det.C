/***************************************************************
 * Library for all functions concerning numbering and localisation of bars, modules according to the convention.
 *
 * This library fixes the numbering convention.
 *
 * Author  : N. Produit, N. Gauvin
 * Date    : October 2011
 * Version : 1.11
 *
 ***************************************************************/
#include <cmath>
#include "TMath.h"
#include "lib_det.h"
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
// Already defined in header
// const int scramble[64]={29,25,21,17,15,11, 7, 3,
//                         31,27,23,19,13, 9, 5, 1,
//                         30,26,22,18,12, 8, 4, 0,
//                         28,24,20,16,14,10, 6, 2,
//                         35,39,43,45,49,53,57,61,
//                         33,37,41,47,51,55,59,63,
//                         32,36,40,46,50,54,58,62,
//                         34,38,42,44,48,52,56,60};
// const int inv_scramble[64]={23,15,31, 7,22,14,30, 6,
// 			    21,13,29, 5,20,12,28, 4,
// 			    27, 3,19,11,26, 2,18,10,
// 			    25, 1,17, 9,24, 0,16, 8,
// 			    48,40,56,32,49,41,57,33,
// 			    50,42,58,34,59,35,51,43,
// 			    60,36,52,44,61,37,53,45,
// 			    62,38,54,46,63,39,55,47};
/*pm
  20 21 22 23 24
  15 16 17 18 19
  10 11 12 13 14
  5  6  7  8  9
  0  1  2  3  4 
  */
/*bars in pm
  56 57 58 59 60 61 62 63
  48 49 50 51 52 53 54 55 
  40 41 42 43 44 45 46 47 
  32 33 34 35 36 37 38 39 
  24 25 26 27 28 29 30 31
  16 17 18 19 20 21 22 23
  8  9 10 11 12 13 14 15
  0  1  2  3  4  5  6  7
  */
/*all bars
  1560                            1599
  ...
  320
  280
  240
  200
  160
  120
  80                              119
  40                               79 
  0  1  2  3  4  5  6  7  8  9 .. 39
  */
// Input : pm (0..24), bar (0..63)
// Output: index (0..1599)
int get_index(int pm,int bar){
    int pm_col=pm%5;
    int pm_row=pm/5;
    int bar_col=bar%8;
    int bar_row=bar/8;
    return pm_row*5*8*8+bar_row*5*8+pm_col*8+bar_col;
}
// Input : row (0..39), col (0..39)
// Output: index (0..1599)
int get_index_row_col(int row,int col){
    return row*40+col;
}
// Input : index (0..1599)
// Output: pm (0..24), bar (0..63)
void get_pm_bar(int index,int& pm,int& bar){
    int pm_col=(index%40)/8;
    int pm_row=index/320;
    int bar_col=index%8;
    int bar_row=(index/40)%8;
    pm=pm_row*5+pm_col;
    bar=bar_row*8+bar_col;
}
// Get the module id of a given index on all bars (0...1599)
// Input : index (0..1599)
// Output: pm (0..24)
int get_pm(int index){
    int pm_col=(index%40)/8;
    int pm_row=index/320;
    int pm=pm_row*5+pm_col;
    return pm;
}
// Get the module id of given global column and row number (0...39). 
// Input : row (0..39), col (0..39)
// Output: pm (0..24)
int get_pm(int row, int col){
    int icol = col/8;
    int irow = row/8;
    return ( icol + irow*5 ); // SO 20120229
}
// Input : index (0..1599)
// Output: bar (0..63)
int get_bar(int index){
    int bar_col=index%8;
    int bar_row=(index/40)%8;
    int bar=bar_row*8+bar_col;
    return bar;
}
// Input : index (0..1599)
// Output: row (0..39)
int get_row(int index){
    return (int)(index/40);
}
int get_column_90(int index){
    return (int)(index/40);
}
// Input : index (0..63)
// Output: row (0..7)
int get_row_1mod(int index){
    return (int)(index/8);
}
// Input : index (0..1599)
// Output: col (0..39)
int get_column(int index){
    return (int)(index%40);
}
int get_row_90(int index){
    return (int)(40-(index%40));
}
// Input : index (0..63)
// Output: col (0..7)
int get_column_1mod(int index){
    return (int)(index%8);
}
// Are two bars neighbours ?
bool AreNeighbours(int row1, int row2, int col1, int col2){
    if ((row1==row2)&&(col1==col2)) return 0;// we are not our own neighbours
    return (TMath::Abs(row1-row2)<=1) && (TMath::Abs(col1-col2)<=1);
}
// Are two bars closest neighbours ?
bool AreClosestNeighbours(int row1, int row2, int col1, int col2){
    return (TMath::Abs(row1-row2) + TMath::Abs(col1-col2) == 1 );
}
// Are two bars closest neighbours in the very same module ?
bool AreClosestNeighboursInMod(int row1, int row2, 
        int col1, int col2){
    bool n = AreClosestNeighbours(row1, row2, col1, col2);
    bool sm = ( get_pm(row1,col1)==get_pm(row2,col2));
    return n&&sm;
}
// Are two bars neighbours in the very same module ?
bool AreNeighboursInMod(int row1, int row2, int col1, int col2){
    bool n = AreNeighbours(row1, row2, col1, col2);
    bool sm = ( get_pm(row1,col1)==get_pm(row2,col2));
    return n&&sm;
}
// Are two bars neighbours in the very same module ?
// mod : if true, bar numbering for 1 module (t2module)
bool AreNeighboursInMod(int bar1, int bar2, bool mod){
    if(mod)
        return AreNeighboursInMod(get_row_1mod(bar1),get_row_1mod(bar2),
                get_column_1mod(bar1),get_column_1mod(bar2) );
    else 
        return AreNeighboursInMod(get_row(bar1),get_row(bar2),
                get_column(bar1),get_column(bar2) );  
}
// Are two bars closest neighbours in the very same module ?
// mod : if true, bar numbering for 1 module (t2module)
bool AreClosestNeighboursInMod(int bar1, int bar2, bool mod){
    if(mod)
        return AreClosestNeighboursInMod(get_row_1mod(bar1),
                get_row_1mod(bar2),
                get_column_1mod(bar1),
                get_column_1mod(bar2) );
    else 
        return AreClosestNeighboursInMod(get_row(bar1),get_row(bar2),
                get_column(bar1),
                get_column(bar2) );  
}
// Is bar on the side of a module ?
// mod : if true, bar numbering for 1 module (t2module)
bool IsSideInMod(int bar, bool mod){
    if( mod ){
        int col = get_column_1mod(bar);
        if( col == 0 || col == 7 ) return true;
        int raw = get_row_1mod(bar);
        if( raw == 0 || raw == 7 ) return true;
    } else {
        int colr = get_column(bar)%8;
        if( colr==7 || colr==0 ) return true;
        int rowr = get_row(bar)%8;
        if( rowr==7 || rowr==0 ) return true;
    }
    return false;
}
// Is bar on the side of the rectangle of 6 ESRF2012 modules
// Input : index (0..1599)
bool IsSideInESRF12(int index){
    int col = get_column(index);
    if( col==16 || col==31 ) return true;
    int row = get_row(index);
    if( row==0 || row==23 ) return true;
    return false;
}

void get_bar_center(float &x,float &y,int index)
{
    int row=get_row(index);
    int col=get_column(index);
    get_bar_center(x,y,row,col);

}
void get_bar_center(float &x,float &y,int row,int col){
    y=offset_y + bar_pitch/2. + pm_pitch/2. + bar_pitch*row + pm_pitch*(int)(row/8);
    x=offset_x + bar_pitch/2. + pm_pitch/2. + bar_pitch*col + pm_pitch*(int)(col/8);
    // this is the correct convention x along col and y along row
}
void get_bar_center_90(float &x,float &y,int row,int col){
    y=offset_y + bar_pitch/2. + pm_pitch/2. + bar_pitch*(40-row) + pm_pitch*(int)((40-row)/8);
    x=offset_x + bar_pitch/2. + pm_pitch/2. + bar_pitch*col + pm_pitch*(int)(col/8);
    // this is the correct convention x along col and y along row
}
void get_rand_pos(float &x,float &y,int row,int col){
    get_bar_center(x,y,row,col);
    x-=bar_width/2.;
    x+=gRandom->Uniform(bar_width);
    y-=bar_width/2.;
    y+=gRandom->Uniform(bar_width);
}
void get_bar_from_pos(double x,double y,int &row,int &col){
    x-=offset_x + pm_pitch/2.;
    y-=offset_y + pm_pitch/2.;
    double xx=x/bar_pitch;
    int i;
    for (i=0;i<5;i++){
        if (xx<8) break;
        xx-=pm_pitch/bar_pitch;
        xx-=8;
    }
    col=i*8+(int)xx;
    double yy=y/bar_pitch;
    for (i=0;i<5;i++){
        if (yy<8) break;
        yy-=pm_pitch/bar_pitch;
        yy-=8;
    }
    row=i*8+(int)yy;
}
TH1D* order_in_readout(TH1D* h){
    TString nn=h->GetName();
    nn+="ro";
    TH1D* hro=(TH1D*)h->Clone(nn);
    nn=hro->GetXaxis()->GetTitle();
    nn+=" in readout order";
    hro->SetXTitle(nn);
    int n=hro->GetNbinsX();
    double* new_content=new double[n+2];
    double* new_error=new double[n+2];
    new_content[0]=hro->GetBinContent(0);
    new_content[n+1]=hro->GetBinContent(n+1);
    new_error[0]=hro->GetBinError(0);
    new_error[n+1]=hro->GetBinError(n+1);
    for (int i=0;i<n;i++){
        new_content[scramble[i]+1]=hro->GetBinContent(i+1);
        new_error[scramble[i]+1]=hro->GetBinError(i+1);
    }
    for (int i=0;i<n+2;i++){
        hro->SetBinContent(i,new_content[i]);
        hro->SetBinError(i,new_error[i]);
    }
    delete [] new_content;
    delete [] new_error;
    hro->SetDirectory(0);
    return hro;
}
TH2D* order_in_readout(TH2D* h){
    TString nn=h->GetName();
    nn+="ro";
    TH2D* hro=(TH2D*)h->Clone(nn);
    nn=hro->GetXaxis()->GetTitle();
    nn+=" in readout order";
    hro->SetXTitle(nn);
    nn=hro->GetYaxis()->GetTitle();
    nn+=" in readout order";
    hro->SetYTitle(nn);
    int n1=hro->GetNbinsX();
    int n2=hro->GetNbinsY();
    double* new_content=new double[(n1+2)*(n2+2)];
    double* new_error=new double[(n1+2)*(n2+2)];
    for (int i=0;i<n1+2;i++){
        new_content[i*(n2+2)+0]=hro->GetBinContent(i,0);
        new_content[i*(n2+2)+n2+1]=hro->GetBinContent(i,n2+1);
        new_error[i*(n2+2)+0]=hro->GetBinError(i,0);
        new_error[i*(n2+2)+n2+1]=hro->GetBinError(i,n2+1);
    }
    for (int i=0;i<n2+2;i++){
        new_content[0*(n2+2)+i]=hro->GetBinContent(0,i);
        new_content[(n1+1)*(n2+2)+i]=hro->GetBinContent(n1+1,i);
        new_error[0*(n2+2)+i]=hro->GetBinError(0,i);
        new_error[(n1+1)*(n2+2)+i]=hro->GetBinError(n1+1,i);
    }
    for (int i=0;i<n1;i++){
        for (int j=0;j<n2;j++){
            new_content[(scramble[i]+1)*(n2+2)+scramble[j]+1]=hro->GetBinContent(i+1,j+1);
            new_error  [(scramble[i]+1)*(n2+2)+scramble[j]+1]=hro->GetBinError(i+1,j+1);
        }
    }
    for (int i=0;i<(n1+2)*(n2+2);i++){
        hro->SetBinContent(i,new_content[i]);
        hro->SetBinError(i,new_error[i]);
    }
    delete [] new_content;
    delete [] new_error;
    hro->SetDirectory(0);
    return hro;
}
//check in root:
/*
   root
   .L scramble.C
   for (int i=39;i>=0;i--){for (int j=0;j<40;j++){int pm;int bar;get_pm_bar(i*40+j,pm,bar);printf("%2d ",pm);}printf("\n");}
   for (int i=39;i>=0;i--){for (int j=0;j<40;j++){int pm;int bar;get_pm_bar(i*40+j,pm,bar);printf("%2d ",bar);}printf("\n");}
   for (int i=39;i>=0;i--){for (int j=0;j<40;j++){int pm;int bar;index=i*40+j;get_pm_bar(index,pm,bar);printf("%4d ",index);}printf("\n");}
   for (int i=39;i>=0;i--){for (int j=0;j<40;j++){int pm;int bar;index=i*40+j;get_pm_bar(index,pm,bar);assert(index==get_index(pm,bar));}}
   float x;
   float y;
   int row;
   int col;
   for (int i=0;i<40;i++){for (int j=0;j<40;j++){for (int n=0;n<1000;n++){get_rand_pos(x,y,i,j);get_bar_from_pos(x,y,row,col);if (i!=row&&j!=col){printf("%4d %4d %4d %4d %g %g\n",i,row,j,col,x,y);}}}}
   */
// Computes xi angle in degree from positions
double GetXi( float x0, float y0, float x1, float y1 ){
    float xideg=180.*atan2(y0-y1,x0-x1)/TMath::Pi();
    // the above will give value in ]-180,180], we want ]0,360]
    if( xideg<0 ) xideg += 360;
    // if(xideg>180)xideg=xideg-180;
    return xideg;
}
int official2FLEX(int official){
    return official%5;
}
int official2posFLEX(int official){
    return 4-official/5;
}
int FLEX2official(int flex,int ninflex){
    return flex+5*(4-ninflex);
}
/*
void print_bar_center()
{
        
        for(int j=0;j<40;j++)
        {
            float x,y;
            int index=j*40;
            get_bar_center(x,y,index);
            cout<<y<<endl;
        }


}


*/
