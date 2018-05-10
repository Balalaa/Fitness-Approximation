# Fitness-Approximation

This project was created in the process of writing a paper("Is it worth to approximate fitness function by machine learning")
## Abstract  
It is usual to need an approximate model in evolutionary
computation when fitness function is deemed to be abstract or
expected to have a long computation time. In these cases, research
on possibility of fitness approximation should proceed before
applying an evolutionary algorithm in real-world problems. In this
paper, it was found that we could train machine learning
algorithms with the sampled solutions when problem size is large,
if there is a possibility of fitness approximation at small problem
sizes.

## Files
세 가지 기계 학습 알고리즘을 사용해서 모델을 만들고, 해당 모델로 적합도를 근사
DNN의 경우 Tensorflow로 구현 
LR, SVR의 경우 WEKA로 구현

## How to use code

```markdown
