#include <stdlib.h>
#include <stdio.h>
#include "nacp.h"
#include "filepath.h"

void nacp_process(hbp_settings_t *settings, cnmt_ctx_t *cnmt_ctx)
{
    filepath_t nacp_filepath;
    filepath_init(&nacp_filepath);
    filepath_copy(&nacp_filepath, &settings->control_romfs_dir);
    filepath_append(&nacp_filepath, "control.nacp");

    printf("Getting TitleID\n");
    // Read TitleID
    FILE *fl;
    fl = os_fopen(nacp_filepath.os_path, OS_MODE_EDIT);
    if (fl == NULL)
    {
        fprintf(stderr, "Failed to open %s!\n", nacp_filepath.char_path);
        exit(EXIT_FAILURE);
    }
    fseeko64(fl, 0x3038, SEEK_SET);
    if (fread(&cnmt_ctx->cnmt_header.title_id, 1, 0x8, fl) != 0x8)
    {
        fprintf(stderr, "Failed to read file %s\n", nacp_filepath.char_path);
        exit(EXIT_FAILURE);
    }
    // change logo handeling to 0
    printf("Changing logo handeling to auto\n");
    fseeko64(fl, 0x30F1, SEEK_SET);
    fputc(0, fl);

    fclose(fl);
}