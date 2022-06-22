// that starting part, till which the simulations starts is similar for all the decoders.
// that part is explained in bec_hard_monte_carlo.cpp with comments.
//the algorithms are mentioned in the ppt.
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
  // hmain is the h matrix. it is basically a 2d vector. Hmain is the given name because later on we will be copying this to a temp h matrix.
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
      // -'0' is used to convert the character into integer/dobule.
      file >> tempchar;
      hmain[i][j] = (tempchar - '0');
      file>> tempchar;
    }
    file >> tempchar;
    hmain[i][j] = (tempchar - '0');
  }
  file.close();

   // wr will be no. of 1s in each row, basically degree of check node is wr.
  int wr = 0;
  for(int i=0;i<m;i++)
  {
    wr += hmain[0][i];
  }
  //degree of check node=wr.
  
  int wc=0; //wc will be no. of 1s in each column
  for(int i=0;i<n;i++)
  {
    wc += hmain[i][0];
  }
  //degree of variable node=wc.
  // check node 2d array
  int check[n][wr];// this will store index of variable nodes connected to every check node.
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
  int var[wc][m]; // this will store index of check nodes connected to every variable nodes.
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
  int c[m]; // initializing c to all 0 values.
  for(int i=0;i<m;i++)
  {
    c[i] = 0;
  }
  // temp h matrix is named h.
  int **h; 
  h = (int**)malloc(n*sizeof(int*));
  for(int i=0;i<n;i++)
  {
    h[i] = new int[m];
  }
  // h is also a 2d array, but it's made dynamically.

  // this vector is used to store 100 values (success ratio for each probabilities)
  // but later on we just printed the success ratio for each probabilites of error, that's 100 values and plotted the graph from there itself.
  vector<float> forprob;
  for(float q = 0;q<1.01;q += 0.01) // 1 goes from 0 to 1 in steps of 0.01.
  {
    int sim = 5000; //no of simulations is 5k.
    int nsim=sim;
    float summation = 0;
    while(sim--)
    {
      // everytime we will replace the values that were changed in the temp h matrix by original hmain matrix.
      // it takes less time to change only the values that were changed compared to copying the whole hmain.
      for(int i=0;i<n;i++)
      {
        for(int j=0;j<wr;j++)
        {
          h[i][check[i][j]] = hmain[i][check[i][j]];
        }
      }
      /**********************************************/ 
      // this is our channel for bec hard decision decoding.
      float ax = q;
      int tx = 1;
      // while(1)
      // {
      //   tx *= 10;
      //   ax *= 10;
      //   stringstream ss;
      //   ss<<abs(ax-(int)ax);
      //   string s;
      //   ss>>s;
      //   size_t found = s.find('.');
      //   if(found==string::npos)
      //   {
      //     break;
      //   }
      // }
      int *r = new int[m];
      for(int i=0;i<m;i++)
      {
        int bx = random(tx);
        //our eraser is shown by the value 2.
        if(bx<=ax)
        {
          r[i] = 2;
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
      
      //hard decoding starting
      int t = 100; // t are the iterations, that is equal to 300.
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
          if(r[i]==2)
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
    cout<<q<<" "<<(float)summation/nsim <<endl;
    forprob.push_back(summation/nsim);
  }
}