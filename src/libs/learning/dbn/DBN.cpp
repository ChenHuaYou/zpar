// Created by : Harsh Vardhan Tiwari on 18/05/2013
// Harsh Tiwari and Yue Zhang

#include <iostream>
#include <math.h>
#include "DBN.h"
#include "latest_bitarray.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

inline double uniform(double min,double max)
{
    return rand()/(RAND_MAX + 1.0) * (max - min) + min;
}

inline double sigm(double x)
{
    return 1.0/(1.0+exp(-x));
}

inline double binomial(int n,double p){
  double c=0;
  if(p < 0 || p > 1) return 0;
  double r;

  for(int i=0;i<n;i++){
     r=rand() / (RAND_MAX + 1.0);
     if(r < p) c++;
     }
  return c;

}
inline std::istream& operator>>(std::istream& is,RBM& rbm)
{
  int i,j,vsize,hsize;
  std::string line;

  getline(is,line);
  std::istringstream iss(line);
  iss>>vsize>>hsize;

  rbm.initialize(vsize,hsize);


 for(i=0;i<hsize;i++){
     getline(is,line);
     std::istringstream wss(line);

    for(j=0;j<vsize;j++){
          wss>>rbm.W[i][j];
    }
  }

  getline(is,line);
  std::istringstream ws(line);
  for(i=0;i<vsize;i++)
     ws>>rbm.v_bias[i];

  getline(is,line);
  std::istringstream vs(line);
  for(i=0;i<hsize;i++)
      vs>>rbm.h_bias[i];

  return is;

}

inline std::ostream& operator<<(std::ostream& os,RBM& rbm)
{
    int i,j;
    os<<rbm.v_size<<' '<<rbm.h_size<<std::endl;
    for(i=0;i<rbm.h_size;i++)
        {for(j=0;j<rbm.v_size;j++)
        os<<rbm.W[i][j]<<' ';
        os<<std::endl;
    }
    for(i=0;i<rbm.v_size;i++)
        os<<rbm.v_bias[i]<<' ';

    os<<std::endl;

    for(i=0;i<rbm.h_size;i++)
        os<<rbm.h_bias[i]<<' ';

    return os;
}


RBM::RBM(int vsize,int hsize):v_size(vsize), h_size(hsize),W(0),P(0),Q(0),v_samples(0),h_samples(0),h_bias(0),v_bias(0)
{
    if(v_size!=0 && h_size!=0)
    initialize(vsize,hsize);

}

RBM::~RBM()
{
   if(h_size!=0 && v_size!=0)
    finalize();
}

int RBM::gethsize() const {return h_size;}

int RBM::getvsize() const {return v_size;}

void RBM::initialize(int v_size,int h_size)
{
    if(this->v_size!=0 && this->h_size!=0)
    finalize();


    this->v_size=v_size;
    this->h_size=h_size;

    v_samples=new double *[v_size];
    for(int i=0; i<v_size; i++)
        v_samples[i]=new double[2];

    v_bias=new double [v_size];


    h_samples=new double *[h_size];
    for(int i=0; i<h_size; i++)
        h_samples[i]=new double[2];

    h_bias=new double [h_size];


    W=new double *[h_size];
    for(int i=0; i<h_size; i++)
        W[i]=new double[v_size];

    Q=new double *[h_size];
    for(int i=0; i<h_size; i++)
        Q[i]=new double[2];

    P=new double[v_size];


}
void RBM::manual_initialize()
{
    int i,j=0;
    std::cout<<("Enter values for V0 from the training distribution for the RBM\n");
    for(i=0;i<v_size;i++)
        std::cin>>v_samples[i][j];

    std::cout<<("Enter your weight matrix\n");

     for(i=0;i<h_size;i++)
        for(j=0;j<v_size;j++)
            std::cin>>W[i][j];


    std::cout<<("Enter the value for the bias units in visible layer in order\n");

     for(i=0;i<v_size;i++)
            std::cin>>v_bias[i];

    std::cout<<"\n";

    std::cout<<("Enter the value for the bias units in hidden layer in order\n");

     for(i=0;i<h_size;i++)
            std::cin>>h_bias[i];

     std::cout<<"\n";

}

void RBM::random_initialize()

