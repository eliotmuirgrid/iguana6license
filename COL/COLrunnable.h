#ifndef __COL_RUNNABLE_H__
#define __COL_RUNNABLE_H__
//-----------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  COLrunnable
//
// Author:  Andrew Vajoczki
//
// Date:    5 Nov 2013
//
// Description:
//
// This is good - but COLclosure0 does the same thing so we really should deprecate it.
//
//   A deferred task suitable for use with queues.
//   Arguments are copied and stored in the instance - similar to a closure.
//-----------------------------------------------------------------------------

class COLrunnable
{
public:
   COLrunnable() {}
   virtual ~COLrunnable() {}
   virtual void run() = 0;
   virtual COLrunnable* clone() const = 0;
   virtual void runAndDelete() { run(); delete this; }
private:
   COLrunnable(const COLrunnable&);
   COLrunnable& operator=(const COLrunnable&);
};

class COLrunnableFunction0 : public COLrunnable
{
public:
   typedef void (*Function)();
   COLrunnableFunction0() {
      function_ = 0;
   }
   COLrunnableFunction0(Function fn) {
      function_ = fn;
   }
   COLrunnableFunction0(const COLrunnableFunction0& that) {
      function_ = that.function_;
   }
   COLrunnableFunction0& operator=(const COLrunnableFunction0& that) {
      if (this != &that) {
         function_ = that.function_;
      }
      return *this;
   }
   ~COLrunnableFunction0() {
   }
   void run() {
      if (function_) {
         (*function_)();
      }
   }
   COLrunnableFunction0* clone() const {
      return new COLrunnableFunction0(*this);
   }
private:
   Function function_;
};

template <typename A1> class COLrunnableFunction1 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1);
   COLrunnableFunction1() {
      function_ = 0;
   }
   COLrunnableFunction1(Function fn, A1 a1)
   : a1_(a1)
   {
      function_ = fn;
   }
   COLrunnableFunction1(const COLrunnableFunction1& that)
   : a1_(that.a1_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction1& operator=(const COLrunnableFunction1& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
      }
      return *this;
   }
   ~COLrunnableFunction1() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_);
      }
   }
   COLrunnableFunction1* clone() const {
      return new COLrunnableFunction1(*this);
   }
private:
   Function function_;
   A1       a1_;
};

template <typename A1, typename A2> class COLrunnableFunction2 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1, A2 a2);
   COLrunnableFunction2() {
      function_ = 0;
   }
   COLrunnableFunction2(Function fn, A1 a1, A2 a2)
   : a1_(a1)
   , a2_(a2)
   {
      function_ = fn;
   }
   COLrunnableFunction2(const COLrunnableFunction2& that)
   : a1_(that.a1_)
   , a2_(that.a2_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction2& operator=(const COLrunnableFunction2& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
      }
      return *this;
   }
   ~COLrunnableFunction2() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_, a2_);
      }
   }
   COLrunnableFunction2* clone() const {
      return new COLrunnableFunction2(*this);
   }
private:
   Function function_;
   A1       a1_;
   A2       a2_;
};

template <typename A1, typename A2, typename A3>
class COLrunnableFunction3 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1, A2 a2, A3 a3);
   COLrunnableFunction3() {
      function_ = 0;
   }
   COLrunnableFunction3(Function fn, A1 a1, A2 a2, A3 a3)
   : a1_(a1)
   , a2_(a2)
   , a3_(a3)
   {
      function_ = fn;
   }
   COLrunnableFunction3(const COLrunnableFunction3& that)
   : a1_(that.a1_)
   , a2_(that.a2_)
   , a3_(that.a3_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction3& operator=(const COLrunnableFunction3& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
         a3_       = that.a3_;
      }
      return *this;
   }
   ~COLrunnableFunction3() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_, a2_, a3_);
      }
   }
   COLrunnableFunction3* clone() const {
      return new COLrunnableFunction3(*this);
   }
private:
   Function function_;
   A1       a1_;
   A2       a2_;
   A3       a3_;
};

