#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nca.h"
#include "utils.h"
#include "settings.h"
#include "pki.h"
#include "extkeys.h"
#include "version.h"
#include "nacp.h"
#include "cnmt.h"
#include "pfs0.h"

/* 4NXCI by The-4n
   Based on hactool by SciresM
   */

// Print Usage
static void usage(void)
{
    fprintf(stderr,
            "Usage: %s [options...]\n\n"
            "Options:\n"
            "-k, --keyset             Set keyset filepath, default filepath is ." OS_PATH_SEPARATOR "keys.dat\n"
            "-h, --help               Display usage\n"
            "--tempdir                Set temp directory filepath, default filepath is ." OS_PATH_SEPARATOR "hacbrewpack_temp" OS_PATH_SEPARATOR "\n"
            "--ncadir                 Set output nca directory path, default path is ." OS_PATH_SEPARATOR "hacbrewpack_nca" OS_PATH_SEPARATOR "\n"
            "--nspdir                 Set output nsp directory path, default path is ." OS_PATH_SEPARATOR "hacbrewpack_nsp" OS_PATH_SEPARATOR "\n"
            "--exefsdir               Set program exefs directory path, default path is ." OS_PATH_SEPARATOR "exefs" OS_PATH_SEPARATOR "\n"
            "--romfsdir               Set program romfs directory path, default path is ." OS_PATH_SEPARATOR "romfs" OS_PATH_SEPARATOR "\n"
            "--logodir                Set program logo directory path, default path is ." OS_PATH_SEPARATOR "logo" OS_PATH_SEPARATOR "\n"
            "--controldir             Set control romfs directory path, default path is ." OS_PATH_SEPARATOR "control" OS_PATH_SEPARATOR "\n"
            "--noromfs                Skips creating program romfs section\n"
            "--nologo                Skips creating program logo section\n",
            USAGE_PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    hbp_settings_t settings;
    cnmt_ctx_t cnmt_ctx;
    memset(&settings, 0, sizeof(settings));
    memset(&cnmt_ctx, 0, sizeof(cnmt_ctx));

    printf("hacBrewPack %s by The-4n\n", HACBREWPACK_VERSION);

    // Hardcode temp directory and create it
    filepath_init(&settings.temp_dir);
    filepath_set(&settings.temp_dir, "hacbrewpack_temp");

    // Hardcode output directory and create it
    filepath_init(&settings.out_dir);
    filepath_set(&settings.out_dir, "hacbrewpack_nca");

    // Hardcode nsp  directory and create it
    filepath_init(&settings.nsp_dir);
    filepath_set(&settings.nsp_dir, "hacbrewpack_nsp");

    // Hardcode Program exeFS directory
    filepath_init(&settings.exefs_dir);
    filepath_set(&settings.exefs_dir, "exefs");

    // Hardcode Program RomFS directory
    filepath_init(&settings.romfs_dir);
    filepath_set(&settings.romfs_dir, "romfs");

    // Hardcode Program Logo directory
    filepath_init(&settings.logo_dir);
    filepath_set(&settings.logo_dir, "logo");

    // Hardcode Control RomFS directory
    filepath_init(&settings.control_romfs_dir);
    filepath_set(&settings.control_romfs_dir, "control");

    filepath_t keypath;
    filepath_init(&keypath);
    pki_initialize_keyset(&settings.keyset);
    // Default keyset filepath
    filepath_set(&keypath, "keys.dat");

    // Parse options
    while (1)
    {
        int option_index;
        int c;
        static struct option long_options[] =
            {
                {"keyset", 1, NULL, 'k'},
                {"help", 0, NULL, 'h'},
                {"tempdir", 1, NULL, 1},
                {"ncadir", 1, NULL, 2},
                {"nspdir", 1, NULL, 3},
                {"exefsdir", 1, NULL, 4},
                {"romfsdir", 1, NULL, 5},
                {"logodir", 1, NULL, 6},
                {"controldir", 1, NULL, 7},
                {"noromfs", 0, NULL, 8},
                {"nologo", 0, NULL, 9},
                {NULL, 0, NULL, 0},
            };

        c = getopt_long(argc, argv, "k:h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
        case 'k':
            filepath_set(&keypath, optarg);
            break;
        case 'h':
            usage();
            break;
        case 1:
            filepath_set(&settings.temp_dir, optarg);
            break;
        case 2:
            filepath_set(&settings.out_dir, optarg);
            break;
        case 3:
            filepath_set(&settings.nsp_dir, optarg);
            break;
        case 4:
            filepath_set(&settings.exefs_dir, optarg);
            break;
        case 5:
            filepath_set(&settings.romfs_dir, optarg);
            break;
        case 6:
            filepath_set(&settings.logo_dir, optarg);
            break;
        case 7:
            filepath_set(&settings.control_romfs_dir, optarg);
            break;
        case 8:
            settings.noromfs = 1;
            break;
        case 9:
            settings.nologo = 1;
            break;
        default:
            usage();
        }
    }

    // Create directories
    os_rmdir(settings.temp_dir.os_path);
    os_makedir(settings.temp_dir.os_path);
    os_rmdir(settings.out_dir.os_path);
    os_makedir(settings.out_dir.os_path);
    os_makedir(settings.nsp_dir.os_path);

    // Try to populate default keyfile.
    FILE *keyfile = NULL;
    keyfile = os_fopen(keypath.os_path, OS_MODE_READ);

    if (keyfile != NULL)
    {
        extkeys_initialize_keyset(&settings.keyset, keyfile);
        pki_derive_keys(&settings.keyset);
        fclose(keyfile);
    }
    else
    {
        fprintf(stderr, "Unable to open keyset '%s'\n"
                        "Use -k or --keyset to specify your keyset path or place your keyset in ." OS_PATH_SEPARATOR "keys.dat\n",
                keypath.char_path);
        return EXIT_FAILURE;
    }

    // Get TitleID from NACP
    printf("----> Process NACP\n");
    nacp_process(&settings, &cnmt_ctx);
    printf("\n");

    // Create NCAs
    nca_create_program(&settings, &cnmt_ctx);
    printf("\n");
    nca_create_control(&settings, &cnmt_ctx);
    printf("\n");
    nca_create_meta(&settings, &cnmt_ctx);
    printf("\n");

    // Create NSP
    printf("----> Creating NSP:\n");
    filepath_t nsp_file_path;
    filepath_init(&nsp_file_path);
    filepath_copy(&nsp_file_path, &settings.nsp_dir);
    filepath_append(&nsp_file_path, "%016" PRIx64 ".nsp", cnmt_ctx.cnmt_header.title_id);
    uint64_t pfs0_size;
    pfs0_build(&settings.out_dir, &nsp_file_path, &pfs0_size);
    printf("\n----> Created NSP: %s\n", nsp_file_path.char_path);

    return EXIT_SUCCESS;
}
