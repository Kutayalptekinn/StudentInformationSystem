#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct courseinfo{
	char courseCode[30];
	char courseName[50];
	int credit;
	int quota;
	struct courseinfo *next;
	long *studentNumbers;
	int studentCount;
}COURSEINFO;


typedef struct student{
  char name[75];
  char surname[30];
  long number;
  int numberofTakenCourses;
  int numberofTakenCredit;
  struct student *next;
  struct student *prev;
}STUDENT;


typedef struct studentcoursesinfo{
  int ID;
  char courseCode[30];
  long studentNumber;
  struct studentcoursesinfo *next;
} STUDENTCOURSESINFO;

COURSEINFO *AddCourse(COURSEINFO *headCrs);
void AddingProcessCrs(COURSEINFO **headCrs,char courseCode[],char courseName[],int credit,int quota);
STUDENT *ReadStudentsFromFile(STUDENT *headStd);
STUDENT *AddStudent(STUDENT *headStd);
void AddingProcessStd(STUDENT **headStd, long number,char name[],char surname[],int numberofTakenCourses,int numberofTakenCredit);
COURSEINFO *ReadCoursesFromFile(COURSEINFO *headCrs);
COURSEINFO *NewCourse(char courseCode[],char courseName[],int credit,int quota);
STUDENT *NewStudent(long number,char name[],char surname[],int numberofTakenCourses,int numberofTakenCredit);
void DeleteStudent(STUDENT **headStd);
void DeleteCourse(COURSEINFO **headCrs);
STUDENTCOURSESINFO *ChooseCourse(STUDENT *headStd,COURSEINFO *headCrs,STUDENTCOURSESINFO *headStdCrs,int maxCourse,int maxCredit);
STUDENTCOURSESINFO *RemoveCourse(STUDENT *headStd,COURSEINFO *headCrs,STUDENTCOURSESINFO *headStdCrs);
STUDENT *SearchStudent(STUDENT **headStd,long schoolNumber);
COURSEINFO *SearchCourse(COURSEINFO **headCrs,char courseCode[]);
void UpdateStudentsFile(STUDENT *headStd);
STUDENTCOURSESINFO *NewStudentCourses(STUDENTCOURSESINFO *head,char courseCode[],long studentNumber);
void UpdateStudentCourseInfo(STUDENTCOURSESINFO **headStdCrs,long studentNumber,char courseCode[]);
int SearchCourseStudent(STUDENTCOURSESINFO *headStdCrs,char courseCode[],long studentNumber);
STUDENTCOURSESINFO *Search(STUDENTCOURSESINFO *headStdCrs,char courseCode[],long studentNumber);




int main(int argc, char *argv[]) {
	
	int loop=1,used=1,choice,maxCourse,maxCredit,choice2,choice3;
	STUDENT *headStd=NULL;
	COURSEINFO *headCrs=NULL;
	STUDENTCOURSESINFO *headStdCrs=NULL;
	headCrs=ReadCoursesFromFile(headCrs);
	headStd=ReadStudentsFromFile(headStd);
    if(used==1){
    	printf("enter max course number:\n");
   	    scanf("%d",&maxCourse);
     	printf("enter max credit number:\n");
    	scanf("%d",&maxCredit);
    }

	
	while(loop){
	    printf("for teacher press 1, for student press 2, for exit press 3:\n");
	    scanf("%d",&choice);
	
	    if(choice==1){
			printf("for add course press 1 for delete course press 2, for add student press 3 for delete student press 4:\n");
			scanf("%d",&choice2);
			if(choice2==1) headCrs=AddCourse(headCrs);
			else if(choice2==2) DeleteCourse(&headCrs);
			else if(choice2==3) headStd=AddStudent(headStd);
			else if(choice2==4) DeleteStudent(&headStd);
	    }
    	else if(choice==2){
			printf("for add course press 1 for delete course press 2:\n");
			scanf("%d",&choice3);
			if(choice3==1) headStdCrs=ChooseCourse(headStd,headCrs,headStdCrs,maxCourse,maxCredit);
			if(choice3==2) headStdCrs=RemoveCourse(headStd,headCrs,headStdCrs);
    	}
    	else if(choice==3) loop=0;
    	else{
    		printf("you keyed wrong,try again one second after:\n");
    		sleep(1);
    		system("cls");
		}
	}
	return 0;
}

