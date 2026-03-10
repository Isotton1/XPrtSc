#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX

#include "nob.h"
#include "nob_config.h"

#define SRC "xprtsc.c"
#define OBJ "xprtsc.o"

#define VERSION_NAME temp_sprintf("xprtsc-%s", VERSION)
#define TAR_NAME temp_sprintf("%s.tar", VERSION_NAME)
#define TAR_GZ_NAME temp_sprintf("%s.gz", TAR_NAME)

#define INSTALL_PATH temp_sprintf("%s%s", DESTDIR, PREFIX)
#define BIN_DIR temp_sprintf("%s/bin", INSTALL_PATH)
#define BIN_PATH temp_sprintf("%s/xprtsc", BIN_DIR)

bool all();

bool obj() {
    Cmd cmd = {0};
    cmd_append(&cmd, CC, CFLAGS, "-c", SRC);
    if (!cmd_run(&cmd))
        return false;
    return true;
}

bool build() {
    obj();
    Cmd cmd = {0};
    cmd_append(&cmd, CC, "-o", "xprtsc", OBJ, LDFLAGS);
    if (!cmd_run(&cmd))
        return false;
    return true;
}

bool clean() {
    Cmd cmd = {0};
    cmd_append(&cmd, "rm", "-f", "xprtsc", OBJ, TAR_GZ_NAME);
    if (!cmd_run(&cmd))
        return false;
    return true;
}

bool dist() {
    if (!clean())
        return false;

    Cmd cmd_mk = {0};
    cmd_append(&cmd_mk, "mkdir", "-p", VERSION_NAME);
    if (!cmd_run(&cmd_mk))
        return false;

    Cmd cmd_cp = {0};
    cmd_append(&cmd_cp, "cp", "-R", "LICENSE", "Makefile", "README.md",
        "config.mk", SRC, VERSION_NAME);
    if (!cmd_run(&cmd_cp))
        return false;
 
    Cmd cmd_tar = {0};
    cmd_append(&cmd_tar, "tar", "-cf", TAR_NAME, VERSION_NAME);
    if (!cmd_run(&cmd_tar))
        return false;

    Cmd cmd_gz = {0};
    cmd_append(&cmd_gz, "gzip", TAR_NAME);
    if (!cmd_run(&cmd_gz))
        return false;

    Cmd cmd_rm = {0};
    cmd_append(&cmd_rm, "rm", "-rf", VERSION_NAME);
    if (!cmd_run(&cmd_rm))
        return false;
 
    return true;
}

bool install() {
    if (!all())
        return false;

    Cmd cmd_mk = {0};
    cmd_append(&cmd_mk, "mkdir", "-p", BIN_DIR);
    if (!cmd_run(&cmd_mk))
        return false;

    Cmd cmd_cp = {0};
    cmd_append(&cmd_cp, "cp", "xprtsc", BIN_DIR);
    if (!cmd_run(&cmd_cp))
        return false;

    Cmd cmd_cm = {0};
    cmd_append(&cmd_cm, "chmod", "755", BIN_PATH);
    if (!cmd_run(&cmd_cm))
        return false;

    return true;
}

bool uninstall() {
    Cmd cmd = {0};
    cmd_append(&cmd, "rm", "-f", BIN_PATH);
    if (!cmd_run(&cmd))
        return false;

    return true;
}

bool all() {
    return build();
}

int main(int argc, char** argv) {
    shift(argv, argc);
    if (argc == 0) {
        if (!all()) return 1;
    }
    while (argc) {
        char* arg = shift(argv, argc);
        if (strcmp(arg, "build") == 0) {
            if (!build()) return 1;
        }
        else if (strcmp(arg, "clean") == 0) {
            if (!clean()) return 1;
        }
        else if (strcmp(arg, "dist") == 0) {
            if (!dist()) return 1;
        }
        else if (strcmp(arg, "install") == 0) {
            if (!install()) return 1;
        }
        else if (strcmp(arg, "uninstall") == 0) {
            if (!uninstall()) return 1;
        }
    }
    return 0;
}
