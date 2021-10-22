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

'''
try:
    d = np.sum(w, axis=0)
    l = np.diag(d) - w
    lam, u = np.linalg.eig(l)
    plt.figure()
    plt.plot(np.sort(lam)[:20])
    plt.show()
    plt.figure()
    c = u[:, np.argsort(lam, axis=0)]
    for i in range(10):
        plt.plot(c[:, i])
    plt.show()
except:
    pass
'''

# np.where(u[:, 0] == 0)
# (array([8697, 8953, 9415], dtype=int64),)

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

for n in range(CLUSTER_NUM):
    new_OD_pairs = OD_pairs[labels == n, :]
    np.savetxt(r"C:\Users\admin\Desktop\Error-Test-Data-SC\OD_Cluster_" + str(n) + ".csv", new_OD_pairs, fmt=('%d', '%d', '%d', '%d', '%d', '%.2f'), delimiter=',', header="id, originX, originY, destX, destY, lambda", comments='')

