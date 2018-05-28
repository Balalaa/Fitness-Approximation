import tensorflow as tf
import os
import argparse
import sys
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument(
    '--solution',
    type=str
)
parser.add_argument(
    '--type',
    type=str
)
parser.add_argument(
    '--genes',
    type=int
)
FLAGS, unparsed = parser.parse_known_args()

"""
sess = tf.Session()
sess.run(tf.global_variables_initializer())
sess.run(tf.local_variables_initializer())
"""
model_path = '../model_high_done/DNN_Models/' + FLAGS.type + str(FLAGS.genes) + '_7' + '/model'
par_dir = os.path.split(model_path)[0]
meta_file = '{}.meta'.format(model_path)
ckpt_file = '{}/checkpoint'.format(par_dir)
"""
model = tf.train.import_meta_graph(meta_file, clear_devices=True)
model.restore(sess, tf.train.latest_checkpoint(par_dir)) 
graph = tf.get_default_graph()
"""

x_ = FLAGS.solution
input_xx = x_.split('/')
input_x = list()
for i in range(0,len(input_xx)):
    for j in range(0,FLAGS.genes):
        input_x.append(int(input_xx[i][j]))
    
input_x = np.array(input_x).reshape((len(input_xx),FLAGS.genes))
input_y = np.zeros((len(input_xx),1)) 

# genes가 100이라면, 100개의 0,1로 이루어진 문자열이 입력

with tf.device('/device:{}:3'.format('GPU')):
    
    config = tf.ConfigProto(allow_soft_placement=True)
    sess = tf.Session(config=config) 
    sess.run(tf.global_variables_initializer())
    sess.run(tf.local_variables_initializer())
   
    model = tf.train.import_meta_graph(meta_file, clear_devices=True)
    model.restore(sess, tf.train.latest_checkpoint(par_dir))
    
    graph = tf.get_default_graph()

    x = graph.get_tensor_by_name('input_x:0')
    y = graph.get_tensor_by_name('input_y:0')

    keep_prob = graph.get_tensor_by_name('keep_prob:0')
    y_predict = graph.get_tensor_by_name('y_predict/BiasAdd:0')


    prediction = sess.run(
        y_predict, 
        feed_dict={x: input_x, y: input_y, keep_prob:1.0}
    )

tf.reset_default_graph()
sess.close()
f = open('result','w')
for i in range(0,len(input_xx)):
    f.write(str(prediction[i][0]))
    f.write(' ')
f.close()

