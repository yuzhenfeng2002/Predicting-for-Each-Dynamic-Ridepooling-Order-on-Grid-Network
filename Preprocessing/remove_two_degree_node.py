import csv


def calculate_node_degree(link_file: str):
    node_degree = {}
    with open(link_file, encoding="gbk", mode="r") as f:
        f_csv = csv.reader(f)
        headings: list = next(f_csv)
        from_node_col = headings.index('from_node_id')
        to_node_col = headings.index('to_node_id')
        for row in f_csv:
            from_node_id = int(row[from_node_col])
            to_node_id = int(row[to_node_col])

            if from_node_id not in node_degree:
                node_degree[from_node_id] = [0, 1]
            else:
                node_degree[from_node_id][1] = node_degree[from_node_id][1] + 1

            if to_node_id not in node_degree:
                node_degree[to_node_id] = [1, 0]
            else:
                node_degree[to_node_id][0] = node_degree[from_node_id][0] + 1
    return node_degree
