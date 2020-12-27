#include <stdio.h>
#include <omp.h>

double integrable_func(double x);
double rectangle_method_func(long num, double a, double b, int threads_amount);

int main (void)
{
    long num = 1000;//number of splits
    int threads_amount = 24;
    double a = 0.0;
    double b = 3.0;
    
    float start, end, delta;
    double integral;
    for(int i = 1; i <= threads_amount; i+=6){
        start = omp_get_wtime( );
        integral = rectangle_method_func(num, a, b, i);
        end = omp_get_wtime( );
        delta = end - start;

        printf(" \nIntegral by rectangle method method: %e\n" , integral);
        printf("Threads : %i\n", i);
        printf("Time : %f\n", delta);
        printf("\n\n");

        if(i == 1)
            i = 0;
    }

}
/*
parallel rectangle algorithm
*/
double rectangle_method_func(long num, double a, double b, int threads_amount)
{
    double sum = 0;
    double h = (b - a) / (double)num;
    double x;
    long i;

    #pragma omp parallel num_threads( threads_amount ) private(x, i)
    {
        #pragma omp for schedule(guided) reduction(+: sum)
            for (i = 0; i < num; i++){
                x = a + (double)i * h; 
                sum += integrable_func(x);
            }
    }

    double result = h*sum;

    return result;
}

/*
*integrable function (x^3)
*if you want you can change it
*/
double integrable_func(double x)
{
    return x*x*x; 
}
