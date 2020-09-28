#ifndef VECTOR_H_
#define VECTOR_H_
#define DEAUFAULT_CAPACITY 3

#include<iostream>
#include "Fib.h"

typedef int Rank;   //秩

template <typename T>
class Vector
{
protected:
	Rank _size;             //规模
	int _capacity;          //容量
	T * _elem;              //数据区
	void copyFrom(T const * A, Rank lo, Rank hi);         //从数组间复制
	void expand();          //向量容量扩充
	void shrink();          //向量容量缩减
	void bubbleSort(Rank lo, Rank hi); //起泡排序算法
	//Rank max(Rank lo, Rank hi); //选取最大元素
	//void selectionSort(Rank lo, Rank hi); //选择排序算法
	void merge(Rank lo, Rank mi, Rank hi); //归并算法
	void mergeSort(Rank lo, Rank hi); //归并排序算法
	//void heapSort(Rank lo, Rank hi);  //堆排序（稍后结合完全堆讲解）
	//Rank partition(Rank lo, Rank hi); //轴点构造算法
	//void quickSort(Rank lo, Rank hi); //快速排序算法
	//void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
	//构造函数
	Vector(int c = DEAUFAULT_CAPACITY, int s = 0, T v = 0)     //默认构造函数
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const * A, Rank n) { copy(A, 0, n); }                       //数组复制    
	Vector(T const * A, Rank lo, Rank hi) { copy(A, lo, hi); }           //数组区间复制
	Vector(Vector<T> const & V, Rank lo, Rank hi) { copy(A, lo, hi); }   //向量区间复制
	Vector(Vector<T> const & V) { copy(V._elem, 0, V._size); }           //复制构造函数
	//析构函数
	~Vector() { delete[] _elem; }

	//只读访问接口
	Rank size() const { return _size; }         //规模
	bool empty() const { return _size == 0; }   //判空
	int disordered() const;                     //逆序数，反映紊乱程度

	Rank find(T const & e, Rank lo = 0, Rank hi = _size) const;     //无序向量查找
	Rank search(T const & e, Rank lo = 0, Rank hi = _size) const;   //有序向量查找

	//可写访问接口
	T & operator[](Rank r);                  //重载[],可作为左值
	const T & operator[](Rank r) const;      //重载[],可作为右值

	Vector<T> & operator=(Vector<T> const & V);     //重载赋值操作符

	Rank insert(Rank r, T const & e);               //插入
	void insert(T const & e);                       //插入尾元素

	int remove(Rank lo, Rank hi);            //删除区间[lo,hi)
	T remove(Rank r);                        //删除秩为r的元素

	void sort(Rank lo = 0, Rank hi = _size);   //排序
	//void unsort(Rank lo = 0, Rank hi = _size); //置乱

	int deduplicate();                   //无序去重
	int uniquify();                      //有序去重

    //遍历
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
	if (_size < _capacity)     //尚未满员
		return;
	else
	{
		_capacity = (_capacity > DEAUFAULT_CAPACITY) ? _capacity : DEAUFAULT_CAPACITY;
		T * oldElem = _elem;
		_elem = new T[_capacity <<= 1];         //容量加倍
		for (int i = 0; i < _size; i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;               //释放原空间
	}
}

template <typename T>
void Vector<T>::shrink()
{
	if (4 * _size < _capacity)     //规模小于1/4
	{
		_capacity = 0.5*_capacity;           //容量减半
		_capacity = (_capacity > DEAUFAULT_CAPACITY) ? _capacity : DEAUFAULT_CAPACITY;
		T * oldElem = _elem;
		_elem = new T[_capacity];
		for (int i = 0; i < _size; i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;              //释放原空间
	}
}

template <typename T>
Rank  Vector<T>::find(T const & e, Rank lo, Rank hi) const
{
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;                   //hi<lo意味着查找失败；否则hi即元素e的最大秩
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
Rank Vector<T>::insert(Rank r, T const & e)       //插入第i个元素e
{
	expand();      //查看是否需要扩容
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1];
	_elem[r] = e;
	_size++;
	return r;
}
template <typename T>
void Vector<T>::insert(T const & e)               //插入末元素e
{
	expand();      //查看是否需要扩容
	_size++;
	_elem[_size - 1] = e;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)            //删除区间[lo,hi)
{
	if (lo == hi)
		return 0;
	while (hi < _size)                             //顺序前移
		_elem[lo++] = _elem[hi++];
	_size -= hi - lo;
	shrink();                                     //更新规模，若有必要缩容
	return hi - lo;                               //返回被删除元素的数目
}
template <typename T>
T  Vector<T>::remove(Rank r)
{
	T e = _elem[r];     //备份被删除的元素
	remove(r, r + 1);
	return e;           //返回被删除的元素
}

template <typename T>
int Vector<T>::deduplicate()
{
	int oldSize = _size;            //备份规模大小
	Rank i = 1;
	while (i < _size)
		find(_elem[i], 0, i) < 0 ? i++ : remove(i);           //不断剔除前i个与秩为i的元素值相等的元素
	return oldSize - _size;           //返回删除的重复元素数
}

template <typename T>
int Vector<T>::disordered() const
{
	int n = 0;                                  //相邻逆序对的计数器
	for (int i = 1; i < _size; i++)
		n += (_elem[i - 1] > _elem[i]);
	return n;                                   //仅当n=0时有序
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
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const              //查找
{
	return (rand() % 2) ? binSearch(_elem, e, lo, hi) : finSearch(_elem, e, lo, hi);
}
template <typename T>
static Rank binSearch(T * A, T const & e, Rank lo, Rank hi)    //二分查找法
{
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;           //取中值
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
static Rank fibSearch(T * A, T const & e, Rank lo, Rank hi)    //Fibonacci查找法
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
static Rank inpSearch(T * A, T const & e, Rank lo, Rank hi)    //interpolation查找法
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
		bubbleSort(lo, hi);                 //冒泡排序
		break;
	//case 2:
	//	selectionSort(lo, hi);              //选择排序
	//	break;
	case 3:
		mergeSort(lo, hi);                  //归并排序
		break;
	//case 4:
	//	heapSort(lo, hi);                   //堆排序
	//	break;
	//case 5:
	//	quickSort(lo, hi);                  //快速排序
	//	break;
	//case 6:
	//	shellSort(lo, hi);                  //shell排序
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

