
#include "Coincidence.hh"
#include "lib_det.h"
#include <math.h>
#include <TVector3.h>
float Point2LineDistance(float px0, float py0, float px1, float py1, float px2, float py2)
{
	float dist_source=abs((py2-py1)*px0-(px2-px1)*py0-px1*py2+px2*py1)/sqrt((px1-px2)*(px1-px2)+(py1-py2)*(py1-py2));
	return dist_source;
}
#include <iostream>
using namespace std;
bool GeometryCoincidence(int bar1, int bar2, float sx, float sy, float &dist2source, float &distlim, float &opening_angle)
{


		dist2source=-1;

	float px1, px2, py1, py2;
	get_bar_center(px1,py1,bar1);
	get_bar_center(px2,py2,bar2);
	float barwidth=5.8;
	float pad=barwidth/2.;

	dist2source=Point2LineDistance(sx, sy, px1, py1, px2, py2);
	bool ok=false;

	float maxdist=0;
		//int maxi=0;
	float vertex_x[4]={px1+pad, px1+pad, px1-pad,px1-pad };
	float vertex_y[4]={py1-pad, py1+pad, py1+pad, py1-pad};
	float dist;


	float dx1,dx2,dy1,dy2;

	dx1=vertex_x[0]-px1;
	dy1=vertex_y[0]-py1;
	dx2=px2-px1;
	dy2=py2-py1;
	TVector3 v1(dx1,dy1,0);
	TVector3 v2(dx2,dy2,0);
	double angle=v1.Angle(v2);//between 0-180
	double  di1=1.414*barwidth *0.5 *abs(sin(angle));
	double  di2=1.414*barwidth *0.5 *abs(cos(angle));
	double pi=3.1415926;

	dx1=sx-px1;
	dy1=sy-py1;
	dx2=sx-px2;
	dy2=sy-py2;
	TVector3 v3(dx1,dy1,0);
	TVector3 v4(dx2,dy2,0);
	opening_angle=v3.Angle(v4)*180./pi;//between 0-180


	distlim=di1;
	if(di2>di1)distlim=di2;
	if((distlim)>=dist2source && opening_angle>=120)ok=true;
	//if(ok)
	//{
	//	cout<<"distlim"<<distlim<<" dist2source: "<<dist2source<<" "<<opening_angle<<endl;
	//	cout<<"bar1,bar2:"<<bar1<<" "<<bar2<<endl;
	//}
	return ok;
	//return true;

}
