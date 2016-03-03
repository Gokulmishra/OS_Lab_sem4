#include<stdio.h>
/* final bug free version in my opinion
*  although quite more complex i.e.
*  more number of instructions
*  will reduce wherever functions can be reduced
*/

/*structure for a type PROCESS
 * with resquired properties
 * pid = process ID
 * bt = burst time
 * at = arrival time
 * ct = completion time/ finifsh time
 * tt = turnaround time
 * wt = wait time
 */

typedef struct 
{
	int pid,bt,at,ct,tt,wt;
}process;
/*declaring two arrays 
 * which will be used to work as queues
 * P holds processed  entered by user
 * R is ready queue
 * we will update ready queue as per arrival times of processes
 */
process P[50],RQ[50];

/* pn is total number of processes in P[] and rn is total
 * number of processes in R[]
 * StartRQ works as a pointer to process which will be executed
 * cputime will work as clock
 * we will note starting time and finishing time
 * by this
 */
int pn=0,rn=0,startRQ=0,cputime=0;

/* A simple program to display processes in Qs and their 
 * associated properties
 */
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

/* this will do simple swapping
 * used mainly in sorting etc
 * whiel arranging the Qs process
 */

void swap(process *a,process *b)
{
	process temp=*b;
	*b=*a;
	*a=temp;
}

/* was used for arranging processes
 * in initial versions
 * can be removed now as 
 * only used one time
 * obvioiusly after replacing it will appropriate code
 * at function calling line
 */

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

/*This funcyion will comprae two processes  by their 
 * PIDs and swap if required
 * while arranging for Execution
 */
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
/*This funcyion will comprae two processes  by their 
 * Arrival time and swap if required
 *ie.e only swap if former value is greater else
 * if values are same then compare by their PIDs
 * while arranging for Execution
 */
int compareAndSortByAt(process *a,process *b)
{
	if((a->at) > (b->at)){

		swap(a,b);
		return 1;
	}
	else if((a->at) < (b->at)){
		return -1;
	}
	else{
		compareAndSortByPid(a,b);
		return 0;
	}
}

/*This is one of the main hero(s) in program ;)
 * This will sort the process(in ready Q) according to
 *their Burst Times, if any ambiguity is
 * encountered then refer Arrival time
 * if the ambiguity still persists go for PIDs
 */

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
				compareAndSortByAt(&a[i],&a[j]);
			}
		}
	}
}

/*This is another hero :- execute Shortest Job in Q
 * it will EXECUTE process pointed by StartRQ
 * FOR 1 Time UNIT only
 *Execution is simply done by reducing burst time by one
 *
 *and note the completion time if process gets finishes after being executed
 * it is cputime  + 1 due to the way cputime is incremented 
 * in main program
 */

void executeSJ(process a[])
{
	if(a[startRQ].bt!=0)
	a[startRQ].bt = a[startRQ].bt -1;
	if(a[startRQ].bt == 0)
	{
		a[startRQ].ct = cputime+1;
		startRQ++;
	}
}

/* This is the other companion hero of previous function
 * when there are no more processes left to enter into ready q
 * the scheduling becomes kind of non-preemptive
 * and remaining processes execute one by
 * obviously in shortest to longest burst time order
 */

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

/*this function updates the ready queue unless all process have been transfered to R Q
 * from new state
 * this fucntion matches cputime with arrival time
 * of process and transits them into ready Q
 */

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
	int i,j,maxAT=0,totalWT=0,totalTT=0;
	/*maxAt will store maximum arrival time
	 * among all the prcesses
	 */
	float avgTT=0,avgWT=0;
	/* Entering required Data */
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
		P[i].ct=0;/* initializing with zero */
		P[i].tt=0;/* initializing with zero */
		P[i].wt=0;/* initializing with zero */
	}
	do{
		makeRQ(); /* make and upadte ready Q */
		sortByBt(RQ,0,rn); /* sort the ready Q */
		/* rn!=0 means ready Q not empty rn!=pn means that total no
		 * of process in ready Q doesn't equal to total no. of processes
		 * as in that case all process can be executed in 
		 * non-preemptive manner
		 */
		if(rn!=0 && rn!=pn) 
			executeSJ(RQ);
		else if(rn==pn)
			executeFull(RQ);
		cputime++;/* increase the clock counter */
	}while(startRQ<rn || (cputime <= maxAT));
	/*there may be instances when minimum arrival time is more
	 * than ZERO, in that case cputime should increment 
	 * i.e. loop should go on
	 * as the startRQ is initialized as zero
	 * it may create problem in above said case
	 * so insure that loop runs 
	 * here condition (cputime <= maxAT) is used
	 */


/* after all the processes has been terminated
 * the ready Q is sorted by PID
 * so that CTscan be assigned easily without comparing the PIDs
 */
	sortByPid(RQ,0,rn);
 /* assiging and calculating the CT and remaining things */
	for(i=0;i<pn;i++)
		{
			
			P[i].ct=RQ[i].ct;
			P[i].tt=P[i].ct-P[i].at;
			P[i].wt=P[i].tt-P[i].bt;
			totalTT+=P[i].tt;
			totalWT+=P[i].wt;
		}
		avgTT=(float)totalTT/pn;/* average Turnaround time calculation*/
		avgWT=(float)totalWT/pn;/* average Waiting time calculation*/
		displayAll(P,0,pn);/* Displaying the process details*/
		printf("\nAverage TurnAround Time : %.2f",avgTT);
		printf("\nAverage Waiting time: %.2f\n",avgWT);
	return 0;
}