#ifndef __COL_PLACE_ITERATOR_H__
#define __COL_PLACE_ITERATOR_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLplaceIterator
//
// Description:
// 
// LEGACY - Ignore.
//
// COLplaceIterator class. Provides iteration interface for 
//
// COLhashmap
// COLmap
// COLlist
//
// Typical usage:
//
// tyepdef COLmap<COLuint32,COLstring> TList; //or COLhashmap or COLlist
// TList MyList; 
// typedef TList::TIterator TIter;
// for( TIter Iter(MyVect); Iter.next();)
// {
//    Iter.key();   //associative collections only
//    Iter.value();
// }
//
// Author: Nasron Cheong
// Date:   Monday, September 18th, 2006 @ 10:59:52 AM
//
//---------------------------------------------------------------------------

#include "COLminimumInclude.h"

class COLplaceIteratorPrivate;

template<class TCollection>
class COL_DLL COLplaceIteratorConst
{
public:
   COLplaceIteratorConst(const TCollection& Collection) 
      : pCollection(&Collection)
   {
      COLPRECONDITION(pCollection)
      reset();
   }
   ~COLplaceIteratorConst()
   {
   }

   bool next()
   {
      if (validIndex() || Position == -1)
      {
         Position++;
         if (Position == 0)
         {
            Place = pCollection->first();
         }
         else
         {
            Place = pCollection->next(Place);
         }
      }
      return validIndex();
   }

   bool last()
   {
      if (pCollection->size())
      {
         Position = pCollection->size() - 1;
         Place = pCollection->last();
      }
      else
      {
         Position = 0;
         Place = 0;
      }
      return validIndex();
   }

   bool previous()
   {
      if (validIndex())
      {
         Position--;
         Place = pCollection->previous(Place);
      }
      else if (Position == (COLint32)pCollection->size())
      {
         if (pCollection->size() == 0)
         {
            return false;
         }
         return last();
      }
      return validIndex();
   }
   
   void reset()
   {
      Position = -1;
      Place = 0;
   }

   const TCollection& collection() const
   {
      return *pCollection;
   }

   typename TCollection::TPlace place() const
   {
      return Place;
   }
   
   const typename TCollection::TValue& value() const
   {
      return (*pCollection)[Place];
   }

   bool operator > (const COLplaceIteratorConst& Rhs) const
   {
      return Position > Rhs.Position;
   }

   bool operator >= (const COLplaceIteratorConst& Rhs) const
   {
      return Position >= Rhs.Position;
   }

   bool operator < (const COLplaceIteratorConst& Rhs) const
   {
      return Position < Rhs.Position;
   }

   bool operator <= (const COLplaceIteratorConst& Rhs) const
   {
      return Position <= Rhs.Position;
   }

   bool operator == (const COLplaceIteratorConst& Rhs) const
   {
      return Position == Rhs.Position;
   }

private:
   bool validIndex() const
   {
      return Position < (COLint32)pCollection->size() && 
             Position >= 0;
   }
   
   COLint32 Position;
   typename TCollection::TPlace Place;
   const TCollection* pCollection;
};

template<class TCollection>
class COL_DLL COLplaceIterator
{
public:
   COLplaceIterator(TCollection& Collection) 
      : pCollection(&Collection),
        ConstIterator(Collection)
   {
      reset();
   }
   ~COLplaceIterator()
   {
   }

   bool next()
   {
      return ConstIterator.next();
   }

   bool last()
   {
      return ConstIterator.last();
   }

   bool previous()
   {
      return ConstIterator.previous();
   }

   void reset()
   {
      ConstIterator.reset();
   }

   TCollection& collection() const
   {
      return pCollection;
   }

   typename TCollection::TPlace place() const
   {
      return ConstIterator.place();
   }
   
   typename TCollection::TValue& value() const
   {
      return (*pCollection)[place()];
   }

   bool operator > (const COLplaceIterator& Rhs) const
   {
      return ConstIterator > Rhs.ConstIterator;
   }

   bool operator >= (const COLplaceIterator& Rhs) const
   {
      return ConstIterator >= Rhs.ConstIterator;
   }

