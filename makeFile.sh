gcc -o GA GA-Problems.c

#for((i=0;i<50;i++))
#do
#    ./GA nk 20 nk RANDOM 7
#done

#for((i=0;i<50;i++))
#do
#    ./GA nk 25 nk RANDOM 7
#done

#for((i=0;i<50;i++))
#do
#    ./GA nk 50 nk RANDOM 7
#done

for((i=0;i<50;i++))
do
    ./GA nk 100 nk RANDOM 7
done
