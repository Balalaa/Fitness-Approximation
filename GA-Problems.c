/*
	유전 알고리즘의 적합도 함수를 근사하는 논문을 작성하는 과정에서
	기존의 GA-Problems.c를 수정한 것으로 기존 코드와 다음이 다르다.
	* landscape가 같아야 했기 때문에, 파일 형태로 저장되어 있는 landscape를 읽는 부분
	* 적합도를 텐서플로우로 만든 딥러닝 모델을 통해 얻는 부분  
	* 적합도를 웨카로 만든 머신러닝 모델(Support vector Regression)을 통해 얻는 부분
	* (LR 모델을 로드하는 과정에서 에러를 수정하지 못해, 가중치를 곱해 적합도를 구해서 LR의 경우에는 
	모델을 로드하지 않았다.)
	* nk_create(), fitnessCheckForNK, fitnessCheck 등의 코드가 달라졌다. 기존의 Landscape를 불러와야 하는 부분이 존재하기 때문
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define INDIVIDUAL 50 // 20
#define GENERATION 50 // 100
#define PROBABILITY_CROSSOVER 0.7
#define PROBABILITY_MUTATION 0.1
#define PROBABILITY_TORNAMENT 0.7
#define NUM_OF_ELITE 4
#define ROYAL_CONSTANT 1

int GENES=0;
int NK_N=0;
int NK_K=0;
int RANDOM=0;
int NEXTDOOR=1;
char* TYPE;
char* LOCATION;
char* PROBLEM;
/* NK_K는 반드시 0 <= NK_K <= NK_N-1 을 만족해야 한다. */

/* 유전자의 개수를 바꿀 때 조심해야 합니다.
royalRoad 함수에서 8로 설정했기 때문에 8의 배수가 되어야 합니다. */

int lastGeneration = 0;
/* 마지막 세대인지를 확인하기 위한 전역 변수
fitnessCheck와 fitnessCheckForNK에서 사용한다. */

int **population;
int **next_population;
int *randomArr;

typedef struct fitness
{
	double ideal;
	double average;
	int indexOfIdeal;
	double fit[INDIVIDUAL];
}Fitness;
/* 세대 별 최적, 평균, 모든 fit을 저장하는 구조체 */

typedef struct nk
{
	int epi;
	int **Rand;
	double **land;

}NK;

int power(int num);
void nk_create(NK* landscape);
void nk_free(NK* landscape);
double nk_fitness(int index,NK* landscape);
void initialize();
void crossover(int* chromosome1,int* chromosome2,int index1,int index2);
void mutation(int* chromosome);
int royalRoad(int index);
int sumOfGene(int index);
int tornament(Fitness* generation);
void elitism(int** eliteGenes);
void fitnessCheck(Fitness* generation);
void fitnessCheckForNK(Fitness* generation,NK* landscape);
void resultToFile(Fitness gener[GENERATION],int num);

int power(int num)
{
	int result=1;

	for(int i=0;i<num;i++)
		result*=2;

	return result;
}

