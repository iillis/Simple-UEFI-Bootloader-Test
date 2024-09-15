#ifndef _EFI_TYPES_H
#define _EFI_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef void        VOID;
typedef int         INT;
typedef uint8_t     BOOLEAN;
typedef int64_t     INTN;
typedef uint64_t    UINTN;
typedef int8_t      INT8;
typedef uint8_t     UINT8;
typedef int16_t     INT16;
typedef uint16_t    UINT16;
typedef int32_t     INT32;
typedef uint32_t    UINT32;
typedef int64_t     INT64;
typedef uint64_t    UINT64;
typedef INT64       INT128;
typedef UINT64      UINT128;
typedef char        CHAR8;
typedef wchar_t     CHAR16;
typedef VOID       *EFI_EVENT;
typedef VOID       *EFI_HANDLE;
typedef uint64_t    EFI_LBA;
typedef uint64_t    EFI_PHYSICAL_ADDRESS;
typedef uint64_t    EFI_STATUS;
typedef uint64_t    EFI_TL;
typedef uint64_t    EFI_TPL;
typedef uint64_t    EFI_VIRTUAL_ADDRESS;

#ifndef NULL
    #define NULL 0
#endif

#define IN 
#define OUT 
#define OPTIONAL 
#define CONST 

#define EFIAPI 

#define EFI_SUCCESS 0
#define EFI_LOAD_ERROR 1
#define EFI_INVALID_PARAMETER 2
#define EFI_UNSUPPORTED 3
#define EFI_BAD_BUFFER_SIZE 4
#define EFI_BUFFER_TOO_SMALL 5
#define EFI_NOT_READY 6
#define EFI_DEVICE_ERROR 7
#define EFI_WRITE_PROTECTED 8
#define EFI_OUT_OF_RESOURCES 9
#define EFI_VOLUME_CORRUPTED 10
#define EFI_VOLUME_FULL 11
#define EFI_NO_MEDIA 12
#define EFI_MEDIA_CHANGED 13
#define EFI_NOT_FOUND 14
#define EFI_ACCESS_DENIED 15
#define EFI_NO_RESPONSE 16
#define EFI_NO_MAPPING 17
#define EFI_TIMEOUT 18
#define EFI_NOT_STARTED 19
#define EFI_ALREADY_STARTED 20
#define EFI_ABORTED 21
#define EFI_ICMP_ERROR 22
#define EFI_TFTP_ERROR 23
#define EFI_PROTOCOL_ERROR 24
#define EFI_INCOMPATIBLE_VERSION 25
#define EFI_SECURITY_VIOLATION 26
#define EFI_CRC_ERROR 27
#define EFI_END_OF_MEDIA 28
#define EFI_END_OF_FILE 31
#define EFI_INVALID_LANGUAGE 32
#define EFI_COMPROMISED_DATA 33
#define EFI_IP_ADDRESS_CONFLICT 34
#define EFI_HTTP_ERROR 35
#define EFI_WARN_UNKNOWN_GLYPH 1
#define EFI_WARN_DELETE_FAILURE 2
#define EFI_WARN_WRITE_FAILURE 3
#define EFI_WARN_BUFFER_TOO_SMALL 4
#define EFI_WARN_STALE_DATA 5
#define EFI_WARN_FILE_SYSTEM 6
#define EFI_WARN_RESET_REQUIRED 7

typedef struct {
    UINT64 Signature;  
    UINT32 Revision;   
    UINT32 HeaderSize; 
    UINT32 CRC32;      
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

#endif