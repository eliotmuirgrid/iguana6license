//---------------------------------------------------------------------------
// Copyright (C) 1997-2009 iNTERFACEWARE Inc.  All Rights Reserved
//
// Module: COLutils
//
// Description:
//
// Implementation
//
// Author: Eliot Muir
// Date:   Wednesday, February 11th, 2009 @ 09:49:46 AM
//
//---------------------------------------------------------------------------
#include "COLprecomp.h"
#pragma hdrstop

#include "COLutils.h"
#include <stdlib.h>
#include <ctype.h>

#ifndef WIN32
#   ifndef CHM_HPUX
#      include <sys/select.h>
#   endif
#   include <sys/time.h>
#   include <unistd.h>
#   include <sched.h>
#endif

void COLsleep( COLuint32 TimeOutInMilliSeconds )
{
#ifdef WIN32
   // no return code from Sleep
   ::Sleep( TimeOutInMilliSeconds );
#else
   // The POSIX implementation does not have a hard guarantee on sleep duration.
   // It may be interrupted and return early from time to time.

   double Seconds = TimeOutInMilliSeconds * 0.001;
   if (Seconds < 0.0)
   {
      // Do not return immediately - sleeping for 0 seconds is legimate to
      // yield the thread.
      Seconds = 0.0;
   }

   struct timeval tv;
   tv.tv_sec = (COLuint64)Seconds;
   tv.tv_usec = (COLuint64)((Seconds - tv.tv_sec) * 1000000);

   // no need to check return code and errno, as we don't care
   // and will not throw an exception.
   ::select(0, NULL, NULL, NULL, &tv);
#endif
}

static int getVarNameLength(const char* Position)
{
   int VarLength = 0;
   while (Position[0] && Position[0] != '}')
   {
      ++VarLength;
      ++Position;
   }
   return Position[0] == 0 ? -1 :  VarLength;
}

COLstring COLexpandEnvironmentVariables(const COLstring& Original)
{
   COLstring Expanded;
   Expanded.setCapacity(Original.size());
   if (const char* Position = Original.c_str())
   {
      while (char Ch = Position[0])
      {
         if (Ch == '$')
         {
            int VarNameLength = 0;
            if (Position[1] == '{' && ((VarNameLength = getVarNameLength(Position+2)) >= 0)) //}
            {
               COLstring VarName = COLstring(Position+2, VarNameLength);
               if (const char* VarValue = ::getenv(VarName.c_str()))
               {
                  Expanded += VarValue;
               }
               Position += 2 + VarNameLength + 1; // skip over ${VAR}
               continue;
            }
         }
         Expanded += Ch;
         ++Position;
      }
   }
   return Expanded;
}


// pattern is text, except that it contains question marks that match any character.
static bool subpatternMatches(const char* Pattern, const char* Module, int Length)
{
   for (; Length && *Pattern && *Module; ++Pattern, ++Module, --Length)
   {
      if (*Pattern != *Module && *Pattern != '?')
      {
         return false;
      }
   }
   return Length == 0;
}

bool COLglobMatch(const char* Pattern, const char* ModuleToMatch)
{
   bool MatchCandidate = false;

   // the glob list loop
   while (*Pattern)
   {
      // skip whitespace before glob
      while (COLisspace(*Pattern)) { ++Pattern; }
      if (*Pattern == 0)
      {
         break;
      }

      // Pattern pointer now points to the beginning of a glob pattern.
      // A glob ends when first whitespace character or NIL is reached.

      bool IsPositiveMatchMode = true;
      if (*Pattern == '-')
      {
          // globs with a leading '-' are negative patterns
          IsPositiveMatchMode = false;
          ++Pattern; // skip the leading '-'
      }

      const char* Module = ModuleToMatch;
      bool SkipRestOfGlob = false;

      // the glob loop
      while (*Pattern && !COLisspace(*Pattern))
      {
         if (*Pattern == '*')
         {
            // have glob advance past all contiguous '*' characters.
            while (*Pattern == '*') { ++Pattern; }

            // *Pattern is now after the * and can be a letter, whitespace or NIL.
            if (*Pattern == 0 || COLisspace(*Pattern))
            {
               // end of glob reached - a successful match.
               Module = ""; // point to any NIL - faster than advancing Module pointer.
               break;
            }

            // Handle characters immediately after the '*' in the glob.
            const char* Target = Pattern;
            int TargetLength = 0;
            char Ch = 0;
            while ((Ch = Target[TargetLength]) && !COLisspace(Ch) && Ch != '*')
            {
               TargetLength++;
            }
            Pattern += TargetLength;
            if (TargetLength)
            {
               // search for Target string in Module
               bool IsTargetAtEnd = (Ch == 0 || COLisspace(Ch));
               bool TargetFound = false;
               while (*Module)
               {
                  if (subpatternMatches(Target, Module, TargetLength)
                     && (!IsTargetAtEnd || (IsTargetAtEnd && Module[TargetLength] == 0)))
                  {
                     // Target pattern found in Module
                     TargetFound = true;
                     Module += TargetLength;
                     break;
                  }
                  ++Module;
               }
               if (!TargetFound)
               {
                  // failed to find the target string after the *
                  SkipRestOfGlob = true;
                  break;
               }
               else if (*Pattern == 0 || COLisspace(*Pattern))
               {
                  // at end of glob, so break out of the glob loop
                  break;
               }
               // still must process rest of this glob
               continue;
            }
         }

         if (*Pattern == *Module || *Pattern == '?')
         {
            // glob character matches, so advance
            ++Pattern;
            ++Module;
            if (*Module == 0)
            {
               // We have reached the end of the Module string,
               // but current glob may not yet be finished.
               // The only valid remaining pattern at this point is "*".
               while (*Pattern == '*') { ++Pattern; }
               if (*Pattern && !COLisspace(*Pattern))
               {
                  // failed since there is still more of the glob to process
                  SkipRestOfGlob = true;
               }
               // end of Module - must break out of glob loop unconditionally
               break;
            }
         }
         else
         {
            // glob does not match or is longer than the module - no match.
            SkipRestOfGlob = true;
            break;
         }
      }

      if (SkipRestOfGlob)
      {
         while (*Pattern && !COLisspace(*Pattern)) { ++Pattern; }
      }
      else if (0 == *Module)
      {
         // reached end of glob pattern and end of Module - successful match
         MatchCandidate = IsPositiveMatchMode;
      }
   }
   return MatchCandidate;
}

