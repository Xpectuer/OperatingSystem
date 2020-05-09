/*
基于链表实现的LRU淘汰算法
基于链表实现的FIFO算法
基于链表？实现的OPT算法
*/
//TODO:加上页表

//TODO:手写验证算法

#include <stdio.h>
#include <iostream>
#define PageNum 5
#define referenceString 20
using namespace std;



//Reference String for testing

//int static RS[referenceString] = { 5,0,1,2,0,3,0,4,2,3 };
//PF 100%
//int static RS[referenceString] = { 1,2,3,4,5,1,2,3,4,5 };
int static RS[referenceString] ={7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };
int queue[PageNum][referenceString];//记录页面置换记录
//int queueTemp[PageNum][referenceString];//记录每一次循环的结果

//页
typedef struct page{
	//存取rs，空页填-1
	int num;
	
	int time;
}Page;

//建立起一个链表，作为进入内存的记录
struct LinkedPage{
	int num;
    int time;
	LinkedPage * next;
    
    //LinkedPage* prev;
	LinkedPage(int num):num(num),next(nullptr){}
};

Page pn[PageNum];
//head node
LinkedPage* head;
//tail node ,used for deletion
LinkedPage* tail;
int nodeCounter=0;
//page fault counter
int PgFaultCtr=0;


//pub functions 



void clearAll(){
    
    LinkedPage* temp=head;
    while(temp->next){
    
        
        if(head->next){
            temp=temp->next;
            head->next=head->next->next;
            free(temp);
        }
    }
    free(head);
}

void init(){
    for(int i=0;i<PageNum;++i){
        pn[i].num=-1;
        pn[i].time=0;
    }

    head=new LinkedPage(-1);
    head->time=-1;
    tail=head;
    
    nodeCounter=0;
    PgFaultCtr=0;
}

bool isFull(){
    if(nodeCounter>=PageNum)
        return true;
    else
        return false;
}

void printAll(LinkedPage* head){
    LinkedPage* node=head;
    while(node->next){
        node=node->next;
        printf( "%d " , node->num);
    }
    printf("\n");
    
    
}
//---------------------------------------------------
//FIFO

bool isValid(LinkedPage* head,int rs){
    //遍历链表
    LinkedPage * temp=head;
    for(int i=0;i<nodeCounter&&temp->next;i++){
        
        if(temp->num==rs){
            temp->time=0;
            return true;
        }
        temp=temp->next;
    }
    return false;
}

//Insertion
void insertTail(LinkedPage* head,int rs){
    //initialize newnode
        LinkedPage* temp=new LinkedPage(rs);
        
        temp->next=nullptr;
        tail->next=temp;
    
        tail=temp;

        nodeCounter++;
    
}
//delete last node
void delHead(LinkedPage* tail){
        
        LinkedPage* temp;
        //recycle memory
        
        
        //TODO:修复tail无法前移
    if(head->next){
        temp=head->next;
        head->next = temp->next;
        free(temp);
        nodeCounter--;
    }
    
}


//check is it valid

void FIFO(){
	
	init();
	for(int i=0;i<referenceString;i++){
       printAll(head);
       if(isValid(head,RS[i])){
            //page hit
           // printAll(head);
           //printAll(head);
           
            continue;
        }
		if(!isFull()){
            //TODO:无限输出
			insertTail(head,RS[i]);
			PgFaultCtr++;
         
			continue;
		}
		else{
			
			//page fault occur
			
            delHead(tail);
            insertTail(head,RS[i]);
            PgFaultCtr++;
			//  printAll(head);
		}
        
	

	}
     
  clearAll();
}
//------------------------------------------
//LRU
//In LRU Algo, The linked list regarded as a recorder

int checkBlank(){
    for(int i=0;i<PageNum;i++){
        if(pn[i].num==-1){
            //blank page
            return i;
        }
       
    }
     return -1;//full
    
    
}
//val:value in RS
int findPg(int val){
    for(int i=0;i<PageNum;i++){
        if(pn[i].num==val)
            return i;
        
    }
    return false;
}



