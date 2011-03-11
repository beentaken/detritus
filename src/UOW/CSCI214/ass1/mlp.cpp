/************************************************************
*      mlp.cpp - Implements a 2 layer back-propagation MLP
*      CSCI214 Ass1 Spring 2003
*      Ver1: Koren Ward - 15 March 2003
*      Ver2: Koren Ward - 21 July  2003 - Dynamic memory added
*
*************************************************************/
#include<iostream.h>
#include<fstream.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

const int MAXN = 30;     // Max neurons in any layer
const int MAXPATS = 250; // Max training patterns
const int MAXITS = 50;   // Max training iterations

void TrainNet(float **x,float **d,int NumIPs,int NumOPs,int NumPats);
float **Aloc2DAry(int m,int n);
void Free2DAry(float **Ary2D,int n);

int main(){
  ifstream fin;
  int i,j,NumIPs,NumOPs,NumPats;
  char Line[500],FName[20]="data1.txt";
//  cout<<"Enter data filename: ";
//  cin>>FName; cin.ignore();
  fin.open(FName);
  if(!fin.good()){cout<<"File not found!\n";exit(1);}
  do{fin.getline(Line,500);}while(Line[0]==';'); //eat comments
  sscanf(Line,"%d%d%d",&NumIPs,&NumOPs,&NumPats); //read data specs
  if(NumIPs<1||NumIPs>MAXN||NumOPs<1||NumOPs>MAXN||NumPats<1||NumPats>MAXPATS){
    cout<<"NumIPs, NumOPs or NumPats invalid in data file!\n";
    exit(1);
  }
  float **IPData= Aloc2DAry(NumPats,NumIPs);
  float **OPData= Aloc2DAry(NumPats,NumOPs);
  for(i=0;i<NumPats;i++){
    for(j=0;j<NumIPs;j++)
      fin>>IPData[i][j];
    for(j=0;j<NumOPs;j++)
      fin>>OPData[i][j];
  }
  fin.close();
  TrainNet(IPData,OPData,NumIPs,NumOPs,NumPats);
  Free2DAry(IPData,NumIPs);
  Free2DAry(OPData,NumOPs);
  cout<<"End of program.\n";
  return 0;
}

