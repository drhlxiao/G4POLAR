#include <TFile.h>  
#include <TTree.h>  
#include <TNamed.h>  
#include <iostream>
#include <string>    
using namespace std;
void Help()
{
    cout<<"A program to read and print simulation conditions"<<endl;
    cout<<"readMacros <FILENAME>"<<endl;
    
}
int main(int argc, char *argv[])
{

    if(argc!=2)
    {
        Help();
        return 0;
    }
    TFile f(argv[1]);
    if(!f.IsOpen())
    {
        cout<<"can not open the file"<<endl;
        return 0;
    }
    TNamed *macros=(TNamed*)f.Get("macros");
    if(!macros)
    {
        cout<<"can not read the macros"<<endl;
    }
    cout<<macros->GetTitle()<<endl;

    f.Close();

     
    return 0;
} 


