
#ifndef _lib_det_h
#define _lib_det_h 1
#include "TH1D.h"
#include "TH2D.h"
/***************************************************************
 * Library for all functions concerning numbering and localisation of bars, modules according to the convention.
 *
 Modules pos in QM (this is not the CT numbering!):
 ===================
 LV
 ----------------
 | 20 21 22 23 24
 | 15 16 17 18 19
 HV | 10 11 12 13 14
 |  5  6  7  8  9
 |  0  1  2  3  4 
 ----------------
 CT 
 connector big    connector small
 ^                ^ 
 |                |
 Ibox             28V
 Bars in module :
 ================
 56 57 58 59 60 61 62 63
 48 49 50 51 52 53 54 55 
 40 41 42 43 44 45 46 47 
 32 33 34 35 36 37 38 39 
 24 25 26 27 28 29 30 31
 16 17 18 19 20 21 22 23
 8  9 10 11 12 13 14 15
 0  1  2  3  4  5  6  7
 All bars in QM :
 ================
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
 For CT (LV bit numbering is the same 1 is least significat bit):
 ========
 21 16 11  6  1
 22 17 12  7  2
 23 18 13  8  3
 24 19 14  9  4
 25 20 15 10  5
 For Flexes :
 ===========
 Flex # on LV (this is correct for bit numbering of flex inside CT
 for some reason it goes opposite then module numbering for CT)
 0   1  2  3  4
 Module numbering on flex
 0
 1
 2
 3
 4
 ESRF 2012
 =========
 tilt=-45 and the detector is seen from the back
 @author  N. Produit, N. Gauvin
 @date    June 2012
 @version 2.46
 ***************************************************************/
const int scramble[64]={29,25,21,17,15,11, 7, 3,
	31,27,23,19,13, 9, 5, 1,
	30,26,22,18,12, 8, 4, 0,
	28,24,20,16,14,10, 6, 2,
	35,39,43,45,49,53,57,61,
	33,37,41,47,51,55,59,63,
	32,36,40,46,50,54,58,62,
	34,38,42,44,48,52,56,60};
const int inv_scramble[64]={23,15,31, 7,22,14,30, 6,
	21,13,29, 5,20,12,28, 4,
	27, 3,19,11,26, 2,18,10,
	25, 1,17, 9,24, 0,16, 8,
	48,40,56,32,49,41,57,33,
	50,42,58,34,59,35,51,43,
	60,36,52,44,61,37,53,45,
	62,38,54,46,63,39,55,47};
// Default HV values for modules (ID)
// Default for 662 keV photons
const int ModHV_set1[] = {0,0,0,0,0,0,0,0,0,0,  //0 this was up to 7 Dec 11:00
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 50
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 100
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 120 
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 150
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 200
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,700,675,0,675,675, // 220
	725,680,725,675,650,700,700,675,675,675,
	675,675,675,675,667,675,700,650,675,700, // 240
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 260
	0,0,0,0,0,0,0,0,0,0, 
	0,0,0,0,0,0,0,0,0,0, // 280
	0,0,0,0,0,0,0,0,0,0,
	650,650,0,0,0,0,0,0,0,0, // 300
	0,0,0,0,0,0,0,0,0,0 };
const int ModHV[] = {0,0,0,0,0,0,0,0,0,0,  //0
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 50
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 100
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 120 
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 150
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 200
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,725,700,0,700,700, // 220
	750,705,750,700,675,725,725,700,700,700,
	700,700,700,700,692,700,725,675,700,725, // 240
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, // 260
	0,0,0,0,0,0,0,0,0,0, 
	0,0,0,0,0,0,0,0,0,0, // 280
	0,0,0,0,0,0,0,0,0,0,
	675,675,700,0,0,0,0,0,0,0, // 300
	0,0,0,0,0,0,0,0,0,0 };
// Relation between HV numbering and Positions
const int HV2Pos[25] = { 21, 23, 20, 15, 13,
	11, 14, 10,  7,  5,
	8,  2,  3,  4,  1, 
	0, 22, 24, 18, 17,
	16, 19, 12,  9,  6 };
const int Pos2HV[25] = { 15, 14, 11, 12, 13,
	9, 24,  8, 10, 23,
	7,  5, 22,  4,  6,
	3, 20, 19, 18, 21,
	2,  0, 16,  1, 17 };
// Relation between LV numbering and Positions
const int LV2Pos[25] = { 24, 19, 14,  9,  4,
	23, 18, 13,  8,  3,
	22, 17, 12,  7,  2,
	21, 16, 11,  6,  1,
	20, 15, 10,  5,  0 };
const int Pos2LV[25] =  { 24, 19, 14,  9,  4,
	23, 18, 13,  8,  3,
	22, 17, 12,  7,  2,
	21, 16, 11,  6,  1,
	20, 15, 10,  5,  0 };//it is is own inverse!
/** a namespace for default values 
  It always refers to the current module situation.
  */
