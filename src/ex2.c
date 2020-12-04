#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char WIN_NEW_LINE_LE[4] = {0x0d, 0x00, 0x0a, 0x00};
const unsigned char WIN_NEW_LINE_BE[4] = {0x00, 0x0d, 0x00, 0x0a};
const unsigned char MAC_NEW_LINE_LE[2] = {0x0d, 0x00};
const unsigned char MAC_NEW_LINE_BE[2] = {0x00, 0x0d};
const unsigned char UNIX_NEW_LINE_LE[2] = {0x0a, 0x00};
const unsigned char UNIX_NEW_LINE_BE[2] = {0x00, 0x0a};
const int ONE_BYTE = 1;
const int TWO_BYTES = 2;
const int FOUR_BYTES = 4;
const int TRUE = 1;
const int FALSE = 0;
const int FIRST_BYTE = 0;
const int SECOND_BYTE = 1;
const unsigned int BOM_FIRST_BYTE = 0xFE;
const unsigned int BOM_SECOND_BYTE = 0xFF;
const int FIRST_OPTION = 3;
const int SECOND_OPTION = 5;
const int THIRD_OPTION = 6;

void fileCopy(char *src, char *dst) {
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");
    unsigned char buffer[TWO_BYTES];
    if (srcFile == NULL || dstFile == NULL)
        return;
    while (fread(buffer, ONE_BYTE, sizeof(buffer), srcFile)) {
        fwrite(buffer, ONE_BYTE, sizeof(buffer), dstFile);
    }
    fclose(srcFile);
    fclose(dstFile);
}

int isLittleEndianness(FILE *src, FILE *dst) {
    unsigned char buffer[TWO_BYTES];
    fread(buffer, ONE_BYTE, sizeof(buffer), src);
    fwrite(buffer, ONE_BYTE, sizeof(buffer), dst);
    if (buffer[FIRST_BYTE] == BOM_SECOND_BYTE && buffer[SECOND_BYTE] == BOM_FIRST_BYTE)
        return TRUE;
    else
        return FALSE;
}

const unsigned char *mapEncoding(const char *system, int isLittleEndianness) {
    if (strcmp(system, "-unix") == 0)
        return (isLittleEndianness) ? UNIX_NEW_LINE_LE : UNIX_NEW_LINE_BE;
    else if (strcmp(system, "-mac") == 0)
        return (isLittleEndianness) ? MAC_NEW_LINE_LE : MAC_NEW_LINE_BE;
    else if (strcmp(system, "-win") == 0)
        return (isLittleEndianness) ? WIN_NEW_LINE_LE : WIN_NEW_LINE_BE;
}

int areEqual(const unsigned char *encoding, const unsigned char *buffer) {
    for (int i = 0; i < TWO_BYTES; i++) {
        if (encoding[i] != buffer[i])
            return FALSE;
    }
    return TRUE;
}

void systemConvert(const char *src, const char *dst, const char *systemOld, const char *systemNew) {
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");

    if (srcFile == NULL || dstFile == NULL) {
        fclose(srcFile);
        fclose(dstFile);
        return;
    }

    int writeSize = (strcmp(systemNew, "-win") == 0) ? FOUR_BYTES : TWO_BYTES;
    unsigned char buffer[TWO_BYTES];

    int isLE = isLittleEndianness(srcFile, dstFile);
    const unsigned char *oldLineEncoding = mapEncoding(systemOld, isLE);
    const unsigned char *newLineEncoding = mapEncoding(systemNew, isLE);

    while (fread(buffer, ONE_BYTE, sizeof(buffer), srcFile)) {
        if (areEqual(oldLineEncoding, buffer)) {
            if (strcmp(systemOld, "-win") == 0) {
                if (!fread(buffer, ONE_BYTE, sizeof(buffer), srcFile))
                    break;
                if (areEqual(oldLineEncoding + TWO_BYTES, buffer))
                    fwrite(newLineEncoding, ONE_BYTE, writeSize, dstFile);
                else {
                    fwrite(oldLineEncoding, ONE_BYTE, writeSize, dstFile);
                    fwrite(buffer, ONE_BYTE, writeSize, dstFile);
                }
            } else {
                fwrite(newLineEncoding, ONE_BYTE, writeSize, dstFile);
            }
        } else {
            fwrite(buffer, ONE_BYTE, sizeof(buffer), dstFile);
        }
    }
    fclose(srcFile);
    fclose(dstFile);
}

void endiannessConvert(char *src, char *dst, char *systemOld, char *systemNew, char *status) {

}


int main(int argc, char *argv[]) {
    if (argc == FIRST_OPTION)
        fileCopy(argv[1], argv[2]);
    if (argc == SECOND_OPTION)
        systemConvert(argv[1], argv[2], argv[3], argv[4]);
    if (argc == THIRD_OPTION)
        endiannessConvert(argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}