void nk_create(NK* landscape)
{
	int i,j;
	int pow=power(NK_K);
	int exist=0;
	
    if(strcmp(TYPE,"RANDOM")==0)
		landscape->epi=RANDOM;

	else if(strcmp(TYPE,"NEXTDOOR")==0)
		landscape->epi=NEXTDOOR;
<<<<<<< HEAD:GA-Problems.c

    FILE* pFile = NULL;
    char str[200];
    sprintf(str, "../landscape/land%d_7",GENES);
    /*
	landscape가 존재하는 적절한 경로를 설정해야 한다.
	landscape는 GA_Problems를 통해 얻을 수 있다.
    */     
    pFile = fopen(str,"r");

=======
    
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
	landscape->Rand=(int**)malloc(sizeof(int*)*NK_N);
	landscape->land=(double**)malloc(sizeof(double*)*NK_N);

	for(i=0;i<NK_N;i++)
	{
		landscape->land[i]=(double*)malloc(sizeof(double)*pow*2);
		landscape->Rand[i]=(int*)malloc(sizeof(int)*NK_K);
	}


	char str[200];
   	sprintf(str, "../landscape/land%d_%d",NK_N,NK_K);
    FILE* pFile = fopen(str,"r");
	
    for(int a=0;a<NK_N;a++)
    {
        for(int b=0;b<pow*2;b++)
            fscanf(pFile,"%lf",&landscape->land[a][b]);
    }
    for(int a=0;a<NK_N;a++)
    {
        for(int b=0;b<NK_K;b++)
            fscanf(pFile,"%d",&landscape->Rand[a][b]);
    }
    fclose(pFile);

	/*
	 * fitness approximation을 위해 추가된 부분 
	 * landscape가 존재하는 적절한 경로를 설정해야 한다.
	 * landscape는 GA_Problems를 통해 얻을 수 있다.
    */
	/*	
	for(i=0;i<NK_N;i++)
	{
		for(j=0;j<pow*2;j++)
			landscape->land[i][j] = ( (double)rand() / RAND_MAX);

		for(j=0;j<NK_K;j++)
		{
			landscape->Rand[i][j] = rand() % GENES;
			if(landscape->Rand[i][j]==i)
			{
				j--;
				continue;
			}

			for(int idx=0;idx<j;idx++)
				if(landscape->Rand[i][j] == landscape->Rand[i][idx])
					exist=1;
			if(exist)
			{
				j--;
				exist=0;
				continue;
			}
		}
	}
	*/
    /* 파이썬 코드를 돌리기 위해 기존에 DNN 모델을 학습시킬 때 필요했던
    landscape를 불러오기 위해 주석 처리한 부분 */
    /* 만약 기존의 landscape를 필요하지 않고, 새로 만들어도 된다면 주석 부분을 적절히 
    처리하면 된다. */
}

double nk_fitness(int index, NK* landscape)
{

	int i,j;
	double sum=0;
	int num=0;
	int binary=0;

	if(landscape->epi == RANDOM)
	{
		for(i=0;i<NK_N;i++)
		{
			binary=0;
			int k=NK_K;

			binary += population[index][i] * power(k);
			for(j=0;j<NK_K;j++)
			{
				k--;
				num=landscape->Rand[i][j];
				binary += (population[index][num] * power(k));
			}
			sum += landscape->land[i][binary];
		}

		return sum;
	}

	else if(landscape->epi == NEXTDOOR)
	{
		for(i=0;i<NK_N;i++)
		{
			binary=0;
			int k=NK_K;

			for(j=0;j<NK_K+1;j++)
			{
				if( i+j >= GENES)
					num = (i+j) % GENES;
				else
					num = i+j;

				binary += (population[index][num] * power(k));
				k--;
			}
			sum += (landscape->land[i][binary]);
		}
		return sum;
	}
}

void nk_free(NK* landscape)
{
	int i;
	for(i=0;i<NK_N;i++)
	{
		free(landscape->land[i]);
		free(landscape->Rand[i]);
	}
	free(landscape);
}

void initialize()
{
    int i, j;
    for (i = 0; i < INDIVIDUAL; i++)
	{
        for (j = 0; j < GENES; j++)
		{
            population[i][j] = rand() % 2;
        }
    }
}

void crossover(int* chromosome1, int* chromosome2, int index1, int index2)
{
    int j;
    int gene = rand() % GENES;

    for (j = 0; j < gene; j++)
	{
        chromosome1[j] = population[index1][j];
    }

    for (j = gene; j <GENES; j++)
	{
        chromosome1[j] = population[index2][j];
    }

    for (j = 0; j < gene; j++)
	{
        chromosome2[j] =population[index2][j];
    }

    for (j = gene; j < GENES; j++)
	{
        chromosome2[j] = population[index1][j];
    }
}