COURSEINFO *ReadCoursesFromFile(COURSEINFO *headCrs)
{
	FILE *fp;	
	fp = fopen("courses.txt", "r");
	if(fp==NULL){
		printf("Not found");
		return NULL;
	}
	
	COURSEINFO *memoryForCourse;
	memoryForCourse = (COURSEINFO*)malloc(sizeof(COURSEINFO));
	while(fscanf(fp,"%s %s %d %d\n",memoryForCourse->courseCode,memoryForCourse->courseName,&memoryForCourse->credit,&memoryForCourse->quota)!=EOF)
	{
		AddingProcessCrs(&headCrs,memoryForCourse->courseCode,memoryForCourse->courseName,memoryForCourse->credit,memoryForCourse->quota);
		memoryForCourse = (COURSEINFO*)malloc(sizeof(COURSEINFO));
	}
	fclose(fp);
    return headCrs;
}
COURSEINFO *AddCourse(COURSEINFO *headCrs)
{
	char courseCode[20];
	printf("Enter course code:");
	scanf("%s",courseCode);
	if(SearchCourse(&headCrs,courseCode)==0){
		COURSEINFO *tmp;
	    tmp=(COURSEINFO*)malloc(sizeof(COURSEINFO));
        strcpy(tmp->courseCode,courseCode);
	    printf("Enter course name:");
	    scanf("%s",&tmp->courseName);
	    printf("Enter course credit:");
	    scanf("%d",&tmp->credit);
	    printf("Enter course quota:");
	    scanf("%d",&tmp->quota);
	
    	AddingProcessCrs(&headCrs,tmp->courseCode,tmp->courseName,tmp->credit,tmp->quota);
    	return headCrs;
	}
	else{
		printf("this lesson has already been added\n");
		return headCrs;
	}

	
}
void AddingProcessCrs(COURSEINFO **headCrs,char courseCode[],char courseName[],int credit,int quota)
{
	COURSEINFO *tmp,*prev=NULL;
	FILE *data;
	
	COURSEINFO *newCourse=NewCourse(courseCode,courseName,credit,quota);
	
	if((*headCrs)==NULL) (*headCrs)=newCourse;

	else{
		tmp=(*headCrs);
		while(tmp!=NULL && strcmp(courseCode,tmp->courseCode)>0){
			prev=tmp;
			tmp=tmp->next;
		}
		if(prev==NULL){
			(*headCrs)=newCourse;
			(*headCrs)->next=tmp;
		}
		else if(tmp==NULL){
			prev->next=newCourse;
			newCourse->next=NULL;
		}
		else{
			prev->next=newCourse;
			newCourse->next=tmp;
		}
	}
	
	data=fopen("courses.txt","w");
	if(data!=NULL){
		tmp = (*headCrs);	
		while(tmp!=NULL)
		{
			fprintf(data,"%s %s %d %d\n",tmp->courseCode,tmp->courseName,tmp->credit,tmp->quota);
			tmp=tmp->next;	
		}
		fclose(data);
	}	
	printf("succesfull!!\n");

}
COURSEINFO *NewCourse(char courseCode[],char courseName[],int credit,int quota)
{
	COURSEINFO *tmp;
	tmp = (COURSEINFO*)malloc(sizeof(COURSEINFO));	
	strcpy(tmp->courseName,courseName);
	strcpy(tmp->courseCode,courseCode);
	tmp->credit = credit;
	tmp->quota = quota;
	tmp->next = NULL;
	tmp->studentCount=0;
	tmp->studentNumbers=(long*)malloc(sizeof(long));
	tmp->studentNumbers[0]=0;
	return tmp;
}
void DeleteCourse(COURSEINFO **headCrs)
{
	COURSEINFO *tmp,*prev=NULL;
	char courseCode[20];
	FILE *data;
	printf("Enter course code:");
	scanf("%s",courseCode);
	
	if((*headCrs)==NULL) printf("not found\n");
	
	else{
		tmp=(*headCrs);
		while(tmp!=NULL && strcmp(courseCode,tmp->courseCode)>0){
			prev=tmp;
			tmp=tmp->next;
		}
		if(prev==NULL && strcmp(courseCode,tmp->courseCode)==0){
			(*headCrs)=tmp->next;
			tmp->next=NULL;
			printf("deleted: %s\n",tmp->courseCode);
			free(tmp);
		}
		else if(tmp->next==NULL && strcmp(courseCode,tmp->courseCode)==0){
			printf("deleted: %s\n",tmp->courseCode);
			free(tmp);
			prev->next=NULL;
		}
		else if(strcmp(courseCode,tmp->courseCode)==0){
			prev->next=tmp->next;
			tmp->next=NULL;
			printf("deleted: %s\n",tmp->courseCode);
			free(tmp);
		}
		else{
			printf("not found\n");
		}
	}
	
	data=fopen("courses.txt","w");
	if(data!=NULL){
		tmp = (*headCrs);	
		while(tmp!=NULL){
			fprintf(data,"%s %s %d %d\n",tmp->courseCode,tmp->courseName,tmp->credit,tmp->quota);
			tmp=tmp->next;	
		}
		fclose(data);
	}
}
STUDENT *ReadStudentsFromFile(STUDENT *headStd)
{
	FILE *fp;	
	fp = fopen("students.txt", "r");
	if(fp==NULL){
		printf("Not found\n");
		return NULL;
	}
	
	STUDENT *memoryForStudent;
	memoryForStudent = (STUDENT*)malloc(sizeof(STUDENT));
	while(fscanf(fp,"%ld %s %s %d %d\n",&memoryForStudent->number,memoryForStudent->name,memoryForStudent->surname,&memoryForStudent->numberofTakenCourses,&memoryForStudent->numberofTakenCredit)!=EOF)
	{
        AddingProcessStd(&headStd,memoryForStudent->number,memoryForStudent->name,memoryForStudent->surname,memoryForStudent->numberofTakenCourses,memoryForStudent->numberofTakenCredit);
		memoryForStudent = (STUDENT*)malloc(sizeof(STUDENT));
	}
	fclose(fp);
	return headStd;
}
STUDENT *AddStudent(STUDENT *headStd)
{
	long number;
	printf("Enter number:\n");
	scanf("%ld",&number);
	if(SearchStudent(&headStd,number)==0){
		STUDENT *tmp;
    	tmp=(STUDENT*)malloc(sizeof(STUDENT));
	    tmp->number=number;
	    printf("Enter name:");
	    scanf("%s",tmp->name);
	    printf("Enter surname:");
	    scanf("%s",&tmp->surname);
        tmp->numberofTakenCourses=0;
        tmp->numberofTakenCredit=0;       
    
        AddingProcessStd(&headStd,tmp->number,tmp->name,tmp->surname,tmp->numberofTakenCourses,tmp->numberofTakenCredit);
        return headStd;
	}
	else{
		printf("this student has already been added\n");
		return headStd;
	}

}
void AddingProcessStd(STUDENT **headStd, long number,char name[],char surname[],int numberofTakenCourses,int numberofTakenCredit)
{
    
	STUDENT *tmp,*prev=NULL;
	FILE *data;
	
	STUDENT *newStudent=NewStudent(number,name,surname,numberofTakenCourses,numberofTakenCredit);
	
	if((*headStd)==NULL) (*headStd)=newStudent;
	else{
		tmp=(*headStd);
		while(tmp!=NULL && number>tmp->number){
			prev=tmp;
		    tmp=tmp->next;
		}
		if(prev==NULL){
			(*headStd)=newStudent;
			(*headStd)->next=tmp;
			tmp->prev=(*headStd);
		}
		else if(tmp==NULL){
			prev->next=newStudent;
			newStudent->prev=prev;
		}
		else{
			prev->next=newStudent;
			newStudent->prev=prev;
			newStudent->next=tmp;
			tmp->prev=newStudent;
		}
	}
	UpdateStudentsFile(*headStd);
	printf("succesfull!!\n");

}

