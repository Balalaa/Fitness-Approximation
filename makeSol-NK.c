#include <stdio.h>
#include <stdlib.h>

#define AllOrPart 1
typedef struct nk 
{
	int** Rand;
	double** land;

}NK;

int power(int num)
{
	int result=1;

	for(int i=0;i<num;i++)
		result*=2;

	return result;
}
double landscape(int genes, int nk_k, char* binary)
{
	int pow=power(nk_k);
	NK* landscape = (NK*)malloc(sizeof(NK));	
	landscape->Rand=(int**)malloc(sizeof(int*)*genes);
	landscape->land=(double**)malloc(sizeof(double*)*genes);

	for(int i=0;i<genes;i++)
	{
		landscape->land[i]=(double*)malloc(sizeof(double)*pow*2);
		landscape->Rand[i]=(int*)malloc(sizeof(int)*genes);
	}

	char str[200];
  	sprintf(str, "../landscape/land%d_%d",genes,nk_k);
    FILE* pFile = fopen(str,"r");
	
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
int main(int argc, char** argv)
{
	int genes=atoi(argv[1]);
	int nk_k=atoi(argv[2]);
	char* binary=(char*)malloc(sizeof(char)*genes*2+1);
	char filename[30];
	sprintf(filename, "nk%d_%d",genes,nk_k);
	FILE* fp = fopen(filename,"w");
	if(AllOrPart)
	{
		for(int i=0;i<power(genes);i++)
		{
			int ge=i;
			int divider = genes-1;
			double fitness = 0;
			for(int j=0;j<genes;j++)
			{
				if( ge / power(divider) == 1)
				{
					binary[2*j] = '1';
					ge -= power(divider);
				}
				else
					binary[2*j] = '0';
				divider--;
				binary[2*j+1] = ',';
			}
			binary[genes*2+1] = '\0';
			fitness = landscape(genes,nk_k,binary);
			fprintf(fp, "%s%lf\n",binary,fitness);
		}
	}
	else
	{
	}

	fclose(fp);
}
