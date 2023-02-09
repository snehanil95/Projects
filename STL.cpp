
#include<iostream>
using namespace std;

#pragma pack(1)
template <class T>
struct nodeS
{
    T data;
    struct nodeS * next;
};
template <class T>
struct nodeD
{
    T data;
    struct nodeD<T>* prev;
    struct nodeD<T> * next;
};


template <class T>
class DoublyCL
{
    public:
        struct nodeD<T> *First;
       // struct nodeD<T> *Last;

        DoublyCL();

        void InserFirst(T no);
        void InserLast(T no);
        void InsertAtPosition(T no,int ipos);

        void DeleteFirst();
        void DeleteLast();
        void DeleteAtPosition(int ipos);

        void Display();
        int Count();  
};

template <class T>
DoublyCL <T>::DoublyCL()
{
    First=NULL;
    //Last=NULL;
}
template <class T>
void DoublyCL <T>:: InserFirst(T no)
{
    struct nodeD <T> *newn=new nodeD<T>;

    newn->data=no;
    newn->next=NULL;
    newn->prev=NULL;

    if(First==NULL)
    {
         First=newn;
        //Last=newn;
        
       
    }
    else
    {
        newn->next=First;
        (First)->prev=newn;
        First=newn;
  
 }
    //(First)->prev=Last;
  //(Last)->next=First;
       
}

template <class T>
void DoublyCL <T>::InserLast(T no)
{
    struct nodeD <T> *newn=new nodeD<T>;

    newn->data=no;
    newn->next=NULL;
    newn->prev=NULL;

    if(First==NULL)
    {
         First=newn;
        //Last=newn;
        
    }
    else
    {
        struct nodeD<T> *temp=First;

        while(temp->next != NULL)
        {
            temp=temp->next;
        }
    temp->next=newn;
    newn->prev=temp;
    //Last=newn;

    }
    //(First)->prev=Last;
  //(Last)->next=First;
       
}
template <class T>
void DoublyCL <T>:: Display()
{
    struct nodeD<T> *temp=First;
    if(First==NULL)
    {
        cout<<"Linked list is empty"<<"\n";
        return ;
    }
    cout<<"Elements of linkedList:\n"<<"\n";
    do
    {
        cout<<temp->data<<"<=>";
        temp=temp->next;
    }
    while(temp->next != NULL);

    cout<<"\n";
}
template <class T>
int DoublyCL<T> :: Count()
{
    int iCnt=0;
struct nodeD <T> *temp=First;
if(First==NULL)
    {
        printf("Linked list is empty");
        return 0;
    }
    
    do
    {
        iCnt++;
        temp=temp->next;
    }
    while(temp->next != NULL);

   return iCnt;

}
template <class T>
void DoublyCL <T>:: DeleteFirst()
{
    if(First==NULL)
    {
        return;
    }
    else if(First->next == NULL)
    {
        delete First;

        First=NULL;
        //Last=NULL;

    }
    else{

        First=(First)->next;
         delete (First)->next;    //free(First)->prev;

    (First)->prev=NULL;
   // (Last)->next=First;
    }
   
   
}

template <class T>
void DoublyCL <T>:: DeleteLast()
{
    if(First==NULL)
    {
        return;
    }
    
    else{
        struct nodeD<T> *temp=First;

        while(temp->next->next != NULL)
        {
            temp=temp->next;
        }
    temp->next=NULL;
   // newn->prev=temp;
        
    }


}
template <class T>
void DoublyCL <T>:: InsertAtPosition(T no ,int ipos)
{
    struct nodeD<T> *newn=new nodeD<T>;
    struct nodeD <T> *temp=NULL;
 int iCnt=0;
    int inodeDCount=Count();

    if((ipos <1 )||(ipos>inodeDCount+1))
    {
        cout<<"Invalid position";
        return ;
    }
    if(ipos==1)
    {
        InserFirst(no);
    }
 else if(ipos==inodeDCount+1)
    {
        InserLast(no);
    }
    else{
        newn->data=no;
        newn->prev=NULL;
        newn->next=NULL;

        temp=First;

        for(iCnt=1;iCnt<ipos-1;iCnt++)
        {
            temp=temp->next;
        }
        newn->next=temp->next;
        temp->next->prev=newn;

        temp->next=newn;
        newn->prev=temp;
    }
}

