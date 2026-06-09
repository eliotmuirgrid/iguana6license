#ifndef __COL_FUNCTOR_H__
#define __COL_FUNCTOR_H__
//-----------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  COLfunctor
//
// Author:  Andrew Vajoczki
//
// Date:    5 Nov 2013
//
// Description:
//
// This is handy but COLclosure does the same thing so it should be deprecated.
//
// Function objects for generic callbacks.
//
// One potential way to use these is create a pointer like this:
//  COLfunctor1<void, FOObar*>* pFunctor = COLnewFunctor(this, &FOOclass::onFooBar);
//  (*pFunctor)(pFooBar);
// Not sure if it's the best way... but it does work :-) (Eliot)
//-----------------------------------------------------------------------------

struct COLfunctor{
    COLfunctor() { }
    virtual ~COLfunctor() {}
    virtual COLfunctor* clone() const = 0;
};

template <typename R>
struct COLfunctor0 : public COLfunctor{
    typedef R ReturnType;
    virtual R operator()() = 0;
    virtual COLfunctor0* clone() const = 0;
};

template <typename R, typename A1>
struct COLfunctor1 : public COLfunctor{
    typedef R ReturnType;
    typedef A1 Arg1;
    virtual R operator()(A1) = 0;
    virtual COLfunctor1* clone() const = 0;
};

template <typename R, typename A1, typename A2>
struct COLfunctor2 : public COLfunctor{
    typedef R ReturnType;
    typedef A1 Arg1;
    typedef A2 Arg2;
    virtual R operator()(A1,A2) = 0;
    virtual COLfunctor2* clone() const = 0;
};

