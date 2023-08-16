


void define_root() {
  
  
  for(int i=0;i<MAX_NUM_DETTYPES;i++)
  {
    TRACE_branch.push_back(std::vector<std::vector<int>>());
    for(int j=0;j<MAX_NUM_DET;j++)
      TRACE_branch[i].push_back(std::vector<int>());

  }
    
  gROOT->ProcessLine("#include <vector>"); 
  tree = new TTree("ids", "Isolde Decay Station Tree");

//  tree->Branch("Multiplicity", &MULT_branch, "MULT/s");
  tree->Branch("Multiplicity", &MULT_branch, "MULT/I");
  tree->Branch("Time_vs_ref", &TIME_REF_branch, "TIME_REF/l");
  tree->Branch("Timestamp", &TIME_RUN_branch, "TIMESTAMP/l"); //l : a 64 bit unsigned integer (ULong64_t)


              
  
    //saving the comment line for naming ROOT branches
  for (i=0; i<detnum; i++)
    if (comment_string[i][0] != '\0' )
      sprintf(root_string[config_coding[i][1]], "%s", comment_string[i]);
    else if (root_string[config_coding[i][1]][0] == '\0')
      sprintf(root_string[config_coding[i][1]], "%d", config_coding[i][1]);
    
    
    
  //Array for each branch
  for (i=1; i<=dettypes; i++) {
    // std::cout<<"dettypes = "<<dettypes<<" i = "<<i<<" name = "<<root_string[i] <<" maxnum = "<<maxnum[i] <<std::endl;

    tree->Branch(Form("Energy_%s", root_string[i]), E_branch[i], Form("E_%s[%d]/D",root_string[i],maxnum[i])); 
     
   for(int j =0;j<maxnum[i];j++)
    {
      TString traceBranchName = Form("Trace_%s_%i", root_string[i],j);
      // std::cout<<Form("branch created for i = %i and j = %i. maxnum[i] = %i and the branch is called: ", i,j,maxnum[i])
        <<traceBranchName<<std::endl;
      tree->Branch(traceBranchName,&TRACE_branch[i][j]);
    }
    tree->Branch(Form(  "Time_%s", root_string[i]), &T_branch[i], Form("T_%s[%d]/D",root_string[i],maxnum[i]));
    tree->Branch(Form(  "Mult_%s", root_string[i]),&M_branch[i], Form("M_%s/I"    ,root_string[i]          ));
    
  }

  //define histograms for each input defined in the config file
  //they are incremented in read_ldf.hh

  hStats = new TH1F("hStats", "Statistics", detnum, 0, detnum);
  for (i = 0; i < detnum; i++)
     h[i] = new TH1F(Form("h%d_%s", i, comment_string[i]),
                     Form("%s_Singles", comment_string[i]),
                     65535, 1, 65536);
  
  for (j = 1; j < (dettypes+1); j++)
     htot[j] = new TH1F(Form("tot2%d_%s", j, root_string[j]),
                     Form("%s_total2", root_string[j]),
                     65535, 1, 65536);
  
     
//  for (j = 1; j < 7; j++)
//     htot[j] = new TH1F(Form("tot%d_%s", j, root_string[j]),
//                     Form("%s_total", root_string[j]),
//                     65535, 1, 65536);
  
}
