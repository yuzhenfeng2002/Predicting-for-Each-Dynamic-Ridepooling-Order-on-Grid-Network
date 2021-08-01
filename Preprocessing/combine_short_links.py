import csv
import pandas as pd
import pickle
from geopy.distance import geodesic


def extract_short_road(link_file: str, max_length: int):
    """
    The function is aimed to extract links shorter than max_length and return the adjacency list of them
    :param link_file: address of link file
    :param max_length: the maximum length of link you want to combine
    :return: the adjacency list of links shorter than max_length
    """
    adj_list = {}
    with open(link_file, encoding="gbk", mode="r") as f:
        f_csv = csv.reader(f)
        headings: list = next(f_csv)
        id_col = headings.index('link_id')
        from_node_col = headings.index('from_node_id')
        to_node_col = headings.index('to_node_id')
        length_col = headings.index('length')
        for row in f_csv:
            length = float(row[length_col])
            if length < max_length:
                link_id = int(row[id_col])
                from_node_id = int(row[from_node_col])
                to_node_id = int(row[to_node_col])

                if from_node_id not in adj_list:
                    adj_list[from_node_id] = [(link_id, to_node_id, length)]
                else:
                    adj_list[from_node_id].append((link_id, to_node_id, length))
    return adj_list


def find_scc(adj_list: dict):
    """
    To find sccs the distance of nodes in which is small
    :param adj_list: the adjacency list
    :return: (scc and nodes contained, the map between index of node and scc)
    """
    scc = {}
    node_scc = {}
    for key, item in adj_list.items():
        if key == 58:
            a = 1
        if key not in node_scc:
            node_scc[key] = len(scc)
            scc[len(scc)] = item
        else:
            scc_idx = node_scc[key]
            scc[scc_idx].extend(item)
        for neighbor in item:
            node_scc[neighbor[1]] = node_scc[key]
    return scc, node_scc


def simplify_node(old_node_file: str, new_node_file: str,
                  new_node_binary_file: str, old_new_map_binary_file: str,
                  scc: dict, node_scc: dict, save_to_file=False):
    """
    To simplify nodes by combine sccs
    :param old_node_file: the address of initial node file
    :param new_node_file: the address of combined node file
    :param new_node_binary_file: the address of combined node binary file
    :param old_new_map_binary_file: the address of map between old and new binary file
    :param scc: scc and nodes contained
    :param node_scc: the map between index of node and scc
    :param save_to_file: choose whether to save these data to file
    :return: (a list of new nodes, the map between new and old nodes, a dict of old nodes)
    """
    old_node_data = {}
    new_node_data = []
    old_new_map_dict = {}
    with open(old_node_file, encoding="gbk", mode="r") as f:
        f_csv = csv.reader(f)
        headings: list = next(f_csv)
        # print(headings)
        id_col = headings.index('node_id')
        x_coord_col = headings.index('x_coord')
        y_coord_col = headings.index('y_coord')
        for row in f_csv:
            node_id = int(row[id_col])
            x_coord = float(row[x_coord_col])
            y_coord = float(row[y_coord_col])
            old_node_data[node_id] = (x_coord, y_coord)
    for key, item in old_node_data.items():
        if key not in node_scc:
            old_new_map_dict[key] = len(new_node_data)
            new_node_data.append((len(new_node_data), item[0], item[1]))
        else:
            if key not in old_new_map_dict:
                scc_idx = node_scc[key]
                node_of_scc = scc[scc_idx]
                x = old_node_data[key][0]
                y = old_node_data[key][1]
                old_new_map_dict[key] = len(new_node_data)
                for node in node_of_scc:
                    node_idx = node[1]
                    x += old_node_data[node_idx][0]
                    y += old_node_data[node_idx][1]
                    old_new_map_dict[node_idx] = len(new_node_data)
                x /= (len(node_of_scc) + 1)
                y /= (len(node_of_scc) + 1)
                new_node_data.append((len(new_node_data), x, y))
    print("The number of nodes has decreased from {0} to {1}".format(len(old_node_data), len(new_node_data)))

    if not save_to_file:
        return new_node_data, old_new_map_dict, old_node_data
    # save data to file
    f = open(new_node_file, encoding="gbk", mode="w")
    data = list(map(lambda i: str(i)[1:-1] + '\n', new_node_data))
    f.write("node_id,x_coord,y_coord\n")
    f.writelines(data)
    f.close()
    f = open(new_node_binary_file, mode="wb")
    pickle.dump(new_node_data, f)
    f.close()
    f = open(old_new_map_binary_file, mode="wb")
    pickle.dump(old_new_map_dict, f)
    f.close()
    return new_node_data, old_new_map_dict, old_node_data


def simplify_link(old_link_file: str, new_link_file: str,
                  new_node_data: list, old_new_map_dict: dict, save_to_file=False):
    """
    To simplify links by combine short links
    :param old_link_file: the address of initial link file
    :param new_link_file: the address of combined link file
    :param new_node_data: a list of combined nodes
    :param old_new_map_dict: the map between new and old nodes
    :param save_to_file: choose whether to save these data to file
    :return: the map between new and old nodes
    """
    new_link_data = []
    old_link_num = 0
    with open(old_link_file, encoding="gbk", mode="r") as f:
        f_csv = csv.reader(f)
        headings: list = next(f_csv)
        # print(headings)
        from_node_col = headings.index('from_node_id')
        to_node_col = headings.index('to_node_id')
        length_col = headings.index('length')
        geometry_col = headings.index('geometry')
        # print(next(f_csv))
        for row in f_csv:
            old_link_num += 1
            from_node_id = int(row[from_node_col])
            to_node_id = int(row[to_node_col])
            length = float(row[length_col])
            geometry = row[geometry_col]
            if from_node_id in old_new_map_dict:
                from_node_id = old_new_map_dict[from_node_id]
                geometry_point_str = geometry[12:-1]
                begin_node = tuple(map(float, geometry_point_str.split(',')[0].split(' ')))
                end_node = (new_node_data[from_node_id][1], new_node_data[from_node_id][2])
                length += geodesic((begin_node[1], begin_node[0]), (end_node[1], end_node[0])).m
                geometry_point_str = str(end_node[0]) + ' ' + str(end_node[1]) + ', ' + geometry_point_str
                geometry = 'LINESTRING (' + geometry_point_str + ')'
            if to_node_id in old_new_map_dict:
                to_node_id = old_new_map_dict[to_node_id]
                geometry_point_str = geometry[12:-1]
                begin_node = tuple(map(float, geometry_point_str.split(',')[-1][1:].split(' ')))
                end_node = (new_node_data[to_node_id][1], new_node_data[to_node_id][2])
                length += geodesic((begin_node[1], begin_node[0]), (end_node[1], end_node[0])).m
                geometry_point_str = geometry_point_str + ', ' + str(end_node[0]) + ' ' + str(end_node[1])
                geometry = 'LINESTRING (' + geometry_point_str + ')'
            if from_node_id != to_node_id:
                new_link_data.append((from_node_id, to_node_id, length, geometry))
    print("The number of links has decreased from {0} to {1}".format(old_link_num, len(new_link_data)))

    if not save_to_file:
        return new_link_data
    # save to file
    heading = ["from_node_id", "to_node_id", "length", "geometry"]
    csv_data = list(map(list, new_link_data))
    f_csv = pd.DataFrame(data=csv_data, columns=heading)
    f_csv.to_csv(new_link_file, encoding='gbk')
    return new_link_data
