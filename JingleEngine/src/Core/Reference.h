#pragma once

#include <string>
#include <sstream>

template <class T>
class Ref;

template<class T>
std::string PointerToString(T* val)
{
	std::stringstream ss;
	ss << static_cast<const void*>(val);
	return ss.str();
}

class BaseClass
{
public:
	virtual ~BaseClass()
	{
	}

	virtual std::string ToString()
	{
		return "Address=" + PointerToString(this);
	}

	std::string AsString()
	{
		return "[" + ToString() + "]";
	}

	template <typename T>
	T& Cast() 
	{
		return *dynamic_cast<T*>(this);
	}
};

class Countable : public BaseClass
{
private:
	mutable int m_RefCount = 0;

public:
	void RefInc() const
	{
		++m_RefCount;
	}

	void RefDec() const
	{
		--m_RefCount;

		if (GetRefCount() <= 0)
		{
			delete this;
		}
	}

	int GetRefCount() const
	{
		return m_RefCount;
	}
};

template <class T>
class Ref
{
private:
	T* m_Pointer;

public:
	Ref() : m_Pointer(nullptr)
	{
	}

	Ref(T* ptr) : m_Pointer(ptr)
	{
		if (m_Pointer != nullptr)
			m_Pointer->RefInc();
	}

	//Ref(const T* ptr) : m_Pointer(ptr)
	//{
	//	if (m_Pointer != nullptr)
	//		m_Pointer->RefInc();
	//}

	Ref(const Ref& other) : m_Pointer(other.m_Pointer)
	{
		RefInc();
	}

	~Ref()
	{
		RefDec();
	}

	void RefInc() const
	{
		if (m_Pointer == nullptr)
			return;

		m_Pointer->RefInc();
	}

	void RefDec()
	{
		if (m_Pointer == nullptr)
			return;

		m_Pointer->RefDec();
	}

	Ref& operator=(T* other)
	{
		if (other != nullptr)
			other->RefInc();
		if (m_Pointer)
			m_Pointer->RefDec();
		m_Pointer = other;
		return (*this);
	}

	Ref& operator=(const Ref& other)
	{
		return (*this) = other.m_Pointer;
	}

	inline T* Get() const
	{
		return m_Pointer;
	}

	T* operator->() const
	{
		return m_Pointer;
	}

	T& operator*() const
	{
		return *m_Pointer;
	}

	operator T* () const
	{
		return m_Pointer;
	}

	operator bool() const
	{
		return m_Pointer != nullptr;
	}

	bool IsNull()
	{
		return m_Pointer == nullptr;
	}

	std::string AsString()
	{
		if (m_Pointer == nullptr)
		{
			return "null";
		}

		return "{" + m_Pointer->AsString() + "}";
	}

	//bool operator==(const Ref& other)
	//{
	//	return m_Pointer == other.m_Pointer;
	//}

	//bool operator==(const T* other)
	//{
	//	return m_Pointer == other;
	//}

	template <typename T2>
	Ref<T2> Cast() const
	{
		return Ref<T2>(dynamic_cast<T2*>(m_Pointer));
	}

	template <typename T2>
	Ref<T2> As() const
	{
		return Ref<T2>(static_cast<T2*>(m_Pointer));
	}

	template <class T>
	friend bool operator==(const Ref<T>& a, const Ref<T>& b);

	template <class T>
	friend bool operator==(const Ref<T>& a, const T& b);
};

template <class T>
bool operator==(const Ref<T>& a, const Ref<T>& b)
{
	return a.m_Pointer == b.m_Pointer;
}

template <class T>
bool operator==(const Ref<T>& a, const T& b)
{
	return a.m_Pointer == b;
}