void mutation(int* chromosome)
{
    int gene = rand() % GENES;

	if (chromosome[gene] == 0)
        chromosome[gene] = 1;

	else
        chromosome[gene] = 0;
}
double LR_royal_nk(int index)
{
	char route[100];
	sprintf(route,"./tmp/%s%d.txt",PROBLEM,GENES);
	FILE* fp = fopen(route,"r");
	char ch1,ch2;
	double parameter=0;
	double sum=0;
	char* str=(char*)malloc(sizeof(char)*200);
	char* ptr=(char*)malloc(sizeof(char)*200);
	while(!feof(fp))
	{
		fscanf(fp, "%lf %c %s %c",&parameter,&ch1,str,&ch2);
		if(ch1 == '0')
		{
			sum += parameter;
			break;
		}
		char* ptr = &str[1];
		int i=atoi(ptr);

		sum += population[index][i] * parameter;
		
	}
	fclose(fp);
	free(str);
	free(ptr);
	return sum;
}
int royalRoad(int index)
{
	int i,j;
	int sum=0;
	int correct=0;
	int num=GENES/ROYAL_CONSTANT;

	for(i=0;i<num;i++)
	{
		for(j=0;j<ROYAL_CONSTANT;j++)
		{
			int num=ROYAL_CONSTANT*i+j;

			if(population[index][num]==0)
			{
				correct=1;
				break;
			}
		}

		if(correct==0)
			sum+=ROYAL_CONSTANT;

		correct=0;
	}
	return sum;
}

int sumOfGene(int index)
{
    int sum = 0;
    for (int i = 0; i < GENES; i++)
        sum += population[index][i];

    return sum;
}
int LR_decption(int index)
{
	int sum = sumOfGene(index);
	return sum-1;
}
int decption(int index)
{
    int sum = sumOfGene(index);
    int fit = 0;

    if(sum == 0)
        fit = GENES;
    else
        fit = sum - 1;

    return fit; 
}

int randomFitness(int index)
{
	int seed=0;

	for(int i=0;i<GENES;i++)
		seed = power(i) * population[index][i];


	return randomArr[seed];
}

int tornament(Fitness* generation)
{
	int rand1,rand2;
 	double prob;

	rand1=rand()%INDIVIDUAL;
	rand2=rand()%INDIVIDUAL;
	prob=((double)rand()/(RAND_MAX));

	if(prob < PROBABILITY_TORNAMENT)
	{
		if(generation->fit[rand1]>generation->fit[rand2])
			return rand1;
		else
			return rand2;
	}
	else
	{
		if(generation->fit[rand1]>generation->fit[rand2])
			return rand2;
		else
			return rand1;
	}
}
/* 랜덤으로 두개를 골라서 두 개중의 더 적합한 것의 인덱스를 반환 */

void elitism(int** eliteGenes)
{
	int i,j;
	for(i=0;i<NUM_OF_ELITE;i++)
	{
		for(j=0;j<GENES;j++)
			population[i][j]=eliteGenes[i][j];
	}
}