template <typename A1, typename A2, typename A3, typename A4>
class COLrunnableFunction4 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1, A2 a2, A3 a3, A4 a4);
   COLrunnableFunction4() {
      function_ = 0;
   }
   COLrunnableFunction4(Function fn, A1 a1, A2 a2, A3 a3, A4 a4)
      : a1_(a1)
      , a2_(a2)
      , a3_(a3)
      , a4_(a4)
   {
      function_ = fn;
   }
   COLrunnableFunction4(const COLrunnableFunction4& that)
      : a1_(that.a1_)
      , a2_(that.a2_)
      , a3_(that.a3_)
      , a4_(that.a4_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction4& operator=(const COLrunnableFunction4& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
         a3_       = that.a3_;
         a4_       = that.a4_;
      }
      return *this;
   }
   ~COLrunnableFunction4() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_, a2_, a3_, a4_);
      }
   }
   COLrunnableFunction4* clone() const {
      return new COLrunnableFunction4(*this);
   }
private:
   Function function_;
   A1       a1_;
   A2       a2_;
   A3       a3_;
   A4       a4_;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5>
class COLrunnableFunction5 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);
   COLrunnableFunction5() {
      function_ = 0;
   }
   COLrunnableFunction5(Function fn, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
      : a1_(a1)
      , a2_(a2)
      , a3_(a3)
      , a4_(a4)
      , a5_(a5)
   {
      function_ = fn;
   }
   COLrunnableFunction5(const COLrunnableFunction5& that)
      : a1_(that.a1_)
      , a2_(that.a2_)
      , a3_(that.a3_)
      , a4_(that.a4_)
      , a5_(that.a5_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction5& operator=(const COLrunnableFunction5& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
         a3_       = that.a3_;
         a4_       = that.a4_;
         a5_       = that.a5_;
      }
      return *this;
   }
   ~COLrunnableFunction5() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_, a2_, a3_, a4_, a5_);
      }
   }
   COLrunnableFunction5* clone() const {
      return new COLrunnableFunction5(*this);
   }
private:
   Function function_;
   A1       a1_;
   A2       a2_;
   A3       a3_;
   A4       a4_;
   A5       a5_;
};

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class COLrunnableFunction6 : public COLrunnable
{
public:
   typedef void (*Function)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);
   COLrunnableFunction6() {
      function_ = 0;
   }
   COLrunnableFunction6(Function fn, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
      : a1_(a1)
      , a2_(a2)
      , a3_(a3)
      , a4_(a4)
      , a5_(a5)
      , a6_(a6)
   {
      function_ = fn;
   }
   COLrunnableFunction6(const COLrunnableFunction6& that)
      : a1_(that.a1_)
      , a2_(that.a2_)
      , a3_(that.a3_)
      , a4_(that.a4_)
      , a5_(that.a5_)
      , a6_(that.a6_)
   {
      function_ = that.function_;
   }
   COLrunnableFunction6& operator=(const COLrunnableFunction6& that) {
      if (this != &that) {
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
         a3_       = that.a3_;
         a4_       = that.a4_;
         a5_       = that.a5_;
         a6_       = that.a6_;
      }
      return *this;
   }
   ~COLrunnableFunction6() {
   }
   void run() {
      if (function_) {
         (*function_)(a1_, a2_, a3_, a4_, a5_, a6_);
      }
   }
   COLrunnableFunction6* clone() const {
      return new COLrunnableFunction6(*this);
   }
private:
   Function function_;
   A1       a1_;
   A2       a2_;
   A3       a3_;
   A4       a4_;
   A5       a5_;
   A6       a6_;
};

template <class T> class COLrunnableMethod0 : public COLrunnable
{
public:
   typedef void (T::*Function)();
   COLrunnableMethod0() {
      instance_ = 0;
      function_ = 0;
   }
   COLrunnableMethod0(T* instance, Function fn) {
      instance_ = instance;
      function_ = fn;
   }
   COLrunnableMethod0(const COLrunnableMethod0& that) {
      instance_ = that.instance_;
      function_ = that.function_;
   }
   COLrunnableMethod0& operator=(const COLrunnableMethod0& that) {
      if (this != &that) {
         instance_ = that.instance_;
         function_ = that.function_;
      }
      return *this;
   }
   ~COLrunnableMethod0() {
   }
   void run() {
      if (instance_ && function_) {
         (instance_->*function_)();
      }
   }
   COLrunnableMethod0* clone() const {
      return new COLrunnableMethod0(*this);
   }
private:
   T*       instance_;
   Function function_;
};

