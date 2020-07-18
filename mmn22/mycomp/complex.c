#include "complex.h"
complex tmp;

void read_comp(complex* var, float real, float img){
    var->real=real;
    var->img=img;
}

void print_comp(complex* var){
    if(var->img>=0){
        printf("%.2f + (%.2f)i", var->real, var->img);
    }else{
        printf("%.2f - (%.2f)i", var->real, (var->img)*(-1));
    }
}

void add_comp(complex* var1, complex* var2 ){
    read_comp(&tmp, var1->real+var2->real, var1->img+var2->img);
    print_comp(&tmp);
}

void sub_comp(complex* var1, complex* var2){
    read_comp(&tmp, var1->real-var2->real, var1->img-var2->img);
    print_comp(&tmp);
}

void mult_comp_real(complex* var1, float x){
    read_comp(&tmp, var1->real*x, var1->img*x);
    print_comp(&tmp);
}

void mult_comp_img(complex* var1, float y) {
    read_comp(&tmp, -y*var1->img, y*var1->real);
    print_comp(&tmp);
}

void mult_comp_comp(complex* var1, complex* var2){
    read_comp(&tmp, (var1->real*var2->real)-(var1->img*var2->img),(var1->real*var2->img+var1->img*var2->real));
    print_comp(&tmp);
}
void abs_comp(complex* var1) {
    read_comp(&tmp, sqrt(pow((var1->real),2)+pow((var1->img),2)), 0);
    print_comp(&tmp);
}
void stop() {
    exit(0);
    fflush(stdin);
}