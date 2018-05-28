#!/usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os
from dnn import Dnn

DEVICE = 'GPU'
EPOCH = 50
BATCH_SIZE = 100
MODEL_PATH = './DNN_Models/'
HNODES_NUM = [1024, 1024, 512]

if __name__ == '__main__':
   
    #for (path,dir,files) in os.walk("path"):
    for (path, dir, files) in os.walk("/home/dong/dataset"):
        for filename in files:
            my_dnn = Dnn()
            my_dnn.set_device(DEVICE)
            my_dnn.set_epoch(EPOCH)
            my_dnn.set_batch_size(BATCH_SIZE)
            my_dnn.load_dataset(path + '/' + filename, shuffle=True)
            MODEL_PATH = MODEL_PATH+os.path.splitext(filename)[0]+'/model'
            my_dnn.train(hnodes_num=HNODES_NUM, model_save_path=MODEL_PATH)
            MODEL_PATH = './DNN_Models/'
