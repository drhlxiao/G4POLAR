int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
        float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}
bool lineSegmentIntersection
(float Ax, float Ay,float Bx, float By,float Cx, float Cy,float Dx, float Dy,float *X, float *Y) 
{

    bool YES=true;
    bool NO=false;
  float  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
  if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return NO;

  //  Fail if the segments share an end-point.
  if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
  ||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
    return NO; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.) return NO;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB) return NO;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;

  //  Success.
  return YES; 
}
TH1F hx("hx","hx",200,-100,100);
TH1F hy("hy","hy",200,-100,100);
TH2F h2("h2","h2",200,-100,100,200,-100,100);
void calculate_source_position(char filename[256])
{

    float x1,y1,x2,y2,x3,y3,x4,y4,cx,cy;

    ifstream infile(filename); 

    if(!infile.good())
    { 
        cout<<"can not open file..."<<endl; 
        return;
    } 
    while (!infile.eof())
    {
        infile>>x1>>y1>>x2>>y2;
        infile>>x3>>y3>>x4>>y4;

        if(x1<0&&y1<0&&x2<0&&y2<0&&x3<0&&x4<0&&y3<0&&y4<0)
        {

            if( lineSegmentIntersection(x1,y1,x2,y2,x3,y3,x4,y4,&cx,&cy))
            {
                hx.Fill(cx);
                hy.Fill(cy);
                h2.Fill(cx,cy);
            }
        }

    }


    infile.close(); 

}