STUDENT *NewStudent(long number,char name[],char surname[],int numberofTakenCourses,int numberofTakenCredit)
{
	STUDENT* tmp;
	tmp = (STUDENT*) malloc(sizeof(STUDENT));	
	tmp->number=number;
	strcpy(tmp->name,name);
	strcpy(tmp->surname,surname);
	tmp->numberofTakenCourses = numberofTakenCourses;
	tmp->numberofTakenCredit = numberofTakenCredit;
	tmp->next = NULL;
	tmp->prev = NULL;
	return tmp;
}

void DeleteStudent(STUDENT **headStd)
{
	long studentNumber;
	STUDENT *tmp,*prev;
	FILE *data;
	
	printf("Enter student number:");
	scanf("%ld",&studentNumber);
	
	tmp=(*headStd);
	if(tmp->number==studentNumber){
		(*headStd)=tmp->next;
		printf("%d deleted\n",tmp->number);
		free(tmp);	
	}
	else{
		while(tmp!=NULL && tmp->number!=studentNumber){
			prev=tmp;
			tmp = tmp->next;
		}
		if(tmp== NULL){
			printf("not found!\n");
		}
		else{
			prev->next=tmp->next;
			if(tmp->next!=NULL) tmp->next->prev=prev;
			printf("\n%d deleted\n",tmp->number);
			free(tmp);
		}
	}
	
    UpdateStudentsFile(*headStd);
}

