// that starting part, till which the simulations starts is similar for all the decoders.
// that part is explained in bec_hard_monte_carlo.cpp with comments.
//the algorithms are mentioned in the ppt.
#include<bits/stdc++.h>
#include <random>
using namespace std;

int random(int limit)
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1,limit);
  return dis(gen);
}

int main()
{
  string filename;
  cout<<"Enter the H matrix file name: ";
  cin>>filename;
  
  int m,n;
  // n is no. of columns
  cout<<"Input rows :";
  cin>>n;
  // m is no of rows
  cout<<"Input colums :";
  cin>>m;

  vector< vector<int> > hmain(n, vector<int>(m, -1));
  ifstream file;

  file.open(filename);
  // H matrix will be now stored in h 2d array.
  for (int i = 0; i < n; i++)
  {
    int j = 0;
    char tempchar;
    for (j = 0; j < m - 1; j++)
    {
      file >> tempchar;
      hmain[i][j] = (tempchar - '0');
      file>> tempchar;
    }
    file >> tempchar;
    hmain[i][j] = (tempchar - '0');
  }
  file.close();

  int wr = 0;
  for(int i=0;i<m;i++)
  {
    wr += hmain[0][i];
  }
  
  int wc=0; //wc will be no. of 1s in each column
  for(int i=0;i<n;i++)
  {
    wc += hmain[i][0];
  }

  // check node 2d array
  int check[n][wr];
  int k=0;
  for(int i=0;i<n;i++)
  {
    k=0;
    for(int j=0;j<m;j++)
    {
      if(hmain[i][j]==1)
      {
        check[i][k++]=j;
      }
    }
  }
 
  // variable node 2d array
  int var[wc][m];
  k=0;
  for(int i=0;i<m;i++)
  {
    k = 0;
    for(int j=0;j<n;j++)
    {
      if(hmain[j][i]==1)
      {
        var[k++][i]=j;
      }
    }
  }

  int *c = new int[m];
  for(int i=0;i<m;i++)
  {
    c[i] = 0;
  }
  for(double q = 0;q<1.01;q += 0.01)
  {
    double summation = 0;
    int sim  = 5000;
    int nsim = sim;
    while(sim--)
    {
      int *r = new int[m];
      double ax = q*1000;
      int tx = 1000;
      for(int i=0;i<m;i++)
      {
        int bx = random(tx);
        if(bx<=ax)
        {
          r[i] = (c[i]+1)%2;
        }
        else
        {
          r[i] = c[i];
        }
      }
      int iter = 100;
      while(iter--)
      {
        int *tempr = new int[m];
        for(int i=0;i<m;i++)
        {
          tempr[i] = r[i];
        }
        for(int i=0;i<m;i++)
        {
          int kk =0;
          int *sum = new int[wc+1];
          for(int j=0;j<wc+1;j++)
          {
            sum[j] = 0;
          }
          sum[kk++] = r[i];
          for(int j=0;j<wc;j++)
          {
            int ind = var[j][i];
            for(int k=0;k<wr;k++)
            {
              if(i!=check[ind][k])
              {
                sum[kk] += r[check[ind][k]];
              } 
            }
            kk++;
          }
          int count0 = 0;
          int count1 = 0;
          for(int j=0;j<wc+1;j++)
          {
            sum[j] = sum[j]%2;
            if(sum[j]==0)
            {
              count0++;
            }
            else
            {
              count1++;
            }
          }
          if(count0>count1)
          {
            tempr[i] = 0;
          }
          if(count1>count0)
          {
            tempr[i] = 1;
          }
          for(int i=0;i<m;i++)
          {
            r[i] = tempr[i];
          }
        }
        
        bool b = true;
        
        for(int i=0;i<m;i++)
        {
          if(r[i]==1)
          {
            b=false;
            break;
          }
        }
        if(b)
        {
          summation++;
          break;
        }
      }
    }
    cout<<q<<" "<<(double)summation/nsim<<endl;
  }
}