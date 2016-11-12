#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){
	int f[150]={0};
	int i,n;
	FILE *fp=fopen("input.txt","wt");
	srand(time(NULL));
	for(i=0;i<80;i++){
		n=rand()%150;
		if(f[n]==0){
			f[n]=1;
			fprintf(fp,"%d\n",n+1);
		}else{
			i--;
		}
	}
	fclose(fp);
}