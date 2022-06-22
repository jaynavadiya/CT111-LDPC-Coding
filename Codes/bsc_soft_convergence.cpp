#include <bits/stdc++.h>
#include <random>

using namespace std;

int random(int limit)
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(1, limit);
  return dis(gen);
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  string filename;
  cout << "Enter the H matrix file name: ";
  cin >> filename;

  int m, n;
  cout << "Input rows :";
  cin >> n;
  cout << "Input columns :";
  cin >> m;

  vector< vector<double> > hmain(n, vector<double>(m, -1));
  ifstream file;

  file.open(filename);

  for (int i = 0; i < n; i++)
  {
    int j = 0;
    char tempchar;
    for (j = 0; j < m - 1; j++)
    {
      file >> tempchar;
      hmain[i][j] = (tempchar - '0');
      file >> tempchar;
    }
    file >> tempchar;
    hmain[i][j] = (tempchar - '0');
  }
  file.close();

  // wr will be no. of 1s in each row
  double wr = 0;
  for (int i = 0; i < m; i++)
  {
    wr += hmain[0][i];
  }

  double wc = 0; //wc will be no. of 1s in each column
  for (int i = 0; i < n; i++)
  {
    wc += hmain[i][0];
  }

  // check node 2d array
  int check[n][(int)wr];
  int k = 0;
  for (int i = 0; i < n; i++)
  {
    k = 0;
    for (int j = 0; j < m; j++)
    {
      if (hmain[i][j] == 1)
      {
        check[i][k++] = j;
      }
    }
  }

  // variable node 2d array
  int var[(int)wc][m];
  k = 0;
  for (int i = 0; i < m; i++)
  {
    k = 0;
    for (int j = 0; j < n; j++)
    {
      if (hmain[j][i] == 1)
      {
        var[k++][i] = j;
      }
    }
  }
  int c[m]; // just initialize it to 0.
  for (int i = 0; i < m; i++)
  {
    c[i] = 0;
  }
  double **h;
  h = (double **)malloc(n * sizeof(double *));
  for (int i = 0; i < n; i++)
  {
    h[i] = new double[m];
  }
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      h[i][j] = hmain[i][j];
    }
  }
  int iter = 100;
  vector<double> forprob;
  for (double q = 0; q < 1.01; q += 0.1)
  {
    cout<<"For probability :"<<q<<endl;
    int sim = 100;
    int nsim=sim;
    double *err = new double[iter];
    for(int i=0;i<iter;i++)
    {
      err[i] = 0;
    }
    while (sim--)
    {
      for (int i = 0; i < n; i++)
      {
        for (int j = 0; j < wr; j++)
        {
          h[i][check[i][j]] = hmain[i][check[i][j]];
        }
      }
      /**/
      /**/
      double ax = q * 100;
      int tx = 100;
      /*while(1)
      {
        tx *= 10;
        ax *= 10;
        stringstream ss;
        ss<<ax;
        string s;
        ss>>s;
        size_t found = s.find('.');
        if(found==string::npos)
        {
          break;
        }
      }
      if(tx<100)
      {
        ax *= 10;
        tx *= 10;
      }*/
      int *r = new int[m];
      for (int i = 0; i < m; i++)
      {
        int bx = random(tx);
        if (bx <= ax)
        {
          r[i] = (c[i] + 1) % 2;
        }
        else
        {
          r[i] = c[i];
        }
      }
      /**/
      double s[m];
      double p[m];
      double q1;
      if(q>0.5)
      {
        for(int i=0; i<m; i++)
        {
          r[i]=(r[i]+1)%2;
        }
        q1=1-q;
      }else{
        q1=q;
      }
      for (int i = 0; i < m; i++)
      {
        if (r[i] == 0)
        {
          s[i] = q1;
          p[i] = q1;
        }
        else
        {
          s[i] = 1 - q1;
          p[i] = 1 - q1;
        }
      }
      for (int i = 0; i < m; i++)
      {
        for (int j = 0; j < wc; j++)
        {
          h[var[j][i]][i] = s[i];
        }
      }
      int t = iter;
      while (t--)
      {
        for(int i=0;i<m;i++)
        {
          if(q>0.5 && r[i]==0)
          {
            err[iter-t-1]++;
          }
          if(q<=0.5 && r[i]==1)
          {
            err[iter-t-1]++;
          }
        }
        bool b = true;
        for (int i = 0; i < n; i++)
        {
          int sum = 0;
          for (int j = 0; j < wr; j++)
          {
            sum += r[check[i][j]];
          }
          if (sum % 2 != 0)
          {
            b = false;
            break;
          }
        }
        if (b)
        {
          break;
        }
        for (int i = 0; i < n; i++)
        {
         
          double z = 1;
          for (int j = 0; j < wr; j++)
          {
            z *= (1 - 2 * h[i][check[i][j]]);
          }
          for (int j = 0; j < wr; j++)
          {
            h[i][check[i][j]] = (double)1 - (0.5 + 0.5 * ((double)z / ((double)1 - 2 * h[i][check[i][j]])));
          }
        }
        for (int i = 0; i < m; i++)
        {
          double *temp = new double[(int)wc];
          for (int j = 0; j < wc; j++)
          {
            double x1 = 1;
            double x0 = 1;
            for (int k = 0; k < wc; k++)
            {
              if (k == j)
              {
                continue;
              }
              x1 *= h[var[k][i]][i];
              x0 *= 1 - h[var[k][i]][i];
            }
            x0 *= 1 - p[i];
            x1 *= p[i];
            temp[j] = x1 / (x1 + x0);
          }
          for (int j = 0; j < wc; j++)
          {
            h[var[j][i]][i] = temp[j];
          }
        }
        for (int i = 0; i < m; i++)
        {
          double dec0 = 1;
          double dec1 = 1;
          for (int j = 0; j < wc; j++)
          {
            dec1 *= h[var[j][i]][i];
            dec0 *= 1 - h[var[j][i]][i];
          }
          dec1 *= p[i];
          dec0 *= 1 - p[i];
          if (dec1 > dec0)
          {
            r[i] = 1;
            p[i] = 1 - q1;
          }
          else if (dec0 > dec1)
          {
            r[i] = 0;
            p[i] = q1;
          }
          else
          {
            r[i] = r[i];
            p[i] = p[i];
          }
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