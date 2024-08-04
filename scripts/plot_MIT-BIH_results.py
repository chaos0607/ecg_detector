import os
import numpy as np
import json
import matplotlib.pyplot as plt

rpeaks_dir = "/home/cc/work/project/code/dataset/MIT-BIH"
all_detectors = ["twoaverage", "wqrs", "engzee"]

def load_results_from_file(detector):
    file_path = os.path.join(rpeaks_dir, f"{detector}_results.json")
    if os.path.exists(file_path):
        with open(file_path, 'r') as f:
            return json.load(f)
    else:
        print(f"File {file_path} does not exist.")
        return {}

def calculate_and_plot_statistics():
    means = {"sensitivity": [], "ppv": [], "ja": []}
    stds = {"sensitivity": [], "ppv": [], "ja": []}

    for detector in all_detectors:
        results = load_results_from_file(detector)
        sensitivities = []
        ppvs = []
        ja_values = []

        for value in results.values():
            if "TP" in value and "FN" in value and "FP" in value:
                TP = value["TP"]
                FN = value["FN"]
                FP = value["FP"]
                sensitivity = TP / (TP + FN) if (TP + FN) > 0 else 0
                ppv = TP / (TP + FP) if (TP + FP) > 0 else 0
                sensitivities.append(sensitivity)
                ppvs.append(ppv)
            if "ja" in value:
                ja_values.append(value["ja"])

        if sensitivities:
            means["sensitivity"].append(np.mean(sensitivities))
            stds["sensitivity"].append(np.std(sensitivities))
        if ppvs:
            means["ppv"].append(np.mean(ppvs))
            stds["ppv"].append(np.std(ppvs))
        if ja_values:
            means["ja"].append(np.mean(ja_values))
            stds["ja"].append(np.std(ja_values))

        print(f"Detector: {detector}, Mean Sensitivity: {means['sensitivity'][-1]}, Std Sensitivity: {stds['sensitivity'][-1]}")
        print(f"Detector: {detector}, Mean PPV: {means['ppv'][-1]}, Std PPV: {stds['ppv'][-1]}")
        print(f"Detector: {detector}, Mean ja: {means['ja'][-1]}, Std ja: {stds['ja'][-1]}")

    detectors = all_detectors
    x = np.arange(len(detectors))
    width = 0.2

    fig, ax = plt.subplots()
    fig.set_size_inches(12, 8)

    rects1 = ax.bar(x - width, means["sensitivity"], width, yerr=stds["sensitivity"], label='Mean Sensitivity', alpha=0.5, ecolor='black', capsize=10)
    rects2 = ax.bar(x, means["ppv"], width, yerr=stds["ppv"], label='Mean PPV', alpha=0.5, ecolor='black', capsize=10)
    rects3 = ax.bar(x + width, means["ja"], width, yerr=stds["ja"], label='Mean ja', alpha=0.5, ecolor='black', capsize=10)

    ax.set_xlabel('Detectors')
    ax.set_ylabel('Values')
    ax.set_title('Mean and Std of Sensitivity, PPV, and ja for Different Detectors')
    ax.set_xticks(x)
    ax.set_xticklabels(detectors)
    ax.legend()

    def add_labels(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate(f'{height:.2f}',
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom')

    add_labels(rects1)
    add_labels(rects2)
    add_labels(rects3)

    plt.tight_layout()
    plt.show()

calculate_and_plot_statistics()