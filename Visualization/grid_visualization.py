import csv
import matplotlib.pyplot as plt

for i in range(1):
    with open("RESULT_2000.csv", mode='r') as file:
        data = csv.reader(file)
        head: list = next(data)
        id_col = head.index("id")
        x1_col = head.index("originX")
        y1_col = head.index("originY")
        x2_col = head.index("destX")
        y2_col = head.index("destY")
        j = 0
        for row in data:
            id = row[id_col]
            x1 = int(row[x1_col])
            x2 = int(row[x2_col])
            y1 = int(row[y1_col])
            y2 = int(row[y2_col])
            if j <= 2000:
                # plt.plot([x1, x2, x2], [y1, y1, y2], 'b', linewidth=0.5)
                plt.plot(x1, y1, 'ro', markersize=0.5)
                plt.plot(x2, y2, 'ko', markersize=0.5)
            j += 1
        plt.show()
