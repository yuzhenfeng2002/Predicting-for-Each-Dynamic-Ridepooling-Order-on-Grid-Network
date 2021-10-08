import simpy
import random
import csv
import numpy as np

OD_NUM = 2000
MAX_RUNTIME = 1000
OD_pairs = []
OD_filepath = "/Users/fxb/Desktop/拼车预测实验/OD_2000.csv"
output_filepath = "/Users/fxb/Desktop/拼车预测实验/SIMU_RESULT_2000.csv"
with open(OD_filepath, mode='r') as file:
    data = csv.reader(file)
    head = next(data)
    id_col = head.index("id")
    x1_col = head.index("originX")
    y1_col = head.index("originY")
    x2_col = head.index("destX")
    y2_col = head.index("destY")
    lam_col = head.index("lambda")
    j = 0
    for row in data:
        x1 = int(row[x1_col])
        x2 = int(row[x2_col])
        y1 = int(row[y1_col])
        y2 = int(row[y2_col])
        lam = float(row[lam_col])
        OD_pairs.append((x1, y1, x2, y2, lam))
        j += 1
        if j >= OD_NUM:
             break

class Network:
    def __init__(self, env, OD_pairs):
        self.env = env
        self.OD_pairs = OD_pairs
        self.passengers = {}
        self.passenger_num = 0
        self.PICKUP_TIME = 1
        self.MAX_DETOUR_TIME = 5
        self.SEARCH_RADIUS = 5
        self.SPEED = 1
        self.generate_OD_of_network()
        self.record = np.zeros((len(OD_pairs), len(OD_pairs) + 1))
    
    def generate_OD_of_network(self):
        for OD_id in range(len(self.OD_pairs)):
            self.env.process(self.generate_passengers_of_OD(OD_id))

    def generate_passengers_of_OD(self, OD_id):
        while True:
            lam = self.OD_pairs[OD_id][4]
            if lam != 0:
                yield self.env.timeout(random.expovariate(lam))
                self.env.process(self.passenger(OD_id, self.env.now))

    def distance(self, x1, y1, x2, y2):
        return abs(x1 - x2) + abs(y1 - y2)
    
    def passenger(self, OD_id, start_time):
        x1 = self.OD_pairs[OD_id][0]
        y1 = self.OD_pairs[OD_id][1]
        x2 = self.OD_pairs[OD_id][2]
        y2 = self.OD_pairs[OD_id][3]
        x = x1
        y = y1
        passenger_id = self.passenger_num
        self.passenger_num += 1
        if passenger_id % 5000 == 0:
            print(passenger_id, start_time)
        self.record[OD_id, 0] += 1
        self.passengers[passenger_id] = [OD_id, start_time, 0, x, y] # status: 0, seeker; 1, taker; 2, matched
        for id, passenger in list(self.passengers.items()):
            if passenger[2] != 1:
                continue
            taker_x = passenger[3] # x
            taker_y = passenger[4] # y
            distance = abs(x - taker_x) + abs(y - taker_y)
            if distance > self.SEARCH_RADIUS:
                continue
            taker_OD_id = passenger[0]
            taker_origin_x = self.OD_pairs[taker_OD_id][0]
            taker_origin_y = self.OD_pairs[taker_OD_id][1]
            taker_dest_x = self.OD_pairs[taker_OD_id][2]
            taker_dest_y = self.OD_pairs[taker_OD_id][3]
            dist_taker = self.distance(taker_origin_x, taker_origin_y, taker_dest_x, taker_dest_y)
            dist_seeker = self.distance(x1, y1, x2, y2)
            dist_FOFO_taker = self.distance(taker_x, taker_y, taker_origin_x, taker_origin_y) + self.distance(taker_x, taker_y, x1, y1) + self.distance(x1, y1, taker_dest_x, taker_dest_y)
            dist_FOFO_seeker = self.distance(x1, y1, taker_dest_x, taker_dest_y) + self.distance(x2, y2, taker_dest_x, taker_dest_y)
            dist_FOLO_taker = self.distance(taker_x, taker_y, taker_origin_x, taker_origin_y) + self.distance(taker_x, taker_y, x1, y1) + self.distance(x1, y1, x2, y2) + self.distance(x2, y2, taker_dest_x, taker_dest_y)
            dist_FOLO_seeker = self.distance(x1, y1, x2, y2)
            if min(max(dist_FOFO_seeker - dist_seeker, dist_FOFO_taker - dist_taker), max(dist_FOLO_seeker - dist_seeker, dist_FOLO_taker - dist_taker)) > self.MAX_DETOUR_TIME * self.SPEED:
                continue
            else:
                # print("A passenger of OD", OD_id, "matches with another of OD", taker_OD_id)
                self.record[OD_id, taker_OD_id + 1] += 1
                self.record[taker_OD_id, OD_id + 1] += 1
                self.passengers.pop(id)
                self.passengers.pop(passenger_id)
                return
        self.passengers[passenger_id] = [OD_id, start_time, 1, x, y]
        yield self.env.timeout(self.PICKUP_TIME)
        while passenger_id in self.passengers.keys() and x != x2:
            if x < x2: x += 1
            else: x -= 1
            self.passengers[passenger_id] = [OD_id, start_time, 1, x, y]
            yield self.env.timeout(1 / self.SPEED)
        while passenger_id in self.passengers.keys() and y != y2:
            if y < y2: y += 1
            else: y -= 1
            self.passengers[passenger_id] = [OD_id, start_time, 1, x, y]
            yield self.env.timeout(1 / self.SPEED)
        try:
            self.passengers.pop(passenger_id)
        except KeyError:
            pass
    
    def output_result(self, filename):
        np.savetxt(filename, self.record, delimiter=',', fmt='%d')

random.seed(0)
env = simpy.Environment()
network = Network(env, OD_pairs)
try:
    env.run(until=MAX_RUNTIME)
except KeyboardInterrupt:
    pass
network.output_result(output_filepath)