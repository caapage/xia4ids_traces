
void plotTracesVec()
{
      gROOT->ProcessLine("#include <vector>"); 

    TFile *input = new TFile("./Run060.root","read");
    TTree *ids = (TTree *)input->Get("ids");
    double eLaBr[2];
    int tLaBr[2];
    
    std::vector<int> *traceBranch1=0;
    std::vector<int> *traceBranch2=0;

    ids->SetBranchAddress("Trace_LaBr_0",&traceBranch1);//there is now a separate branch for each detector's traces.
    ids->SetBranchAddress("Trace_LaBr_1",&traceBranch2);// this was  easier to implement than a single branch per det type
    ids->SetBranchAddress("Time_LaBr",&tLaBr);
    ids->SetBranchAddress("Energy_LaBr",&eLaBr);
    TH1F *singles1 = (TH1F*)input->Get("h0_LaBr");
    TH1F *singles2 = (TH1F*)input->Get("h1_LaBr");

    TH2F *h1 = new TH2F( "labr1" , "labr1" ,300,-100,200,5000,-1e3,49e3);
    TH2F *h2 = new TH2F( "labr2" , "labr2" ,300,-100,200,5000,-1e3,49e3);
    TH1F *sizeHis = new TH1F("sizes","sizes", 1200,-200,1000);
    double progress =0.;
    ULong64_t nEntries = ids->GetEntries();
    cout<<"nentries = "<<nEntries<<endl;
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    for(int i =0; i< ids->GetEntries();i++)
    {
        ids->GetEntry(i);
        sizeHis->Fill(traceBranch1->size());
        if(eLaBr[0]>1)//play with these gates!!!
        {
        if (traceBranch1->size()>150)
        {
            for (int j =0;j<traceBranch1->size();j++)
                h1->Fill(j,(double)(traceBranch1->at(j)));//-traces[0][traceSize-1]));There seems to be some pileup so this doesn't help!
        }
        }

        if(eLaBr[1]>1)
            for (int j =0;j<traceBranch2->size();j++)
                h2->Fill(j,(double)(traceBranch2->at(j)));//-traces[1][traceSize-1]));There seems to be some pileup so this doesn't help!


        if (i%(nEntries/100)==0)
        {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_time = current_time - start_time;
            auto remaining_time = elapsed_time / (i+1) * (nEntries - (i+1));
            auto secs = std::chrono::duration_cast<std::chrono::seconds>(remaining_time).count();
            auto mins = secs / 60;
            secs = secs % 60;
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
            auto elapsed_minutes = elapsed_seconds / 60;
            elapsed_seconds = elapsed_seconds % 60;

            int barWidth = 70;
            cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos) cout << "=";
                else if (i == pos) cout << ">";
                else cout << " ";
            }
            progress = float(i)/nEntries;
            cout << "] " << int(progress * 100.0) << "%" << " Progress: " <<i << "/" << nEntries <<" in " <<elapsed_minutes << ":" << elapsed_seconds << "  Estimated Time Left:" << mins << ":" << secs <<"\r";
            
            cout.flush();
        }


    }

    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = current_time - start_time;
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
    auto elapsed_minutes = elapsed_seconds / 60;
    elapsed_seconds = elapsed_seconds % 60;    
            
    TString outputName = "traces_out_run_60.root";
    cout<<"\nfinished in "<<elapsed_minutes << ":" << elapsed_seconds <<". Data written to\n"<<outputName<<endl;
    TFile *out = new TFile(outputName,"recreate");

    out->cd();
    h1->Write();
    h2->Write();
    sizeHis->Write();
    singles1->Write();
    singles2->Write();
    out->Close();
    
    
}
