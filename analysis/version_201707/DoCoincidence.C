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
		<<"coincidence OPTIONS -source <source_information_file> -o <output-file> -i  <input-files or pattern> "<<endl
        <<"OPTIONS:"<<endl
        <<endl;
    cout<<"-data_type          pm (default), pm1, pm2,pm3, adc or patter"<<endl
		<<"-dist               set distance cut for event selection. if it not set, hit in the same group are selected"<<endl
		<<"-angle_cut          open angle cut (120 degree by default)"<<endl
		<<"-dist_cut           distance cut, it is 5 mm by default"<<endl
        <<"-source             set the file with source position information"<<endl
        <<"-source             set the file with source position information"<<endl
        <<"-nstore             set the number of event to be stored in the tree."<<endl
        <<"                    0: only store events having coincidence pairs (default),"<<endl
        <<"                    0: store all, >0 store the number of event specified"<<endl
        <<"-print_hits_pos     print hits location only"<<endl
        <<"-n                  number of events to be processed"<<endl
        <<"-ecut               high energy cut for event selection (not used by default)"<<endl
		<<"-thr                use threshold to select hits (default value is 50)"<<endl
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
    TString source_config="";
    TString data_type="pm";
    float threshold=50;
    float dist0=0;
    float distance_cut=300;
    bool  is_hit_dist_cut_set=false;
    int nstore=0;
    float high_energy_cut=0;

    //0 store events having coincidence hit pairs
    //-1  store all events
    //>0  the number of event specified
    
    float angle_cut=0;
    float dist_cut=0;
    bool print_hits_pos=false;

	if(argc==1)
	{
		Help();
		return 0;
	}
    TString command;
    for(int i=0;i<argc;i++)command=command+ " "+argv[i];
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
		else if(sel=="-print_hits_pos")
		{
            print_hits_pos=true;
		}
		else if(sel=="-i")
		{
            ifname=argv[++s];
		}
		else if(sel=="-n")
		{
				str=argv[++s];
				nevents=str.Atoi();
		}
		else if(sel=="-thr")
		{
				str=argv[++s];
				threshold=str.Atof();
		}
        else if(sel=="-dist")
        {
				str=argv[++s];
				dist0=str.Atof();
				str=argv[++s];
				distance_cut=str.Atof();
                is_hit_dist_cut_set=true;
                cout<<"setting distance cut: "<<dist0<<" "<<distance_cut<<endl;
        }
        else if(sel=="-dist_cut")
        {
				str=argv[++s];
				dist_cut=str.Atof();
        }
        else if (sel=="-angle_cut")
        {
				str=argv[++s];
				angle_cut=str.Atof();
        }
        else if(sel=="-nstore")
        {
            str=argv[++s];
            nstore=str.Atoi();
            cout<<"Number of events to be stored: "<<nstore<<endl;
        }
        else if(sel=="-ecut")
        {
            str=argv[++s];
            high_energy_cut=str.Atof();
        }
        else 
        {
            cout<<"Can not understand: " <<sel<<endl;
            Help();
            return 0;
        }
	}

    if(source_config=="")
    {
        cout<<"Source positions are not specified."<<endl;
        Help();
        return 0;
    }
    if(ifname==""||ofname=="")
    {
        cout<<"No input or output"<<endl;
        Help();
        return 0;

    }

    if(high_energy_cut>0&&high_energy_cut<threshold)
    {
        cout<<"invalid high energy cut"<<endl;
        Help();
        return 0;
    }
	Coincidence *run=new Coincidence();
    run->LoadSettings(source_config);
    run->SetDataType(data_type);
    if(is_hit_dist_cut_set)run->SetCoincidenceDistanceBetween(dist0,distance_cut);
    run->SetThreshold(threshold);
    run->SetNStore(nstore);
    if(high_energy_cut>0)run->SetHighEnergyCut(high_energy_cut);
    if(print_hits_pos)run->PrintHitsPosition();
    if(nevents>0)run->SetNEvents(nevents);
    if(angle_cut!=0)run->SetAngleCut(angle_cut);
    if(dist_cut!=0)run->SetDistanceCut(dist_cut);
    run->SetCommand(command);

	run->DoCoincidence(ifname,ofname);

    cout<<"Output: "<<ofname<<endl;


	delete run;
	return 0;
} 

