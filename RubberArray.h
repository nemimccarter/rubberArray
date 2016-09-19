/*
Nehemya McCarter-Ribakoff
December 9th, 2015
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

class IOORE
{
  const char*  _message;
  const int    _i;
  const int    _size;

public:
  IOORE( const char* m, const int i, const int s )
  : _message(m), _i(i), _size(s) { };

  friend ostream& operator<< ( ostream& os, const IOORE& p )
  {
    os <<endl<<endl<< "Exception thrown in: ";
    os << p._message<<endl;
    os << "index: " << p._i << endl;
    os << "range 0 - " << (p._size-1) << endl<<endl;
    return os;
  }
};

template <class T>
class RubberArray
{
  T*        _array; // pointer to the items
  unsigned  _size;  // number of items
  unsigned  _alloc; // number of allocated spaces
  int       _zero;  // virtual zero
public:
  RubberArray (int Vindex = 0);
  RubberArray (const T*, unsigned s, int Vindex);
  RubberArray(const RubberArray&);

  ~RubberArray();

  RubberArray<T>& operator= (const RubberArray<T>&);
  template <typename OS>
  friend ostream& operator<< (ostream&, const RubberArray<OS>&);
  template <typename OS>
  friend ostream& operator<< (ostream&, const RubberArray<OS>*);
  T& operator[] (int Vindex);
  const T& operator[] (int Vindex) const;
  RubberArray operator() (int Vfirst, int Vlast) const;

  void append (const T&);
  void append (const RubberArray&);
  unsigned length() const;
  void add (int Vindex, const T&);
  void add (const T&);

  void remove ();
  void remove (int Vindex);
  void remove (int Vfirst, int Vlast);

  void write(ofstream&) const;
  void read(ifstream&);
};

template <class T>
RubberArray<T>::RubberArray(int Vindex)
{
  _array = NULL;
  _size = 0;
  _alloc = 0;
  _zero = Vindex;
}

template <class T>
RubberArray<T>::RubberArray(const T* ar, unsigned s, int Vindex )
{
  _array = NULL;
  _size = 0;
  _alloc = 0;
  _zero = Vindex;
  for (unsigned i = 0; i < s; i++)
  {
    append(ar[i]);
  }
}

template <class T>
RubberArray<T>::RubberArray(const RubberArray& ar)
{
  _array = NULL;
  _size = 0;
  _alloc = 0;
  _zero = ar._zero;

  for (unsigned i = 0; i < ar._size; i++)
  {
    append(ar._array[i]);
  }
}

template <class T>
RubberArray<T>::~RubberArray()
{
  delete [] _array;
}

template <class T>
RubberArray<T>& RubberArray<T>::operator=(const RubberArray<T>& ar)
{
  if (this != NULL)
    delete [] _array;
  _array = NULL;
  _size = 0;
  _alloc = 0;
  _zero = ar._zero;
  for (unsigned i = 0; i < ar._size; i++)
  {
    append(ar._array[i]);
  }

  return *this;
}

template <typename OS>
ostream& operator<< (ostream& os, const RubberArray<OS>& ar)
{
  if (ar._size == 0)
    cout << "The arrray is empty." << endl;
  else
  {
    for (unsigned i = 0; i < ar._size; i++)
    {
      os << ar._array[i] << " ";
    }
  }
  return os;
}

template <typename OS>
ostream& operator<< (ostream& os, const RubberArray<OS>* ar)
{
  if (ar->_size == 0)
    cout << "The array is empty." << endl;
  else
  {
    for (unsigned i = 0; i < ar->_size; i++)
    {
      os << ar->_array[i] << " ";
    }
  }
  return os;
}

template <class T>
T& RubberArray<T>::operator[] (int Vindex)
{
  int _entry = Vindex - _zero;
  if (_entry < 0 || Vindex > _size)
  {
    throw

    IOORE("T& RubberArray<T>::operator[] (int Vindex)", Vindex, _size);
    exit(1);
  }
  return _array[_entry];
}

template <class T>
const T& RubberArray<T>::operator[] (int Vindex) const
{
  int _entry = Vindex - _zero;
  if (_entry < 0 || Vindex > _size)
  {
    throw

    IOORE("const T& RubberArray<T>::operator[] (int Vindex)", Vindex, _size);
  }
  return _array[_entry];
}

template <class T>
RubberArray<T> RubberArray<T>::operator() (int Vfirst, int Vlast) const
{
  int Afirst = Vfirst - _zero;
  int Alast = Vlast - _zero;

  if (Afirst < 0 || Afirst > Alast)
  {
    throw

    IOORE("RubberArray<T> RubberArray<T>::operator() (int Vfirst, int Vlast)", Vfirst, _size);
    exit(1);
  }
  else if (Alast > _size)
  {
    throw

    IOORE("RubberArray<T> RubberArray<T>::operator() (int Vfirst, int Vlast)", Vlast, _size);
    exit(1);
  }

  RubberArray<T> temp(_zero);
  for (unsigned i = Afirst; i < Alast; i++)
  {
    temp.append(_array[i]);
  }
  return temp;
}

template <class T>
void RubberArray<T>::append (const T& newItem)
{
  if (_size == _alloc)
  {
    _alloc = (2 * _alloc) + 1;
    T* toAppend = new T[_alloc];
    for (unsigned i = 0; i < _size; i++)
    {
      toAppend[i] = _array[i];
    }
    if (_array != NULL)
      delete [] _array;
    _array = toAppend;
  }
  _array[_size++] = newItem;
}

template <class T>
void RubberArray<T>::append (const RubberArray& newItems)
{
  for (unsigned i = 0; i < newItems._size; i++)
  {
    append(newItems._array[i]);
  }
}

template <class T>
unsigned RubberArray<T>::length() const
{
  return _size;
}

template <class T>
void RubberArray<T>::add (int Vindex, const T& newItem)
{
  int Aindex = Vindex - _zero;
  if (Aindex < 0 || Aindex >= _size)
  {
    throw IOORE("void RubberArray<T>::add(int Vindex, const T& newItem)", Vindex, _size);
  }
  RubberArray<T> temp(_zero);
  for (unsigned i = 0; i < _size; i++)
  {
    if (i == Aindex)
      temp.append(newItem);
    temp.append(_array[i]);

  }
  *this = temp;
}

template <class T>
void RubberArray<T>::add (const T& newItem)
{
  RubberArray<T> temp(_zero);
  temp.append(newItem);
  for (unsigned i = 0; i < _size; i++)
  {
    temp.append(_array[i]);
  }
  *this = temp;
}

template <class T>
void RubberArray<T>::remove ()
{
  remove (_size - 1);
}

template <class T>
void RubberArray<T>::remove (int Vindex)
{
  int Aindex = Vindex - _zero;
  if (Aindex < 0 || Aindex >= _size)
  {
    throw IOORE("void RubberArray<T>::remove (int Vindex)", Vindex, _size);
  }
  RubberArray<T> temp(_zero);
  for (unsigned i = 0; i < _size; i++)
  {
    if (i != Aindex)
      temp.append(_array[i]);
  }
  *this = temp;
}

template <class T>
void RubberArray<T>::remove (int Vfirst, int Vlast)
{
  int Afirst = Vfirst - _zero;
  int Alast = Vlast - _zero;
  RubberArray<T> temp(_zero);
  if (Afirst < 0 || Afirst > Alast)
  {
    throw IOORE("void RubberArray<T>::remove (int Vfirst, int Vlast)", Vfirst, _size);
  }
  else if (Alast > _size)
  {
    throw IOORE("void RubberArray<T>::remove (int Vfirst, int Vlast)", Vlast, _size);
  }

  for (unsigned i = 0; i < Afirst; i++)
  {
    temp.append(_array[i]);
  }
  for (unsigned i = Alast; i < _size; i++)
  {
    temp.append(_array[i]);
  }
  *this = temp;
}


template<class T>
void RubberArray<T>::write(ofstream& ofs) const
{
  if (!ofs)
  {
    throw int(1);
  }
  ofs.write(reinterpret_cast<const char*>(&_zero), sizeof(_zero));
  ofs.write(reinterpret_cast<const char*>(&_size), sizeof(_size));
  ofs.write(reinterpret_cast<const char*>(_array), sizeof(T) * _size);
}

template<class T>
void RubberArray<T>::read(ifstream& ifs)
{
  if (!ifs)
  {
    throw int(2);
  }
  ifs.read(reinterpret_cast<char*>(&_zero), sizeof(_zero));
  ifs.read(reinterpret_cast<char*>(&_size), sizeof(_size));
  T* temp = new T[_size];
  ifs.read(reinterpret_cast<char*>(temp), sizeof(T) * _size);
  *this = RubberArray<T>(temp, _size, _zero);
  delete [] temp;
}
