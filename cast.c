#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>

static void *
DebugReadFile(LPCWSTR Name, uint32_t *BytesRead)
{
 void *Ret = 0;
 HANDLE FileHandle = CreateFileW(Name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
 if (FileHandle)
 {
  LARGE_INTEGER FileSize;
  if (GetFileSizeEx(FileHandle, &FileSize))
  {
   assert(FileSize.QuadPart < 0xffffffff);
   void *FileMemory = VirtualAlloc(0, FileSize.QuadPart, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
   if (FileMemory)
   {
    if (ReadFile(FileHandle, FileMemory, (uint32_t)FileSize.QuadPart, BytesRead, 0) && *BytesRead == (uint32_t)FileSize.QuadPart)
    {
     Ret = FileMemory;
    }
    else
    {
     VirtualFree(FileMemory, 0, MEM_RELEASE);
     FileMemory = 0;
     *BytesRead = 0;
    }
   }
  }
  CloseHandle(FileHandle);
 }

 return Ret;
}

int
wmain()
{
 uint32_t WriteLn = 0;
 char *File = DebugReadFile(L".\\sample.c", &WriteLn);
 if (!File)
 {
  return 1;
 }

 // todo lexer, arena impl

 return 0;
}