{
    int i,j=0;
    int vsize=getvsize();
    double a = 1.0/vsize;

    for(i=0;i<h_size;i++)
        for(j=0;j<v_size;j++)
        {
            W[i][j]=uniform(-a,a);

        }


    for(i=0;i<v_size;i++)
        {
            v_bias[i]=0;
        }


    for(i=0;i<h_size;i++)
        {
            h_bias[i]=0;

        }

}
void RBM::update(const CBitArray &v,int K,int N)
{
    for(int i=0;i<v_size;++i)
        v_samples[i][0] = v.isset(i) ? 1.0 : 0.0; //std::cout<<v_samples[i][0]<<' ';}
        //std::cout<<std::endl;
    update(K,N);


}
void RBM::train(const std::string &path)
{   int K=1,iterations=1000;
    std::vector<CBitArray>training_data;
    std::ifstream is(path.c_str());
    CBitArray v;
    std::string line;
    while(getline(is,line)){//std::cout<<line<<std::endl;
    std::istringstream iss(line);
    iss>>v;
    //std::cout<<v<<std::endl;
    training_data.push_back(v);
    }
    is.close();
    int N=training_data.size();
    for(int i=0;i<iterations;i++)
        {for(int j=0;j<N;j++){
        //std::cout<<training_data[j];
        update(training_data[j],K,N);}
        }

}

void RBM::print_parameters()
{   int i,j;

    std::cout<<"The value of W is\n";
    for(i=0;i<h_size;i++)
        {for(j=0;j<v_size;j++)
            {std::cout<<W[i][j]; std::cout<<' ';}
         std::cout<<'\n';
        }


    std::cout<<"\nThe value of b(hidden bias) is\n";
    for(i=0;i<h_size;i++)
        std::cout<<h_bias[i]<<' ';


    std::cout<<"\nThe value of c(visible bias) is\n";
    for(i=0;i<v_size;i++)
        std::cout<<v_bias[i]<<' ';
    std::cout<<std::endl;
}

void RBM::propup(int k)

{
    int i,j;
    double count=0;

            //Calculate Q[k]
            for(i=0;i<h_size;i++)
                {   count=0;
                    for(j=0;j<v_size;j++)
                    {count = count + W[i][j]*v_samples[j][k];}

                    Q[i][k]=sigm(h_bias[i]+count);


                }


}

void RBM::propdown(int k)
{
    int i,j;
    double count=0;
    //Calculate P
            for(j=0;j<v_size;j++)
                {   count=0;
                    for(i=0;i<h_size;i++)
                    {count = count + W[i][j]*h_samples[i][k];}

                    P[j]=sigm(v_bias[j]+count);
                }
}
inline void RBM::sample_h_given_v(int k)
{
            propup(k);
            //sample H[k]
            for(int i=0;i<h_size;i++)
            {
                h_samples[i][k]=binomial(1,Q[i][k]);


            }
}

inline void RBM::sample_v_given_h(int k)
{
            propdown(k);
            //sample V[1]
            for(int i=0;i<v_size;i++)

            {v_samples[i][1]=binomial(1,P[i]); }

}

void RBM::update(int K,int N,double learning_rate)
 {  int i,j;

        //Contrastive Divergence K
        sample_h_given_v(0);
        for(int step=0; step<K; step++){
            if(step==0){
                sample_v_given_h(0);
                sample_h_given_v(1);
            }
            else{
                sample_v_given_h(1);
                sample_h_given_v(1);
                }
        }


    for(i=0;i<h_size;i++){
        for(j=0;j<v_size;j++){
        W[i][j] += learning_rate * (h_samples[i][0] * v_samples[j][0] - Q[i][1] * v_samples[j][1]) / N;
        }
        h_bias[i] += learning_rate * (h_samples[i][0] - Q[i][1]) / N ;
    }

    for(i=0;i<v_size;i++){
    v_bias[i] += learning_rate * (v_samples[i][0] - v_samples[i][1]) / N ;
    }

 }
void RBM::train_RBM()
{
    double set[6][6]={
    {1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0}
    };
    int i,j,l;
    int K=1,N=6;
    //srand(0);
    //random_initialize();
    std::cout<<"\nInitial parameters\n";
    print_parameters();
    for(i=0;i<1000;i++)
    {
       for(j=0;j<N;j++)
           {

            for(l=0;l<v_size;l++)
             v_samples[l][0]=set[j][l];

            update(K,N);



           }


    }
    std::cout<<"\n\nUpdated parameters\n";
    print_parameters();
}




void RBM::predict_RBM()
{
    int i,j;
    double count=0;


    for(j=0;j<v_size;j++)
            {count=0;
                for(i=0;i<h_size;i++)
                 {count = count + W[i][j]*Q[i][0];}

                  P[j]=sigm(v_bias[j]+count);
            }


    std::cout<<"\n\nVisibile values\n"<<std::endl;
    for(i=0;i<v_size;i++)
            {
                std::cout<<P[i]<<' ';
            }



    std::cout<<std::endl;




}

