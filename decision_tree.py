from typing import List
import sys

import pandas as pd

from node import TreeNode, LeafNode


class DecisionTree():
    def __init__(self, train_data: pd.DataFrame, target_name: str):
        self.target_name = target_name
        self.targets = train_data[self.target_name].unique()
        self.__buildTree(train_data)

    def __calculateGiniIndex(self, groups):
        gini_index = 0.0
        for group in groups:
            total_cnt = len(group)
            if total_cnt == 0:
                continue
            score = 1
            for t in self.targets:
                group_t_cnt = len(group[group[self.target_name] == t])
                group_t_prob = group_t_cnt / total_cnt
                score -= group_t_prob * group_t_prob
            gini_index += score
        return gini_index

    def __splitData(self, data: pd.DataFrame):
        """Check every gini index of unique value in every column except target name.
        Memorize the value and column name that calculating the best(lowest) gini index.

        Return order:
        selected column name, seleted value in columnm, splited data based on the seleted value
        """
        best_gini = float("inf")
        selected_column, selected_class, selected_groups = None, None, None
        # iterate over all columns except the taret name column
        for column in data.loc[:, data.columns != self.target_name]:
            for value in data[column].unique():
                splited_data = self.__testSplitedData(data, value, column)
                gini = self.__calculateGiniIndex(splited_data)
                if gini < best_gini:
                    best_gini = gini
                    selected_column, selected_class, selected_groups = column, value, splited_data
        return selected_column, selected_class, selected_groups

    def __testSplitedData(self, data: pd.DataFrame, split_class, column_name) -> List[pd.DataFrame]:
        belong_data = data[data[column_name] == split_class]

        merged_data = data.merge(belong_data, indicator=True, how='outer')
        not_belong_data = merged_data[merged_data["_merge"] == "left_only"]
        del not_belong_data["_merge"]

        return [belong_data, not_belong_data]

    def __countResult(self, data: pd.DataFrame):
        targets: pd.Series = data[self.target_name]
        return targets.value_counts().idxmax()

    def __buildNode(self, data: pd.DataFrame):
        selected_column, selected_class, selected_groups = self.__splitData(
            data)
        belong_data, not_belong_data = selected_groups

        # return if any splited data is empty -> perfect split
        if belong_data.empty or not_belong_data.empty:
            result = self.__countResult(data)
            return LeafNode(result)
        if not belong_data.empty:
            # left tree contains the data satisfying its selected_column belong to selected_class
            left_tree = self.__buildNode(belong_data)
        if not not_belong_data.empty:
            # right tree contains the data satisfying its selected_column NOT belong to selected_class
            right_tree = self.__buildNode(not_belong_data)

        return TreeNode(left_tree, right_tree, selected_column, selected_class)

    def __buildTree(self, data: pd.DataFrame):
        self.root = self.__buildNode(data)

    def __printNode(self, node: TreeNode, depth):
        if isinstance(node, LeafNode):
            print(node.result)
            return
        print("\t" * (depth - 1), "|" * (1 if depth != 0 else 0), "\t" * (1 if depth != 0 else 0),
              "{} == {}".format(node.selected_column, node.selected_class), end="")
        print() if isinstance(node.left, TreeNode) else print(": ", end="")
        self.__printNode(node.left, depth + 1)
        print("\t" * (depth - 1), "|" * (1 if depth != 0 else 0), "\t" * (1 if depth != 0 else 0),
              "{} != {}".format(node.selected_column, node.selected_class), end="")
        print() if isinstance(node.right, TreeNode) else print(": ", end="")
        self.__printNode(node.right, depth + 1)

    def printTree(self):
        print("Decision Tree Structure: ")
        self.__printNode(self.root, 0)

    def __predictResult(self, row, node: TreeNode):
        if isinstance(node, LeafNode):
            return node.result
        # left tree always contains the node with corresponding condition
        if row[node.selected_column] == node.selected_class:
            return self.__predictResult(row, node.left)
        # right tree always does NOT contains the node with corresponding condition
        else:
            return self.__predictResult(row, node.right)

    def predict(self, data: pd.DataFrame):
        results = [self.__predictResult(row, self.root) for _, row in data.iterrows()]
        accuracy = sum([predict == target for predict, target in zip(results, data[self.target_name])]) / len(data)
        print("Accuracy: {}".format(accuracy))
        return results


if __name__ == "__main__":
    try:
        filename = sys.argv[1]
        data: pd.DataFrame = pd.read_csv(filename, sep="\t")
        tree = DecisionTree(data, data.iloc[:, -1].name)
        tree.printTree()
        tree.predict(data)
    except:
        raise Exception("You must give a file!")