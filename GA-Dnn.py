from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
import pandas as pd
import os
import math
import time
import csv

def load_data(train_path, test_path, y_name="fit"):
    train = pd.read_csv(train_path, header=0)
    train_x, train_y = train, train.pop(y_name)
    test = pd.read_csv(test_path, header=0)
    test_x, test_y = test, test.pop(y_name)

    return (train_x, train_y), (test_x, test_y)

def train_input_fn(features, labels, batch_size):
    dataset = tf.data.Dataset.from_tensor_slices((dict(features), labels))
    dataset = dataset.shuffle(1000).repeat().batch(batch_size)
    # tf.data.Dataset represents a potentially large set of elements.
    # A Dataset can be used to represent an input pipeline

    return dataset.make_one_shot_iterator().get_next()

def eval_input_fn(features, labels, batch_size):
    features=dict(features)
    if labels is None:
        inputs = features
    else:
        inputs = (features, labels)

    dataset = tf.data.Dataset.from_tensor_slices(inputs)
    assert batch_size is not None, "batch_size must not be None"
    dataset = dataset.batch(batch_size)

    return dataset.make_one_shot_iterator().get_next()

def main(argv):
 
    tf.logging.set_verbosity(tf.logging.INFO)
    start_time = time.time() 
    train_path = FLAGS['path'] + '/' +  FLAGS['file_name']
    test_path = FLAGS['path'] + '/' +  FLAGS['file_name']
    size = int(os.path.splitext(FLAGS['file_name'])[0].split('_')[-1])
    (train_x, train_y), (test_x, test_y) = load_data(train_path, test_path)
    # load train dataset, test dataset
    print(FLAGS['file_name'])
    my_feature_columns = []
    for key in train_x.keys():
        my_feature_columns.append(tf.feature_column.numeric_column(key=key))

    # feature_columns의 경우 4개의 feature를 저장한다.
    # 4개의 feature가 모두 numeric_column 즉, 각 feature의 값이 numeric 이다. (not nominal)

    regressor = tf.estimator.DNNRegressor(
        feature_columns = my_feature_columns,
        hidden_units = [1024,1024,512],
        optimizer = tf.train.ProximalAdagradOptimizer(
            learning_rate=0.1,
            l1_regularization_strength=0.01
        )

        # optimizer = tf.train.AdamOptimizer()
        # optimizer = tf.train.AdadeltaOptimizer()

    )

    # hidden_units : List of hidden units per layer.
    # All layers are fully connected
    # Ex) [64,32] means first layer has 64 nodes and second one has 32.
    regressor.train(
        input_fn = lambda:train_input_fn(train_x, train_y, FLAGS['batch_size']),
        steps=FLAGS['train_steps']
    )

    # 분류기 즉 DNN을 학습시키는 부분
    # 아래의 평가하는 부분과 다른 점은 입력 데이터셋이 다르다는 것
    eval_result = regressor.evaluate(
        input_fn=lambda:eval_input_fn(test_x, test_y, FLAGS['batch_size'])
    )

    # input_fn passed to train, evaluate, predict.
    # This should be a single Tensor or dict of same.

    # TODO
    # 결과를 csv 파일에 저장
    
    end_time = time.time()

    f = open("result_file", 'a', encoding='utf-8', newline='')
    wr = csv.writer(f)
    wr.writerow([FLAGS['file_name'], end_time-start_time, math.sqrt(eval_result['average_loss']), math.sqrt(eval_result['loss'])]) 
    f.close()

    # TODO
    # loss를 rmse로 하고, loss를 최소화하는 optimizer를 사용

if __name__ == "__main__":
    tf.set_random_seed(486)
    #for (path, dir, files) in os.walk("path"):
    for (path, dir, files) in os.walk("/home/dong/dataset"):
        for filename in files:
            FLAGS = {'batch_size': 10, 'train_steps': 10000, 'file_name': filename, 'path': path}
            main(FLAGS)
    
            del(FLAGS['file_name'])
            del(FLAGS['path'])


