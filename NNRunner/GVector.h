#pragma once

#include <cmath>
#include <vector>


/// Simple but fast vector template for basic linear algebra;
/// class T is intended to be one of the following types: int, float or double.
template<class T> class GVector
{
public:
	GVector(unsigned int length = 3) :
		size(length)
	{
		coordinates = new T[size];
		for (unsigned int i = 0; i < size; i++)
			coordinates[i] = 0;
	}

	GVector(T x, T y) :
		size(2)
	{
		coordinates = new T[size];
		coordinates[0] = x;
		coordinates[1] = y;
	}

	GVector(T x, T y, T z) :
		size(3)
	{
		coordinates = new T[size];
		coordinates[0] = x;
		coordinates[1] = y;
		coordinates[2] = z;
	}

	GVector(const std::vector<T>& src) :
		size(src.size())
	{
		coordinates = new T[size];
		for (unsigned int i = 0; i < size; i++)
			coordinates[i] = src[i];
	}

	GVector(const GVector<T>& src) :
		size(src.size)
	{
		coordinates = new T[size];
		Set(src);
	}

	~GVector(void) { delete [] coordinates; }

	inline unsigned int Size(void) const { return size; }

	bool IsValid(void)
	{
		for (unsigned int i = 0; i < size; i++)
			if (std::isnan(coordinates[i]) || std::isinf(coordinates[i]))
				return false;
		return true;
	}

	void Set(const GVector<T>& v)
	{
		if (v.size == size)
		{
			for (unsigned int i = 0; i < size; i++)
				coordinates[i] = v.coordinates[i];
		}
		else throw "GVector::Set(): vector lengths not equal.";
	}

	void Set(T a)
	{
		for (unsigned int i = 0; i < size; i++)
			coordinates[i] = a;
	}
	
	void CopyTo(GVector<T>& v) const
	{
		if (v.size >= size)
		{
			for (unsigned int i = 0; i < size; i++)
				v.coordinates[i] = coordinates[i];
		}
		else throw "GVector::CopyTo(): destination vector too short.";
	}

	inline T X(void) const { return coordinates[0]; }
	inline T Y(void) const { return coordinates[1]; }
	inline T Z(void) const { return coordinates[2]; }

	T& at(unsigned int index)
	{
		if (index < size) return coordinates[index];
		else throw "GVector::operator[]: index out of range.";
	}

	const T& at(unsigned int index) const
	{
		if (index < size) return coordinates[index];
		else throw "GVector::operator[]: index out of range.";
	}

	inline T& operator [] (unsigned int index) { return coordinates[index]; }
	inline const T& operator [] (unsigned int index) const { return coordinates[index]; }

	GVector<T>& operator=(const GVector<T>& other)
	{
		if (this != &other)
		{
			if (size != other.size)
			{
				delete [] coordinates;
				coordinates = new T[other.size];
				size = other.size;
			}
			Set(other);
		}
		return *this;
	}

	template<class T1, class T2>
	friend double operator*(const GVector<T1>& v1, const GVector<T2>& v2);

	template<class T1>
	friend GVector<T1> operator*(const GVector<T1>& v1, double a);
	template<class T1>
	friend GVector<T1> operator*(const GVector<T1>& v1, float a);
	template<class T1>
	friend GVector<T1> operator*(const GVector<T1>& v1, int a);

	template<class T1>
	friend GVector<T1> operator*(double a, const GVector<T1>& v1);
	template<class T1>
	friend GVector<T1> operator*(float a, const GVector<T1>& v1);
	template<class T1>
	friend GVector<T1> operator*(int a, const GVector<T1>& v1);

	template<class T1>
	friend GVector<T1> operator+(const GVector<T1>& v1, const GVector<T1>& v2);

	template<class T1>
	friend GVector<T1> operator+(const GVector<T1>& v1, double a);
	template<class T1>
	friend GVector<T1> operator+(const GVector<T1>& v1, float a);
	template<class T1>
	friend GVector<T1> operator+(const GVector<T1>& v1, int a);

	template<class T1>
	friend GVector<T1> operator-(const GVector<T1>& v1, const GVector<T1>& v2);

	template<class T1>
	friend GVector<T1> operator-(const GVector<T1>& v1, double a);
	template<class T1>
	friend GVector<T1> operator-(const GVector<T1>& v1, float a);
	template<class T1>
	friend GVector<T1> operator-(const GVector<T1>& v1, int a);

	template<class U>
	GVector<T>& Scale(const GVector<U>& v)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] *= v[i];
		}
		return *this;
	}

	GVector<T>& Scale(double a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] *= a;
		}
		return *this;
	}

	GVector<T>& Scale(float a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] *= a;
		}
		return *this;
	}

	GVector<T>& Scale(int a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] *= a;
		}
		return *this;
	}

	template<class U>
	GVector<T>& ScaleAdd(const GVector<U>& v, double a)
	{
		if (v.size == size)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				coordinates[i] += (T)(a * v[i]);
			}
			return *this;
		}
		else throw "GVector::ScaleAdd(): vector lengths not equal.";
	}

	template<class U>
	GVector<T>& ScaleAdd(const GVector<U>& v, float a)
	{
		if (v.size == size)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				coordinates[i] += (T)(a * v[i]);
			}
			return *this;
		}
		else throw "GVector::ScaleAdd(): vector lengths not equal.";
	}

	template<class U>
	GVector<T>& ScaleAdd(const GVector<U>& v, int a)
	{
		if (v.size == size)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				coordinates[i] += (T)(a * v[i]);
			}
			return *this;
		}
		else throw "GVector::ScaleAdd(): vector lengths not equal.";
	}

	template<class U>
	GVector<T>& Add(const GVector<U>& v)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] += v[i];
		}
		return *this;
	}

	GVector<T>& Add(double a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] += a;
		}
		return *this;
	}

	GVector<T>& Add(float a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] += a;
		}
		return *this;
	}

	GVector<T>& Add(int a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] += a;
		}
		return *this;
	}

	template<class U>
	GVector<T>& Sub(const GVector<U>& v)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] -= v[i];
		}
		return *this;
	}

	GVector<T>& Sub(double a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] -= a;
		}
		return *this;
	}

	GVector<T>& Sub(float a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] -= a;
		}
		return *this;
	}

	GVector<T>& Sub(int a)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] -= a;
		}
		return *this;
	}

	GVector<T>& Abs(void)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			coordinates[i] = fabs(coordinates[i]);
		}
		return *this;
	}

	double Norm2(void) const
	{
		double acc = 0.0;
		for (unsigned int i = 0; i < size; i++)
		{
			acc += coordinates[i] * coordinates[i];
		}
		return acc;
	}

	inline double Norm(void) const { return sqrt(Norm2()); }

	T Sum(void) const
	{
		double acc = 0.0;
		for (unsigned int i = 0; i < size; i++)
		{
			acc += coordinates[i];
		}
		return (T)acc;
	}

	T Max(unsigned int nfirst = 0) const
	{
		if (!nfirst || (nfirst > size)) nfirst = size;

		T max = coordinates[0];
		for (unsigned int i = 1; i < nfirst; i++)
			if (coordinates[i] > max) max = coordinates[i];
		return max;
	}

	T Min(unsigned int nfirst = 0) const
	{
		if (!nfirst || (nfirst > size)) nfirst = size;

		T min = coordinates[0];
		for (unsigned int i = 1; i < nfirst; i++)
			if (coordinates[i] < min) min = coordinates[i];
		return min;
	}

	static GVector<T> Max(const std::vector< GVector<T> >& data)
	{
		if (data.size() < 1) throw "GVector::Max(): vector collection empty.";

		GVector<T> max(data[0]);
		for (unsigned int i = 1; i < data.size(); i++)
		{
			const GVector<T>& h = data[i];
			for (unsigned int j = 0; j < max.size; j++)
			{
				if (h[j] > max[j]) max[j] = h[j];
			}
		}
		return max;
	}

	static GVector<T> Min(const std::vector< GVector<T> >& data)
	{
		if (data.size() < 1) throw "GVector::Min(): vector collection empty.";

		GVector<T> min(data[0]);
		for (unsigned int i = 1; i < data.size(); i++)
		{
			const GVector<T>& h = data[i];
			for (unsigned int j = 0; j < min.size; j++)
			{
				if (h[j] < min[j]) min[j] = h[j];
			}
		}
		return min;
	}

	static float Cos(const GVector<T>& v1, const GVector<T>& v2, const GVector<T>& v3)
	{
		GVector<T> vA(v1);
		vA.Sub(v2);
		GVector<T> vB(v3);
		vB.Sub(v2);
		double mag = sqrt(vA.Norm2() * vB.Norm2());
		double cosine = 0.0;
		if (mag != 0.0) cosine = vA * vB / mag;
		return (float)cosine;
	}

