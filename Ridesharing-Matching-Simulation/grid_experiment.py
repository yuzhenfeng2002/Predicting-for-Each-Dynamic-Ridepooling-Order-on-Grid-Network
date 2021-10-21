from sklearn.cluster import SpectralClustering
import numpy as np
import pickle

with open(r"C:\Users\admin\Desktop\Prediction-Simulation-Data\INIT_D", "rb") as outfile:
    init_w = pickle.load(outfile)

w = init_w + init_w.T
selected_OD = np.sum(w > 0, axis=0) != 1
w = w[selected_OD, :]
w = w[:, selected_OD]

for N_CLUSTERS in range(2, 10):
    for N_COM in range(N_CLUSTERS, N_CLUSTERS * 10, N_CLUSTERS):
        for ASSIGN_LAB in ['kmeans']:
            clustering = SpectralClustering(
                n_clusters=N_CLUSTERS,
                n_components=N_COM,
                affinity='precomputed', 
                random_state=0, 
                assign_labels=ASSIGN_LAB).fit(w)
            labels = clustering.labels_
            for n in range(N_CLUSTERS):
                if np.sum(labels == n) < 50:
                    print('*', end=',')
                    break
                print(n, np.sum(labels == n), end=',')
            print(np.max(labels))
            
            OD_NUM = np.sum(selected_OD)
            index = np.arange(OD_NUM)[labels == 0]
            for n in range(1, N_CLUSTERS):
                index = np.concatenate((index, np.arange(OD_NUM)[labels == n]))
            
            cut = np.zeros(OD_NUM)
            for n in range(OD_NUM):
                cut[n] = np.sum(w[n, labels != labels[n]])
            cut = cut[index]
            print(N_CLUSTERS, N_COM, ASSIGN_LAB, np.mean(cut))