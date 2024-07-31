#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats
from ecgdetectors import Detectors
import json

experiment_names = ['sitting','maths','walking','hand_bike','jogging']

cs = 'chest_strap_V2_V1'

detectors = Detectors()
det_names = ['two_average_detector', 'wqrs_detector']
print (det_names)
plot_names = ['Elgendi et al (Two average)','WQRS']

resultsdir = "results"

alpha = 0.05

minja = 90 # %

def get_ja(detector_name, leads, experiment):
    f = open(resultsdir+"/cpp_jf_"+detector_name+".json","r")
    js = f.read()
    data = json.loads(js)
    s = []
    for i in data[leads][experiment]:
        if i["ja"]:
            s.append(i["ja"]*100)
    return np.array(s)


def get_result(det, leads, experiment):
    
    m = []
    s = []
    for det in det_names:
        print(det,experiment,get_ja(det, leads, experiment))
        m.append(np.mean(get_ja(det, leads, experiment)))
        s.append(np.std(get_ja(det, leads, experiment)))

    return np.array(m),np.array(s)


def print_stat(p):
    if p == None:
        print('--- & ',end='')
        return
    s = ""
    if p < alpha:
        s = "*"
    print('{:03.2f}{} & '.format(p,s),end='')

    
def calc_stats(leads, experiment):
    print("Stats:",leads, experiment)
    print("      & ",end='')
    for det1 in det_names:
        print(det1," & ",end='')
    print("\\\\")
    for det1 in det_names:
        r1 = get_ja(det1, leads, experiment)
        t,p = stats.ttest_1samp(r1,minja,alternative='greater')
        print_stat(p)
    print()

    
def double_plot(data1, std1, data2, std2,data3, std3,data4, std4, data5,std5,  y_label, legend1, legend2, legend3,legend4,legend5,title=None):
    fig, ax = plt.subplots()
    x_pos = np.arange(len(plot_names))

    fig.set_size_inches(10, 7)
    width = 0.1
    rects1 = ax.bar(x_pos, data1, width, yerr=std1, alpha=0.5, ecolor='black', capsize=10)
    rects2 = ax.bar(x_pos+width, data2, width, yerr=std2, alpha=0.5, ecolor='black', capsize=10)
    rects3 = ax.bar(x_pos+2*width, data3, width, yerr=std3, alpha=0.5, ecolor='black', capsize=10)
    rects4 = ax.bar(x_pos+3*width, data4, width, yerr=std4, alpha=0.5, ecolor='black', capsize=10)
    rects5 = ax.bar(x_pos+4*width, data5, width, yerr=std5, alpha=0.5, ecolor='black', capsize=10)
    ax.set_ylim([0,150])
    ax.set_ylabel(y_label)
    ax.set_xlabel('Detector')
    ax.set_xticks(x_pos + width / 2)
    ax.set_xticklabels(plot_names)
    ax.legend((rects1[0], rects2[0],rects3[0],rects4[0],rects5[0]), (legend1, legend2, legend3,legend4,legend5))

    if title!=None:
        ax.set_title(title)

    plt.tight_layout()

    return rects1, rects2, rects3, rects4, rects5


def print_result(title,data,std,legend):
    print("JF Score of cpp detectors:",title)
    for i in zip(legend,data,std):
        print("{}: {:1.1f}+/-{:1.1f}".format(i[0],i[1],i[2]))
    print()


cs_sitting_avg,cs_sitting_std = get_result(det_names, cs, 'sitting')
cs_maths_avg,cs_maths_std = get_result(det_names, cs, 'maths')
cs_walking_avg,cs_walking_std = get_result(det_names, cs, 'walking')
cs_hand_bike_avg,cs_hand_bike_std = get_result(det_names, cs, 'hand_bike')
cs_jogging_avg,cs_jogging_std = get_result(det_names, cs, 'jogging')


print_result('sitting chest strap',cs_sitting_avg,cs_sitting_std,det_names)
print_result('maths chest strap',cs_maths_avg,cs_maths_std,det_names)
print_result('walking chest strap',cs_walking_avg,cs_walking_std,det_names)
print_result('hand bike chest strap',cs_hand_bike_avg,cs_hand_bike_std,det_names)
print_result('jogging chest strap',cs_jogging_avg,cs_jogging_std,det_names)



double_plot(cs_sitting_avg, cs_sitting_std,
            cs_maths_avg, cs_maths_std,
            cs_walking_avg, cs_walking_std,
            cs_hand_bike_avg, cs_hand_bike_std,
            cs_jogging_avg, cs_jogging_std,
            'JF (%)', 'Sitting', 'maths','walking','hand_bike',   'Jogging', 'Chest strap')

print()

calc_stats(cs,"sitting")
calc_stats(cs,"maths")
calc_stats(cs,"walking")
calc_stats(cs,"hand_bike")
calc_stats(cs,"jogging")


plt.show()
