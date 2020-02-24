/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 

void WorkPlan::create()
{		
	head=NULL;
}

void WorkPlan::close()
{
	head->previous->next=NULL; // unlink the next pointer of tail and assign NULL to check	
	while(head!=NULL)
	{
		//cout<<"current head\t:"<<head->name<<endl;		
		Task *tmp;
		Task *compeer=head->counterpart;				
		while(compeer!=NULL)
		{
			tmp=compeer;
			compeer=compeer->counterpart;
			cout<<tmp->name<<" is deleting"<<endl;
			delete tmp->name;
			delete tmp;
		}
		tmp=head;	
		head=head->next;
		cout<<tmp->name<<" is deleting"<<endl;		
		delete tmp->name;		
		delete tmp;
	}
}

void WorkPlan::add(Task *task)
{
	Task *tmp=new Task();
	tmp->name=new char[strlen(task->name)];
	strcpy(tmp->name,task->name);
	tmp->day=task->day;
	tmp->time=task->time;
	tmp->priority=task->priority;
	Task *pivot;
	Task *compeer;
	Task *tail;
	Task *predecessor;
	Task *excompeer;		
	Task *successor;
		
	cout<<"Task "<<tmp->name<<"("<<tmp->priority<<") on "<<tmp->day<<".day at "<<std::setw(2)<< tmp->time <<":00"<<endl;

	tmp->next=tmp;
	tmp->previous=tmp;
	tmp->counterpart=NULL;

	int step=0;

	if(head==NULL)
	{
		cout<<"added first task"<<endl;
		head=tmp;
	}
	else 
	{		
		tail=head->previous;

		bool goback=abs(tail->day - tmp->day) < abs(head->day - tmp->day) ? true: false;

		if (head->day > tmp->day) // adding to head
		{
		 	tmp->next = head;
			tmp->previous = tail;
			head->previous = tmp;
			tail->next = tmp;
			head = tmp;
			cout<<"added before head"<<endl;
			updatePointersOfPreviousDayTasks(tmp);		
		}
		else if(tail->day < tmp->day) // adding to tail
		{
			tail->next=tmp;
			tmp->next=head;
			tmp->previous=tail;
			head->previous=tmp;
			cout<<"added after tail"<<endl;
			updatePointersOfPreviousDayTasks(tmp);
		}
		else
		{
			pivot=head;
			int dif=tmp->day - pivot->day;
			int k=1;
			while(k*dif>0)
			{				
				k=goback?-1:1;
				if (goback)
					pivot=pivot->previous;
				else
					pivot=pivot->next;
				dif=tmp->day - pivot->day;
			}
			
			if (pivot->day==tmp->day) //adding to compeer
			{
				compeer=pivot;
				successor=compeer->next;
				predecessor=compeer->previous;
				char *exname;
				int expriority;
				while (compeer!=NULL && compeer->time <= tmp->time)
				{							
					excompeer=compeer;
					if (compeer->time == tmp->time)
					{
						cout<<"There is an other task in this time"<<endl;
						if(compeer->priority < tmp->priority)
						{
							cout<<"Because of priority level '"<< tmp->name <<"'("<<tmp->priority<<")' has been added instead of "<< compeer->name <<"'("<< compeer->priority <<")"<<endl;  
							exname=new char[strlen(compeer->name)];
							exname=compeer->name;
							expriority=compeer->priority;
							compeer->name=new char [strlen(tmp->name)];
							strcpy(compeer->name,tmp->name);
							compeer->priority=tmp->priority;
							
							tmp->name=new char[strlen(exname)];
							strcpy(tmp->name,exname);
							tmp->priority=expriority;
							delete [] exname;
						}
						else
							cout<<"Because of priority level '"<< tmp->name <<"'("<<tmp->priority<<")' could not be added instead of '"<< compeer->name <<"'("<< compeer->priority <<")"<<endl;
						checkAvailableNextTimesFor(compeer);
						tmp->day=getUsableDay();
						tmp->time=getUsableTime();
						cout<<"First available day and hour :"<<tmp->day<<".day at "<<std::setw(2)<< tmp->time <<":00"<<endl;;
						
						// add modified and suspended task
						add(tmp);		
						step=-1;
						break;
					}
					compeer=compeer->counterpart;
					step++;
				}	

				if(step==0)
				{
					//be carefull we may deal with header

					if(compeer==head)
						head=tmp;		
					cout<<"compeer as first"<<endl;						
					successor->previous=tmp;
					predecessor->next=tmp;
					tmp->next=successor;
					tmp->previous=predecessor;
					tmp->counterpart=compeer;					 
					compeer->previous=NULL;
					compeer->next=successor;

					//update previous and next pointers of all other task in this date
					updatePointersOfPreviousDayTasks(tmp); 
				}
				else if(step>0)
				{
					cout<<"compeer as in "<<step<<endl;		
					excompeer->counterpart=tmp;	
					tmp->counterpart=compeer;
					tmp->previous=NULL;
					tmp->next=successor;
				}
				if(step>=0)
					cout<<"added as compeer"<<endl;
			}			
			else //add before
			{	
				successor=goback?pivot->next:pivot;
				predecessor=successor->previous;
				predecessor->next=tmp;
				tmp->previous=predecessor;
				tmp->next=successor;
				successor->previous=tmp;
				cout<<"added before "<<successor->name<<endl;
				updatePointersOfPreviousDayTasks(tmp);
			}
		}
	}
	if(step!=-1)
	{
		cout<<"'"<< tmp->name <<"'("<<tmp->priority<<")' has been added succesfully"<<endl;
		display(true,false);
	}
} 
void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *compeer;			
		Task *pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}
