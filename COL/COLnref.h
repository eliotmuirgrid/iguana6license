#ifndef __COL_NREF_H__
#define __COL_NREF_H__
//-----------------------------------------------------------------------------
// Copyright (C) 1997-2013 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module:  COLnref
// Author:  Andrew Vajoczki
// Date:    July 10, 2013
//
// Description:
//
// A non-intrusive reference counted pointer class.
//
// Similar to COLref, except that COLref can only manage classes derived 
// from COLrefCounted (intrusive). COLnref has no such restriction and may be
// used to manage any class type. So while COLnref is more flexible than COLref,
// the downside is that a heap allocated ref count instance must be created for
// each object managed by COLnref. 
//
// COLnref is suitable for use with containers like COLvector, and is a safer
// substitute for COLauto in many cases.
//
// A minor optimization was made to not allocate the ref counting impl until 
// the COLnref instance is copied.
//
// Note:
//
// * Do not use COLnref for classes already derived from COLrefCounted - use
//   COLref in such cases as it will be more efficient.
//
// * Circular references are NOT detected by COLnref (or by COLref for that
//   matter).
//
// * COLnref is thread instance safe. Different threads may not access the
//   COLnref instance concurrently.
//-----------------------------------------------------------------------------

#include "COLerror.h"

template <typename T> class COLnref
{
private:
   class COLnrefImp
   {
   public:
      COLnrefImp(T* obj) {
         object_ = obj;
         refcount_ = 1;
      }
      ~COLnrefImp() {
         decRef();
      }
      int refcount() const {
         return refcount_;
      }
      T* sharedInstance() {
         return object_;
      }
      void incRef() {
         ++refcount_;
      }
      // decrement ref count and delete object if necessary
      void decRef() {
         if (refcount_ > 0) {
            --refcount_;
            if (refcount_ == 0) {
               delete object_; object_ = 0;
            }
         }
      }
      // Only able to release object if COLnref count is 1.
      // Returns 0 if cannot release or no object.
      T* release() {
         if (refcount_ <= 1) {
            T* t = object_;
            object_ = 0;
            refcount_ = 0;
            return t;
         }
         return 0;
      }
   private:
      COLnrefImp();
      COLnrefImp(const COLnrefImp& that);
      COLnrefImp& operator=(const COLnrefImp& that);
      T*  object_;
      int refcount_;
   };

public:
   COLnref() {
      isLocal_ = true;
      u_.localObject_ = 0;
   }
   COLnref(T* obj) {
      isLocal_ = true;
      u_.localObject_ = obj;
   }
   COLnref(const COLnref& that) {
      copy_(that);
   }
   COLnref& operator=(const COLnref& that) {
      if (this != &that && this->u_.sharedImp_ != that.u_.sharedImp_) {
         deref_();
         copy_(that);
      }
      return *this;
   }
   COLnref& operator=(T* obj) {
      deref_();
      // don't delete u_.sharedImp_ as it may be used by other COLnref(s)
      isLocal_ = true;
      u_.localObject_ = obj;
      return *this;
   }
   ~COLnref() {
      deref_();
   }
   const T* get() const {
      return isLocal_ ? u_.localObject_ : (u_.sharedImp_ ? u_.sharedImp_->sharedInstance() : 0);
   }
   T* get() {
      return isLocal_ ? u_.localObject_ : (u_.sharedImp_ ? u_.sharedImp_->sharedInstance() : 0);
   }
   T* operator->() {
      T* t = get();
      COLASSERT(t);
      return t;
   }
   T& operator*() {
      T* t = get();
      COLASSERT(t);
      return *t;
   }
   const T* operator->() const {
      const T* t = get();
      COLASSERT(t);
      return t;
   }
   const T& operator*() const {
      const T* t = get();
      COLASSERT(t);
      return *t;
   }
   operator const T*() const {
      return get();
   }
   operator T*() {
      return get();
   }

   // Release an object from ref pointer class if possible.
   // Only able to release object if ref count is 1.
   // Returns 0 if cannot release or no object.
   // Note: this has nothing to do with the similarly named COLrefCounted::Release() method.
   T* release() {
      if (isLocal_) {
         T* t = u_.localObject_;
         u_.localObject_ = 0;
         return t;
      }
      else if (u_.sharedImp_) {
         if (T* t = u_.sharedImp_->release()) {
            COLASSERT(u_.sharedImp_->refcount()==0);
            delete u_.sharedImp_; u_.sharedImp_ = 0;
            u_.localObject_ = 0;
            isLocal_ = true;
            return t;
         }
      }
      return 0;
   }
   // the ref count - very useful for debugging
   int refcount() const {
      if (isLocal_) {
         return u_.localObject_ ? 1 : 0;
      }
      return u_.sharedImp_ ? u_.sharedImp_->refcount() : 0;
   }

private:
   void copy_(const COLnref& that) {
      if (that.isLocal_) {
         COLnref& That = (COLnref&)that;
         That.u_.sharedImp_ = new COLnrefImp(That.u_.localObject_);
         That.isLocal_ = false;
      }
      isLocal_ = false;
      u_.sharedImp_ = that.u_.sharedImp_;
      COLASSERT(u_.sharedImp_);
      u_.sharedImp_->incRef();
      COLASSERT(u_.sharedImp_->refcount() >= 2);
   }
   // decrement the reference count and delete the object and imp if necessary
   void deref_() {
      if (isLocal_) {
         delete u_.localObject_; u_.localObject_ = 0;
      }
      else if (u_.sharedImp_) {
         u_.sharedImp_->decRef();
         if (u_.sharedImp_->refcount() == 0) {
            delete u_.sharedImp_; u_.sharedImp_ = 0;
            u_.localObject_ = 0;
            isLocal_ = true;
         }
      }
   }

private:
   bool isLocal_;
   union {
      T*          localObject_; // used when isLocal_ is true
      COLnrefImp* sharedImp_;   // used when isLocal_ is false
   } u_;
};

#endif

