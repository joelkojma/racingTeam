// Joel Kojma Zad 1 Regulator PID
#include <stdio.h>
#include <math.h>

//SetPoint - wartosc zadana
double prevError=0; //poprzedni odchyl
double integral_sum=0; //suma calki

double PV=0; //Process variable - aktualna wartość obiektu
double computePID(double Kp, double Ki, double Kd, double deltaT, double SetPoint)
{
    double error = SetPoint - PV; // odchyl
    if (error*prevError<0) // Anti-WindUp 
        integral_sum=0;
    
    double proportional = Kp* error; //czesc proporcjonalna
    double integral=error* deltaT*Ki; //czesc calkujaca
    integral_sum += integral;
    double derivative = (error-prevError)*Kd/deltaT; // czesc rozniczkujaca
    double return_value = proportional + integral_sum + derivative;
    prevError=error;
    if (return_value>5) // zwracany sygnal od -5 do 5
        return 5.0;
    else if (return_value<-5)
        return -5.0;
    return return_value;
}

int main()
{
    double Kp=5;
    double Ki=1.5;
    double Kd=1.5;
    double SetPoint = 7; //wartosc zadana
    double deltaT = 0.8; //delta czasu
    const double object = 1 - pow(exp(1), -deltaT/10); // rownanie reprezentujace obiekt
    double ControlOutput; // Wartosc sygnalu z kontrolera PID

    for (int i=0; i<100; i++){
        ControlOutput = computePID(Kp, Ki, Kd, deltaT, SetPoint); // funkcja obliczajaca sygnal
        PV+=object*ControlOutput; // przelozenie sygnalu na zmiane wartosci obiektu
        printf("Iteration: %d setpoint: %.2lf  PV: %.2lf  ControlOutput: %.2lf \n", i, SetPoint, PV, ControlOutput);
    }
    return 0;
}