template <class T>  
void DoublyCL <T>:: DeleteAtPosition(int ipos)
{
    //PnodeD newn=(PnodeD)malloc(sizeof(nodeD));
    struct nodeD <T> *temp=NULL;
  int iCnt=0;
    int inodeDCount=Count();

    if((ipos <1 )||(ipos>inodeDCount))
    {
        cout<<"Invalid position\n";
        return ;
    }
    if(ipos==1)
    {
        DeleteFirst();
    }
 else if(ipos==inodeDCount+1)
    {
        DeleteLast();
    }
    else{
        
         temp=First;

        for(iCnt=1;iCnt<ipos-1;iCnt++)
        {
            temp=temp->next;
        }
    temp->next=temp->next->next;
    delete temp->next->prev;
        temp->next->prev=temp;

        
    }
}
template <class T>
class SinglyCL
{
    public:
    struct nodeS<T> * First;
    struct nodeS<T> * Last;
    SinglyCL();

    void InsertFirst(T no);
    void InsertLast(T no);
    void InsertAtPosition(T no,int ipos);

   void DeleteFirst();
   void DeleteLast();
    void DeleteAtPosition(int ipos);

   void Display();
   int Count();
};

template <class T>
SinglyCL <T>:: SinglyCL()
{
 First=NULL;
 Last=NULL;
}

template <class T>
void SinglyCL <T>::InsertFirst( T no)
{
    struct nodeS <T> *newn = new nodeS<T>;
    newn->data = no;
    newn->next = NULL;

    if((First == NULL) && (Last == NULL))    // Empty LL
    {
        First = Last = newn;
        (Last)->next = First;
    }
    else    // LL contains atleast one nodeS
    {
        newn->next = First;
        First = newn;
        (Last)->next = First;
    }
}

template <class T>
void SinglyCL <T>::InsertLast( T no)
{
    struct nodeS<T> *newn = new nodeS<T>;

    newn->data = no;
    newn->next = NULL;

    if((First == NULL) && (Last == NULL))    // Empty LL
    {
        First = Last = newn;
        (Last)->next = First;
    }
    else    // LL contains atleast one nodeS
    {
        (Last) -> next = newn;
        Last = newn;
        (Last)->next = First;
    }
}
template <class T>
void SinglyCL <T>::Display()
{
    struct nodeS<T> *temp=First;
    cout<<"Elements of Linked list are : \n";

    do
    {
        cout<<"|"<<temp->data<<"|->";
        temp = temp -> next;
    }while(temp != Last->next);
    cout<<"\n";
}
template <class T>
int SinglyCL <T>:: Count()
{
    struct nodeS<T> *temp=First;
    int iCnt = 0;
    do
    {
        iCnt++;
       temp=temp-> next;
    }while(temp != Last->next);
    return iCnt;
}
template <class T>
void SinglyCL <T>:: DeleteFirst()
{
    if((First==NULL)&& (Last==NULL))  //empty LL
    {
        return;
    }
    else if(First == Last)        //single nodeS
    {
       // free(First);
       delete First;
        First=NULL;
        Last=NULL;
    }
    else{
        First=(First)->next;
       // free((Last)->next);
       delete (Last)->next;
        (Last)->next=First;
    }
}
template <class T>
void SinglyCL <T>:: DeleteLast()
{
    struct nodeS<T> *temp=First;

    if((First==NULL)&& (Last==NULL))  //empty LL
    {
        return;
    }
    else if(First == Last)        //single nodeS
    {
        free(First);
        First=NULL;
        Last=NULL;
    }
    else{
        while(temp->next !=(Last))
        {
            temp=temp->next;
        }
        //free(temp->next);  //free(Last)
        delete temp->next;
        Last=temp;
        (Last)->next=First;

    }
}
template <class T>
void SinglyCL <T> :: DeleteAtPosition(int ipos)
{
    int inodeSCnt=0,iCnt=0;
    struct nodeS<T> *temp1=First;
    struct nodeS<T> *temp2=Last;

    inodeSCnt=Count();

    if(ipos<1 || ipos>inodeSCnt+1)
    {
        cout<<"Invalid Position\n";
        return ;
    }
    if(ipos ==1)
    {
        DeleteFirst();
    }
    else if(ipos==inodeSCnt)
    {
        DeleteLast();
    }
    else{
        
        for(iCnt=0;iCnt<ipos-1;iCnt++)
        {
            temp1=temp1->next;
        }
    temp2=temp1->next;
    temp1->next=temp2->next;
    //free(temp2);
    delete temp2;
    }
}
template <class T>
void SinglyCL<T> :: InsertAtPosition(T no,int ipos)
{
    int inodeSCnt=0;
    struct nodeS<T> *newn=NULL;
    struct nodeS<T> *temp=First;
    int iCnt=0;
    inodeSCnt=Count();

    if(ipos<1 || ipos>inodeSCnt+1)
    {
        cout<<"Invalid Position\n";
        return ;
    }
    if(ipos ==1)
    {
        InsertFirst(no);
    }
    else if(ipos==inodeSCnt+1)
    {
        InsertLast(no);
    }
    else{
        newn=new nodeS<T>;
        newn->data=no;
        newn->next=NULL;

        for(iCnt=0;iCnt<ipos-1;iCnt++)
        {
            temp=temp->next;
        }

    newn->next=temp->next;
    temp->next=newn;
    }
}

