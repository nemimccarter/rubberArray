#include "RubberArray.h"

int main()
{
  try
  {
    RubberArray<int> ra;
    ra.append(-3);
    ra.append(-2);
    ra.append(-1);
    for (unsigned i = 0; i < 9; i++)
    {
      ra.append(i);
    }

    RubberArray<int>raCopy(ra);

    // Third constructor will be used in read()
    // operator= called in operator()

    cout << raCopy << endl;
    cout << &ra << endl;
    raCopy[0] = ra[1];             //operator[]
    cout << raCopy << endl;


    // const operator[]????

    raCopy = ra(1, 4);            //operator()
    cout << raCopy << endl;

    ra.append(raCopy);            // append(const RubberArray&)
    cout << ra << endl;

    cout << ra.length() << endl;

    ra.add(0);
    ra.add(5, 0);
    cout << ra << endl;
    ra.remove();
    ra.remove(5);
    ra.remove(9, 11);
    cout << ra << endl;

    ofstream outf("mytext.txt", ios::out | ios::binary);    //Creates a file name: mytext.txt
    ra.write(outf);
    outf.close();

    ifstream inf ("mytext.txt", ios::in | ios::binary);     //Reads from a file named: mytext.txt
    ra.read(inf);
    cout << endl << "Read from file results:\n" << ra << endl;
    inf.close();

  }
    catch (IOORE er)
{
    cout << er << endl;
    exit(200);
}

  return 0;
}

