#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cnmt.h"
#include "filepath.h"
#include "utils.h"

void cnmt_create(cnmt_ctx_t *cnmt_ctx, filepath_t *cnmt_filepath)
{
    // Common values
    cnmt_ctx->cnmt_header.type = 0x80; // Application
    cnmt_ctx->cnmt_header.patch_id = cnmt_ctx->cnmt_header.title_id + 0x800;
    cnmt_ctx->cnmt_header.content_entry_offset = 0x10;
    cnmt_ctx->cnmt_header.content_entry_count = 0x2;

    // Set content record types
    cnmt_ctx->cnmt_content_records[0].type = 0x1;   // Program
    cnmt_ctx->cnmt_content_records[1].type = 0x3;   // Control

    printf("Writing metadata header\n");
    FILE *cnmt_file;
    cnmt_file = os_fopen(cnmt_filepath->os_path, OS_MODE_WRITE);

    if (cnmt_file != NULL)
        fwrite(&cnmt_ctx->cnmt_header, 1, sizeof(cnmt_header_t), cnmt_file);
    else
    {
        fprintf(stderr, "Failed to create %s!\n", cnmt_filepath->char_path);
        exit(EXIT_FAILURE);
    }

    // Write content records
    uint8_t digest[0x20] = {0};
    printf("Writing content records\n");
    fwrite(&cnmt_ctx->cnmt_content_records[0], sizeof(cnmt_content_record_t), 1, cnmt_file);
    fwrite(&cnmt_ctx->cnmt_content_records[1], sizeof(cnmt_content_record_t), 1, cnmt_file);
    fwrite(digest, 1, 0x20, cnmt_file);

    fclose(cnmt_file);
}