template <class T, typename A1> class COLrunnableMethod1 : public COLrunnable
{
public:
   typedef void (T::*Function)(A1);
   COLrunnableMethod1() {
      instance_ = 0;
      function_ = 0;
   }
   COLrunnableMethod1(T* instance, Function fn, A1 a1)
   : a1_(a1)
   {
      instance_ = instance;
      function_ = fn;
   }
   COLrunnableMethod1(const COLrunnableMethod1& that)
   : a1_(that.a1_)
   {
      instance_ = that.instance_;
      function_ = that.function_;
   }
   COLrunnableMethod1& operator=(const COLrunnableMethod1& that) {
      if (this != &that) {
         instance_ = that.instance_;
         function_ = that.function_;
         a1_       = that.a1_;
      }
      return *this;
   }
   ~COLrunnableMethod1() {
   }
   void run() {
      if (instance_ && function_) {
         (instance_->*function_)(a1_);
      }
   }
   COLrunnableMethod1* clone() const {
      return new COLrunnableMethod1(*this);
   }
private:
   T*       instance_;
   Function function_;
   A1       a1_;
};

template <class T, typename A1, typename A2>
class COLrunnableMethod2 : public COLrunnable
{
public:
   typedef void (T::*Function)(A1, A2);
   COLrunnableMethod2() {
      instance_ = 0;
      function_ = 0;
   }
   COLrunnableMethod2(T* instance, Function fn, A1 a1, A2 a2)
   : a1_(a1)
   , a2_(a2)
   {
      instance_ = instance;
      function_ = fn;
   }
   COLrunnableMethod2(const COLrunnableMethod2& that)
   : a1_(that.a1_)
   , a2_(that.a2_)
   {
      instance_ = that.instance_;
      function_ = that.function_;
   }
   COLrunnableMethod2& operator=(const COLrunnableMethod2& that) {
      if (this != &that) {
         instance_ = that.instance_;
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
      }
      return *this;
   }
   ~COLrunnableMethod2() {
   }
   void run() {
      if (instance_ && function_) {
         (instance_->*function_)(a1_, a2_);
      }
   }
   COLrunnableMethod2* clone() const {
      return new COLrunnableMethod2(*this);
   }
private:
   T*       instance_;
   Function function_;
   A1       a1_;
   A2       a2_;
};

template <class T, typename A1, typename A2, typename A3>
class COLrunnableMethod3 : public COLrunnable
{
public:
   typedef void (T::*Function)(A1, A2, A3);
   COLrunnableMethod3() {
      instance_ = 0;
      function_ = 0;
   }
   COLrunnableMethod3(T* instance, Function fn, A1 a1, A2 a2, A3 a3)
   : a1_(a1)
   , a2_(a2)
   , a3_(a3)
   {
      instance_ = instance;
      function_ = fn;
   }
   COLrunnableMethod3(const COLrunnableMethod3& that)
   : a1_(that.a1_)
   , a2_(that.a2_)
   , a3_(that.a3_)
   {
      instance_ = that.instance_;
      function_ = that.function_;
   }
   COLrunnableMethod3& operator=(const COLrunnableMethod3& that) {
      if (this != &that) {
         instance_ = that.instance_;
         function_ = that.function_;
         a1_       = that.a1_;
         a2_       = that.a2_;
         a3_       = that.a3_;
      }
      return *this;
   }
   ~COLrunnableMethod3() {
   }
   void run() {
      if (instance_ && function_) {
         (instance_->*function_)(a1_, a2_, a3_);
      }
   }
   COLrunnableMethod3* clone() const {
      return new COLrunnableMethod3(*this);
   }
private:
   T*       instance_;
   Function function_;
   A1       a1_;
   A2       a2_;
   A3       a3_;
};