namespace DefConf{
	const int ModID2Pos[] = {0,0,18,0,0,0,0,0,0,0,  //0
		24,0,0,23,0,0,19,0,0,0, //10
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0, // 50
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0, // 100
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0, 8, // 120 
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0, // 150
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,22,17,12,7,2,20,15,10,5, // 200
		0,21,16,11,6,1,23,18,13,8, // 210
		24,14,9,3,19,15,19,4,10,5, // 220
		7,6,16,11,12,20,21,1,13,4, // 230
		24,22,23,2,17,3,8,18,14,9,  // 240
		0,0,0,0,0,0,0,0,0,0, // 250
		0,0,0,0,0,0,0,0,0,0, // 260
		0,0,0,0,0,0,0,0,0,0, // 
		0,0,0,0,0,0,0,0,0,0, // 280
		0,0,0,0,0,0,0,0,0,0, // 
		0,7,7,0,0,0,0,0,0,0, // 300
		0,0,0,0,0,0,0,0,0,0, // 
		0,0,0,0,0,0,0,0,0,0, // 
		0,0,0,0,0,0,0,0,0,0};
}
// a namespace for all QM1 stuff...
namespace QM1{

	// Given a position in Polar, find the FE id. -1 means no module
	const int Pos2ModID[25] = { 210, 215, 205, 223, 227,
		209, 214, 204, 219, 222,
		208, 213, 203, 218, 221,
		207, 212, 202, 217, 224,
		206, 211, 201, 216, 220};
}
/** a namespace for all QM2 stuff...
  Refers to the current situation.
  */
namespace QM2{

	// Given a position in Polar, find the FE id.
	// Empty spots yields module nb > 260
	const int Pos2ModID[25] = { 250, 237, 243, 245, 239,
		229, 231, 302, 246, 249,
		228, 233, 234, 238, 248,
		225, 232, 244, 247, 226,
		235, 236, 241, 242, 240 };
}
/** a namespace for MC configuration
  By default, module ids are the same as the position.
  */
namespace MC{
	const int ModID2Pos[] = {0,1,2,3,4,
		5,6,7,8,9,
		10,11,12,13,14,
		15,16,17,18,19,
		20,21,22,23,24};
	const int Pos2ModID[25] = {0,1,2,3,4,
		5,6,7,8,9,
		10,11,12,13,14,
		15,16,17,18,19,
		20,21,22,23,24};
}
const int CT2official[25]={24,19,14,9,4,
	23,18,13,8,3,
	22,17,12,7,2,
	21,16,11,6,1,
	20,15,10,5,0};
const int official2CT[25]={24,19,14, 9,4,
	23,18,13, 8,3,
	22,17,12, 7,2,
	21,16,11, 6,1,
	20,15,10, 5,0};
//interesting CT2official is its own inverse...
//CT numbering in printf statements is from 1 to 25
//but internal from 0 to 24
const double bar_pitch=6.08;//mm
const double pm_pitch=11.36;//mm
const double offset_x=-150;//mm
const double offset_y=-150;//mm
const double bar_width=5.8;//mm
// SO 
// pitch modules: 60mm; pitch bars 6.08mm (6.08*8=48.64); thickness bars 5.8mm
// there are 11.36mm passive (no bars) between 2 modules 
// (x,y)=(0,0) is the center of POLAR
int get_index(int pm,int bar);
int get_index_row_col(int row,int col);
void get_pm_bar(int index,int& pm,int& bar);
int get_pm(int index);
int get_pm(int row, int col);
int get_bar(int index);
int get_row(int index);
int get_row_90(int index);
int get_row_1mod(int index);
int get_column(int index);
int get_column_90(int index);
int get_column_1mod(int index);
bool AreNeighbours(int row1, int row2, int col1, int col2);
bool AreClosestNeighbours(int row1, int row2, int col1, int col2);
bool AreNeighboursInMod(int row1, int row2, int col1, int col2);
bool AreClosestNeighboursInMod(int row1, int row2, int col1, 
		int col2);
bool AreNeighboursInMod(int bar1, int bar2, bool mod=false);
bool AreClosestNeighboursInMod(int bar1, int bar2, bool mod=false);
bool IsSideInMod(int bar, bool mod=false);
bool IsSideInESRF12(int index);
void get_bar_center(float &x,float &y,int row,int col);
void get_bar_center(float &x,float &y,int index); 
void get_bar_center_90(float &x,float &y,int row,int col);
void get_rand_pos(float &x,float &y,int row,int col);
void get_bar_from_pos(double x,double y,int &row,int &col);
TH1D* order_in_readout(TH1D* h);
TH2D* order_in_readout(TH2D* h);
double GetXi( float x0, float y0, float x1, float y1 );
int official2FLEX(int official);
int official2posFLEX(int official);
int FLEX2official(int flex,int ninflex);
double GetAngle(int pmid);
#endif
