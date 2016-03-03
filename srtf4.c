#include<stdio.h>

/* Although Logically incorrect but
 * still I feel that this piece of code
 * can be used for learning purpose
 * such as debugging it,
 * treat it as correct one and start debigging
 * 
 * Tutors may use it as a problem code
 * like by not telling them what's wrong in code
 * and asking students to rectify it.
 */

typedef struct 
{
	int pid,bt,at,ct,tt,wt;
}process;

process P[50],RQ[50];
int pn=0,rn=0,startRQ=0,cputime=0;
/*Comments will be added soon */

void copyQ(process source[],process target[]){
int i=0;
for(i=0;i<pn;i++){
	target[i]=source[i];
}
}

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

void sortByAt(process a[],int start,int end)
{
	int i,j;
	for(i=start;i<end-1;i++)
	{
		for(j=start;j<end;j++)
		{
			if(a[i].at > a[i].at)
			{
				swap(&a[i],&a[j]);
			}
			else if((a[i].at == a[j].at) && (i!=j))
			{
				sortByPid(a,start,end);
				return;
			}
		}
	}
}

void sortByBt(process a[],int start,int end)
{
	int i,j;
	for(i=start;i<end-1;i++)
	{
		for(j=i+1;j<end;j++)
		{
			if(a[i].bt > a[j].bt)
			{
				swap(&a[i],&a[j]);
			}
			else if((a[i].bt == a[j].bt) && (i!=j) && (a[i].bt!=0))
			{
				sortByAt(a,start,end);
				return;
			}
		}
	}
}


void executeSJ(process a[])
{
	a[startRQ].bt = a[startRQ].bt -1;
	if(a[startRQ].bt == 0)
	{
		a[startRQ].ct = cputime+1;
		startRQ++;
	}
}


void executeFull(process a[])
{
	int i,j;
	for(i=0;i<rn;i++){
		a[i].ct = a[i].bt + cputime;
		cputime=cputime + a[i].bt;
		a[i].bt=0;
	}
	startRQ=rn;
}


void makeRQ()
{
	int i,j;
	for(i=0;i<pn;i++)
	{
		if(P[i].at == cputime)
		{
			RQ[rn]=P[i];
			rn++;
		}
	}
}

int main()
{
	int i,j,avgTT=0,avgWT=0,totalWT=0,totalTT=0,maxAT=0;
	printf("Specify total no. of processes: ");
	scanf("%d",&pn);
	printf("Now Enter Process Details bt and at\n");
	for(i=0;i<pn;i++)
	{
		P[i].pid = i+1;
		printf("enter arrival time for process %d :\n",P[i].pid );
		scanf("%d",&P[i].at);
		if(P[i].at > maxAT)
			maxAT=P[i].at;
		printf("enter burst time for process %d :",P[i].pid);
		scanf("%d",&P[i].bt);
		P[i].ct=0;
		P[i].tt=0;
		P[i].wt=0;
	}
	do{
		makeRQ();
		sortByBt(RQ,0,rn);
		if(rn!=0 && rn!=pn)
			executeSJ(RQ);
		else if(rn==pn)
			executeFull(RQ);
		cputime++;
	}while(startRQ<rn || (cputime <= maxAT));

	sortByPid(RQ,0,rn);
	for(i=0;i<pn;i++)
		{
			
			P[i].ct=RQ[i].ct;
			P[i].tt=P[i].ct-P[i].at;
			P[i].wt=P[i].tt-P[i].bt;
			totalTT+=P[i].tt;
			totalWT+=P[i].wt;
		}
		avgTT=totalTT/pn;
		avgWT=totalWT/pn;
		displayAll(P,0,pn);
		printf("\nAverage TurnAround Time : %i",avgTT);
		printf("\nAverage Waiting time: %i\n",avgWT);
	return 0;
}
