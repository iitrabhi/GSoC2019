//--------------------------------------------------
//
//  Name     : Eduardo Patrice
//  Filename : integration_simpson.cpp
//  Date     : 1 Nov 2015
//  
//  Purpose  : This is a parallel program to perform 
//             numerical integration by using
//             Simpson's Rule
//             MPI is used.
//--------------------------------------------------

// To compile, issue this command:
//  mpiCC -o integration_simpson integration_simpson.cpp

// To run:
/* mpirun -np numproc ./integration_simpson n a b flag

Where,
- numproc is the number of processors
- n is the number of intervals that we want to divide from [a,b]
- a is the start point for integration
- b is the end point of the integration
- flag is used to choose which function to integrate
*/
// e.g mpirun -np 6 ./integration_simpson 10000000024 0 3.141592 0

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

int flag = 0;     // Default choose a sine function
double integral = 0; // this holds the integral computed by each process (local)
double total = 0;    // this holds the total of all the integrals (global)
double f(double x);   // Simply return the value of the function
double Simpson(double local_a, int local_n, double h);  // prototype of Simpson function

main(int argc, char** argv)
{
  int my_rank;
  int p;
  double a;
  double b;
  int n;
  if(argc == 5){
    n = atoi(argv[1]);
    a = atof(argv[2]);
    b = atof(argv[3]);
    flag = atoi(argv[4]); /*Note: you should include some functionality to switch between different functions*/
  }
  else{
    /*Note: you can change these to whatever you want*/
    n = 1024;
    a = 0;
    b = 3.141592;
  }
  /*h is the base of the segment*/
  double h = (b-a)/n; /*given by (b-a)/n */
  double local_a;    // this is the starting point of the integral of each process
  int local_n;      // this is the the number of subintervals for each process
  
  // Now initializes MPI and get the rank for each processes and the number of processes p
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  /*
    Include here your code for initializing the variables to the correct values
  */
  
  double startWTime;
  if(my_rank ==0)
  {
    startWTime = MPI_Wtime();
  }
  local_n = n/p;            // fair method -- divide total n with number of processes
  local_a = a+my_rank*(b-a)/p;    // Starting point of each integration for each process
  integral = Simpson(local_a,local_n,h);  // find the integral for each process
  
  // Now sum up all the integrals and put it into total
  MPI_Reduce(&integral, &total, 1, MPI_DOUBLE, MPI_SUM, 0 , MPI_COMM_WORLD);
  
  // At last output the result
  double endWTime;
  if(my_rank ==0){
    endWTime = MPI_Wtime();
    cout<<"With n == "<<n<<" intervals, our estimate"<<endl;
    cout<<"of the integral from "<<a<<" to "<<b<<" is "<<total<<endl;
    cout<<"Number of processors used = "<<p<<endl;
    cout<<"Time elapsed: "<<(endWTime-startWTime)*1000<<"ms"<<endl;
    fflush(stdout); 
  }

  MPI_Finalize();
}


//----------------------------------------------------------------
// Simpson
// Integrate the function from local_a with local_n subintervals
// each subintervals have a width of h
//----------------------------------------------------------------
double Simpson(double local_a, int local_n, double h){
  /*Write your code here to do the integration*/
  double result = 0;
  for(int i =0; i<=local_n; i++)
  {
    // if i is 0 or n, only add f(x_i)
    if((i==0)||(i==local_n))
      result += f(local_a + i* h);
    else if(i%2==0)
      // then if i is even, add 2*f(x_i)
      result += 2* f(local_a + i*h);
    else
      // else if i is odd, add 4*f(x_i)
      result += 4* f(local_a + i*h);  
  }
  result *= h/3;  // multiply the above series by h/3
  return(result);
}

double f(double x){
  double return_val;
  // simply choose a function based on flag
  // 0 => f(x)=cos(x)
  // 1 => f(x)=sin(x)
  // 2 => f(x)=tan(x)
  // 3 => f(x)=1/x
  switch (flag) 
  {
    case 0:
      return_val = sin(x);
      break;
    case 1:
      return_val = cos(x);
      break;   
    case 2:
      return_val = tan(x);
      break;
    case 3:
      return_val = 1/x;
      break;   
    default:
      return_val = cos(x);
      break;
  }
  /*Add your functions here, should be able to switch functions depending on the user's flag passed in*/
  return return_val;
}
