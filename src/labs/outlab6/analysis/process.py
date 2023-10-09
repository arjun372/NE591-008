import os
import json
import pandas as pd


def read_json_files(directory="../results/circuit_matrix"):
    json_files = [pos_json for pos_json in os.listdir(directory) if pos_json.endswith('.json')]
    json_data = []
    for index, js in enumerate(json_files):
        with open(os.path.join(directory, js)) as json_file:
            json_data.append(json.load(json_file))
    return json_data


data = read_json_files()
dataframe = pd.DataFrame()

for item in data:
    inputs = item['inputs']
    order = item['inputs']['order']
    lup_error = item['outputs']['LUP']['l2-error']
    lup_residual = item['outputs']['LUP']['max-residual']
    for method, outputs in item['outputs'].items():
        row = dict()
        row['method'] = method  # Add method name
        row['order'] = order
        row['iterations'] = item['outputs'][method]['iterations']['actual']
        row['iterative-error'] = item['outputs'][method]['iterative-error']['actual']
        row['l2-error'] = format(item['outputs'][method]['l2-error'], '.15e')
        row['max-residual'] = format(item['outputs'][method]['max-residual'], '.15e')
        row['time'] = item['outputs'][method]['wall-time-ns']['mean'] / 1.0e6
        line = pd.Series(row)
        dataframe = pd.concat([dataframe, pd.DataFrame(line).T], ignore_index=True)

dataframe = dataframe.sort_values(by=['method', 'order'])

grouped = dataframe.groupby('method')

LUP = grouped.get_group('LUP')
PJ = grouped.get_group('point-jacobi')
SORJ = grouped.get_group('SORJ')
GS = grouped.get_group('gauss-seidel')
SOR = grouped.get_group('SOR')
SSOR = grouped.get_group('SSOR')

for method, group in grouped:
    print(group)

print(LUP)
