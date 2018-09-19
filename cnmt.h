#ifndef HACBREWPACK_CNMT_H
#define HACBREWPACK_CNMT_H

#include <stdint.h>
#include "settings.h"

#pragma pack(push, 1)
typedef struct {
    uint64_t title_id;
    uint32_t title_version;
    uint8_t type;
    uint8_t _0xD;
    uint16_t content_entry_offset;
    uint16_t content_entry_count;
    uint16_t meta_entry_count;
    uint8_t _0x14[0xC];
    uint64_t patch_id;
    uint64_t min_version;
} cnmt_header_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    unsigned char hash[0x20];
    unsigned char ncaid[0x10];
    uint8_t size[0x06];
    uint8_t type;
    uint8_t _0x37;
} cnmt_content_record_t;
#pragma pack(pop)

typedef struct {
    cnmt_header_t cnmt_header;
    cnmt_content_record_t cnmt_content_records[3];
} cnmt_ctx_t;

void cnmt_create(cnmt_ctx_t *cnmt_ctx, filepath_t *cnmt_filepath);

#endif