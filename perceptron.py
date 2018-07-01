import csv
import random

import numpy as np


def PLA_pocket(data, target, attr_num, alpha, iter_num=200):
    parameter = np.zeros(attr_num, dtype=np.uint8)
    pre_mistakes_cnt = None
    data_len = len(data)
    mistakes = [i for i in range(len(data))]
    
    for _ in range(iter_num):
        pre_mistakes_cnt = len(mistakes)
        i = random.choice(mistakes)

        new_parameter = parameter + target[i] * data[i] * alpha
        new_mistakes = [k for k in range(len(data)) if error(
            data[k], new_parameter, target[k])]
        new_mistakes_cnt = len(new_mistakes)
        if new_mistakes_cnt < pre_mistakes_cnt:
            parameter = new_parameter
            mistakes = new_mistakes            
            accuracy = (data_len - new_mistakes_cnt) / data_len
        
    return parameter, accuracy


def error(data, parameter, target):
    return  predict(parameter, data) != target

def predict(parameter, data):
    return np.sign(np.inner(parameter, data))

def preprocess(file_name, train=True):
    with open(file_name, "r") as f:
        attrs = ["Pclass", "Sex", "Fare", "Age"]
        reader = csv.DictReader(f)

        # Deal with null of age attribute
        valid_age_list = []
        valid_fare_list = []
        for row in reader:
            try:
                valid_age_list.append(int(row["Age"]))
                try:
                    fare = float(row["Fare"])
                    if fare != 0.0:
                        valid_fare_list.append(fare)
                except:
                    pass
            except:
                pass

        avg_age = int(sum(valid_age_list) / len(valid_age_list))
        avg_fare = float(sum(valid_fare_list) / len(valid_fare_list))

        # move the file handler
        f.seek(0)
        next(reader)

        if train:
            data_list, target_list = [], []
            for row in reader:
                data_list.append(np.array(feature_extraction(row, attrs, avg_age, avg_fare)))
                target_list.append(1 if int(row["Survived"]) == 1 else -1)
            return data_list, target_list
        else:
            data_dict = {}
            for row in reader:
                data_dict[row["PassengerId"]] = feature_extraction(row, attrs, avg_age, avg_fare)
            return data_dict

def feature_extraction(row, attrs, avg_age, avg_fare):
    '''
    number of classes
    sex: 2
    Pclass: 3

    k class categorical feature => k - 1 features

    number and order of features
    Pclass: 1:
    Sex: 1(male):
        (0) if Sex == female
        (1) if Sex == male
    Fare: 1
    Age: 1
    Family: 1(SibSp + Parch)
    bias: 1
    '''
    feature = []
    for attr in attrs:
        if attr == "Pclass":
            feature.append(int(row[attr]))
        # Sex == "male"
        elif attr == "Sex" and row[attr] == "male":
            feature.append(1)
        # Sex == "female"
        elif attr == "Sex" and row[attr] == "female":
            feature.append(0)
        elif attr == "Fare":
            try:
                feature.append(float(row[attr]))
            except:
                feature.append(avg_fare)
        elif attr == "Age":
            try:
                feature.append(int(row[attr]))
            except:
                feature.append(avg_age)
    # Relation
    feature.append(int(row["SibSp"]) + int(row["Parch"]))

    # bias
    feature.append(1)
    return feature
        


if __name__ == "__main__":
        attr_len = 6
        train_list, target_list = preprocess("train.csv")
        parameter, accuracy = PLA_pocket(train_list, target_list, attr_len, 0.3)
        print(accuracy)

        test_dict = preprocess("test.csv", train=False)

        predict_list = [{"PassengerId": p_id, "Survived": 1 if predict(parameter, data) == 1 else 0}
                        for p_id, data in test_dict.items()]
        with open("submission.csv", "w", newline='') as f:
            header = ["PassengerId", "Survived"]
            writer = csv.DictWriter(f, fieldnames=header)
            writer.writeheader()
            writer.writerows(predict_list)
            
        



    