void fitnessCheck(Fitness* generation)
{
	double ideal=0;
	int indexOfIdeal=0;
	double sum=0;

    /*
     * 파이썬에서 c를 호출하기 위해 파일 입출력 부분을 삽입
     * fitness를 구하는 부분을 텐서플로우를 통해 얻는다.
     */
	/*
    char str[INDIVIDUAL][500];
    char toParser[500];
    char everyStr[10000];
    memset(everyStr,0,sizeof(everyStr));
    memset(str,0,sizeof(str));
    memset(toParser,0,sizeof(toParser));
    */
    if(lastGeneration == 1)
    {
        for(int i=0;i<INDIVIDUAL;i++)
        {
            if(!strcmp(PROBLEM,"onemax"))
                generation->fit[i]=sumOfGene(i);
            else if(!strcmp(PROBLEM,"royal"))
                generation->fit[i]=royalRoad(i);
            else if(!strcmp(PROBLEM,"decption"))
		        generation->fit[i]=decption(i);
            else if(!strcmp(PROBLEM,"random"))
                generation->fit[i]=randomFitness(i);
        }
    }
    else
    {
		for(int i=0;i<INDIVIDUAL;i++)
		{
			if(!strcmp(PROBLEM,"royal"))
				generation->fit[i]=LR_royal_nk(i);
			else if(!strcmp(PROBLEM,"decption"))
				generation->fit[i]=LR_decption(i);
		}
		/*
        for(int i=0;i<INDIVIDUAL;i++)
	    {
            for(int j=0;j<GENES;j++)
            {
                if(population[i][j] == 0)
                    str[i][j] = '0';
                else
                    str[i][j] = '1';
            }
            if(i != INDIVIDUAL-1)
                str[i][GENES] = '/';
            strcat(everyStr,str[i]); 
	    }
        sprintf(toParser, "/usr/local/anaconda3/envs/tensorflow-gpu/bin/python3 fitness.py --solution %s --genes %d --type %s",everyStr,GENES,PROBLEM); 
		*/
        /* 텐서플로우 모델을 가져와서 적합도를 구하기 위한 시스템 콜 */
        //sprintf(toParser, "java fitness %s %s %d",everyStr,PROBLEM,GENES);
        /* 웨카 모델을 가져와 적합도를 구하기 위한 시스템 콜 */
		/*
        system(toParser);
        
        FILE* fp=fopen("result","r");
        for(int i=0;i<INDIVIDUAL;i++)
        {
            fscanf(fp, "%lf", &generation->fit[i]);
	        printf("%lf\n",generation->fit[i]);
        }
        fclose(fp);    
		*/
    }
    for(int i=0;i<INDIVIDUAL;i++)
    {
        sum += generation->fit[i];
        if(generation->fit[i]>ideal)
        {
            ideal=generation->fit[i];
            indexOfIdeal=i;
        }
    }
	/*
    for(int i=0;i<INDIVIDUAL;i++)
    {
        if(!strcmp(PROBLEM, "onemax"))
            generation->fit[i]=sumOfGene(i);
        else if(!strcmp(PROBLEM, "royal"))
            generation->fit[i]=royalRoad(i);
        else if(!strcmp(PROBLEM, "decption"))
            generation->fit[i]=decption(i);
        else if(!strcmp(PROBLEM, "random"))
            generation->fit[i]=randomFitness(i);

        sum += generation->fit[i];
	    if(generation->fit[i]>ideal)
	    {
		    ideal=generation->fit[i];
		    indexOfIdeal=i;
	    }
    }
	*/
	generation->ideal=ideal;
	generation->average=(double)sum/INDIVIDUAL;
	generation->indexOfIdeal=indexOfIdeal;

}


