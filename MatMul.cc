//
// Created by Rob Datta on 4/29/21.
//

#define  START 0x100000

#include <fstream>
#include "iostream"

using namespace std;
void matmul()
{
    int m=0;
    int n=0;
    cout<<"Start your experiment by entering the value of n for now we will consider a square matrix ";
    cin>>n;
    unsigned addr_A;
    unsigned addr_B;
    unsigned addr_C;
    unsigned matrx_size=n*n*4;
    addr_A=START;
    addr_B=START+matrx_size;
    addr_C=(START+2)*matrx_size;
    ofstream outfile;
    string size="Matmul_size_M"+to_string(m)+"_size_N"+to_string(n)+".t";
    string open_file_path="traces/"+size;
    outfile.open(open_file_path);
    if(outfile.is_open())
    {
        //Copied from matrix multiplication on hw1 see reference code in hw1
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                for(int k=0;k<n;k++)
                {
                    outfile<<"r 0x"<<hex<<addr_A+i*n*4+k*4<<endl;
                    outfile<<"w 0x"<<hex<<addr_B+j*4+k*n<<endl;
                    outfile<<"r 0x"<<hex<<addr_C+i*n*4+j*4<<endl;
                    outfile<<"w 0x"<<hex<<addr_C+i*n*4+j*4<<endl;

                }
            }
        }

    }

}

int main()
{

    matmul();
}