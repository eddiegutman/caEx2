#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char winNewLineLE[4] = {0x0d, 0x00, 0x0a, 0x00};
const unsigned char winNewLineBE[4] = {0x00, 0x0d, 0x00, 0x0a};
const unsigned char macNewLineLE[2] = {0x0d, 0x00};
const unsigned char macNewLineBE[2] = {0x00, 0x0d};
const unsigned char unixNewLineLE[2] = {0x0a, 0x00};
const unsigned char unixNewLineBE[2] = {0x00, 0x0a};

void fileCopy(char *src, char *dst) {
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");
    unsigned char buffer[2];
    if (srcFile == NULL || dstFile == NULL)
        return;
    while (fread(buffer, sizeof(char), sizeof(buffer), srcFile)) {
        fwrite(buffer, sizeof(char), sizeof(buffer), dstFile);
    }
    fclose(srcFile);
    fclose(dstFile);
}

int checkEndianness(FILE *src, FILE *dst) {
    unsigned char bom[2];
    fread(bom, sizeof(char), sizeof(bom), src);
    fwrite(bom, sizeof(char), sizeof(bom), dst);
    if (bom[0] == 0xff && bom[1] == 0xfe)
        return 1;
    else
        return 0;
}

void lineEncoding(unsigned char byteArray[4], const char *system, int isLittleEndianness) {

    if (strcmp(system, "-unix") == 0) {
        if (isLittleEndianness) {
            byteArray[0] = 0x0a;
            byteArray[1] = 0x00;
        } else {
            byteArray[0] = 0x00;
            byteArray[1] = 0x0a;
        }
    }
    if (strcmp(system, "-mac") == 0) {
        if (isLittleEndianness) {
            byteArray[0] = 0x0d;
            byteArray[1] = 0x00;
        } else {
            byteArray[0] = 0x00;
            byteArray[1] = 0x0d;
        }
    }
    if (strcmp(system, "-win") == 0) {
        if (isLittleEndianness) {
            byteArray[0] = 0x0d;
            byteArray[1] = 0x00;
            byteArray[2] = 0x0a;
            byteArray[3] = 0x00;
        } else {
            byteArray[0] = 0x00;
            byteArray[1] = 0x0d;
            byteArray[2] = 0x00;
            byteArray[3] = 0x0a;
        }

    }
}

const unsigned char *mapEncoding(const char *system, int isLittleEndianness) {
    if (strcmp(system, "-unix") == 0)
        return (isLittleEndianness) ? unixNewLineLE : unixNewLineBE;
    if (strcmp(system, "-mac") == 0)
        return (isLittleEndianness) ? macNewLineLE : macNewLineBE;
    if (strcmp(system, "-win") == 0)
        return (isLittleEndianness) ? winNewLineLE : winNewLineBE;
}

int areEqual(const unsigned char *encoding, const unsigned char *buffer) {
    for (int i = 0; i < 2; i++) {
        if (encoding[i] != buffer[i])
            return 0;
    }
    return 1;
}

void systemConvert(const char *src, const char *dst, const char *systemOld, const char *systemNew) {
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");
    int isLittleEndianness;
    int writeSize = (strcmp(systemNew, "-win") == 0) ? 4 : 2;
    unsigned char buffer[2];
    //unsigned char oldLineEncoding[4];
    //unsigned char newLineEncoding[4];

    if (srcFile == NULL || dstFile == NULL)
        return;

    isLittleEndianness = checkEndianness(srcFile, dstFile);
    //lineEncoding(oldLineEncoding, systemOld, isLittleEndianness);
    //lineEncoding(newLineEncoding, systemNew, isLittleEndianness);
    const unsigned char *oldLineEncoding = mapEncoding(systemOld, isLittleEndianness);
    const unsigned char *newLineEncoding = mapEncoding(systemNew, isLittleEndianness);

    while (fread(buffer, sizeof(char), sizeof(buffer), srcFile)) {
        if (areEqual(oldLineEncoding, buffer)) {
            if (strcmp(systemOld, "-win") == 0) {
                if (!fread(buffer, sizeof(char), sizeof(buffer), srcFile))
                    break;
                if (areEqual(oldLineEncoding + 2, buffer))
                    fwrite(newLineEncoding, sizeof(char), writeSize, dstFile);
                else {
                    fwrite(oldLineEncoding, sizeof(char), writeSize, dstFile);
                    fwrite(buffer, sizeof(char), writeSize, dstFile);
                }
            } else {
                fwrite(newLineEncoding, sizeof(char), writeSize, dstFile);
            }
        } else {
            fwrite(buffer, sizeof(char), sizeof(buffer), dstFile);
        }
    }
    fclose(srcFile);
    fclose(dstFile);
}

void endiannessConvert(char *src, char *dst, char *systemOld, char *systemNew, char *status) {

}


int main(int argc, char *argv[]) {
    if (argc == 3)
        fileCopy(argv[1], argv[2]);
    if (argc == 5)
        systemConvert(argv[1], argv[2], argv[3], argv[4]);
    if (argc == 6)
        endiannessConvert(argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}