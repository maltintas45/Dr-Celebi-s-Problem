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


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot;
		Task *compeer;
			
		pivot=head;
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

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::close()
{
	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::add(Task *task)
{
	//THIS FUNCTION WILL BE CODED BY YOU
} 

Task * WorkPlan::getTask(int day, int time)
{
	//THIS FUNCTION WILL BE CODED BY YOU
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::remove(Task *target)
{
	//THIS FUNCTION WILL BE CODED BY YOU
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
