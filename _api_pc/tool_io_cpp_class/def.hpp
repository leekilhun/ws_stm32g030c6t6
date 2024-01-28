#include "mcc_library/mcc_types.hpp"
#include "mcc_library/mcc_conversion.hpp"
#include "mcc_library/mcc_utils.hpp"
#include <bitset>


#define MaKEWORD(a, b)      ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xff))) << 8))
#define MaKEDWORD(a, b)     ((uint32_t)(((uint16_t)(((uint32_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint32_t)(b)) & 0xffff))) << 16))
#define LoWORD(l)           ((uint16_t)(((uint32_t)(l)) & 0xffff))
#define HiWORD(l)           ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))
#define LoBYTE(w)           ((uint8_t)(((uint16_t)(w)) & 0xff))
#define HiBYTE(w)           ((uint8_t)((((uint16_t)(w)) >> 8) & 0xff))