private:
	unsigned int size;
	T* coordinates; //[size]
};

template<class T1, class T2>
double operator*(const GVector<T1>& v1, const GVector<T2>& v2)
{
	if (v1.size == v2.size)
	{
		double acc = 0.0;
		for (unsigned int i = 0; i < v1.size; i++)
		{
			acc += v1.coordinates[i] * v2.coordinates[i];
		}
		return acc;
	}
	else throw "GVector::operator*: vector lengths not equal.";
}

template<class T>
GVector<T> operator*(const GVector<T>& v, double a)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator*(const GVector<T>& v, float a)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator*(const GVector<T>& v, int a)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator*(double a, const GVector<T>& v)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator*(float a, const GVector<T>& v)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator*(int a, const GVector<T>& v)
{
	GVector<T> result(v);
	result.Scale(a);
	return result;
}

template<class T>
GVector<T> operator+(const GVector<T>& v1, const GVector<T>& v2)
{
	if (v1.size == v2.size)
	{
		GVector<T> result(v1);
		result.Add(v2);
		return result;
	}
	else throw "GVector::operator+: vector lengths not equal.";
}

template<class T>
GVector<T> operator+(const GVector<T>& v, double a)
{
	GVector<T> result(v);
	result.Add(a);
	return result;
}

template<class T>
GVector<T> operator+(const GVector<T>& v, float a)
{
	GVector<T> result(v);
	result.Add(a);
	return result;
}

template<class T>
GVector<T> operator+(const GVector<T>& v, int a)
{
	GVector<T> result(v);
	result.Add(a);
	return result;
}

template<class T>
GVector<T> operator-(const GVector<T>& v1, const GVector<T>& v2)
{
	if (v1.size == v2.size)
	{
		GVector<T> result(v1);
		result.Sub(v2);
		return result;
	}
	else throw "GVector::operator-: vector lengths not equal.";
}

template<class T>
GVector<T> operator-(const GVector<T>& v, double a)
{
	GVector<T> result(v);
	result.Sub(a);
	return result;
}

template<class T>
GVector<T> operator-(const GVector<T>& v, float a)
{
	GVector<T> result(v);
	result.Sub(a);
	return result;
}

template<class T>
GVector<T> operator-(const GVector<T>& v, int a)
{
	GVector<T> result(v);
	result.Sub(a);
	return result;
}

