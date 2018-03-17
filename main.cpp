//RSA Summative
//Prateek Bansal
//This Program will
//Use public and private keys to
//encrypt or decrypt a message
//through methods that involve functions
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;


long long powerfunction(long long base, long long exponent);
bool isprime (unsigned long long num);
unsigned long long GCD (unsigned long long Phi_of_p_and_q, unsigned long long e_value);
unsigned long long ModMultInverse(unsigned long long Phi_of_p_and_q, unsigned long long e_value);
unsigned long long ModExpon(long long base, long long exponent, long long mod);
unsigned long long ChinRem (long long encrypted, long long p_value, long long q_value, long long d_value);

int main()
{
    //create p, and q
    unsigned long long p;
    unsigned long long q;
    //Telling user about how to start program
    cout<<"BEFORE RUNNING THIS PROGRAM, ENSURE THAT "<<endl;
    cout<<"TWO PRIME NUMBERS ARE WRITTEN IN THE TEXT FILE "<<endl;
    cout<<"ENCRYPT, IN THE FORMAT OF:p q"<<endl<<endl;

    ifstream inputFile;//input file
    ofstream outputFile;//output file
    inputFile.open("Encrypt.txt");//open file
    outputFile.open("Decrypt.txt",
                    ios_base::out|ios_base::app);
    if (inputFile.is_open())
    {
        //read in the p value, and the q value
        inputFile>>p;
        inputFile>>q;
    }
    //error check
    else
    {
        cout<<"ERROR: unable to open file"<<endl;
        return 0;
    }
    //output statement
    //check for prime, and re-enter number if not prime
    while (!isprime(p))
    {
        cout<<"Re-enter a first value that is prime in the file. ";
        return 0;
    }
    //check for prime, and re-enter number if not prime
    while (!isprime(q))
    {
        cout<<"Re-enter a second value that is prime in the file. ";
        return 0;
    }
    //create n
    unsigned long long n=p*q;
    //calculate Phi of n
    unsigned long long Phi=(p-1)*(q-1);
    //e and d values to use as public and private key
    cout<<"The Possible choices of e and d are: "<<endl;
    long long e;
    long long d;

    for (long long counter=2; counter<=p+q; counter++)
    {
        //prove that e is coprime to the Phi of n
        if (GCD(Phi, counter)==1)//coprime integers have a GCD of 1
        {
            d=ModMultInverse(Phi,counter);
            cout<<counter<<"     "<<d<<endl;
        }
    }
    //ask which values are used in calculations

//I DID NOT KNOW HOW TO DUMMY-PROOF THIS PART SO THAT THEY USE VALUES FROM THE SAME ROW
    cout<<"Which e value will you use? ";
    cin>>e;
    cout<<"Which d value will you use? ";
    cin>>d;

    long long message, encrypted;
    cout<<"Do you wish to Encrypt or Decrypt? ";

    string input="Decrypt";
    cin>>input;
    if(input=="Encrypt")
    {
        cout<<"Enter the message you wish to encrypt: "<<endl;
        cin>>message;
        encrypted=ModExpon(message,e,n);
        cout<<"Encrypted: "<<encrypted<<endl;
        if (inputFile.is_open())
        {
            outputFile<<"The message of "<<message<<" encrypts into "<<encrypted<<" with an n value of "<<n<<" and an e value of: "<<e<<endl<<endl;
            inputFile.close();
        }
        else
        {
            cout<<"ERROR: unable to open file"<<endl;
            return 0;
        }
    }
    else if(input=="Decrypt")
    {
        cout<<"Enter the message you wish to decrypt: "<<endl;
        cin>>encrypted;
        message=ChinRem(encrypted,p,q,e);
        cout<<"Decrypted: "<<message<<endl;
        if (inputFile.is_open())
        {
            outputFile<<"The encrypted message of "<<encrypted<<" decrypts into "<<message<<" with an n value of "<<n<<" and a d value of: "<<d<<endl<<endl;
            inputFile.close();
        }
        else
        {
            cout<<"ERROR: unable to open file"<<endl;
            return 0;
        }
    }
    else
    {
        cout<<"Please enter a valid answer. Encrypt or Decrypt?";
        cin>>input;
    }
}
long long powerfunction(long long base, long long exponent)
{
    //Modified from Dawson Phan's Powerfunction

    long long result=1;//create a resultant for modification
    for (long long i=1; i<=exponent; i++)
    {
        result=result*base;//multiply the result by the amount of times the counter goes up
    }
    return result;
}
bool isprime(unsigned long long num)
{
    while (num<=1 or cin.fail())
    {
        cin.clear();
        cin.ignore(100,'\n');
        return false;
    }
    //counts up to num/2+1, which is number up to which prime can be checked
    for (unsigned int i=2; i<num/2+1; i++)
    {
        //if it factors, it's not prime
        if (num%i==0)
        {
            //return that it's false
            return false;
        }
    }
    return true;
}
unsigned long long GCD (unsigned long long num, unsigned long long e)
{
    //while the number is true
    while (num)
    {
        //placeholder for the largest gcd
        unsigned long long x=e%num;
        //switch the numbers around
        e=num;
        num=x;
    }
    //return the GCD of the two numbers
    return e;
}
unsigned long long ModMultInverse(unsigned long long num, unsigned long long e)
{
    // The process for this algorithm was gotten from
    //https://www.youtube.com/watch?v=Z8M2BTscoD4
    //I just converted it to a function

    //the original number, to be used as rthe beginning point
    long long numleft=num, numright=num;
    //the left number, which if it becomes 1, the number on the right is the required d value
    long long left=e;
    //the beginning point for the d value
    long long right=1;
    //as long as neither reach 0, the program runs
    while (left!=0 or right!=0)
    {
        //the quotient to multiply the numbers by
        long long quotient=numleft/left;
        //formulas for determing the value of the next row
        long long newleft=numleft-quotient*left;
        long long newright=numright-quotient*right;
        //if newright ever reaches 0 or under, make it positive
        while (newright<0)
        {
            newright=newright+num;
        }
        //cycle the code down, so that each row is moved up one, and the math can vbe done again
        numleft=left;
        numright=right;
        left=newleft;
        right=newright;
        //once left finally reaches 1, the number on the right is the required d value
        if (left==1)
        {
            return right;
        }
    }
}
unsigned long long ModExpon(long long base, long long exponent, long long mod)
{
    //using modular exponentiation, this program is made smaller
    //http://en.wikipedia.org/wiki/Modular_exponentiation
    //the pseudo-code was converted into code here
    //breaking the equation into a testing of possibilites
    long long encrypted=1;//initialize the encrypted value
    for (long long counter=1; counter<=exponent; counter++)
    {
        //perform the calculation to update the program
        encrypted=(encrypted*base)%mod;
    }
    return encrypted;
}
unsigned long long ChinRem(long long encrypted, long long p, long long q, long long e)
{
    //using the chinese remainder algorithm, this program is made smaller
    //http://en.wikipedia.org/wiki/RSA_(cryptosystem)
    //website used for basic logic deduction,
    //but no hard code was used from it, only formulas
    long long dp=ModMultInverse(p-1, e);
    long long dq=ModMultInverse(q-1, e);
    long long qinv=ModMultInverse(p, q);
    //calculated the preliminary values to be used in the calculations
    //break the equation down into two modular exponentiations
    long long m1= ModExpon(encrypted, dp, p);
    long long m2= ModExpon(encrypted, dq, q);
    //calculate the placeholder
    long long h=qinv*(m1-m2)%p;
    //if the coefficient would be negative, flip the code to be positive
    if(m2>m1)
    {
        qinv=ModMultInverse(q, p);
        h=qinv*(m2-m1)%q;
    }
    //calculate the final decrypted message
    long long message=m2+(h*q);
    return message;
}