STUDENTCOURSESINFO *ChooseCourse(STUDENT *headStd,COURSEINFO *headCrs,STUDENTCOURSESINFO *headStdCrs,int maxCourse,int maxCredit)
{
	STUDENT *tmp;
	COURSEINFO *tmp2;
	STUDENTCOURSESINFO *tmp3;
	long schoolNumber;
	int choice;
	char courseCode[20];
	int i=0,k;
	printf("Enter school number:\n");
	scanf("%ld",&schoolNumber);
	
	tmp=SearchStudent(&headStd,schoolNumber);
	if(tmp){
		printf("Enter course code:\n");
		scanf("%s",courseCode);
		tmp2=SearchCourse(&headCrs,courseCode);
		if(tmp2){
		 if(SearchCourseStudent(headStdCrs,courseCode,schoolNumber)){	
		   if(tmp->numberofTakenCredit+tmp2->credit<maxCredit && tmp->numberofTakenCourses<maxCourse && tmp2->studentCount<tmp2->quota){
		       while(i<tmp2->studentCount && schoolNumber>tmp2->studentNumbers[i]){
				i++;
			    }
			    for(k=(tmp2->studentCount);k>i;k--){
				tmp2->studentNumbers[k]=tmp2->studentNumbers[k-1];
		    	}
			    tmp2->studentNumbers[i]=schoolNumber;
			    tmp2->studentCount++;
			    tmp2->studentNumbers=(long*)realloc(tmp2->studentNumbers,(tmp2->studentCount+1)*(sizeof(long)));	
			    tmp->numberofTakenCourses++;
			    tmp->numberofTakenCredit+=tmp2->credit;	
			    UpdateStudentsFile(headStd);
			    UpdateStudentCourseInfo(&headStdCrs,schoolNumber,courseCode);
		   }
		   else if(tmp->numberofTakenCredit+tmp2->credit>maxCredit) printf("Exceeds credit\n");
		   else if(tmp->numberofTakenCourses==maxCourse) printf("Exceeds nunmber of taken course\n");
		   else if(tmp2->studentCount==tmp2->quota) printf("full quota\n");
		 }
		}
	}
	return headStdCrs;
}
STUDENTCOURSESINFO *RemoveCourse(STUDENT *headStd,COURSEINFO *headCrs,STUDENTCOURSESINFO *headStdCrs)
{
	FILE *data;
	STUDENT *tmp;
	COURSEINFO *tmp2;
	STUDENTCOURSESINFO *tmp3,*prev;
	tmp3=headStdCrs;
	long schoolNumber;
	int choice;
	char courseCode[20];
	int i=0,k;
	printf("Enter school number:\n");
	scanf("%ld",&schoolNumber);
	
	tmp=SearchStudent(&headStd,schoolNumber);
	if(tmp){
		printf("Enter course code:\n");
		scanf("%s",courseCode);
		tmp2=SearchCourse(&headCrs,courseCode);
		if(tmp2 && Search(headStdCrs,courseCode,schoolNumber)){
			while(i<tmp2->studentCount && schoolNumber>=tmp2->studentNumbers[i]){
				i++;
			}
			for(k=i;k<(tmp2->studentCount);k++){
				tmp2->studentNumbers[k]=tmp2->studentNumbers[k+1];
			}
			tmp2->studentCount--;	
			tmp->numberofTakenCourses--;
			tmp->numberofTakenCredit-=tmp2->credit;	
			UpdateStudentsFile(headStd);
			
			if(tmp3->studentNumber==schoolNumber && strcmp(tmp3->courseCode,courseCode)==0){
	    	headStdCrs=tmp3->next;
	    	printf("%d deleted\n",tmp3->studentNumber);
	    	free(tmp3);	
        	}
        	else{
        		while(tmp3!=NULL && (tmp3->studentNumber!=schoolNumber || strcmp(tmp3->courseCode,courseCode)!=0)){
	        		prev=tmp3;
		        	tmp3 = tmp3->next;
    		    }
    	    	if(tmp3== NULL){
	    	    	printf("not found!\n");
    	    	}
	    	    else{
		    	    prev->next=tmp3->next;
	        		printf("\n%d deleted\n",tmp3->studentNumber);
		        	free(tmp3);
      		    }
        	}
        	
           	data=fopen("StudentCourseRegister.txt","w");
        	if(data!=NULL){
        		tmp3=headStdCrs;	
    		    while(tmp3!=NULL){
    	    		fprintf(data,"%d %s %d\n",tmp3->ID,tmp3->courseCode,tmp3->studentNumber);
        			tmp3=tmp3->next;
     	    	}
    		fclose(data);
        	}
		}
	}
	return headStdCrs;
}
STUDENT *SearchStudent(STUDENT **headStd,long schoolNumber)
{
	STUDENT *tmp;
	tmp=(*headStd);
	while(tmp!=NULL && tmp->number<schoolNumber){
		tmp = tmp->next;
	}
	if(tmp!=NULL && schoolNumber==tmp->number){
		return tmp;
	}
	else{
		printf("not found\n");
		return 0;
	}
}