void TrainNet(float **x,float **d,int NumIPs,int NumOPs,int NumPats ){
// Trains 2 layer back propagation neural network
// x[][]=>input data, d[][]=>desired output data

  int NumHN1=(NumIPs+NumOPs)/2.0+0.5; // Number of neurons in hidden layer 1
  float LrnRate=1.2;    // Learning rate
  float Mtm1=1.6;       // Momentum(t-1)
  float Mtm2=0.8;       // Momentum(t-2)
  float ObjErr = 0.05;  // Objective error
  float *h1 = new float[NumHN1]; // O/Ps of hidden layer
  float *y  = new float[NumOPs]; // O/P of Net
  float *ad1= new float[NumHN1]; // HN1 back prop errors
  float *ad2= new float[NumOPs]; // O/P back prop errors
  float PatErr,MinErr,AveErr,MaxErr;  // Pattern errors
  int p,i,j;     // for loops indexes
  int ItCnt=0;  // Iteration counter

  cout<<"NetArch: IP:"<<NumIPs<<" H1:"<<NumHN1<<" OP:"<<NumOPs<<endl;

  // Allocate memory for weights
  float **w1   = Aloc2DAry(NumIPs,NumHN1);// 1st layer wts
  float **w11  = Aloc2DAry(NumIPs,NumHN1);
  float **w111 = Aloc2DAry(NumIPs,NumHN1);
  float **w2   = Aloc2DAry(NumHN1,NumOPs);// 2nd layer wts
  float **w22  = Aloc2DAry(NumHN1,NumOPs);
  float **w222 = Aloc2DAry(NumHN1,NumOPs);

  // Init wts between -0.5 and +0.5
  srand(time(0));
  for(i=0;i<NumIPs;i++)
    for(j=0;j<NumHN1;j++)
      w1[i][j]=w11[i][j]=w111[i][j]= float(rand())/RAND_MAX - 0.5;
  for(i=0;i<NumHN1;i++)
    for(j=0;j<NumOPs;j++)
      w2[i][j]=w22[i][j]=w222[i][j]= float(rand())/RAND_MAX - 0.5;

  for(;;){// Main learning loop
    MinErr=3.4e38; AveErr=0; MaxErr=-3.4e38;
    for(p=0;p<NumPats;p++){ // for each pattern...
      // Cal neural network output
      for(i=0;i<NumHN1;i++){ // Cal O/P of hidden layer 1
        float in=0;
        for(j=0;j<NumIPs;j++){
          in+=w1[j][i]*x[p][j];
        }
        h1[i]=(float)(1.0/(1.0+exp(-in)));// Sigmoid fn
      }
      for(i=0;i<NumOPs;i++){ // Cal O/P of output layer
        float in=0;
        for(j=0;j<NumHN1;j++){
          in+=w2[j][i]*h1[j];
        }
        y[i]=(float)(1.0/(1.0+exp(-in)));// Sigmoid fn
      }
      // Cal error for this pattern
      PatErr=0.0;
      for(i=0;i<NumOPs;i++){
        float err=y[i]-d[p][i]; // actual-desired O/P
        if(err>0)PatErr+=err; else PatErr-=err;
      }
      PatErr/=NumOPs;
      if(PatErr<MinErr)MinErr=PatErr;
      if(PatErr>MaxErr)MaxErr=PatErr;
      AveErr+=PatErr;

      // Learn pattern with back propagation
      for(i=0;i<NumOPs;i++){ // Modify layer 2 wts
        ad2[i]=(d[p][i]-y[i])*y[i]*(1.0-y[i]);
        for(j=0;j<NumHN1;j++){
          w2[j][i]+=LrnRate*h1[j]*ad2[i]+
          Mtm1*(w2[j][i]-w22[j][i])+
          Mtm2*(w22[j][i]-w222[j][i]);
          w222[j][i]=w22[j][i];
          w22[j][i]=w2[j][i];
        }
      }
      for(i=0;i<NumHN1;i++){ // Modify layer 1 wts
        float err=0.0;
        for(j=0;j<NumOPs;j++){
          err+=ad2[j]*w2[i][j];
        }
        ad1[i]=err*h1[i]*(1.0-h1[i]);
        for(j=0;j<NumIPs;j++){
          w1[j][i]+=LrnRate*x[p][j]*ad1[i]+
          Mtm1*(w1[j][i]-w11[j][i])+
          Mtm2*(w11[j][i]-w111[j][i]);
          w111[j][i]=w11[j][i];
          w11[j][i]=w1[j][i];
        }
      }
    }// end for each pattern
    ItCnt++;
    AveErr/=NumPats;
    cout<<ItCnt<<":\t"<<MinErr<<'\t'<<AveErr<<'\t'<<MaxErr<<endl;
    if((AveErr<=ObjErr)||(ItCnt==MAXITS)) break;
  }// end main learning loop

  // Free memory
  Free2DAry(w1,NumHN1); Free2DAry(w11,NumHN1); Free2DAry(w111,NumHN1);
  Free2DAry(w2,NumOPs); Free2DAry(w22,NumOPs); Free2DAry(w222,NumOPs);
  delete[]h1; delete[]y; delete[]ad1; delete[]ad2;

  cout<<"Press return to continue.";
  char s[20]; cin.getline(s,20);
}


float **Aloc2DAry(int m,int n){
//Allocates memory for 2D array
  float **Ary2D= new float*[m];
  if(Ary2D==NULL){cout<<"No memory!\n";exit(1);}
  for(int i=0;i<m;i++){
    Ary2D[i]= new float[n];
    if(Ary2D[i]==NULL){cout<<"No memory!\n";exit(1);}
  }
  return Ary2D;
}

void Free2DAry(float **Ary2D,int n){
//Frees memory in 2D array
  for(int i=0;i<n;i++)
    delete [] Ary2D[i];
  delete [] Ary2D;
}

