import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import argparse

parser = argparse.ArgumentParser(description='Analyze and plot data distribution.')
parser.add_argument('filename', type=str, help='The name of the file containing the data')

args = parser.parse_args()

data = np.loadtxt(args.filename)

mean = np.mean(data)
median = np.median(data)
std_dev = np.std(data)

print(f"Mean: {mean}")
print(f"Median: {median}")
print(f"Standard Deviation: {std_dev}")

plt.figure(figsize=(12, 6))

sns.histplot(data, kde=True, bins=30, color='blue')

plt.title('Data Distribution')
plt.xlabel('Value')
plt.ylabel('Frequency')

plt.show()