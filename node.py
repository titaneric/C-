class LeafNode():
    def __init__(self, result):
        self.result = result


class TreeNode():
    def __init__(self, left, right, selected_column, selected_class):
        self.left = left
        self.right = right
        self.selected_column = selected_column
        self.selected_class = selected_class