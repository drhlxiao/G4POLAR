
double get_sigma(double *x,double *par)
{
    float noise=0;
    float r2=0.12*0.12;
    float res_exp=0.4;
    float statFactor=80*(res_exp*res_exp-r2-(noise/80.)*(noise/80));
    double dep=x[0];
    if(dep<=2)continue;
    //  double ped=gRandom->Gaus(0,2); 
    //pedestal & electronics noise
    double eSigma= dep* sqrt( statFactor/dep + r2);
    return eSigma/dep;
}