   bool operator < (const COLplaceIterator& Rhs) const
   {
      return ConstIterator < Rhs.ConstIterator;
   }

   bool operator <= (const COLplaceIterator& Rhs) const
   {
      return ConstIterator <= Rhs.ConstIterator;
   }

   bool operator == (const COLplaceIterator& Rhs) const
   {
      return ConstIterator == Rhs.ConstIterator;
   }

private:
   COLplaceIteratorConst<TCollection> ConstIterator;
   TCollection* pCollection;
};

template<class TCollection>
class COL_DLL COLplaceIteratorAssociativeConst
{
public:
   COLplaceIteratorAssociativeConst(const TCollection& Collection) 
      : ConstIterator(Collection)
   {
      reset();
   }
   ~COLplaceIteratorAssociativeConst()
   {
   }

   bool next()
   {
      return ConstIterator.next();
   }

   bool last()
   {
      return ConstIterator.last();
   }

   bool previous()
   {
      return ConstIterator.previous();
   }

   void reset()
   {
      ConstIterator.reset();
   }
 
   const TCollection& collection() const
   {
      return ConstIterator.collection();
   }

   typename TCollection::TPlace place() const
   {
      return ConstIterator.place();
   }

   const typename TCollection::TKey& key() const
   {
      return ConstIterator.collection().key(ConstIterator.place());
   }

   const typename TCollection::TValue& value() const
   {
      return ConstIterator.value();
   }

   bool operator > (const COLplaceIteratorAssociativeConst& Rhs) const
   {
      return ConstIterator > Rhs.ConstIterator;
   }

   bool operator >= (const COLplaceIteratorAssociativeConst& Rhs) const
   {
      return ConstIterator >= Rhs.ConstIterator;
   }

   bool operator < (const COLplaceIteratorAssociativeConst& Rhs) const
   {
      return ConstIterator < Rhs.ConstIterator;
   }

   bool operator <= (const COLplaceIteratorAssociativeConst& Rhs) const
   {
      return ConstIterator <= Rhs.ConstIterator;
   }

   bool operator == (const COLplaceIteratorAssociativeConst& Rhs) const
   {
      return ConstIterator == Rhs.ConstIterator;
   }

private:
   COLplaceIteratorConst<TCollection> ConstIterator;
};

template<class TCollection>
class COL_DLL COLplaceIteratorAssociative
{
public:
   COLplaceIteratorAssociative(TCollection& Collection) 
      : pCollection(&Collection),
        ConstIterator(Collection)
   {
      COLPRECONDITION(pCollection)
      reset();
   }
   ~COLplaceIteratorAssociative()
   {
   }

   bool next()
   {
      return ConstIterator.next();
   }

   bool last()
   {
      return ConstIterator.last();
   }

   bool previous()
   {
      return ConstIterator.previous();
   }

   void reset()
   {
      ConstIterator.reset();
   }
 
   TCollection& collection() const
   {
      return *pCollection;
   }

   typename TCollection::TPlace place() const
   {
      return ConstIterator.place();
   }

   const typename TCollection::TKey& key() const
   {
      return ConstIterator.key();
   }

   typename TCollection::TValue& value() const
   {
      return (*pCollection)[ConstIterator.place()];
   }

   bool operator > (const COLplaceIteratorAssociative& Rhs) const
   {
      return ConstIterator > Rhs.ConstIterator;
   }

   bool operator >= (const COLplaceIteratorAssociative& Rhs) const
   {
      return ConstIterator >= Rhs.ConstIterator;
   }

   bool operator < (const COLplaceIteratorAssociative& Rhs) const
   {
      return ConstIterator < Rhs.ConstIterator;
   }

   bool operator <= (const COLplaceIteratorAssociative& Rhs) const
   {
      return ConstIterator <= Rhs.ConstIterator;
   }

   bool operator == (const COLplaceIteratorAssociative& Rhs) const
   {
      return ConstIterator == Rhs.ConstIterator;
   }

private:
   COLplaceIteratorAssociativeConst<TCollection> ConstIterator;
   TCollection* pCollection;
};

#endif // end of defensive include
