#ifndef VECTOR_H_
#define VECTOR_H_
#define DEAUFAULT_CAPACITY 3

#include<iostream>
#include "Fib.h"

typedef int Rank;   //��

template <typename T>
class Vector
{
protected:
	Rank _size;             //��ģ
	int _capacity;          //����
	T * _elem;              //������
	void copyFrom(T const * A, Rank lo, Rank hi);         //������临��
	void expand();          //������������
	void shrink();          //������������
	void bubbleSort(Rank lo, Rank hi); //���������㷨
	//Rank max(Rank lo, Rank hi); //ѡȡ���Ԫ��
	//void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	//void heapSort(Rank lo, Rank hi);  //�������Ժ�����ȫ�ѽ��⣩
	//Rank partition(Rank lo, Rank hi); //��㹹���㷨
	//void quickSort(Rank lo, Rank hi); //���������㷨
	//void shellSort(Rank lo, Rank hi); //ϣ�������㷨
public:
	//���캯��
	Vector(int c = DEAUFAULT_CAPACITY, int s = 0, T v = 0)     //Ĭ�Ϲ��캯��
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const * A, Rank n) { copy(A, 0, n); }                       //���鸴��    
	Vector(T const * A, Rank lo, Rank hi) { copy(A, lo, hi); }           //�������临��
	Vector(Vector<T> const & V, Rank lo, Rank hi) { copy(A, lo, hi); }   //�������临��
	Vector(Vector<T> const & V) { copy(V._elem, 0, V._size); }           //���ƹ��캯��
	//��������
	~Vector() { delete[] _elem; }

	//ֻ�����ʽӿ�
	Rank size() const { return _size; }         //��ģ
	bool empty() const { return _size == 0; }   //�п�
	int disordered() const;                     //����������ӳ���ҳ̶�

	Rank find(T const & e, Rank lo = 0, Rank hi = _size) const;     //������������
	Rank search(T const & e, Rank lo = 0, Rank hi = _size) const;   //������������

	//��д���ʽӿ�
	T & operator[](Rank r);                  //����[],����Ϊ��ֵ
	const T & operator[](Rank r) const;      //����[],����Ϊ��ֵ

	Vector<T> & operator=(Vector<T> const & V);     //���ظ�ֵ������

	Rank insert(Rank r, T const & e);               //����
	void insert(T const & e);                       //����βԪ��

	int remove(Rank lo, Rank hi);            //ɾ������[lo,hi)
	T remove(Rank r);                        //ɾ����Ϊr��Ԫ��

	void sort(Rank lo = 0, Rank hi = _size);   //����
	//void unsort(Rank lo = 0, Rank hi = _size); //����

	int deduplicate();                   //����ȥ��
	int uniquify();                      //����ȥ��

    //����
	template <typename VST> void traverse(VST &);

};