COURSEINFO *SearchCourse(COURSEINFO **headCrs,char courseCode[])
{
	COURSEINFO *tmp;
	tmp=(*headCrs);
	while(tmp!=NULL && strcmp(courseCode,tmp->courseCode)>0){
		tmp = tmp->next;
	}
	if(tmp!=NULL && strcmp(courseCode,tmp->courseCode)==0){
		return tmp;
	}
	else{
		printf("not found\n");
		return 0;
	}
}

STUDENTCOURSESINFO *Search(STUDENTCOURSESINFO *headStdCrs,char courseCode[],long studentNumber)
{
	STUDENTCOURSESINFO *tmp;
	tmp=headStdCrs;
	while(tmp!=NULL && (strcmp(courseCode,tmp->courseCode)!=0 || studentNumber!=tmp->studentNumber)){
		tmp = tmp->next;
	}
	if(tmp!=NULL && strcmp(courseCode,tmp->courseCode)==0 && studentNumber==tmp->studentNumber){
		return tmp;
	}
	else{
		printf("not found\n");
		return 0;
	}
}

int SearchCourseStudent(STUDENTCOURSESINFO *headStdCrs,char courseCode[],long studentNumber)
{
	STUDENTCOURSESINFO *tmp;
	tmp=headStdCrs;
	while(tmp!=NULL && (strcmp(courseCode,tmp->courseCode)!=0 || studentNumber!=tmp->studentNumber)){
		tmp = tmp->next;
	}
	if(tmp==NULL){
		return 1;
	}
	else{
		printf("same lesson and same student has already added\n");
		return 0;
	}
}

void UpdateStudentsFile(STUDENT *headStd)
{
	FILE *data;
	STUDENT *tmp;
	
	data=fopen("students.txt","w");
	if(data!=NULL){
		tmp=(headStd);	
		while(tmp!=NULL){
			fprintf(data,"%ld %s %s %d %d\n",tmp->number,tmp->name,tmp->surname,tmp->numberofTakenCourses,tmp->numberofTakenCredit);
			tmp=tmp->next;
		}
		fclose(data);
	}	
}

void UpdateStudentCourseInfo(STUDENTCOURSESINFO **headStdCrs,long studentNumber,char courseCode[])
{
	FILE *data;
	STUDENTCOURSESINFO *prev;
	STUDENTCOURSESINFO *tmp3;
	STUDENTCOURSESINFO *newStudentCourses=NewStudentCourses(*headStdCrs,courseCode,studentNumber);
	int i=1;
    
    if((*headStdCrs)==NULL){
    	newStudentCourses->ID=i;
    	(*headStdCrs)=newStudentCourses;
	}
	else{
        tmp3=(*headStdCrs);
        while(tmp3!=NULL){
        	prev=tmp3;
        	tmp3=tmp3->next;
        	i++;
		}
		newStudentCourses->ID=i;
		prev->next=newStudentCourses;
	}
	
	data=fopen("StudentCourseRegister.txt","w");
	if(data!=NULL){
		tmp3=(*headStdCrs);	
		while(tmp3!=NULL){
			fprintf(data,"%d %s %d\n",tmp3->ID,tmp3->courseCode,tmp3->studentNumber);
			tmp3=tmp3->next;
		}
		fclose(data);
	}
	printf("succesfull!!\n");	
}

STUDENTCOURSESINFO *NewStudentCourses(STUDENTCOURSESINFO *head,char courseCode[],long studentNumber)
{
	STUDENTCOURSESINFO* tmp;
	tmp = (STUDENTCOURSESINFO*)malloc(sizeof(STUDENTCOURSESINFO));	
	strcpy(tmp->courseCode,courseCode);
    tmp->studentNumber=studentNumber;
    tmp->next=NULL;
	return tmp;	
}
