#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
This code will process the rpeak data from local files,which is genetated by the c++ ecg detectors. 
this is only copy, it should be run under project:https://github.com/berndporr/ECG-GUDB
"""

import sys
import os
import numpy as np
import json
from ecg_gudb_database import GUDb
from ecgdetectors import Detectors
import pathlib # For local file use
from multiprocessing import Process
import time
# The JA analysis for a detector
import jf_analysis

# directory where the results are stored
resultsdir = "results"
try:
    os.mkdir(resultsdir)
except OSError as error:
    pass

# Get the sampling rate
fs = GUDb.fs

# Get an instance of all detectors
detectors = Detectors(fs)

current_dir = pathlib.Path(__file__).resolve()

# Detectors, recording leads and experiments can be added/removed from lists as required
all_recording_leads=["chest_strap_V2_V1"] # can be expanded if required
all_experiments = ["sitting","maths","walking","hand_bike","jogging"]

def detected_peaks_default(obj):
    if isinstance(obj, np.ndarray):
        return obj.tolist()
    raise TypeError("Object of type '%s' is not JSON serializable" % type(obj).__name__)

def evaluate_detector(detector):

    detectorname = detector[1].__name__
    detectorfunc = detector[1]
    
    print("Processing:",detector[0])

    analysed=0 # overall count of analysed subjects

    jf_leads = {} # initialise for data to be saved by lead and detector
    detected_peaks_save = {}

    for record_lead in all_recording_leads: # loop for all chosen leads
        
        jf_experiments = {}
        detected_peaks_save[record_lead] = {}
        
        for experiment in all_experiments: # loop for all chosen experiments
            if experiment not in detected_peaks_save[record_lead]:
                detected_peaks_save[record_lead][experiment] = {}
            jf_subjects=[]
            
            for subject_number in range(0, 25): # loop for all subjects
                
                print("Analysing subject {}, {}, {}, {}".format(subject_number, experiment, record_lead, detector[0]))
    
                # creating class which loads the experiment
        
                # For online GUDB access
                ecg_class = GUDb(subject_number, experiment)
            
                # For local GUDB file access:
                # from ecg_gla_database import Ecg # For local file use
                # data_path = str(pathlib.Path(__file__).resolve().parent.parent/'experiment_data')
                # ecg_class = Ecg(data_path, subject_number, experiment)
                
                # getting the raw ECG data numpy arrays from class
                chest_strap_V2_V1 = ecg_class.cs_V2_V1
                einthoven_i = ecg_class.einthoven_I
                einthoven_ii = ecg_class.einthoven_II
                einthoven_iii = ecg_class.einthoven_III
        
                # getting filtered ECG data numpy arrays from class
                ecg_class.filter_data()
                chest_strap_V2_V1_filt = ecg_class.cs_V2_V1_filt
                einthoven_i_filt = ecg_class.einthoven_I_filt
                einthoven_ii_filt = ecg_class.einthoven_II_filt
                einthoven_iii_filt = ecg_class.einthoven_III_filt
            
                data=eval(record_lead) # set data array (i.e. recording to be processed)
               
                if 'chest' in record_lead:
                    if ecg_class.anno_cs_exists:
                        data_anno = ecg_class.anno_cs
                        exist=True
                        analysed=analysed+1
                    else:
                        exist=False
                        print("No chest strap annotations exist for subject %d, %s exercise" %(subject_number, experiment))
                else:
                    if ecg_class.anno_cables_exists:
                        data_anno = ecg_class.anno_cables
                        exist=True
                        analysed=analysed+1
                    else:
                        exist=False
                        print("No cables annotations exist for subject %d, %s exercise" %(subject_number, experiment))
                        
                #%% Detection
        
                ### Applying detector to each subject ECG data set then correct for mean detector
                # delay as referenced to annotated R peak position
                # Note: the correction factor for each detector doesn't need to be exact,
                # but centres the detection point for finding the nearest annotated match
                # It may/will be different for different subjects and experiments
                if detectorname == "two_average_detector":
                    rpeaks_file = "rpeaks_twoaverage.tsv"
                elif detectorname == "wqrs_detector":
                    rpeaks_file = "rpeaks_wqrs.tsv"
                elif detectorname == "engzee_detector":
                    rpeaks_file = "rpeaks_engzee.tsv"

                rpeaks_dir = "/home/cc/work/project/code/my-ecg-detector-test-data/experiment_data"
                file_path= rpeaks_dir + "/" + ("subject_%02d" % subject_number) + "/"  + experiment + "/"+ rpeaks_file
        
                if exist==True: # only proceed if an annotation exists
                    detected_peaks = np.loadtxt(file_path, delimiter="\n") # load detected peaks from file
                    detected_peaks_save[record_lead][experiment][subject_number] = detected_peaks

                    jf_result = jf_analysis.evaluate(detected_peaks, data_anno, fs, len(data)) # perform interval based analysis
                    jf_subjects.append(jf_result)
                    
            # ^ LOOP AROUND FOR NEXT SUBJECT

            jf_experiments[experiment] = jf_subjects
                        
        # ^ LOOP AROUND FOR NEXT EXPERIMENT
        
        # Add data for analysis by lead to (full array) 'data_det_lead' dictionary
        
        jf_leads[record_lead] = jf_experiments
        
    # ^ LOOP AROUND FOR NEXT LEAD
    serialized_data = json.dumps(jf_leads,indent="\t")
    
    f = open(resultsdir+"/cpp_jf_"+detectorname+".json","w")
    f.write(serialized_data)
    f.close

    
    f = open(resultsdir+"/cpp_detected_peaks"+detectorname+".json","w")
    detected_peaks_data = json.dumps(detected_peaks_save,indent=4,default=detected_peaks_default)
    f.write(detected_peaks_data)
    f.close


def main():
        evaluate_detector(detectors.detector_list[0])
        evaluate_detector(detectors.detector_list[7])
        #valuate_detector(detectors.detector_list[3])



if __name__ == '__main__':
    main()