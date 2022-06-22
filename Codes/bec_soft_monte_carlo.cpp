// that starting part, till which the simulations starts is similar for all the decoders.
//in soft decision decoding the h matrix will be of data type double.
// that part is explained in bec_hard_monte_carlo.cpp with comments.
//the algorithms are mentioned in the ppt.
#include<iostream>
#include<cmath>
#include<bits/stdc++.h>
#include<string>
#include<fstream>
#include<time.h>
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

  vector< vector<double> > hmain(n, vector<double>(m, -1));
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

   // wr will be no. of 1s in each row
   double wr = 0;
  for(int i=0;i<m;i++)
  {
    wr += hmain[0][i];
  }
  
  double wc=0; //wc will be no. of 1s in each column
  for(int i=0;i<n;i++)
  {
    wc += hmain[i][0];
  }

  // check node 2d array
  int check[n][(int)wr];
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
  cout<<"Check completed.\n";
  // variable node 2d array
  int var[(int)wc][m];
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
  cout<<"Var completed.\n";
  double c[m]; // just initialize it to 0.
  for(int i=0;i<m;i++)
  {
    c[i] = 0;
  }
  double **h; 
  h = (double**)malloc(n*sizeof(double*));
  for(int i=0;i<n;i++)
  {
    h[i] = new double[m];
  }
  for(int i=0;i<n;i++)
  {
    for(int j=0;j<m;j++)
    {
      h[i][j] = hmain[i][j];
    }
  }
  cout<<"Hello\n";
  vector<float> forprob;
  for(double q = 0;q<1.01;q += 0.01)
  {
    int sim = 5000;
    int nsim=sim;
    float summation = 0;
    while(sim--)
    {
      for(int i=0;i<n;i++)
      {
        for(int j=0;j<wr;j++)
        {
          h[i][check[i][j]] = hmain[i][check[i][j]];
        }
      }
      /**********************************************/ 
      double ax = q*100;
      int tx = 100;
      
      double *r = new double[m];
      for(int i=0;i<m;i++)
      {
        int bx = random(tx);
        if(bx<=ax)
        {
          r[i] = 0.5;
        }
        else
        {
          r[i] = c[i];
        }
      }
      /***********************************************/
      int me = 0;
      for(int i=0;i<m;i++)
      {
        // cout<<r[i]<<" ";
        if(r[i]==0.5)
        {
          me++;
        }
      }
      int re[me];
      me = 0;
      for(int i=0;i<m;i++)
      {
        if(r[i]==0.5)
        {
          re[me++] = i;
        }
      }
      //monte carlo-->10k, 0 to 1.. gap 0.1
      //hard decoding starting
      int t = 100;
      while(t--)
      { 
        for(int i=0;i<m;i++)
        {
          for(int j=0;j<wc;j++)
          {
            h[var[j][i]][i]=r[i];
          }
        }
        // checking if the code is successfully decoded or not.
        bool b = true;
        for(int i=0;i<m;i++)
        {
          if(r[i]==0.5)
          {
            b = false;
            break;
          }
        }
        if(b)
        {
          //cout<<"Code is successfully decoded.\n";
          summation++;
          break;
        }
        double sum = 0;
        for(int i=0;i<me;i++)
        {
          int ind = re[i];
          if(r[ind]!=0.5)
          {
            continue;
          }
          for(int j=0;j<wc;j++)
          {
            sum = 0;
            int rind = var[j][ind];
            int count = 0;
            for(int k=0;k<wr;k++)
            {
              if(h[rind][check[rind][k]]==0.5)
              {
                count++;
              }
              sum += h[rind][check[rind][k]];
            }
            if(count>1)
            {
              continue;
            }
            else
            {
              r[ind] = (int)sum%2;
              //b = true;
              break; 
            }
          }
          //if(b)
          //{
            //break;
          //}
        }
      }
    }
    cout<<q<<" "<<(float)summation/nsim<<endl;
    forprob.push_back(summation/nsim);
  }
}