#ifndef __FIL_BINARY_FILE_H__
#define __FIL_BINARY_FILE_H__
//---------------------------------------------------------------------------
// Copyright (C) 1997-2006 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: FILbinaryFile
//
// Description:
//
// This object represents a binary file.  There are a couple of
// implementations under the hood.  One is using the unbuffered
// IO - open et al. which may be optimal for many applications.
// The operating system does sector buffering with that type of IO.
//
// The other implementation is using fopen etc. which is buffered
// IO.  
//
// The implementation that you use depends on the second argument
// to the constructor - which is defaulted to the buffered IO (for
// now at least)
//
// One reason I have named this FILbinaryFile is to avoid one of
// the classic mistakes with fopen which is to forget to put in the "b"
// to indicate that you wish to open the file in binary mode.  Without
// that flag the OS will helpfully translate newlines for you which
// results in some perplexing hard to find bugs.
//
// For this reason we will have FILtextFile in the FIL library too.
//
// Notice that FILbinaryFile implements COLreadable, COLsourceBinary
// and COLsink - this means that that it integrates very nicely with
// our whole sink-source framework ideas.  The base interfaces for these
// are defined in the COL library which you can consult for documentation.
//
// The destructor of FILbinaryFile will automatically close the file
// it may have open.
//
// FILbinaryFile is a lot cleaner to use than using the fopen etc.
// calls directly since it gives a simpler interface, better error checking
// and hides some of nasty operating system implementation dependent
// details underneath.
//
// Author: Eliot Muir
// Date:   Fri 01/16/2004 
//
//
//---------------------------------------------------------------------------

#include <COL/COLreadable.h>
#include <COL/COLsink.h>


class FILbinaryFilePrivate;
class COLstring;

typedef COLuint64 FILindex;  // allow large files.

class COL_DLL FILbinaryFile 
: public COLsink,
  public COLreadable
{
public:
      
   enum FILtype
   {
      Buffered,
      Unbuffered
   };
   
   enum FILmode
   {
      // open for reading only
      Read,
      // open for reading and writing
      Write,      
      // open and seek to the end
      Append,      
      // open, truncate any existing file
      Rewrite  
   };
   /**
    * The last parameter is useful if this file object is being 
    * used as a COLsource:
    *
    * a. The pDestination parameter is used to specify the sink to write to.
    *
    * It need not be set in the constructor can be set later on
    * explicitly sing the setNext() method on the COLsource interface.
    */
   FILbinaryFile(const COLstring& FileName, FILmode Mode, FILtype Type=Buffered);
   FILbinaryFile(FILtype Type=Buffered);
   virtual ~FILbinaryFile();

   virtual void open(const COLstring& FileName, FILmode Mode);
   virtual void close();
   
   bool isOpen() const;

   virtual COLindex position() const;
   virtual void setPosition(COLindex NewPosition);

   // a pair of methods for positioning in files
   // with 64 bit addressing
   COLuint64 position64() const;
   void setPosition64(COLuint64 NewPosition);

   virtual COLuint32 read(void* pBuffer, COLuint32 SizeOfBuffer);

   // COLsink interface method used to write the passed in data
   // to the sink object
   virtual COLuint32 write(const void* pBuffer, COLuint32 SizeOfBuffer);

   // Note that there is also a FILfileSize function in FILutils.h.  That
   // works fine so long as the operating system does not have the file open.
   // However if you have the file open then only this method is guaranteed
   // to give the correct size of the file.
   FILindex size() const;

   const COLstring& name() const;

   virtual void flush();

   /**
    * Resets the position of the source (written out in writeChunk)
    * to be the beginning of the source.
    * 
    * @See COLsource::resetSourcePosition().
    */
   void resetSourcePosition();

   /**
    * Sets the position of the source to be the given offset.
    *
    * @See COLsource::setSourcePosition().
    */
   void setSourcePosition(COLuint32 ByteOffset);

   /**
    * Read all elements from the file into the destination sink.
    */
   void readAll(COLsink* pDest);

   /***************************************************************
    * COLreadable interface methods below...
    ***************************************************************/
   
   /**
    * Returns the current position within the readable.  This is
    * set by setReadPosition() and incremented by read().
    *
    * This implementation returns same value as position()
    */
   virtual COLindex readPosition() const;

   /**
    * Sets the position within the source which will be the 
    * location of the next read.
    *
    * This implementation simply calls setPosition()
    */
   virtual void setReadPosition(COLindex Position);

   /**
    * Returns true if there is nothing left to read (ie. the
    * position is now at the end of the readable object).
    */
   virtual bool isEndOfReadable() const;

#ifdef WIN32
   //
   // The WIN32 HANDLE representing the file. 0 if the file has not been opened
   //
   void* win32Handle();
#endif

protected:
   COLuint32 sourceEndPosition() const;
   void setName(const COLstring& NewName);
private:
   FILbinaryFilePrivate* pMember;

   FILbinaryFile(const FILbinaryFile& Orig); // not allowed
   FILbinaryFile& operator=(const FILbinaryFile& Orig); // not allowed
};

#endif // end of defensive include
