#include<stdio.h>

typedef struct{
int pid,at,bt,ct,tt,wt;
} process;

process P[50];
int cputime=0,n;

void displayAll(process a[],int start,int end)// start is exact pointer whereas end is UB
{
	int i;
	printf("Displaying Processes and associated Details:\n");
	printf("------------------------------------------------\n");
	printf("PID\tAT\tBT\tCT\tTT\tWT");
	printf("\n-----------------------------------------------\n");
	if(start >= 0)
	for(i=start;i<end;i++)
	{
		printf("%i \t%i \t%i \t%i \t%i \t%i\n",a[i].pid,a[i].at,a[i].bt,a[i].ct,a[i].tt,a[i].wt);
	}
}

void swap(process *a,process *b)
{
	process temp=*b;
	*b=*a;
	*a=temp;
}

void sortByPid(process a[],int start,int end)
{
	int i,j;
	for(i=start;i<end-1;i++)
	{
		for(j=i+1;j<end;j++)
		{
			if(a[i].pid > a[j].pid)
			{
				swap(&a[i],&a[j]);
			}
		}
	}
}

int compareAndSortByPid(process *a,process *b)
{
	if((a->pid) > (b->pid)){

		swap(a,b);
		return 1;
	}
	else{
		
		return -1;
	}
}

void executeFull(process a[])
{
	int i,j;
	for(i=0;i<n;i++){
		a[i].ct = a[i].bt + cputime;
		cputime=cputime + a[i].bt;
	}
}

void sortByAt(process a[],int start,int end){
	int i,j;
	for(i=start;i<end-1;i++)
	{
		for(j=i+1;j<end;j++)
		{
			if(a[i].at > a[j].at)
			{
				swap(&a[i],&a[j]);
			}
			else if((a[i].at == a[j].at) && (i!=j))
			{
				compareAndSortByPid(&a[i],&a[j]);
			}
		}
	}
}

int main(){
	int i,totalTT=0,totalWT=0;
	float avgTT=0.0,avgWT=0.0;
	printf("Enter total number of processes:-");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		P[i].pid=i+1;
		printf("Specify Arrival time for Process %i: ",P[i].pid);
		scanf("%d",&P[i].at);
		printf("Specify Burst time for Process %i: ",P[i].pid);
		scanf("%d",&P[i].bt);
		P[i].ct=0;
		P[i].tt=0;
		P[i].wt=0;
	}
	sortByAt(P,0,n);
	executeFull(P);
	sortByPid(P,0,n);
	for(i=0;i<n;i++)
	{
			P[i].tt=P[i].ct-P[i].at;
			P[i].wt=P[i].tt-P[i].bt;
			totalTT+=P[i].tt;
			totalWT+=P[i].wt;
	}
	printf("\n");
	displayAll(P,0,n);
	avgTT=(float)totalTT/n;/* average Turnaround time calculation*/
	avgWT=(float)totalWT/n;/* average Waiting time calculation*/
	printf("\nAverage TurnAround Time : %.2f",avgTT);
	printf("\nAverage Waiting time: %.2f\n",avgWT);

	return 0;
}
