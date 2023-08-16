# xia4ids
 XIA DGF Pixie-16 .ldf data file converter into ROOT and GASPware format for IDS (The IDS Collaboration https://isolde-ids.web.cern.ch/ ) 

This is a modified version of the code written by Razvan Lica and Khai Phan which can be found at https://github.com/rlica/xia4ids

Parts of this code were modified from Pixie Acquisition and Analysis Software Suite (PAASS) https://github.com/pixie16/paass, which is licensed under the GNU GPL v. 3.0. In particular, some classes are modified and adapted from PAASS of branch 'dev' at https://github.com/pixie16/paass/tree/dev/Analysis/ScanLibraries.



## Author
 * Chris Page, University of York, chris.page@york.ac.uk

## Installation and running
 1. Download via github.com or using the command line  
 `git clone https://github.com/rlica/xia4ids`
 2. Compile with `make`.
 3. [optional] Add in `$HOME/.bashrc` or `$HOME/.profile`       
 `PATH=$PATH:/your_path_here/xia4ids/bin/`
 4. Run: `xia4ids config_file calibrationFile[optional]`
 5. Enjoy!


## Getting Started
### The 'xia4ids' converter requires a configuration file in which the user specifies:
 * runName  = location of raw data including the name of the runs, not including the numbers
 * timegate = coincidence timegate
 * cs-tac, pair-tac, flagtype = specific detector types (only for GASPware - for ROOT set them to zero)
 * reftype  = the reference type (proton pulse)
 * ref_unit  = the unit for the event time versus the reference (Low Resolution Time)
 * run_unit  = the unit for the timestamp
 * Format = the format in which to convert the data 
      * gasp = GASPware
      * root = ROOT
      * list = binary event lists
      * stat = print statistics for the entire run, the event builder will be skipped
      * rate = only print statistics for the last buffer, the event builder will be skipped
 * Fold = the number of coincidences that will trigger an event 
 * The detector configuration depending on the selected output format (see below).
 * Correlation mode (optional) - the program will skip the event builder and will only histogram the time
 differences between the selected modules and channels. 


See the 'etc' folder for some examples of different configuration and calibration files.

To test the code, a raw data file from 152Eu source and configuration file cand be downloaded from: 
https://cernbox.cern.ch/index.php/s/1C5pXrtTSCdneZm


Xia4ids Kashyyk - 16/08/2023

This version of the sort code can be used to include a branch in the ROOT tree for trace. Onboard CFD time is also included in the HRT branch
(eg. Time_LaBr/T_LaBr, Time_clov/T_clov etc) so this data type has been changed to be a double. This sort code version has the HRT for 500MHz modules
implemented correctly. No tests have been done with this version to sort into formats other than root (eg. GASP) although these are unlikely to be affected
by the changes made. No tests have been done to combine 500MHz and 250 MHz modules either.

Traces are stored as a vector. See plotTracesVec.c for an example of how to use these.
The length of this vector varies: if no trace was recorded, the length of this vector is zero, but when
traces were recorded the length of this vector is an integer N, multiplied by TRACE_LENGTH[MODULE][CHANNEL]/MODULE_FREQUENCY. TRACE_LENGTH is
the value written to pixie in poll2 which is is micro seconds. The factor of N comes from when multiple events occur during the prompt time window
given in the config file. For example, if a two gamma rays are measured in the same detector 490 ticks apart with "timegate 500" in the config file,
traces for the second event will be appended to the end of the vector immediately after the end of the first trace. The times of these two event relative
to each other does NOT correspond to the time between the traces.

If different detectors have different TRACE_LENGTH values, the code will automatically make each the correct length but
care should be taken in any analysis macros.

To disable the inclusion of the onboard CFD time for 500 MHz modules, remove/comment out lines 299-301 of Unpacker.cpp. This was found
to be needed when calculating the CFD from traces offline.

The sort code example root files and macros were written during the 500MHz pixie tests in 2023. The run used was run 60 
which is backed up to eos in:

/eos/experiment/ids/2023/500MHz/RAW/

In this run, a 60Co source was placed roughly in the middle of two LaBrs. The DYN outputs were connected to Mod 5 channels 4 & 5
while the SIG outputs were connected to Mod 5 channels 6 & 7. Module 5 was a 500 MHz, 14 bit pixie module.

Known issues:
- Memory leak in the event builder related to the trace vector can lead to memory usage increasing over time.This is usually only a problem when sorting large amounts of data (> ~20 GB) per run but is something that should be investigated.


for questions, contact Chris Page: chris.page@cern.ch / chris.page@york.ac.uk