template <class T>

class SinglyLL
{
    public :
    struct nodeS<T> * first;
    SinglyLL();
    void InsertFirst(T no);
    void InsertLast(T no);
    void InsertAtPosition(T no,int iPos);
    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPosition(int iPos);
    void Display();
    int Count();

};
template <class T>
SinglyLL <T>:: SinglyLL()
{
    first=NULL;
}

template <class T>
void SinglyLL <T>::InsertFirst(T no)
{
   struct nodeS <T> * newn=new nodeS<T>;

     newn->data=no;
   newn->next=NULL;
    if(first==NULL)
    {
       first=newn;
       
    }
    else
    {
        
        newn->next=first;
        first=newn;
    }

}
template <class T>
void SinglyLL <T>:: InsertLast(T no)
{
    // Step1 : Allocate memory for nodeS
    struct nodeS <T> *newn = new nodeS<T>;

    // Step2 : Initialise nodeS
    newn->data = no;
    newn->next =  NULL;

    // Step3 : Check if LL is empty or not
    if(first == NULL)   // if(iCount == 0)
    {
        first = newn;
        
    }
    else    // If LL contains atleast one nodeS
    {
        struct nodeS<T> *temp=first;

        while(temp->next != NULL)
        {
            temp=temp->next;
        }
        temp->next = newn;;
        
        
    }
}
template <class T>
void SinglyLL <T>:: InsertAtPosition(T no, int ipos)
{
    int iCount=Count();
    if((ipos < 1) || (ipos > iCount+1))
    {
        cout<<"Invalid position"<<"\n";
        return;
    }

    if(ipos == 1)
    {
        InsertFirst(no);
    }
    else if(ipos == iCount + 1)
    {
        InsertLast(no);
    }
    else
    {
        struct nodeS <T> *newn = new nodeS<T>;

        newn->data = no;
        newn->next = NULL;

        struct nodeS <T>* temp = first;
        for(int iCnt = 1; iCnt < ipos-1; iCnt++)
        {
            temp = temp->next;
        }

        newn->next = temp->next;
        temp->next = newn;

        
    }
}
template <class T>
void SinglyLL<T> :: DeleteFirst()
{
    if(first == NULL)   // if(iCount == 0)
    {
        return;
    }
    else if(first -> next == NULL) // else if(iCount == 1)
    {
        delete first;
        first = NULL;
        
    }
    else
    {
        struct nodeS <T> *temp = first;
        first = first -> next;
        delete temp;
        
    }
}
template <class T>
void SinglyLL<T> :: DeleteLast()
{    
    if(first == NULL)   // if(iCount == 0)
    {
        return;
    }
    else if(first -> next == NULL) // else if(iCount == 1)
    {
        delete first;
        first = NULL;
        
    }
    else
    {
        struct nodeS <T> *temp = first;

        while(temp->next->next != NULL)
        {
            temp = temp->next;
        }

        delete temp->next;
        temp->next = NULL;

        
    }
}
template <class T>
void SinglyLL <T>:: DeleteAtPosition(int ipos)
{
    int iCount=Count();
    if((ipos < 1) || (ipos > iCount))
    {
        cout<<"Invalid position"<<"\n";
        return;
    }

    if(ipos == 1)
    {
        DeleteFirst();
    }
    else if(ipos == iCount)
    {
        DeleteLast();
    }
    else
    {
         struct nodeS <T> *temp = first;
         
        int iCnt=1;
        while(iCnt != ipos-1)
        {
            temp = temp->next;
            iCnt++;
        }
        struct nodeS <T> *temp2=temp->next;
        temp->next =temp2->next;
        delete temp2;
        
        

    }
}

template <class T>
void SinglyLL <T>::Display()
{
    struct nodeS<T> * temp=first;
   while(temp !=NULL)
   {
    cout<<temp->data<<"\t";
    temp=temp->next;
   }
}
template <class T>
int SinglyLL <T>:: Count()
{
    int iCnt=0;
 struct nodeS<T> * temp=first;
   while(temp !=NULL)
   {
    iCnt++;
    temp=temp->next;
   }
 return iCnt;
}


int main()
{
  
    return 0;
}