//
// Convenience functions to make non-heap runnables of up to 4 args.
//

//
// functions
//

inline COLrunnableFunction0
COLmakeRunnable(void (*fn)())
{
   return COLrunnableFunction0(fn);
}

template <typename A1>
inline COLrunnableFunction1<A1>
COLmakeRunnable(void (*fn)(A1), A1 a1)
{
   return COLrunnableFunction1<A1>(fn, a1);
}

template <typename A1, typename A2>
inline COLrunnableFunction2<A1,A2>
COLmakeRunnable(void (*fn)(A1,A2), A1 a1, A2 a2)
{
   return COLrunnableFunction2<A1,A2>(fn, a1, a2);
}

template <typename A1, typename A2, typename A3>
inline COLrunnableFunction3<A1,A2,A3>
COLmakeRunnable(void (*fn)(A1,A2,A3), A1 a1, A2 a2, A3 a3)
{
   return COLrunnableFunction3<A1,A2,A3>(fn, a1, a2, a3);
}

template <typename A1, typename A2, typename A3, typename A4>
inline COLrunnableFunction4<A1,A2,A3,A4>
COLmakeRunnable(void (*fn)(A1,A2,A3,A4), A1 a1, A2 a2, A3 a3, A4 a4)
{
   return COLrunnableFunction4<A1,A2,A3,A4>(fn, a1, a2, a3, a4);
}

//
// non-const methods
//

template <class T>
inline COLrunnableMethod0<T>
COLmakeRunnable(T* instance, void (T::*fn)())
{
   return COLrunnableMethod0<T>(instance, fn);
}

template <class T, typename A1>
inline COLrunnableMethod1<T,A1>
COLmakeRunnable(T* instance, void (T::*fn)(A1), A1 a1)
{
   return COLrunnableMethod1<T,A1>(instance, fn, a1);
}

template <class T, typename A1, typename A2>
inline COLrunnableMethod2<T,A1,A2>
COLmakeRunnable(T* instance, void (T::*fn)(A1,A2), A1 a1, A2 a2)
{
   return COLrunnableMethod2<T,A1,A2>(instance, fn, a1, a2);
}

template <class T, typename A1, typename A2, typename A3>
inline COLrunnableMethod3<T,A1,A2,A3>
COLmakeRunnable(T* instance, void (T::*fn)(A1,A2,A3), A1 a1, A2 a2, A3 a3)
{
   return COLrunnableMethod3<T,A1,A2,A3>(instance, fn, a1, a2, a3);
}

//
// const methods
//

template <class T>
inline COLrunnableMethod0<T>
COLmakeRunnable(T* instance, void (T::*fn)() const)
{
   return COLrunnableMethod0<T>(instance, (void(T::*)())fn);
}

template <class T, typename A1>
inline COLrunnableMethod1<T,A1>
COLmakeRunnable(T* instance, void (T::*fn)(A1) const, A1 a1)
{
   return COLrunnableMethod1<T,A1>(instance, (void(T::*)(A1))fn, a1);
}

template <class T, typename A1, typename A2>
inline COLrunnableMethod2<T,A1,A2>
COLmakeRunnable(T* instance, void (T::*fn)(A1,A2) const, A1 a1, A2 a2)
{
   return COLrunnableMethod2<T,A1,A2>(instance, (void(T::*)(A1,A2))fn, a1, a2);
}

template <class T, typename A1, typename A2, typename A3>
inline COLrunnableMethod3<T,A1,A2,A3>
COLmakeRunnable(T* instance, void (T::*fn)(A1,A2,A3) const, A1 a1, A2 a2, A3 a3)
{
   return COLrunnableMethod3<T,A1,A2,A3>(instance, (void(T::*)(A1,A2,A3))fn, a1,a2,a3);
}

//
// Convenience functions to make heap allocated runnables of up to 4 args.
//

//
// functions
//

inline COLrunnableFunction0*
COLnewRunnable(void (*fn)())
{
   return new COLrunnableFunction0(fn);
}

