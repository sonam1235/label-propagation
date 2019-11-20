#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include<time.h>
struct node
{
  int node_num;
  int count;
};

// return the number of neighbour of ith node
float find_degree(int i,int n,int adj[n][n])
{
  float count=0.0;
  for(int j=0;j<n;j++)
  {
    if(adj[i][j]==1)
     count++;
  }
  return count;
}

// calculate B matrix.
void calculate_b(int n,int adj[n][n],float B[n][n],int m,int n1,int n2)
{
   for(int i=0;i<n1;i++)
   {
     float d1=find_degree(i,n,adj);
     //printf("degree of %d node is %d",n1,n2);
     for(int j=n1;j<n;j++)
      {
          float d2=find_degree(j,n,adj);
          float d3= (d1 *d2)/(2*m);
          //printf("degree of %d node is %f",i,d3);
          B[i][j]=adj[i][j]-d3;
          B[j][i]=B[i][j];
  
      }
   }
}
 

// this function sort the array of structure on the basis of count.
void sort(struct node freq[],int n)
{
    struct node temp;
    
    for (int i = 0; i <n; i++)
    {
        for (int j = i+1; j <n; j++)
        {
            if ((freq[i].count) < (freq[j].count))
            {
                temp = (freq[j]);
                (freq[j]) = (freq[i]);
                (freq[i]) = temp;
            } 
        }
    }
}
void LP(int i,int n,int adj[n][n],int label[n])
{
    int count=find_degree(i,n,adj);
    struct node freq[count];
    int k=0;
   for(int j=0;j<n;j++)
    {
       if(adj[i][j]==1)
         {
            freq[k].node_num=j;
            freq[k].count=0;
            k++;
         }
      }
   
    for(int j=0;j<count;j++)
    {
      for(int k=j+1;k<count;k++)
       {
          if(label[freq[j].node_num]==label[freq[k].node_num])
            {
                freq[j].count++;
            }
       }
    }
    
     sort(freq,count);
      int j=0;
      while(freq[j].count==freq[j+1].count)
         { 
              j++;
         }
    
    time_t t;
    srand((unsigned) time(&t));

    if(j!=0)
    {
     int myRand=(rand()%j);
     label[i]=label[freq[myRand].node_num];
     printf("here the label of node i %d is %d\n",i,label[freq[myRand].node_num]);
   }  
 else
  {
    label[i]=label[freq[j].node_num];
    printf("here the label of node i %d is %d\n",i,label[freq[j].node_num]);
  }

}
// this function check whether a node have label of neighbours max label or not.
int check_max_label(int i,int n,int adj[n][n],int label[n])
{
    int count=find_degree(i,n,adj);
   struct node freq[count];
    int k=0;
   for(int j=0;j<n;j++)
    {
       if(adj[i][j]==1)
         {
            freq[k].node_num=j;
            freq[k].count=0;
            k++;
         }
      }
    for(int j=0;j<count;j++)
    {
      for(int k=j+1;k<count;k++)
       {
          if(label[freq[j].node_num]==label[freq[k].node_num])
            {
                
                freq[j].count++;
            }
       }
    }
    
     sort(freq,count);
       int j=0;
      while(freq[j].count==freq[j+1].count) 
       j++;
       int max_label_count=freq[0].count;
     for(int k=0;k<=j;k++)
       {
          if(label[i]==label[freq[k].node_num]&& freq[k].count==max_label_count)
            return 1;
       }
     return 0;

}
// calculating delta function............................................................
int delta(int n,int label[],int i,int j)
{
   if(label[i]==label[j])   
      return 1;
   else
   return 0;
}
//calculate modularity ..................................................................................................................................................................
  void modularity(int n,float B[n][n],int label[],int m,int n1,int n2)
  {
     float Q=0.0;
     float sum=0.0;
      for(int i=0;i<n1;i++)
       {
         for(int j=n1;j<n;j++)
          {
             int d=delta(n,label,i,j);
             float p=B[i][j]*d;
             sum+=p;
          }
       }
      Q=sum/m;
      printf("MODULARITY IS %f \n",Q);
      
      
  }
//driver program.........................................................................................................................................................
int main(int argc,char const* argv[])
{
 FILE *f;
 f=fopen(argv[1],"r");
 int m,n1,n2;
 fscanf(f,"%d",&m);
 fscanf(f,"%d",&n1);
 fscanf(f,"%d",&n2);
 int n=n1+n2;
 int adj[n][n];float B[n][n];
 for(int i=0;i<n;i++)
 {
   for(int j=0;j<n;j++) 
    {
       adj[i][j]=0;
       B[i][j]=0;
    }
 }
 
 int origin,destin;
 for(int i=0; i<m; i++)
	{
		fscanf(f,"%d",&origin);
                fscanf(f,"%d",&destin);
                origin--;
                destin=destin+n1-1;
         	adj[origin][destin] = 1;
                adj[destin][origin] = 1;
	}
 

   for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
       {
         printf("%d\t",adj[i][j]);
       }
      printf("\n");
    }
    calculate_b(n,adj,B,m,n1,n2);
    printf("B matrix is \n");
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
       {
         printf("%f\t",B[i][j]);
       }
      printf("\n");
    }
   int label[n];
   for(int i=0;i<n;i++)
   {
      label[i]=i;
   }
    
   while(1)
   { 
    for(int i=0;i<n;i++)
    LP(i,n,adj,label);
    printf("\n");
   modularity(n,B,label,m,n1,n2);
    int check=0;
    for(int i=0;i<n;i++)
    {
       
      int p=check_max_label(i,n,adj,label);
        if(p==1)
          continue;
      else
        {
           check=1;
           break;
        }
    }
   if(check==0)
     {
       break; 
     } 

   }
  //modularity(n,B,label,m,n1,n2);
 return 0;
}
