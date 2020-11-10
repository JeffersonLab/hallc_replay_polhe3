
This repository contains all analysis scripts used for d2n experiment (2020).





***********************************************************************************************************
A1n analysis before starting d2n analysis:

1. May 1st, 2020: git clone from https://eicweb.phy.anl.gov/jlab/hallc/exp/polhe3/hallc_replay.git

2. May 26th, 2020: Created branch murchhana_replays. All A1n analysis work was done in this branch. 

3. May 27th, 2020 : merge request created for the fixed typo in hms replay script h_fadc_debug_param. Merge branch "murchhana_replays" into "master".

4. October 6th, 2020: Pulled "master" to branch "murchhana_replays". 







***********************************************************************************************************
Starting point for d2n analysis:

1. August 18th, 2020: Created new branch murchhana_d2n. All d2n analysis work is being done in this branch.

2. October 12th, 2020: Pulled "master" to branch "murchhana_d2n".

3. Most recent SHMS/HMS run numbers were not present in the standard.kinematics files. Added those run numbers manually to replay required SHMS/HMS runs.


***************** Reference timing cuts ******************

1. Changed all t_shms_TdcTimeWindowMin and t_hms_TdcTimeWindowMin to 0 and t_shms_TdcTimeWindowMax and t_hms_TdcTimeWindowMax to 100000.

2. Did  a 200000 replay for SHMS runs 11395,11396,11398,11358,11369,11210,11217,11498,11538 and HMS runs 4195,4196,4192,4148,4160,3993,3994,4306,4331.

3. The script to check reference time cuts are in /group/c-polhe3/Users/murchhana/d2n_2020/Ref_time_analysis folder.

4. Checked reference time cuts for all those runs and the cuts were determined to be same as A1n.


******************** Detector time window cuts *****************

1. Performed the detector time window check for HMS and SHMS Noble gas Cherenkov.

2. Set the determined reference time cuts in param files.
/group/c-polhe3/Users/murchhana/hallc_replay/PARAM/HMS/GEN/h_reftime_cut.param
/group/c-polhe3/Users/murchhana/hallc_replay/PARAM/SHMS/GEN/p_reftime_cut.param
/group/c-polhe3/Users/murchhana/hallc_replay/PARAM/TRIG/thms.param
/group/c-polhe3/Users/murchhana/hallc_replay/PARAM/TRIG/tshms.param

3. Made the software time window cuts wider fot Cherenkov (pngcer_cuts.param, hCER_cuts.param), Hodoscope (phodo_cuts.param, hhodo_cuts.param), Calorimeter (pcal_cuts.param, hcal_cuts.param), Drift Chamber (pdc_cuts.param, hdc_cuts.param).

4. Did a 200000 replay for SHMS runs 11498,11538,11395,11396,11398,11358,11369,11210,11217 and HMS runs 4306,4331,4195,4192,4196,3994,4148.

5. Junhao, Mingyu and Melanie used these root files to check the Hodoscope, DC and calorimeter time window. I did the Cherenkov time window check.

6. All Cherenkov time window scripts are in /group/c-polhe3/Users/murchhana/d2n_2020/Detector_time_window_cuts/cherenkov folder.

7. The Cherenkov time window cuts were found to be for SHMS/ HMS were same as A1n.


**************** Cherenkov Calibration ******************

1. Set the obtained detector time window cuts were set in hcer_cuts.param/p_ngcer_cuts.param filed in hallc_replay/PARAM folder.

2. Did a 200000 replay of HMS runs 4306,4267,4195,3732 and SHMS run 11538.

3. Both HMS and SHMS calibration scripts are in /group/c-polhe3/Users/murchhana/d2n_2020/cherenkov_calibration folder.

4. HMS Cherenkov calibration was done using both Method 1 and Method 2 and the result differed by significat amount for run 4306. As Method 1 is reliable because we directly fit the single photo electron peak, chose the calibration constant obtained from cosmic run 03732 (lowest background gived better peak location). Set the calibration constant in hcer_calib.param file.

5. SHMS CHerenkov calibration was done using Method 2 only for run 11538.The old calibration constants present in the param file were obtained from run 9643,9644,9645. The new calibration constants obtained from run 11538 agrees well with the old oned for PMT2, PMT 3 and PMT 4. I will update the SHMS calibration constant in the p_ngcer_calib.param file after discussing with others.