template <typename A1>
inline COLrunnableFunction1<A1>*
COLnewRunnable(void (*fn)(A1), A1 a1)
{
   return new COLrunnableFunction1<A1>(fn, a1);
}

template <typename A1, typename A2>
inline COLrunnableFunction2<A1,A2>*
COLnewRunnable(void (*fn)(A1,A2), A1 a1, A2 a2)
{
   return new COLrunnableFunction2<A1,A2>(fn, a1, a2);
}

template <typename A1, typename A2, typename A3>
inline COLrunnableFunction3<A1,A2,A3>*
COLnewRunnable(void (*fn)(A1,A2,A3), A1 a1, A2 a2, A3 a3)
{
   return new COLrunnableFunction3<A1,A2,A3>(fn, a1, a2, a3);
}

template <typename A1, typename A2, typename A3, typename A4>
inline COLrunnableFunction4<A1,A2,A3,A4>*
COLnewRunnable(void (*fn)(A1,A2,A3,A4), A1 a1, A2 a2, A3 a3, A4 a4)
{
   return new COLrunnableFunction4<A1,A2,A3,A4>(fn, a1, a2, a3, a4);
}

template <typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLrunnableFunction5<A1,A2,A3,A4,A5>*
COLnewRunnable(void (*fn)(A1,A2,A3,A4,A5), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
   return new COLrunnableFunction5<A1,A2,A3,A4,A5>(fn, a1, a2, a3, a4, a5);
}

template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
inline COLrunnableFunction6<A1,A2,A3,A4,A5,A6>*
COLnewRunnable(void (*fn)(A1,A2,A3,A4,A5,A6), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
   return new COLrunnableFunction6<A1,A2,A3,A4,A5,A6>(fn, a1, a2, a3, a4, a5, a6);
}

//
// non-const methods
//

template <class T>
inline COLrunnableMethod0<T>*
COLnewRunnable(T* instance, void (T::*fn)())
{
   return new COLrunnableMethod0<T>(instance, fn);
}

template <class T, typename A1>
inline COLrunnableMethod1<T,A1>*
COLnewRunnable(T* instance, void (T::*fn)(A1), A1 a1)
{
   return new COLrunnableMethod1<T,A1>(instance, fn, a1);
}

template <class T, typename A1, typename A2>
inline COLrunnableMethod2<T,A1,A2>*
COLnewRunnable(T* instance, void (T::*fn)(A1,A2), A1 a1, A2 a2)
{
   return new COLrunnableMethod2<T,A1,A2>(instance, fn, a1, a2);
}

template <class T, typename A1, typename A2, typename A3>
inline COLrunnableMethod3<T,A1,A2,A3>*
COLnewRunnable(T* instance, void (T::*fn)(A1,A2,A3), A1 a1, A2 a2, A3 a3)
{
   return new COLrunnableMethod3<T,A1,A2,A3>(instance, fn, a1, a2, a3);
}

//
// const methods
//

template <class T>
inline COLrunnableMethod0<T>*
COLnewRunnable(T* instance, void (T::*fn)() const)
{
   return new COLrunnableMethod0<T>(instance, (void(T::*)())fn);
}

template <class T, typename A1>
inline COLrunnableMethod1<T,A1>*
COLnewRunnable(T* instance, void (T::*fn)(A1) const, A1 a1)
{
   return new COLrunnableMethod1<T,A1>(instance, (void(T::*)(A1))fn, a1);
}

template <class T, typename A1, typename A2>
inline COLrunnableMethod2<T,A1,A2>*
COLnewRunnable(T* instance, void (T::*fn)(A1,A2) const, A1 a1, A2 a2)
{
   return new COLrunnableMethod2<T,A1,A2>(instance, (void(T::*)(A1,A2))fn, a1, a2);
}

template <class T, typename A1, typename A2, typename A3>
inline COLrunnableMethod3<T,A1,A2,A3>*
COLnewRunnable(T* instance, void (T::*fn)(A1,A2,A3) const, A1 a1, A2 a2, A3 a3)
{
   return new COLrunnableMethod3<T,A1,A2,A3>(instance, (void(T::*)(A1,A2,A3))fn, a1,a2,a3);
}

#endif

