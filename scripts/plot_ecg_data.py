import numpy as np
import matplotlib.pyplot as plt

ecg_data = np.loadtxt('../output/filtered_ecg.txt')

plt.figure(figsize=(10, 4))
plt.plot(ecg_data, label='ECG Data')
plt.title('ECG Signal')
plt.xlabel('Sample')
plt.ylabel('Amplitude')
plt.legend()
plt.grid(True)
plt.show()