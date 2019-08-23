void r() {
   TFile f("response.root");
   TString reg = "*";
   TRegexp re(reg,kTRUE);
   TIter next(f.GetListOfKeys());
   TKey *key;
   while ((key= (TKey*)next())) {
      TString st = key->GetName();
      key->Print();
   }
}
