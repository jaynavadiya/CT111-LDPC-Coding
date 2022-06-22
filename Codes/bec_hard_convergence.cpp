#include<iostream>
#include<cmath>
#include<bits/stdc++.h>
#include<string>
#include<fstream>
#include<time.h>
#include<random>

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

   // wr will be no. of 1s in each row
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
  int c[m]; // just initialize it to 0.
  for(int i=0;i<m;i++)
  {
    c[i] = 0;
  }
  int **h; 
  h = (int**)malloc(n*sizeof(int*));
  for(int i=0;i<n;i++)
  {
    h[i] = new int[m];
  }

  vector<float> forprob;
  int iter = 100;
  for(double q = 0;q<1.01;q += 0.1)
  {
    cout<<"For probability "<<q<<endl;
    double *err = new double[iter];
    for(int i=0;i<iter;i++)
    {
      err[i] = 0;
    }
    int sim = 2500;
    int nsim=sim;
    while(sim--)
    {
      for(int i=0;i<n;i++)
      {
        for(int j=0;j<wr;j++)
        {
          h[i][check[i][j]] = hmain[i][check[i][j]];
        }
      }
      /******/ 
      float ax = q;
      int tx = 1;
      while(1)
      {
        tx *= 10;
        ax *= 10;
        stringstream ss;
        ss<<abs(ax-(int)ax);
        string s;
        ss>>s;
        size_t found = s.find('.');
        if(found==string::npos)
        {
          break;
        }
      }
      //ax *= 1000;
      //tx *= 1000;
      int *r = new int[m];
      for(int i=0;i<m;i++)
      {
        int bx = random(tx);
        if(bx<=ax)
        {
          r[i] = 2;
        }
        else
        {
          r[i] = c[i];
        }
      }
      /*******/
      int me = 0;
      for(int i=0;i<m;i++)
      {
        // cout<<r[i]<<" ";
        if(r[i]==2)
        {
          me++;
        }
      }
      int re[me];
      me = 0;
      for(int i=0;i<m;i++)
      {
        if(r[i]==2)
        {
          re[me++] = i;
        }
      }
      //monte carlo-->10k, 0 to 1.. gap 0.1
      //hard decoding starting
      int t = iter;
      while(t--)
      {
        for(int i=0;i<m;i++)
        {
          if(r[i]==2)
          {
            err[iter-t-1]++;
            
          }
          for(int j=0;j<wc;j++)
          {
            h[var[j][i]][i]=r[i];
          }
        }
        // checking if the code is successfully decoded or not.
        bool b = true;
        for(int i=0;i<m;i++)
        {
          if(r[i]==2)
          {
            b = false;
            break;
          }
        }
        if(b)
        {
          //cout<<"Code is successfully decoded.\n";
          break;
        }
        int sum = 0;
        for(int i=0;i<me;i++)
        {
          int ind = re[i];
          b = false;
          if(r[ind]!=2)
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
              if(h[rind][check[rind][k]]==2)
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
              r[ind] = sum%2;
              b = true;
              break; 
            }
          }
          // if(b)
          // {
          //   break;
          // }
        }
      }
    }
    for(int i=0;i<iter;i++)
    {
      err[i] = err[i]/nsim;
      cout<<(int)err[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
  }
}