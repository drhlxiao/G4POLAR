#include <TApplication.h> 
#include <TString.h> 
#include <TROOT.h> 
#include <iostream>

#include "Coincidence.h"
using namespace std; 
void Help()
{	cout<<endl
	<<"A program to look for 2 coincident hit pairs of calibration events. "
		<<endl<<endl;

	cout<<"USAGE"<<endl
		<<"Compile first the program by calling 'make' in the "
		<<"terminal."<<endl
		<<"Execute then the program by calling its name."<<endl<<endl

		<<"SYNOPSIS"<<endl
		<<"coincidence -n <events> -data_type <data type> -thr <threshold> -dist <from,  to> -source <source_information_file> -o <output-file> -i  <input-files or pattern> "<<endl
		<<"-data_type  <pm or patter>"<<endl
		<<"-thr use threshold to select hits"<<endl
		<<"-dist  distance from .. to"<<endl
        <<"-source set the file with source information"<<endl
        <<endl;

} 
int main(int argc, char *argv[])
{
	Int_t s=0;
	TString sel;
	TString ofname;
	TString ifname;
	Long64_t nevents=0;

    TString str;
    TString source_config="source.conf";
    TString data_type="pm";
    float threshold=30;
    float dist0=0;
    float dist1=20;

	if(argc==1)
	{
		Help();
		return 0;
	}
	
	while (s<argc-1) 
	{
		sel = argv[++s];
		if( sel == "help" || sel=="-h" ||sel=="--help")
		{
			Help();
			return 0;
		} 
		else if(sel == "-o")
		{

			ofname=argv[++s];
			if(!ofname.EndsWith(".root"))
			{
				cout<<"output-filename should be ended with .root"<<endl;
				return 0;
			}
			cout<<"output file:"<<endl;
			cout<<" -- "<<ofname<<endl;
            
		}
		else if(sel=="-data_type")
		{
            data_type=argv[++s];
		}
		else if(sel=="-source")
		{
            source_config=argv[++s];
		}
		else if(sel=="-i")
		{
            ifname=argv[++s];
		}
		else if(sel=="-n")
		{
				TString str=argv[++s];
				nevents=str.Atoi();
		}
		else if(sel=="-thr")
		{
				TString str=argv[++s];
				threshold=str.Atof();
		}
        else if(sel=="-dist")
        {
				str=argv[++s];
				dist0=str.Atof();
				str=argv[++s];
				dist1=str.Atof();
                cout<<"setting distance: "<<dist0<<" "<<dist1<<endl;
        }
	}

	cout<<"Input Files: "<<endl;

	Coincidence *run=new Coincidence();
    run->LoadSettings(source_config);
    run->SetDataType(data_type);
    run->SetCoincidenceDistanceBetween(dist0,dist1);
    run->SetThreshold(threshold);
    if(nevents>0)run->SetNEvents(nevents);

	run->DoCoincidence(ifname,ofname);

	delete run;
	return 0;
} 
