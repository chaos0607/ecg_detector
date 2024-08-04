import os
import numpy as np
import json
import jf_analysis

fs = 360
all_detectors = ["twoaverage", "wqrs","engzee"]
rpeaks_dir = "/home/cc/work/project/code/dataset/MIT-BIH"
anno_files = [f for f in os.listdir(rpeaks_dir) if f.startswith('r_') and f.endswith('.tsv')]

results = {detector: {} for detector in all_detectors}

for detector in all_detectors:
    for anno in anno_files:
        base_name = anno.split('_')[1].split('.')[0]
        rpeaks_file = f"rpeak_{detector}_d1_{base_name}.tsv"
        data_file = f"d1_{base_name}.tsv"
        data_file_path = os.path.join(rpeaks_dir, data_file)

        file_path = os.path.join(rpeaks_dir, rpeaks_file)
        
        if os.path.exists(file_path):
            detected_peaks = np.loadtxt(file_path, delimiter=None)
            data_anno = np.loadtxt(os.path.join(rpeaks_dir, anno), delimiter=None)
            data = np.loadtxt(data_file_path, delimiter="\t")
            jf_result = jf_analysis.evaluate(detected_peaks, data_anno, fs, len(data))
            results[detector][base_name] = jf_result
        else:
            print(f"File {file_path} does not exist.")

    output_file = os.path.join(rpeaks_dir, f"{detector}_results.json")
    with open(output_file, 'w') as f:
        json.dump(results[detector], f, indent=4)

    print(f"Results for {detector} saved to {output_file}")


def calculate_and_plot_statistics(results):
    means = {}
    stds = {}

    for detector, result in results.items():
        jf_values = list(result.values())
        means[detector] = np.mean(jf_values)
        stds[detector] = np.std(jf_values)
        print(f"Detector: {detector}, Mean: {means[detector]}, Std: {stds[detector]}")

    detectors = list(means.keys())
    mean_values = list(means.values())
    std_values = list(stds.values())

    x = np.arange(len(detectors))
    width = 0.35

    fig, ax = plt.subplots()
    ax.bar(x - width/2, mean_values, width, label='Mean')
    ax.bar(x + width/2, std_values, width, label='Std')

    ax.set_xlabel('Detectors')
    ax.set_ylabel('Values')
    ax.set_title('Mean and Std of Different Detectors')
    ax.set_xticks(x)
    ax.set_xticklabels(detectors)
    ax.legend()

    plt.show()

calculate_and_plot_statistics(results)