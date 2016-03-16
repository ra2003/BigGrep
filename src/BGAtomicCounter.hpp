//
// BGAtomicCounter.hpp
//
// Definition of the BGAtomicCounter class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// This version of this class was modified by Carnegie Mellon University to
// use a 64-bit unsigned innteger in place of the original signed int type.

class BGAtomicCounter

	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	/// Typical usage of BGAtomicCounter is for implementing
	/// reference counting and similar things.
	///
	///   - GCC 4.1+ (Intel platforms only)
{
public:
	//typedef int ValueType; /// The underlying integer type.
	typedef uint64_t ValueType; /// The underlying integer type.
	
	BGAtomicCounter();
		/// Creates a new BGAtomicCounter and initializes it to zero.
		
	explicit BGAtomicCounter(ValueType initialValue);
		/// Creates a new BGAtomicCounter and initializes it with
		/// the given value.
	
	BGAtomicCounter(const BGAtomicCounter& counter);
		/// Creates the counter by copying another one.
	
	~BGAtomicCounter();
		/// Destroys the BGAtomicCounter.

	BGAtomicCounter& operator = (const BGAtomicCounter& counter);
		/// Assigns the value of another BGAtomicCounter.
		
	BGAtomicCounter& operator = (ValueType value);
		/// Assigns a value to the counter.

	operator ValueType () const;
		/// Returns the value of the counter.
		
	ValueType value() const;
		/// Returns the value of the counter.
		
	ValueType operator ++ (); // prefix
		/// Increments the counter and returns the result.
		
	ValueType operator ++ (int); // postfix
		/// Increments the counter and returns the previous value.
		
	ValueType operator -- (); // prefix
		/// Decrements the counter and returns the result.
		
	ValueType operator -- (int); // postfix
		/// Decrements the counter and returns the previous value.
		
	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

private:
	typedef uint64_t ImplType;
	//typedef int ImplType;
	ImplType _counter;
};

//
// GCC 4.1+ atomic builtins.
//
inline BGAtomicCounter::operator BGAtomicCounter::ValueType () const
{
	return _counter;
}

	
inline BGAtomicCounter::ValueType BGAtomicCounter::value() const
{
	return _counter;
}


inline BGAtomicCounter::ValueType BGAtomicCounter::operator ++ () // prefix
{
	return __sync_add_and_fetch(&_counter, 1);
}

	
inline BGAtomicCounter::ValueType BGAtomicCounter::operator ++ (int) // postfix
{
	return __sync_fetch_and_add(&_counter, 1);
}


inline BGAtomicCounter::ValueType BGAtomicCounter::operator -- () // prefix
{
	return __sync_sub_and_fetch(&_counter, 1);
}

	
inline BGAtomicCounter::ValueType BGAtomicCounter::operator -- (int) // postfix
{
	return __sync_fetch_and_sub(&_counter, 1);
}

	
inline bool BGAtomicCounter::operator ! () const
{
	return _counter == 0;
}

//
// GCC 4.1+ atomic builtins.
//
BGAtomicCounter::BGAtomicCounter():
	_counter(0)
{
}

	
BGAtomicCounter::BGAtomicCounter(BGAtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}


BGAtomicCounter::BGAtomicCounter(const BGAtomicCounter& counter):
	_counter(counter.value())
{
}


BGAtomicCounter::~BGAtomicCounter()
{
}


BGAtomicCounter& BGAtomicCounter::operator = (const BGAtomicCounter& counter)
{
	__sync_lock_test_and_set(&_counter, counter.value());
	return *this;
}

	
BGAtomicCounter& BGAtomicCounter::operator = (BGAtomicCounter::ValueType value)
{
	__sync_lock_test_and_set(&_counter, value);
	return *this;
}