template <typename R, typename A1, typename A2, typename A3>
struct COLfunctor3 : public COLfunctor{
    typedef R ReturnType;
    typedef A1 Arg1;
    typedef A2 Arg2;
    typedef A3 Arg3;
    virtual R operator()(A1,A2,A3) = 0;
    virtual COLfunctor3* clone() const = 0;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
struct COLfunctor4 : public COLfunctor{
    typedef R ReturnType;
    typedef A1 Arg1;
    typedef A2 Arg2;
    typedef A3 Arg3;
    typedef A4 Arg4;
    virtual R operator()(A1,A2,A3,A4) = 0;
    virtual COLfunctor4* clone() const = 0;
};

template <typename R,
typename A1, typename A2, typename A3, typename A4, typename A5>
struct COLfunctor5 : public COLfunctor{
    typedef R ReturnType;
    typedef A1 Arg1;
    typedef A2 Arg2;
    typedef A3 Arg3;
    typedef A4 Arg4;
    typedef A5 Arg5;
    virtual R operator()(A1,A2,A3,A4,A5) = 0;
    virtual COLfunctor5* clone() const = 0;
};

template <typename R>
class COLfunctionFunctor0 : public COLfunctor0<R>{
public:
    typedef R (*Function)();
    COLfunctionFunctor0(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor0(const COLfunctionFunctor0& that)
    : COLfunctor0<R>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor0& operator=(const COLfunctionFunctor0& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()() {
        return (*function_)();
    }
    COLfunctionFunctor0* clone() const {
        return new COLfunctionFunctor0(*this);
    }
private:
    Function function_;
};

template <typename R, typename A1>
class COLfunctionFunctor1 : public COLfunctor1<R,A1>{
public:
    typedef R (*Function)(A1 a1);
    COLfunctionFunctor1(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor1(const COLfunctionFunctor1& that)
    : COLfunctor1<R,A1>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor1& operator=(const COLfunctionFunctor1& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1) {
        return (*function_)(a1);
    }
    COLfunctionFunctor1* clone() const {
        return new COLfunctionFunctor1(*this);
    }
private:
    Function function_;
};

template <typename R, typename A1, typename A2>
class COLfunctionFunctor2 : public COLfunctor2<R,A1,A2>{
public:
    typedef R (*Function)(A1 a1, A2 a2);
    COLfunctionFunctor2(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor2(const COLfunctionFunctor2& that)
    : COLfunctor2<R,A1,A2>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor2& operator=(const COLfunctionFunctor2& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2) {
        return (*function_)(a1, a2);
    }
    COLfunctionFunctor2* clone() const {
        return new COLfunctionFunctor2(*this);
    }
private:
    Function function_;
};

template <typename R, typename A1, typename A2, typename A3>
class COLfunctionFunctor3 : public COLfunctor3<R,A1,A2,A3>{
public:
    typedef R (*Function)(A1 a1, A2 a2, A3 a3);
    COLfunctionFunctor3(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor3(const COLfunctionFunctor3& that)
    : COLfunctor3<R,A1,A2,A3>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor3& operator=(const COLfunctionFunctor3& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3) {
        return (*function_)(a1, a2, a3);
    }
    COLfunctionFunctor3* clone() const {
        return new COLfunctionFunctor3(*this);
    }
private:
    Function function_;
};

template <typename R, typename A1, typename A2, typename A3, typename A4>
class COLfunctionFunctor4 : public COLfunctor4<R,A1,A2,A3,A4>{
public:
    typedef R (*Function)(A1 a1, A2 a2, A3 a3, A4 a4);
    COLfunctionFunctor4(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor4(const COLfunctionFunctor4& that)
    : COLfunctor4<R,A1,A2,A3,A4>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor4& operator=(const COLfunctionFunctor4& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3, A4 a4) {
        return (*function_)(a1, a2, a3, a4);
    }
    COLfunctionFunctor4* clone() const {
        return new COLfunctionFunctor4(*this);
    }
private:
    Function function_;
};

template
<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class COLfunctionFunctor5 : public COLfunctor5<R,A1,A2,A3,A4,A5>{
public:
    typedef R (*Function)(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);
    COLfunctionFunctor5(Function fn) {
        //COLASSERT(fn != 0);
        function_ = fn;
    }
    COLfunctionFunctor5(const COLfunctionFunctor5& that)
    : COLfunctor5<R,A1,A2,A3,A4,A5>(that) {
        function_ = that.function_;
    }
    COLfunctionFunctor5& operator=(const COLfunctionFunctor5& that) {
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        return (*function_)(a1, a2, a3, a4, a5);
    }
    COLfunctionFunctor5* clone() const {
        return new COLfunctionFunctor5(*this);
    }
private:
    Function function_;
};

template <typename R, class T>
class COLmethodFunctor0 : public COLfunctor0<R>{
public:
    typedef T Class;
    typedef R (T::*Function)();
    COLmethodFunctor0(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)() const;
    COLmethodFunctor0(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor0(const COLmethodFunctor0& that)
    : COLfunctor0<R>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor0& operator=(const COLmethodFunctor0& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()() {
        return (instance_->*function_)();
    }
    COLmethodFunctor0* clone() const {
        return new COLmethodFunctor0(*this);
    }
private:
    T*       instance_;
    Function function_;
};

template <typename R, class T, typename A1>
class COLmethodFunctor1 : public COLfunctor1<R,A1>{
public:
    typedef T Class;
    typedef R (T::*Function)(A1);
    COLmethodFunctor1(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)(A1) const;
    COLmethodFunctor1(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor1(const COLmethodFunctor1& that)
    : COLfunctor1<R,A1>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor1& operator=(const COLmethodFunctor1& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1) {
        return (instance_->*function_)(a1);
    }
    COLmethodFunctor1* clone() const {
        return new COLmethodFunctor1(*this);
    }
private:
    T*       instance_;
    Function function_;
};

template <typename R, class T, typename A1, typename A2>
class COLmethodFunctor2 : public COLfunctor2<R,A1,A2>{
public:
    typedef T Class;
    typedef R (T::*Function)(A1, A2);
    COLmethodFunctor2(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)(A1, A2) const;
    COLmethodFunctor2(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor2(const COLmethodFunctor2& that)
    : COLfunctor2<R,A1,A2>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor2& operator=(const COLmethodFunctor2& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2) {
        return (instance_->*function_)(a1, a2);
    }
    COLmethodFunctor2* clone() const {
        return new COLmethodFunctor2(*this);
    }
private:
    T*       instance_;
    Function function_;
};

template <typename R, class T, typename A1, typename A2, typename A3>
class COLmethodFunctor3 : public COLfunctor3<R,A1,A2,A3>{
public:
    typedef T Class;
    typedef R (T::*Function)(A1, A2, A3);
    COLmethodFunctor3(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)(A1, A2, A3) const;
    COLmethodFunctor3(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor3(const COLmethodFunctor3& that)
    : COLfunctor3<R,A1,A2,A3>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor3& operator=(const COLmethodFunctor3& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3) {
        return (instance_->*function_)(a1, a2, a3);
    }
    COLmethodFunctor3* clone() const {
        return new COLmethodFunctor3(*this);
    }
private:
    T*       instance_;
    Function function_;
};

template
<typename R, class T, typename A1, typename A2, typename A3, typename A4>
class COLmethodFunctor4 : public COLfunctor4<R,A1,A2,A3,A4>{
public:
    typedef T Class;
    typedef R (T::*Function)(A1, A2, A3, A4);
    COLmethodFunctor4(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)(A1, A2, A3, A4) const;
    COLmethodFunctor4(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor4(const COLmethodFunctor4& that)
    : COLfunctor4<R,A1,A2,A3,A4>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor4& operator=(const COLmethodFunctor4& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3, A4 a4) {
        return (instance_->*function_)(a1, a2, a3, a4);
    }
    COLmethodFunctor4* clone() const {
        return new COLmethodFunctor4(*this);
    }
private:
    T*       instance_;
    Function function_;
};

template
<typename R, class T,
typename A1, typename A2, typename A3, typename A4, typename A5>
class COLmethodFunctor5 : public COLfunctor5<R,A1,A2,A3,A4,A5>{
public:
    typedef T Class;
    typedef R (T::*Function)(A1, A2, A3, A4, A5);
    COLmethodFunctor5(T* instance, Function fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = fn;
    }
    typedef R (T::*ConstFunction)(A1, A2, A3, A4, A5) const;
    COLmethodFunctor5(T* instance, ConstFunction fn) {
        //COLASSERT(instance);
        //COLASSERT(fn != 0);
        instance_ = instance;
        function_ = (Function)fn;
    }
    COLmethodFunctor5(const COLmethodFunctor5& that)
    : COLfunctor5<R,A1,A2,A3,A4,A5>(that) {
        instance_ = that.instance_;
        function_ = that.function_;
    }
    COLmethodFunctor5& operator=(const COLmethodFunctor5& that) {
        instance_ = that.instance_;
        function_ = that.function_;
        return *this;
    }
    R operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        return (instance_->*function_)(a1, a2, a3, a4, a5);
    }
    COLmethodFunctor5* clone() const {
        return new COLmethodFunctor5(*this);
    }
private:
    T*       instance_;
    Function function_;
};

// Convenience functions to create non-heap functors of up to 5 arguments.

//
// functions
//

template <typename R>
inline COLfunctionFunctor0<R>
COLmakeFunctor(R (*fn)()){
    return COLfunctionFunctor0<R>(fn);
}

template <typename R, typename A1>
inline COLfunctionFunctor1<R, A1>
COLmakeFunctor(R (*fn)(A1)){
    return COLfunctionFunctor1<R,A1>(fn);
}

template <typename R, typename A1, typename A2>
inline COLfunctionFunctor2<R,A1,A2>
COLmakeFunctor(R (*fn)(A1,A2)){
    return COLfunctionFunctor2<R,A1,A2>(fn);
}

template <typename R, typename A1, typename A2, typename A3>
inline COLfunctionFunctor3<R,A1,A2,A3>
COLmakeFunctor(R (*fn)(A1,A2,A3)){
    return COLfunctionFunctor3<R,A1,A2,A3>(fn);
}

template <typename R, typename A1, typename A2, typename A3, typename A4>
inline COLfunctionFunctor4<R,A1,A2,A3,A4>
COLmakeFunctor(R (*fn)(A1,A2,A3,A4)){
    return COLfunctionFunctor4<R,A1,A2,A3,A4>(fn);
}

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLfunctionFunctor5<R,A1,A2,A3,A4,A5>
COLmakeFunctor(R (*fn)(A1,A2,A3,A4,A5)){
    return COLfunctionFunctor5<R,A1,A2,A3,A4,A5>(fn);
}

//
// non-const methods
//

template <typename R, class T>
inline COLmethodFunctor0<R, T>
COLmakeFunctor(T* instance, R (T::*fn)()){
    return COLmethodFunctor0<R,T>(instance, fn);
}

template <typename R, class T, typename A1>
inline COLmethodFunctor1<R,T,A1>
COLmakeFunctor(T* instance, R (T::*fn)(A1)){
    return COLmethodFunctor1<R,T,A1>(instance, fn);
}

template <typename R, class T, typename A1, typename A2>
inline COLmethodFunctor2<R,T,A1,A2>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2)){
    return COLmethodFunctor2<R,T,A1,A2>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3>
inline COLmethodFunctor3<R,T,A1,A2,A3>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3)){
    return COLmethodFunctor3<R,T,A1,A2,A3>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4>
inline COLmethodFunctor4<R,T,A1,A2,A3,A4>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4)){
    return COLmethodFunctor4<R,T,A1,A2,A3,A4>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4,A5)){
    return COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>(instance, fn);
}

//
// const methods
//

template <typename R, class T>
inline COLmethodFunctor0<R, T>
COLmakeFunctor(T* instance, R (T::*fn)() const){
    return COLmethodFunctor0<R,T>(instance, fn);
}

template <typename R, class T, typename A1>
inline COLmethodFunctor1<R,T,A1>
COLmakeFunctor(T* instance, R (T::*fn)(A1) const){
    return COLmethodFunctor1<R,T,A1>(instance, fn);
}

template <typename R, class T, typename A1, typename A2>
inline COLmethodFunctor2<R,T,A1,A2>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2) const){
    return COLmethodFunctor2<R,T,A1,A2>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3>
inline COLmethodFunctor3<R,T,A1,A2,A3>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3) const){
    return COLmethodFunctor3<R,T,A1,A2,A3>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4>
inline COLmethodFunctor4<R,T,A1,A2,A3,A4>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4) const){
    return COLmethodFunctor4<R,T,A1,A2,A3,A4>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>
COLmakeFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4,A5) const){
    return COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>(instance, fn);
}

//
// Convenience functions to create heap allocated functors of up to 5 arguments.
//

//
// functions
//

template <typename R>
inline COLfunctionFunctor0<R>*
COLnewFunctor(R (*fn)()){
    return new COLfunctionFunctor0<R>(fn);
}

template <typename R, typename A1>
inline COLfunctionFunctor1<R, A1>*
COLnewFunctor(R (*fn)(A1)){
    return new COLfunctionFunctor1<R,A1>(fn);
}

template <typename R, typename A1, typename A2>
inline COLfunctionFunctor2<R,A1,A2>*
COLnewFunctor(R (*fn)(A1,A2)){
    return new COLfunctionFunctor2<R,A1,A2>(fn);
}

template <typename R, typename A1, typename A2, typename A3>
inline COLfunctionFunctor3<R,A1,A2,A3>*
COLnewFunctor(R (*fn)(A1,A2,A3)){
    return new COLfunctionFunctor3<R,A1,A2,A3>(fn);
}

template <typename R, typename A1, typename A2, typename A3, typename A4>
inline COLfunctionFunctor4<R,A1,A2,A3,A4>*
COLnewFunctor(R (*fn)(A1,A2,A3,A4)){
    return new COLfunctionFunctor4<R,A1,A2,A3,A4>(fn);
}

template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLfunctionFunctor5<R,A1,A2,A3,A4,A5>*
COLnewFunctor(R (*fn)(A1,A2,A3,A4,A5)){
    return new COLfunctionFunctor5<R,A1,A2,A3,A4,A5>(fn);
}

//
// non-const methods
//

template <typename R, class T>
inline COLmethodFunctor0<R, T>*
COLnewFunctor(T* instance, R (T::*fn)()){
    return new COLmethodFunctor0<R,T>(instance, fn);
}

template <typename R, class T, typename A1>
inline COLmethodFunctor1<R,T,A1>*
COLnewFunctor(T* instance, R (T::*fn)(A1)){
    return new COLmethodFunctor1<R,T,A1>(instance, fn);
}

template <typename R, class T, typename A1, typename A2>
inline COLmethodFunctor2<R,T,A1,A2>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2)){
    return new COLmethodFunctor2<R,T,A1,A2>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3>
inline COLmethodFunctor3<R,T,A1,A2,A3>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3)){
    return new COLmethodFunctor3<R,T,A1,A2,A3>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4>
inline COLmethodFunctor4<R,T,A1,A2,A3,A4>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4)){
    return new COLmethodFunctor4<R,T,A1,A2,A3,A4>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4,A5)){
    return new COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>(instance, fn);
}

