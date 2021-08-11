import csv
import matplotlib.pyplot as plt

t, l, rho, ps, pt = [], [], [], [], []
with open("ITERATION_RESULT.csv", mode='r') as file:
    data = csv.reader(file)
    head: list = next(data)
    time_col = head.index("time")
    x1_col = head.index("lambda")
    x2_col = head.index("rho")
    x3_col = head.index("p_s")
    x4_col = head.index("p_t")
    for row in data:
        id = row[time_col]
        x1 = float(row[x1_col])
        x2 = float(row[x2_col])
        x3 = float(row[x3_col])
        x4 = float(row[x4_col])
        t.append(int(id))
        l.append(x1); rho.append(x2); ps.append(x3); pt.append(x4)
p1 = plt.plot(t, l, label="lambda")
p2 = plt.plot(t, rho, label="rho")
p3 = plt.plot(t, ps, label="ps")
p4 = plt.plot(t, pt, label="pt")
plt.legend()
plt.show()