template <typename T>
void Vector<T>::copyFrom(T const * A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

template <typename T>
void Vector<T>::expand()
{
	if (_size < _capacity)     //��δ��Ա
		return;
	else
	{
		_capacity = (_capacity > DEAUFAULT_CAPACITY) ? _capacity : DEAUFAULT_CAPACITY;
		T * oldElem = _elem;
		_elem = new T[_capacity <<= 1];         //�����ӱ�
		for (int i = 0; i < _size; i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;               //�ͷ�ԭ�ռ�
	}
}

template <typename T>
void Vector<T>::shrink()
{
	if (4 * _size < _capacity)     //��ģС��1/4
	{
		_capacity = 0.5*_capacity;           //��������
		_capacity = (_capacity > DEAUFAULT_CAPACITY) ? _capacity : DEAUFAULT_CAPACITY;
		T * oldElem = _elem;
		_elem = new T[_capacity];
		for (int i = 0; i < _size; i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;              //�ͷ�ԭ�ռ�
	}
}

template <typename T>
Rank  Vector<T>::find(T const & e, Rank lo, Rank hi) const
{
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;                   //hi<lo��ζ�Ų���ʧ�ܣ�����hi��Ԫ��e�������
}


template <typename T>
Vector<T> & Vector<T>::operator=(Vector<T> const & V)
{
	if (this == &V)
		return;
	delete[] _elem;
	copyFrom(V, 0, V._size);
}

template <typename T>
T & Vector<T>::operator[](Rank r)
{
	return _elem[r];
}
template <typename T>
const T & Vector<T>::operator[](Rank r) const
{
	return _elem[r];
}

template <typename T> template <typename VST>
void Vector<T>::traverse(VST & visit)
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const & e)       //�����i��Ԫ��e
{
	expand();      //�鿴�Ƿ���Ҫ����
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1];
	_elem[r] = e;
	_size++;
	return r;
}
template <typename T>
void Vector<T>::insert(T const & e)               //����ĩԪ��e
{
	expand();      //�鿴�Ƿ���Ҫ����
	_size++;
	_elem[_size - 1] = e;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)            //ɾ������[lo,hi)
{
	if (lo == hi)
		return 0;
	while (hi < _size)                             //˳��ǰ��
		_elem[lo++] = _elem[hi++];
	_size -= hi - lo;
	shrink();                                     //���¹�ģ�����б�Ҫ����
	return hi - lo;                               //���ر�ɾ��Ԫ�ص���Ŀ
}
template <typename T>
T  Vector<T>::remove(Rank r)
{
	T e = _elem[r];     //���ݱ�ɾ����Ԫ��
	remove(r, r + 1);
	return e;           //���ر�ɾ����Ԫ��
}

template <typename T>
int Vector<T>::deduplicate()
{
	int oldSize = _size;            //���ݹ�ģ��С
	Rank i = 1;
	while (i < _size)
		find(_elem[i], 0, i) < 0 ? i++ : remove(i);           //�����޳�ǰi������Ϊi��Ԫ��ֵ��ȵ�Ԫ��
	return oldSize - _size;           //����ɾ�����ظ�Ԫ����
}

template <typename T>
int Vector<T>::disordered() const
{
	int n = 0;                                  //��������Եļ�����
	for (int i = 1; i < _size; i++)
		n += (_elem[i - 1] > _elem[i]);
	return n;                                   //����n=0ʱ����
}

template <typename T>
int Vector<T>::uniquify()
{
	Rank i = 0, j = 0;
	while (++j < _size)
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return j - i;
}

template <typename T>
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const              //����
{
	return (rand() % 2) ? binSearch(_elem, e, lo, hi) : finSearch(_elem, e, lo, hi);
}
template <typename T>
static Rank binSearch(T * A, T const & e, Rank lo, Rank hi)    //���ֲ��ҷ�
{
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;           //ȡ��ֵ
		if (e < A[mi])
			hi = mi;
		else if (A[mi] < e)
			lo = mi;
		else
			return mi;
	}
	return -1;
}

template <typename T>
static Rank fibSearch(T * A, T const & e, Rank lo, Rank hi)    //Fibonacci���ҷ�
{
	while (lo < hi)
	{
		Rank mi = lo + (hi - lo)*(e - A[lo])*(A[hi] - A[lo]);
		if (e < A[mi])
			hi = mi;
		else if (A[mi] < e)
			lo = mi;
		else
			return mi;
	}
	return -1;
}

template <typename T>
static Rank inpSearch(T * A, T const & e, Rank lo, Rank hi)    //interpolation���ҷ�
{
	Fib fib(hi - lo);
	while (lo < hi)
	{
		int i = 1;
		while (hi - lo < fib[i])
			i++;
		Rank mi = lo + fib[i - 1] - 1;
		if (e < A[mi])
			hi = mi;
		else if (A[mi] < e)
			lo = mi;
		else
			return mi;
	}
	return -1;
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	switch (rand() % 6)
	{
	case 1:                                      
		bubbleSort(lo, hi);                 //ð������
		break;
	//case 2:
	//	selectionSort(lo, hi);              //ѡ������
	//	break;
	case 3:
		mergeSort(lo, hi);                  //�鲢����
		break;
	//case 4:
	//	heapSort(lo, hi);                   //������
	//	break;
	//case 5:
	//	quickSort(lo, hi);                  //��������
	//	break;
	//case 6:
	//	shellSort(lo, hi);                  //shell����
	//	break;
	}
}
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (lo < --hi)
		for (Rank i = lo; i < hi; i++)
			if (_elem[i] > _elem[i + 1])
				std::swap(_elem[i], _elem[i + 1]);
}
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2)
		return;
	else
	{
		int mi = (lo + hi) >> 1;
		mergeSort(lo, mi);
		mergeSort(mi, hi);
		merge(lo, mi, hi);
	}
}
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi,Rank hi)
{
	T * A = _elem + lo;
	int lb = mi - lo;
	T * B = new T[lb];
	for (Rank i = 0; i < lb; i++)
		B[i] = A[i];
	int lc = hi - mi;
	T * C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; j < lb;)
		A[i++] = (lc <= k || B[j] <= C[k]) ? B[j++] : C[k++];
	delete[] B;

}
#endif // !VECTOR
#pragma once

