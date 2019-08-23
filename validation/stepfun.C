

TF1 *stepfun;
	
double stepf(double *x,double *par) //step function
{
	return par[0]+par[1]*TMath::Erfc((x[0]-par[2])*par[3]);
}

void load(double low,double hig)
{
	stepfun=new TF1("stepfun",stepf,low,hig,4); //step function is defined in global_lib
	stepfun->SetParNames("offset","scale1","CE","scale2"); 
	cout<<"step function is loaded"<<endl;
	stepfun->SetParameters(8,95,9684,5.2e-4);
}
void get_energy_resolution()
{
	double par[4];
	stepfun->GetParameters(par);
	double scale2=par[3];
	double CE=par[2];
	energy_resolution=(2/scale2)/CE;
	cout<<"energy_resolution is:"<<energy_resolution<<endl;
}