void WorkPlan::updatePointersOfPreviousDayTasks(Task *tmp)
{
	Task * predecessor;
	Task * compeer;
	if(tmp->previous!=NULL)
	{
		predecessor=tmp->previous;
		compeer=predecessor->counterpart;
		while(compeer!=NULL)
		{
			compeer->next=tmp;
			compeer->previous=NULL;
			compeer=compeer->counterpart;
		}
	}
}

Task * WorkPlan::getTask(int day, int time)
{
	Task *target=new Task;

	target->day=day;
	target->time=time;	
	
	if (head!=NULL)
	{
		Task *tail=head->previous;
		bool goback=abs(tail->day - target->day) < abs(head->day - target->day) ? true: false;
		Task *pivot;

		pivot=head;
		int k=1;
		int dif=target->day - pivot->day;
		while(k*dif>0)
		{				
			k=goback?-1:1;
			if (goback)
				pivot=pivot->previous;
			else
				pivot=pivot->next;
			dif=target->day - pivot->day;
		}
		if(pivot->day==target->day)
		{
			while(pivot!=NULL)
			{	
				if (pivot->time==target->time)
				{
					cout<<pivot->name<<" is on "<<pivot->day<<".day at "<<std::setw(2)<< pivot->time <<":00"<<endl;
					return pivot;
				}
				pivot=pivot->counterpart;				
			}
			cout<<"There is not any task on "<<target->day<<".day at "<<std::setw(2)<< target->time <<":00"<<endl;
		}		
		else
			cout<<"There is not any task on "<<target->day<<".day"<<endl;
	}
	else
		cout<<"There is no task yet!"<<endl;
	delete target;
	return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	//if there is not any task in day do not add the delayed task and do not delay task after the last hour 
	int begin_hour=8;
	int end_hour=16;
	bool first_assigned=false;
	//default delayed date and time if no available time exists
	usable_day=delayed->time+1<end_hour?delayed->day:delayed->day+1;
	usable_time=delayed->time+1<end_hour?delayed->time+1:8;
	if (delayed!=NULL)
	{		
		int current_hour=delayed->time;
		cout<<"Next available times for the task that is on "<<delayed->day<<".day at "<<std::setw(2)<< delayed->time <<":00"<<endl;
		Task *pivot;
		Task *compeer;
		bool stop=false;
		for(pivot=delayed;!stop;pivot=pivot->next)
		{
			cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;			
			while(compeer!=NULL)
			{
				for(;current_hour<compeer->time;current_hour++)
				{
					if(current_hour>end_hour)
						break; 		
					if (!first_assigned)
					{
						usable_day=compeer->day;
						usable_time=current_hour;
						first_assigned=true;
					}
					cout<<"\t"<<std::setw(2)<<current_hour<<":00"<<endl;
					
				}
				current_hour++;
				if(current_hour>end_hour)
					break; 				
				compeer=compeer->counterpart;
			}
			current_hour=begin_hour;
			if(pivot->next->day<pivot->day)
				stop=true;
		}
	}
	else
		cout<<"There is no task that you have tried to suspend!"<<endl;
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	//delay all task of a day to first available time with respect to current order
	if (head!=NULL)
	{		
		bool euraka=false;
		int initial_day=day;	
		Task *pivot=head;
		do
		{
			if (pivot->day==day)
			{
				euraka=true;
				break;
			}			
			pivot=pivot->next;
		}while(pivot!=head);
		if(euraka)
		{
			// unlink the first task of the day 
			Task * predecessor;
			Task * successor;
			Task * endtask;			
			predecessor=pivot->previous;
			successor=pivot->next;
			// be carefull you may deal with head
			if (pivot==head)	
				head=successor;			
			predecessor->next=successor;	
			successor->previous=predecessor;
			// delay all task to the first available times respect to their orders
			int day,time;
			//if you are dealing with the end day, delay it one day later not to head day
			// get the end task of day
			endtask=pivot;	
			while(endtask->counterpart!=NULL)
				endtask=endtask->counterpart;
			for(;pivot!=NULL;pivot=pivot->counterpart)
			{
				checkAvailableNextTimesFor(endtask);
				day=getUsableDay();
				time=getUsableTime();
				if (day==initial_day)//that means there is not available time
				{
					day++;
					time=8;
				}
				pivot->day=day;
				pivot->time=time;
				add(pivot);
				// to avoid to change order
				endtask=predecessor->next;
				while (endtask->day<pivot->day)
					endtask=endtask->next;
				while (endtask->time<pivot->time)
					endtask=endtask->counterpart;
			}
		}
		else
			cout<<"There is no task on "<<day<<".day"<<endl;

	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}

void WorkPlan::remove(Task *target)
{
	if (head!=NULL && target!=NULL)
	{
		Task *tail=head->previous;
		bool goback=abs(tail->day - target->day) < abs(head->day - target->day) ? true: false;
		Task *pivot;
		Task *newpivot;
		Task *predecessor;
		Task *excompeer;		
		Task *successor;
		pivot=head;
		int step=0;
		int k=1;
		int dif=target->day - pivot->day;
		while(k*dif>0)
		{				
			k=goback?-1:1;
			if (goback)
				pivot=pivot->previous;
			else
				pivot=pivot->next;
			dif=target->day - pivot->day;
		}
		if(pivot->day==target->day)
		{	
			successor=pivot->next;
			predecessor=pivot->previous;
			while(pivot->time!=target->time && pivot!=NULL)
			{
				excompeer=pivot;
				pivot=pivot->counterpart;
				step++;
			}
			if(pivot->time==target->time)
			{
				if(step==0)
				{
					if(pivot->counterpart!=NULL)
						newpivot=pivot->counterpart;
					else
					{
						newpivot=pivot->next;
						successor=newpivot->next;
					}
					//be carefull we may deal with header
					if (pivot==head)
						head=newpivot;				
					
					successor->previous=newpivot;
					predecessor->next=newpivot;
					newpivot->previous=predecessor;
					newpivot->next=successor; updatePointersOfPreviousDayTasks(newpivot);											
				}
				else
				{
					excompeer->counterpart=pivot->counterpart;
				}				
				delete pivot->name;
				delete pivot;
				cout<<"Succesfully deleted the task on "<<target->day<<".day at "<<std::setw(2)<< target->time <<":00"<<endl;
			}
			else
				cout<<"There is not any task on "<<target->day<<".day at "<<std::setw(2)<< target->time <<":00"<<endl;
		}		
		else
			cout<<"There is not any task on "<<target->day<<".day"<<endl;
	}
	else
		cout<<"There is no task yet or the task you want to delete does not exist!"<<endl;
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot;
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