//
// const methods
//

template <typename R, class T>
inline COLmethodFunctor0<R, T>*
COLnewFunctor(T* instance, R (T::*fn)() const){
    return new COLmethodFunctor0<R,T>(instance, fn);
}

template <typename R, class T, typename A1>
inline COLmethodFunctor1<R,T,A1>*
COLnewFunctor(T* instance, R (T::*fn)(A1) const){
    return new COLmethodFunctor1<R,T,A1>(instance, fn);
}

template <typename R, class T, typename A1, typename A2>
inline COLmethodFunctor2<R,T,A1,A2>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2) const){
    return new COLmethodFunctor2<R,T,A1,A2>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3>
inline COLmethodFunctor3<R,T,A1,A2,A3>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3) const){
    return new COLmethodFunctor3<R,T,A1,A2,A3>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4>
inline COLmethodFunctor4<R,T,A1,A2,A3,A4>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4) const){
    return new COLmethodFunctor4<R,T,A1,A2,A3,A4>(instance, fn);
}

template <typename R, class T, typename A1, typename A2, typename A3, typename A4, typename A5>
inline COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>*
COLnewFunctor(T* instance, R (T::*fn)(A1,A2,A3,A4,A5) const){
    return new COLmethodFunctor5<R,T,A1,A2,A3,A4,A5>(instance, fn);
}

#endif // end of defensive include

