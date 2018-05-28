#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct nk
{
    int** Rand;
    double** land;
}NK;

int power(int num)
{
    int result=1;

    for(int i=0;i<num;i++)
        result *= 2;

    return result;
}

double sumOfGenes(char* str, int genes)
{
    double fit=0;
    for(int i=0;i<genes;i++)
        fit += atoi(&str[i*2]);
    
    return fit;
}
double royal(char* str, int genes)
{
    double fit=0;
    int correct=0;

    for(int i=0;i<genes/2;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(str[i*4+j*2] == '0')
            {
                correct = 1;
                break;
            }
        }

        if(correct == 0)
            fit += 2;

        correct = 0;
    }
    return fit;
}
double nk(char* binary, int genes, int nk_k)
{
    /*
        TODO
        nk landscape 파일을 읽어서
        적합도를 구한다.
        사실, 시간을 비교하기 때문에 
        landscape가 같을 필요는 없을 것 같다.
    */

    int pow=power(nk_k);
	NK* landscape = (NK*)malloc(sizeof(NK));	
	landscape->Rand=(int**)malloc(sizeof(int*)*genes);
	landscape->land=(double**)malloc(sizeof(double*)*genes);

	for(int i=0;i<genes;i++)
	{
		landscape->land[i]=(double*)malloc(sizeof(double)*pow*2);
		landscape->Rand[i]=(int*)malloc(sizeof(int)*genes);
	}

	char pstr[200];
  	sprintf(pstr, "../landscape/land%d_%d",genes,nk_k);
    FILE* pFile = fopen(pstr,"r");
	
    for(int a=0;a<genes;a++)
    {
        for(int b=0;b<pow*2;b++)
		{
            fscanf(pFile,"%lf",&landscape->land[a][b]);
		}
    }
    for(int a=0;a<genes;a++)
    {
        for(int b=0;b<nk_k;b++)
		{
            fscanf(pFile,"%d",&landscape->Rand[a][b]);
		}
    }
    fclose(pFile);

	int bin=0;
	int num=0;
	double sum=0;

	for(int i=0;i<genes;i++)
	{
		bin=0;
		int k=nk_k;

		bin += atoi(&binary[2*i]) * power(k);
		for(int j=0;j<nk_k;j++)
		{
			k--;
			num=landscape->Rand[i][j];
			bin += (atoi(&binary[num]) * power(k));
		
		}
		sum += landscape->land[i][bin];
	}
	
	for(int i=0;i<genes;i++)
	{
		free(landscape->land[i]);
		free(landscape->Rand[i]);
	}
	free(landscape);

	return sum; 
    
}
double deception(char* str, int genes)
{
    double fit = sumOfGenes(str,genes);
    if(fit == 0)
        fit = genes;
    else
        fit = fit - 1;
}
int main(int argc, char** argv)
{
    int genes = atoi(argv[1]);
    char* type = (char*)malloc(sizeof(char)*50);
    type = argv[2]; 
    int nk_k;
    char* firstLine = (char*)malloc(sizeof(char)*3*genes+1);
    char* line = (char*)malloc(sizeof(char)*4*genes+1);
    char fileName[30];
    if(!strcmp(type,"nk"))
    {
        nk_k = atoi(argv[3]);
        sprintf(fileName,"%s_%d_%d",type,genes,nk_k);
    }
    else
        sprintf(fileName,"%s_%d",type,genes);

    FILE* fp=fopen(fileName,"w"); 
    srand(time(NULL));

    for(int i=0;i<genes;i++)
    {
        char str[10];
        sprintf(str,"%s%d,","a",i);
        strcat(firstLine,str);
    }
    strcat(firstLine,"fit");
    fprintf(fp,"%s\n",firstLine);
    for(int i=0;i<10000;i++)
    {
        memset(line,0,sizeof(line));
        double fitness = 0;
        for(int j=0;j<genes;j++)
        {
            int random = rand()%2;
            char str[10];
            sprintf(str, "%d,",random);
            strcat(line,str);
        }
        if(!strcmp(type,"onemax"))
            fitness = sumOfGenes(line,genes);  
        else if(!strcmp(type,"royal"))
            fitness = royal(line,genes);
        else if(!strcmp(type,"deception"))
            fitness = deception(line,genes);
        else if(!strcmp(type,"nk"))
            fitness = nk(line,genes,nk_k);

        char tmp[20];
        sprintf(tmp, "%lf",fitness);
        strcat(line,tmp);
        fprintf(fp, "%s\n",line);
    }
    
    fclose(fp);
}
