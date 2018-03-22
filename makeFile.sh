gcc -o GA GA_Problems.c
#javac -cp $CLASSPATH fitness.java

for((j=0;j<50;j++))
do
    ./GA nk 20 nk RANDOM 2
    ./GA royal 20 royal
	#./GA decption 20 decption
done

for((j=0;j<50;j++))
do
    ./GA nk 25 nk RANDOM 2
    ./GA royal 25 royal
	#./GA decption 25 decption
done

for((j=0;j<50;j++))
do
    ./GA nk 50 nk RANDOM 2
    ./GA royal 50 royal
	#./GA decption 50 decption
done

for((j=0;j<50;j++))
do
    ./GA nk 100 nk RANDOM 2
    ./GA royal 100 royal
	#./GA decption 100 decption
done

# 1. SVR로 실험

# 4. LR로 실험

# 2. 목적함수 직접 풀기

# 3. DNN으로 실험 # 진행중!
