import csv
import glob
import os

csv_files = glob.glob('*.csv')

for csv_file in csv_files:
    base_name = os.path.splitext(csv_file)[0]
    mlii_file_name = f'd2_{base_name}.tsv'

    with open(csv_file, 'r') as csvfile, \
         open(mlii_file_name, 'w') as mlii_file:
        
        reader = csv.reader(csvfile)
        mlii_writer = csv.writer(mlii_file, delimiter='\t')
        
        sample_number = 0

        next(reader)
        next(reader)
        
        for row in reader:
            sample_number += 1
            mlii_writer.writerow([row[3]])