bool isValidLRU(int rs){
    LinkedPage* temp=head;
    while(temp->next){
        temp=temp->next;
        if(temp->num==rs)
            return true;
        
    }
    return false;
    
    
}
void insertHead(LinkedPage* head,int rs){
    //insert in head
    
    LinkedPage* temp=new LinkedPage(rs);
    temp->next=head->next;
    head->next=temp;
  
    
    nodeCounter++;
    
}

void deleteNode(LinkedPage* head, int rs){
    //TODO:删除指定值节点
    LinkedPage* curr=head, *temp;
    
    while(curr->next){
        if(curr->next->num==rs){
             temp=curr->next;
            curr->next=curr->next->next;
           
            free(temp);
            break;
        }
        curr=curr->next;
        
    }
}

int deleteTail(LinkedPage* head){
    int val;
    //TODO:删除尾节点
    LinkedPage* curr=head,*temp;
    //TODO: 访问安全性
    while(curr->next&&curr->next->next){
        curr=curr->next;
    }
    temp=curr->next;
    val=temp->num;
    curr->next=curr->next->next;
    free(temp);
    return val;
}

void printPg(){
    for(int i=0;i<PageNum;i++){
        if(pn[i].num!=-1)
            printf("%d ",pn[i].num);
        
    }
    printf("\n");
}

void LRU(){
    init();
    for(int i=0;i<referenceString;i++){
        printPg();
        if(isValidLRU(RS[i])){
            //hit
           
            deleteNode(head, RS[i]);
            insertHead(head, RS[i]);
            
            continue;
        }
        else{
            int blank=checkBlank();
            if(blank!=-1){
                
                insertHead( head, RS[i] );
                
                pn[blank].num=RS[i];
                
                PgFaultCtr++;
            }
            else{
                //full
                //改进版，返回值
                int index=findPg(deleteTail(head));
                insertHead(head, RS[i]);
               //页替换
                pn[index].num=RS[i];
                
                PgFaultCtr++;
                
            }
            
        }
        
        }
        clearAll();
        
    }
//---------------------
//OPT

//set Frequency in FUTURE
void setTime(LinkedPage * head,int p){
    if(!head->next)
        return;
    
    LinkedPage* ptr=head->next;
    
    while(ptr){
           
           ptr->time=0;
           for(int index=p;index<referenceString;index++){
               if(ptr->num==RS[index]){
                   break;
               }
               
               ptr->time=ptr->time+1;
               
           }
        ptr=ptr->next;
       }
}


LinkedPage* leastFrequent(LinkedPage* head,int p){
    if(!head->next)
        return nullptr;
    LinkedPage* longestTime=head->next;
    LinkedPage* Ptr=head->next;
    
    setTime(head, p);
    //max-time
    while(Ptr->next){
        
       // longestTime=Ptr;
        if(Ptr->next->time>Ptr->time)
            longestTime=Ptr->next;
        Ptr=Ptr->next;
    }
    return longestTime;
    
}

void substitute(LinkedPage* ptr,int val){
    //for better impl
    ptr->num=val;
}

void OPT(){
    init();
    int val;
    for(int i=0;i<referenceString;++i){
        val=RS[i];
        printAll(head);
        if(isValid(head, val)){
            //hit
            continue;
        }
       
        
        else {
        
            //showTable
           
            if(!isFull()){
                insertHead(head, val);
                PgFaultCtr++;
            }
            else{
                //PE occurs
                //find leastFrequent in the future
                LinkedPage* temp=leastFrequent(head, i);
                //TODO:无法替换正确的位置
                
                substitute(temp,val);
                PgFaultCtr++;
            }
            
            
            
        }
        
        
    }
    
    
    
    clearAll();
}
    
    

    
    




int main()
{
	/* FIFO */
	FIFO();
	double PgRate=(100*((double)PgFaultCtr/(double)referenceString));
    printf("FIFO Page Fault Rate is: %f %% \n",PgRate);
    /* LRU */
    LRU();
    PgRate=(100*((double)PgFaultCtr/(double)referenceString));
    printf("LRU Page Fault Rate is: %f %% \n",PgRate);

    
    OPT();
    PgRate=(100*((double)PgFaultCtr/(double)referenceString));
    printf("OPT Page Fault Rate is: %f %% \n",PgRate);
    
	return 0;
}
