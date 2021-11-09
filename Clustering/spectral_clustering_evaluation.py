"""
codes here are for evaluation of global and local prediction
"""
from sklearn.cluster import SpectralClustering
import numpy as np
import csv
import pickle
import matplotlib.pyplot as plt


with open(r"C:\Users\admin\Desktop\Prediction-Simulation-Data\W_INIT_10000", "rb") as outfile:
    init_w = pickle.load(outfile)
init_w = init_w[:, 1:]
w = init_w + init_w.T
us_OD_idx = [7736, 7859, 8379, 8436, 8667, 8702, 8720, 8959, 9357, 9422, 9464, 9948]
selected_OD = np.sum(w > 0, axis=0) != 1
selected_OD[us_OD_idx] = False
w = w[selected_OD, :]
w = w[:, selected_OD]
print(np.mean(np.sum(w > 0, axis = 0)))

CLUSTER_NUM = 5
clustering = SpectralClustering(
    n_clusters=CLUSTER_NUM,
    n_components=CLUSTER_NUM,
    affinity='precomputed', 
    random_state=0, 
    assign_labels='kmeans').fit(w)
labels = clustering.labels_
for n in range(CLUSTER_NUM):
    print(n, np.sum(labels == n))

OD_NUM = np.sum(selected_OD)
OD_filepath = r"C:\Users\admin\Desktop\Prediction-Simulation-Data\OD_1e4.csv"
OD_pairs = np.zeros((OD_NUM, 6))
with open(OD_filepath, mode='r') as file:
    data = csv.reader(file)
    head = next(data)
    id_col = head.index("id")
    x1_col = head.index("originX")
    y1_col = head.index("originY")
    x2_col = head.index("destX")
    y2_col = head.index("destY")
    lam_col = head.index("lambda")
    i = j = 0
    for row in data:
        if selected_OD[i]:
            x1 = int(row[x1_col])
            x2 = int(row[x2_col])
            y1 = int(row[y1_col])
            y2 = int(row[y2_col])
            lam = float(row[lam_col])
            OD_pairs[j, :] = np.array([[j, x1, y1, x2, y2, lam]])
            j += 1
        i += 1

OD_filepath = r"C:\Users\admin\Desktop\Prediction-Simulation-Data\RESULT_1e4_1e-5.csv"
global_predict_result = np.zeros((OD_NUM, 3))
with open(OD_filepath, mode='r') as file:
    data = csv.reader(file)
    head = next(data)
    pw_col = head.index("Pw")
    lw_col = head.index("Lw")
    ew_col = head.index("ew")
    i = j = 0
    for row in data:
        if selected_OD[i]:
            pw = float(row[pw_col])
            lw = float(row[lw_col])
            ew = float(row[ew_col])
            global_predict_result[j, 0] = pw
            global_predict_result[j, 1] = lw
            global_predict_result[j, 2] = ew
            j += 1
        i += 1
        
predict_result = np.zeros((OD_NUM, 3))
j = 0
for n in range(CLUSTER_NUM):
    with open(r"C:\Users\admin\Desktop\Error-Test-Data-SC\OD_Cluster_Result_" + str(n) + ".csv", mode='r') as file:
        data = csv.reader(file)
        head = next(data)
        pw_col = head.index("Pw")
        lw_col = head.index("Lw")
        ew_col = head.index("ew")
        for row in data:
            pw = float(row[pw_col])
            lw = float(row[lw_col])
            ew = float(row[ew_col])
            predict_result[j, 0] = pw
            predict_result[j, 1] = lw
            predict_result[j, 2] = ew
            j += 1
            
index = np.arange(OD_NUM)[labels == 0]
for n in range(1, CLUSTER_NUM):
    index = np.concatenate((index, np.arange(OD_NUM)[labels == n]))
    
global_predict_result = global_predict_result[index, :]
predict_result = predict_result[global_predict_result[:, 1] != 0, :]
global_predict_result = global_predict_result[global_predict_result[:, 1] != 0, :]
rel_error = np.abs(global_predict_result - predict_result) / global_predict_result * 100


pw_error = np.sort(rel_error[:, 0])
ew_error = np.sort(rel_error[:, 1])
lw_error = np.sort(rel_error[:, 2])
plt.figure(figsize=(6.4 * 3, 6.4), dpi=200)
plt.violinplot([pw_error[:], ew_error[:], lw_error[:]], positions=[0, 2, 4], showmeans=True, showextrema=True, showmedians=True, widths=2)
plt.xticks(np.arange(3) * 2, ['pw', 'lw', 'ew'])
plt.ylabel("Relative error /%")
plt.show()