void fitnessCheckForNK(Fitness* generation,NK* landscape)
{
	double ideal=0;
	int indexOfIdeal=0;
	double sum=0;

    /*
     * 파이썬에서 c를 호출하기 위해 파일 입출력 부분을 삽입
     * fitness를 구하는 부분을 텐서플로우를 통해 얻는다.
     */
    /* --------------------------------------------------- */
	char str[INDIVIDUAL][500];
    char toParser[500];
    char everyStr[10000];
    memset(everyStr,0,sizeof(everyStr));
    memset(str,0,sizeof(str));
    memset(toParser,0,sizeof(toParser));
    if(lastGeneration == 1)
    {
		for(int i=0;i<INDIVIDUAL;i++)
    	{
        	generation->fit[i]=nk_fitness(i,landscape);
			sum+=generation->fit[i];

			if(generation->fit[i]>ideal)
			{
				ideal=generation->fit[i];
				indexOfIdeal=i;
			}
    	}
    }
	/* 마지막 세대일 경우, 적합도를 기존의 방법으로 구해야 했다. */
    else
    {
<<<<<<< HEAD:GA-Problems.c
        
		//for(int i=0;i<INDIVIDUAL;i++)
		//{
			//generation->fit[i]=LR_royal_nk(i);
		//}
        
=======
		/*
		for(int i=0;i<INDIVIDUAL;i++)
			generation->fit[i]=LR_royal_nk(i);
		*/
		/*
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
	    for(int i=0;i<INDIVIDUAL;i++)
	    {
            for(int j=0;j<GENES;j++)
            {
                if(population[i][j] == 0)
                    str[i][j] = '0';
                else
                    str[i][j] = '1';
            }
            if(i != INDIVIDUAL-1)
                str[i][GENES] = '/';
            strcat(everyStr, str[i]);
        }

        // sprintf(toParser, "python3 fitness.py --solution %s --type %s --genes %d",everyStr,PROBLEM,GENES);
        // 입력 파일은 csv형태로, 다음과 같은 형태이다.
        // a1,a2,a3,fit
        // 0,1,1,2
        // 1,1,1,3
        // 텐서플로우 모델을 통해 적합도를 얻기 위한 시스템 콜 
        sprintf(toParser, "java fitness %s %s %d",everyStr,PROBLEM,GENES);
        // 입력 파일은 텐서플로우 모델을 사용했을 때와 동일
        // 하지만 프로그램을 수행하기 전에 자바 파일을 컴파일해서
        // fitness.class 파일을 만들어야 한다. 
        // 웨카 모델을 통해 적합도를 얻기 위한 시스템 콜 
		system(toParser);
        
        FILE* fp=fopen("result","r");
        for(int i=0;i<INDIVIDUAL;i++)
        {
            fscanf(fp,"%lf",&generation->fit[i]);
        }
        fclose(fp);
<<<<<<< HEAD:GA-Problems.c
    }
=======
		
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
	
    	for(int i=0;i<INDIVIDUAL;i++)
    	{
        	sum+=generation->fit[i];
        
<<<<<<< HEAD:GA-Problems.c
        if(generation->fit[i]>ideal)
        {
            ideal=generation->fit[i];
            indexOfIdeal=i;
        }
    }
    /* ------------------------------------------------------------- */
    /*
    for(int i=0;i<INDIVIDUAL;i++)
    {
        generation->fit[i]=nk_fitness(i,landscape);
		sum+=generation->fit[i];

		if(generation->fit[i]>ideal)
		{
			ideal=generation->fit[i];
			indexOfIdeal=i;
		}
    }
    */
    /* ------------------------------------------------------------- */
=======
        	if(generation->fit[i]>ideal)
        	{
            	ideal=generation->fit[i];
            	indexOfIdeal=i;
        	}
    	}
		*/

		/* 윗 부분의 코드는 텐서플로우 모델을 통해 적합도를 구해야 했기 때문에
	 	 * 필요했던 부분, 기존의 landscape를 통해 적합도를 구하는 것은 아래의 코드를
	 	 * 수행하면 된다. 
	 	 */

    	for(int i=0;i<INDIVIDUAL;i++)
    	{
        	generation->fit[i]=nk_fitness(i,landscape);
			sum+=generation->fit[i];

			if(generation->fit[i]>ideal)
			{
				ideal=generation->fit[i];
				indexOfIdeal=i;
			}
    	}
	}
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
	generation->ideal=ideal;
	generation->average=(double)sum/INDIVIDUAL;
	generation->indexOfIdeal=indexOfIdeal;
}

void resultToFile(Fitness gener[GENERATION],int num)
{
    FILE *fp;
    int index;

    fp = fopen(LOCATION, "a+");

	//if(num != GENERATION)
	fprintf(fp,"%lf\n",gener[num-1].ideal);
	/*
	fprintf(fp,"#fitness1.dat\n");
	fprintf(fp,"#First data block (index ideal)\n#X Y\n");

	for(index=0;index<num;index++)
		fprintf(fp," %d %lf\n",index+1,gener[index].ideal);
	fprintf(fp,"\n\n#Second data block (index average)\n#X Y\n");
	for(index=0;index<num;index++)
		fprintf(fp," %d %lf\n",index+1,gener[index].average);
	*/
	fclose(fp);
}
/* 이 때의 num은 세대를 다 거치기 전 끝난 경우를 위한 변수 */