void RBM::test_RBM()
{   double *a,count=0;
    a=new double[v_size];
    double set[6][6]={
    {1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {1, 1,  1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0}
    };
    int i,j,k,l,m;

    for(k=0;k<6;k++)
  {
    std::cout<<"\nExample no"<<k<<std::endl;

    for(l=0;l<v_size;l++)
    {v_samples[l][0]=set[k][l]; std::cout<<v_samples[l][0]<<' ';}

    propup(0);
    std::cout<<"\n\nHidden values\n"<<std::endl;
    for(i=0;i<h_size;i++)
            {
                std::cout<<Q[i][0]<<' ';

            }
    std::cout<<std::endl;

    predict_RBM();

    }

}




void RBM::finalize()
{
    int i;

	if (W != 0)
	{
		for (i = 0; i < h_size; i++)
	  	{
  			delete[] W[i];
	  	}
		delete[] W;
	}

	W = NULL;

	if (Q != 0)
	{
		for (i = 0; i < h_size; i++)
	  	{
  			delete[] Q[i];
	  	}
		delete[] Q;
	}

	Q = NULL;

    if(P!=0) delete[] P;

    if(v_samples!=0){
    for (i = 0; i < v_size; i++)
	  	{
  			delete[] v_samples[i];
	  	}
		delete[] v_samples;
    }

    if(v_bias!=0)
    delete[] v_bias;

    if(h_samples!=0){
    for (i = 0; i < h_size; i++)
	  	{
  			delete[] h_samples[i];
	  	}
		delete[] h_samples;
    }
    if(h_bias!=0)
    delete[] h_bias;

        h_size=0;
        v_size=0;
}

void DBN::initialize(int n_layers)
{if(this->n_layers!=0)
 finalize();

 this->n_layers = n_layers;
 rbm_layers = new RBM[n_layers+1];


}

void DBN::finalize()
{
    if(rbm_layers!=0)
    delete []rbm_layers;
}

inline std::istream& operator>>(std::istream &is,DBN &dbn)
{
    int nls;
    std::string line;
    if(getline(is,line)){
    std::istringstream iss(line);
    iss>>dbn.n_inputs>>dbn.n_outputs>>nls;
    }

    dbn.initialize(nls);

    for(int i=0;i<=dbn.n_layers;i++)
    is>>dbn.rbm_layers[i];

    return is;

}

inline std::ostream& operator<<(std:: ostream &os,DBN &dbn)
{
 for(int l=0;l<dbn.n_layers+1;l++)
    os<<dbn.rbm_layers[l]<<std::endl;;

}

DBN::DBN(int n_ins,int *hidden_layer_sizes,int n_outs,int n_ls):n_inputs(n_ins),n_outputs(n_outs),n_layers(n_ls),rbm_layers(0)
{
    int vsize,hsize;
    if(n_layers!=0)
   {initialize(n_ls);
   for(int i=0;i<=n_layers;i++)
    {
        if( i == 0 )  vsize=n_inputs;
        else  vsize=hidden_layer_sizes[i-1];

        if( i == n_layers )  hsize=n_outputs;
        else  hsize=hidden_layer_sizes[i];

        rbm_layers[i].initialize(vsize,hsize);
        rbm_layers[i].random_initialize();


    }
   }
}

void DBN::train(const std::string &path)
{   double pretraining_lr=0.1;
    int pretraining_iterations=10,N;
    std::vector<CBitArray> training_data;
    std::ifstream is(path.c_str());
    CBitArray v;
    std::string line;
    while(getline(is,line)){
    std::istringstream iss(line);
    iss>>v;
    training_data.push_back(v);
    }
    is.close();
    N=training_data.size();
    int vsize=training_data[0].size();
    double *training_v=new double [N*vsize];
    //for(int i=0;i<N;i++)  training_v[i]=new double[vsize];
    for(int i=0;i<N;i++)
        for(int j=0;j<vsize;j++)
        {training_v[i*vsize+j] = training_data[i].isset(j) ? 1.0 : 0.0;} //std::cout<<training_v[i][j];}

    pretrain(training_v,pretraining_lr,pretraining_iterations,N);
}

void DBN::get_hsamples(const int &layer,CBitArray &v)
{
v.clear();
v.setsize(rbm_layers[layer].gethsize());
for(int i=0;i<rbm_layers[layer].gethsize();i++)
	if(rbm_layers[layer].h_samples[i][0]>0.5)
	v.set(i);
}

void DBN::pretrain(double *input,double lr,int iterations,int N)
{
    int  i,j,k,l,t,vsize,hsize;
    int K=1; //CD-K
    for(i=0; i<n_layers; i++){
        for(j=0; j<iterations; j++){
            for(k=0; k<N ;k++){
                for(l=0; l<=i; l++){
                vsize=rbm_layers[l].getvsize();
                hsize=rbm_layers[l].gethsize();

                if(l==0)
                {   for(t=0; t<n_inputs; t++)
                    {rbm_layers[l].v_samples[t][0] = input[k*n_inputs + t];} //std::cout<<rbm_layers[l].v_samples[t][0]<<' ';}
                }
                else
                {
                    rbm_layers[l-1].sample_h_given_v(0);
                    for(t=0; t<vsize; t++)
                    rbm_layers[l].v_samples[t][0] = rbm_layers[l-1].h_samples[t][0];
                }

                }

                rbm_layers[i].update(K,N,lr);

            }
        }
    }



}
void DBN::softmax(double *x,int n_out)
{   int i;
    double sum=0,max=0;
    for(i=0; i<n_out; i++)  {if(max < x[i]) max = x[i];}
    for(i=0; i<n_out; i++)  {x[i] = exp(x[i]-max);  sum = sum+x[i];}
    for(i=0; i<n_out; i++)  {x[i] /= sum;}
    //for(i=0; i<n_out; i++)  {std::cout<<x[i]<<' ';}
    //std::cout<<std::endl;
}

void DBN::forward_sample(double *x)
{

    int i,j,k,vsize,hsize;
    for(i=0; i<n_layers+1; i++){
        vsize=rbm_layers[i].getvsize();
        hsize=rbm_layers[i].gethsize();
        if(i == 0)
        {for(j=0; j<n_inputs; j++)  {rbm_layers[i].v_samples[j][0] = x[j];} //std::cout<<"\n"<<rbm_layers[i].v_samples[j][0]<<"\n";}
        rbm_layers[i].sample_h_given_v(0);
        }
        else{

        if(i == n_layers)  {
            for(j=0; j<vsize; j++)  rbm_layers[i].v_samples[j][0] = rbm_layers[i-1].h_samples[j][0];

        }
        else
            {
                for(j=0; j<vsize; j++)  rbm_layers[i].v_samples[j][0] = rbm_layers[i-1].h_samples[j][0];
                rbm_layers[i].sample_h_given_v(0);

            }
        }

    }


}

void DBN::logistic_regression_train(double lr,int index,int N)
{   int i,j,vsize,hsize;
    vsize=rbm_layers[index].getvsize();
    hsize=rbm_layers[index].gethsize();
    //std::cout<<"\n"<<vsize<<' '<<hsize<<' ';
    double *y_given_x = new double[hsize];
    double *dy = new double[hsize];
    for(i=0; i<hsize; i++){
        y_given_x[i]=0;
        for(j=0; j<vsize; j++){
            y_given_x[i] += rbm_layers[index].W[i][j]*rbm_layers[index].v_samples[j][0];
        }
        y_given_x[i] += rbm_layers[index].h_bias[i];
    }
    if(index==n_layers)
    {softmax(y_given_x,hsize);}
    else
    for(i=0;i<hsize;i++) {y_given_x[i]=sigm(y_given_x[i]);}

    for(i=0; i<hsize; i++){ dy[i] = rbm_layers[index].h_samples[i][0] - y_given_x[i]; //std::cout<<"delta"<<dy[i]<<' ';
        for(j=0; j<vsize; j++){
            rbm_layers[index].W[i][j] += lr*dy[i]*rbm_layers[index].v_samples[j][0] / N;
        }
        rbm_layers[index].h_bias[i] += lr*dy[i] / N;
    }
delete[] y_given_x;
delete[] dy;
}


void DBN::predict(double *x)
{
    double *temp = new double[n_outputs];
    int i,j,k,vsize,hsize;
    for(i=0; i<n_layers+1; i++){
        vsize=rbm_layers[i].getvsize();
        hsize=rbm_layers[i].gethsize();
        if(i == 0)
        {for(j=0; j<vsize; j++)  rbm_layers[i].v_samples[j][0] = x[j];
        rbm_layers[i].propup(0);
        }
        else{
        for(j=0; j<vsize; j++)  rbm_layers[i].v_samples[j][0] = rbm_layers[i-1].Q[j][0];
        if(i == n_layers)  {
        for(j=0; j<hsize; j++){
            rbm_layers[i].h_samples[j][0]=0;
            for(k=0; k<vsize; k++)
            rbm_layers[i].h_samples[j][0] += rbm_layers[i].W[j][k] * rbm_layers[i].v_samples[k][0];

            rbm_layers[i].h_samples[j][0] += rbm_layers[i].h_bias[j];
        }
        for(j=0; j<hsize; j++)  temp[j] = rbm_layers[i].h_samples[j][0];
        softmax(temp,hsize);
        for(j=0; j<hsize; j++)  rbm_layers[i].h_samples[j][0] = temp[j];

        }
        else rbm_layers[i].propup(0);
        }

    }

delete []temp;
}

void DBN::finetune(double *input,double *label,double lr,int f_iterations,int N)
{   //std::cout<<n_inputs<<n_outputs<<n_layers<<N;
    int i,j,k,m;
    for(m=0;m<f_iterations;m++){
    for(j=0;j<N;j++){forward_sample(input+n_inputs*j);
        for(i=n_layers;i>=0;i--){

            if(i==n_layers) {for(k=0; k<n_outputs; k++) rbm_layers[i].h_samples[k][0] = label[j*n_outputs+k];} //std::cout<<' '<<rbm_layers[i].h_samples[k][0]<<' '; }}
            logistic_regression_train(lr,i,N);

          }

    }
    }


}

void DBN::print_trained_parameters()
{int i,j,l;
 int vsize,hsize;
    std::cout<<"\nUpdated parameter values are\n";
    for(l=0;l<=n_layers;l++)
    {
     vsize=rbm_layers[l].getvsize();
     hsize=rbm_layers[l].gethsize();
     std::cout<<"\nWeight matrix for layer "<<l<<" is\n";
        for(i=0;i<hsize;i++)
            {for(j=0;j<vsize;j++)
            std::cout<<rbm_layers[l].W[i][j]<<' ';
            std::cout<<std::endl;
            }

    std::cout<<"\nv_bias and h_bias values for layer "<<l<<" is\n";
    for(i=0;i<vsize;i++)
        std::cout<<rbm_layers[l].v_bias[i]<<' ';
    std::cout<<std::endl;
    for(i=0;i<hsize;i++)
        std::cout<<rbm_layers[l].h_bias[i]<<' ';

    }

}



DBN::~DBN(){
    if(n_layers!=0)
    finalize();

}



int main()
{    srand(0);



  //int i,j,l;
  //double pretrain_lr = 0.1;
  //int pretraining_epochs = 1000;
  //double finetune_lr = 0.1;
  //int finetune_epochs = 1000;

  //int train_N = 804521;
  //int test_N = 6;
  //int n_ins = 6;
  //int n_outs = 2;
  //int hidden_layer_size[] = {3, 3};
  //int n_layer = sizeof(hidden_layer_size) / sizeof(hidden_layer_size[0]);

  // training data
/*  double train_X[6][6] = {
    {1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0}
  };

  double train_Y[6][2] = {
    {1, 0},
    {1, 0},
    {1, 0},
    {0, 1},
    {0, 1},
    {0, 1}
  }; */

  // construct DBN
  DBN dbn;
  //DBN dbn(n_ins, hidden_layer_size, n_outs, n_layer);
  std::ifstream iss("zero_DBN_input.txt");
  iss>>dbn;
  iss.close();
  //dbn.print_trained_parameters();
  dbn.train("test_dbn_nonlinear.txt");

  // pretrain
  //dbn.pretrain(*train_X, pretrain_lr,pretraining_epochs,train_N);
  dbn.print_trained_parameters();

  // finetune
  //dbn.finetune(*train_X,*train_Y,finetune_lr,finetune_epochs,train_N);
  //dbn.print_trained_parameters();

  std::ofstream oss("output_DBN.txt");
  oss<<dbn;

 /*  double test_X[6][6] = {
    {1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0}
  };


    std::cout<<"\n\nThe test results are\n\n";
  // test
  for(i=0; i<test_N; i++){
    dbn.predict(test_X[i]);
    for(j=0;j<n_outs;j++)
        std::cout<<dbn.rbm_layers[n_layer].h_samples[j][0]<<' ';
        std::cout<<std::endl;
  }
     RBM b;
     std::ifstream is("read_initial_parameters.txt");
     is>>b;
     is.close();
     b.print_parameters();
    //b.train_RBM();
     b.train("RBM_training_set.txt");
     b.test_RBM();
     std::ofstream os("output_RBM.txt");
     os<<b;
     os.close(); */

     return 0;


}