int main(int argc,char** argv)
{
	struct timeval t;
	gettimeofday(&t,NULL);
	srand(t.tv_usec * t.tv_sec * getpid());

    PROBLEM=(char*)malloc(sizeof(char)*30);
    PROBLEM=argv[1];
    GENES=atoi(argv[2]);
    NK_N=GENES;
	LOCATION=(char*)malloc(sizeof(char)*30);
	LOCATION=argv[3];
	/* 어떤 문제를 대상으로 유전 알고리즘을 수행할 것인가 */
    /* 유전자의 개수를 입력받는다, NK_N은 유전자의 개수와 같은 의미 */
	/* 최적해를 저장할 파일의 경로 */ 
    
	if(!strcmp(PROBLEM,"nk"))
    {
	    TYPE=(char*)malloc(sizeof(char)*30);
	    TYPE=argv[4];
        NK_K=atoi(argv[5]);
		/* 문제 유형이 NK-Landscape일 경우, landscape type과 k를 요구 */
    }

   	int i, j, n;
	population=(int**)malloc(sizeof(int*)*INDIVIDUAL);
	next_population=(int**)malloc(sizeof(int*)*INDIVIDUAL);
	
<<<<<<< HEAD:GA-Problems.c
    for(i=0;i<INDIVIDUAL;i++)
=======
	for(i=0;i<INDIVIDUAL;i++)
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
	{
		population[i]=(int*)malloc(sizeof(int)*GENES);
		next_population[i]=(int*)malloc(sizeof(int)*GENES);
	}
   	initialize();

   	Fitness* generation=(Fitness*)malloc(sizeof(Fitness)*GENERATION);
    NK* landscape= NULL;
    if(!strcmp(PROBLEM,"nk"))
    {
	    landscape=(NK*)malloc(sizeof(NK));
	    nk_create(landscape);
    }
    for(i=0;i<GENERATION;i++)
    {
        if(i==GENERATION-1)
            lastGeneration = 1;
		
		if(!strcmp(PROBLEM,"nk"))
        {
            fitnessCheckForNK(&generation[i],landscape);
	        /* NK_LANDSCAPE를 이용한 적합도 계산 함수 호출 */
			/*
            if(i==0)
		    {
			    FILE* fpp;
			    char resultFile[200];
			    sprintf(resultFile,"land%d_%d",NK_N,NK_K);
			    fpp = fopen(resultFile,"a+");
			    for(int a=0;a<NK_N;a++)
			    {
				    for(int b=0;b<power(NK_K)*2;b++)
					    fprintf(fpp,"%lf ",landscape->land[a][b]);
				    fprintf(fpp,"\n");
			    }

			    for(int a=0;a<NK_N;a++)
			    {
				    for(int b=0;b<NK_K;b++)
					    fprintf(fpp,"%d ",landscape->Rand[a][b]);
				    fprintf(fpp,"\n");
			    }

			    for(int a=0;a<INDIVIDUAL;a++)
			    {

				    if(a%10==0)
					    fprintf(fpp,"\n");
				    fprintf(fpp,"%lf ",generation[0].fit[a]);
			    }
				fclose(fpp);
		    }
			*/
			/* 첫 번째 NK_Landscape를 nk10_2의 형태로 파일을 만들고 저장 */
        	
		}
		  
		else
    	    fitnessCheck(&generation[i]);
        	/* One-Max, Royal Road, Decption 적합도 계산을 위한 함수 호출 */
		int ideal_individuo = generation[i].indexOfIdeal;
		double ideal_num = generation[i].ideal;

		int k, l;
<<<<<<< HEAD:GA-Problems.c
		
		//printf("%d Generation: \n", i + 1);
		//k=ideal_individuo;
=======
	
		/*
		printf("%d Generation: \n", i + 1);
		k=ideal_individuo;
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c

		for (l = 0; l < GENES; l++)
			printf("%d ", population[k][l]);

		printf("\n");
	   	printf("%lf", ideal_num);
	   	printf("\n");
		*/
		/* 출력하는 부분 */

		if (ideal_num == GENES)
		    break;
        
        int indices_selec[INDIVIDUAL];

        for(j = 0; j < INDIVIDUAL; j++)
            indices_selec[j] = tornament(&generation[i]);

		/* tornament 방식으로 selection */

		for(j = 0; j < INDIVIDUAL; j++)
			for(int idx=0;idx<GENES;idx++)
				next_population[j][idx]=population[j][idx];

        for(j = 0; j < INDIVIDUAL; j+=2)
		{
            double prob = ((double) rand() / (RAND_MAX));

            if(prob > PROBABILITY_CROSSOVER)
                continue;

            int* chromosome1=(int*)malloc(sizeof(int)*GENES);
            int* chromosome2=(int*)malloc(sizeof(int)*GENES);

            crossover(chromosome1,chromosome2,indices_selec[j],indices_selec[j+1]);
            for(n = 0; n < GENES; n++)
	   		{
               next_population[j][n] = chromosome1[n];
               next_population[j+1][n] = chromosome2[n];
            }
        }
		/* crossover 수행  */
	
		for(j = 0; j < INDIVIDUAL; j+=2)
		{
			double prob = ((double) rand() / (RAND_MAX));
       	    if(prob > PROBABILITY_MUTATION)
				continue;

            int* res=(int*)malloc(sizeof(int)*GENES);
            for(n = 0; n < GENES; n++)
                res[n]=next_population[j][n];

            mutation(res);

            for(n = 0; n <GENES; n++)
				next_population[j][n]=res[n];
		}
		/* mutation 수행 */
	
		int tmp[INDIVIDUAL];
		int** eliteGenes=(int**)malloc(sizeof(int*)*NUM_OF_ELITE);

		for(int m=0;m<NUM_OF_ELITE;m++)
			eliteGenes[m]=(int*)malloc(sizeof(int)*GENES);

		int elite[NUM_OF_ELITE];
		int max=0;
		int index=0;

		for(int m=0;m<INDIVIDUAL;m++)
			tmp[m]=generation[i].fit[m];

		for(int idx=0;idx<NUM_OF_ELITE;idx++)
		{
			for(int m=0;m<INDIVIDUAL;m++)
			{
				if(tmp[m]>max)
				{
					max=tmp[m];
					index=m;
				}
			}
			tmp[index]=0;

			for(int w=0;w<GENES;w++)
				eliteGenes[idx][w]=population[index][w];
		}
	
	    for(j = 0; j < INDIVIDUAL; j++)
	    {
			for(n = 0; n < GENES; n++)
			     population[j][n] = next_population[j][n];
	    }
		/* replacement 수행 */

	    elitism(eliteGenes);
		/* elitism 수행 */
	}
	if(i==GENERATION)
		resultToFile(generation,i);
	else
		resultToFile(generation,i+1);
	/* 파일에 결과를 출력 */
	
	if(!strcmp(PROBLEM,"nk"))
        nk_free(landscape);
<<<<<<< HEAD:GA-Problems.c

    end = clock();
    //gap = (float)(end-start)/(CLOCKS_PER_SEC);
    //FILE* timeFile = fopen("time.txt","a+");
    //fprintf(timeFile,"%f 시간 소요, %s-%d 문제 \n",gap,PROBLEM,GENES);
    //fclose(timeFile);

=======
>>>>>>> cc6a58e45099408d8c106a8d18d877806212e81b:GA-Problems.c
}
