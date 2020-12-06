// 311432082 Edward Gutman
#include <stdio.h>
#include <string.h>

/**
 * here we represent a newLine feed on each operating system, with both endianness.
 * also we declare different constants.
 */

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

const int FIRST_OPTION = 3;
const int SECOND_OPTION = 5;
const int THIRD_OPTION = 6;

// here we declare the "Byte order mark" constants in order to determine the endian of the machine.
const unsigned int BOM_FIRST_BYTE = 0xFE;
const unsigned int BOM_SECOND_BYTE = 0xFF;

/**
 * the "first" requested option.
 * copies a given file to a given destination, as it is.
 *
 * @param src the name of the file to copy from.
 * @param dst name of the file to create and copy to.
 */

void fileCopy(char *src, char *dst) {

    // opening the files.
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");

    // checking if we managed to open them properly.
    if (srcFile == NULL || dstFile == NULL)
        return;

    //creating the reading buffer.
    unsigned char buffer[TWO_BYTES];

    // reading loop - while we can, we read, and write the content to the destination file.
    while (fread(buffer, ONE_BYTE, sizeof(buffer), srcFile)) {
        fwrite(buffer, ONE_BYTE, sizeof(buffer), dstFile);
    }

    // closing of the files.
    fclose(srcFile);
    fclose(dstFile);
}

/**
 * if "toSwap" is true, the function swaps the byte order of each pairs of bytes, in a given buffer.
 * if "toSwap" is false, we do nothing.
 * ie, switches the memory representation from one endian to the other.
 *
 * @param buffer given buffer to swap the bytes of.
 * @param size the size of the buffer.
 * @param toSwap TRUE/FALSE depending if we need to swap.
 */

void bufferSwap(unsigned char *buffer, int size, int toSwap) {
    if (toSwap) {
        for (int i = 0; i < size; i += TWO_BYTES) {
            unsigned char tmp = buffer[i];
            buffer[i] = buffer[i + ONE_BYTE];
            buffer[i + ONE_BYTE] = tmp;
        }
    }
}

/**
 * checks if given file is represented in big/little endian.
 *
 * the function reads the first two bytes, which represent the ROM.
 * then, we check the order of the ROM bytes: FFFE = little endian, FEFF = big endian.
 * after that we swap the order of the bytes if needed, write them to the destination file,
 * and return TRUE/FALSE depending on the endian.
 *
 * @param src the source file.
 * @param dst the destination file.
 * @param toSwap TRUE/FALSE depending if we need to swap.
 * @return TRUE if little endian or FALSE if big endian.
 */

int isLittleEndian(FILE *src, FILE *dst, int toSwap) {

    // init a boolean and a buffer.
    int isLittle;
    unsigned char buffer[TWO_BYTES];

    // read the 2 ROM bytes to the buffer.
    fread(buffer, ONE_BYTE, sizeof(buffer), src);

    // determine big/small endian.
    if (buffer[FIRST_BYTE] == BOM_SECOND_BYTE && buffer[SECOND_BYTE] == BOM_FIRST_BYTE)
        isLittle = TRUE;
    else
        isLittle = FALSE;

    // swap the order of bytes if needed, and write the bytes.
    bufferSwap(buffer, sizeof(buffer), toSwap);
    fwrite(buffer, ONE_BYTE, sizeof(buffer), dst);
    return isLittle;
}

/**
 * returns the correct newLine feed depending on the Operating system and its endian.
 *
 * first we check which operating system is given, and then its endian.
 * based on the result we return one of the constant newLine feeds, which we declared in the beginning.
 *
 * @param system given operating system.
 * @param isLittleEndian indicator of the systems endian.
 * @return the correct newLine feed.
 */

const unsigned char *mapEncoding(const char *system, int isLittleEndian) {
    if (strcmp(system, "-unix") == 0)
        return (isLittleEndian) ? UNIX_NEW_LINE_LE : UNIX_NEW_LINE_BE;
    else if (strcmp(system, "-mac") == 0)
        return (isLittleEndian) ? MAC_NEW_LINE_LE : MAC_NEW_LINE_BE;
    else if (strcmp(system, "-win") == 0)
        return (isLittleEndian) ? WIN_NEW_LINE_LE : WIN_NEW_LINE_BE;
}

/**
 * checks if two "byte arrays" (buffers) are have the same elements.
 *
 * @param buffer1 the first buffer.
 * @param buffer2 the second buffer.
 * @return TRUE if equal elements or FALSE otherwise.
 */

int areEqual(const unsigned char *buffer1, const unsigned char *buffer2) {
    for (int i = 0; i < TWO_BYTES; i++) {
        if (buffer1[i] != buffer2[i])
            return FALSE;
    }
    return TRUE;
}

/**
 * the "third" requested function.
 * copy the content of a given source file with operating system encoding "systemOld", to a given
 * destination file with the operating system encoding "systemNew".
 * also swap the endian depending on the given "status" (keep/swap).
 *
 * the function first does preparations before the actual read/write:
 * we check for the source file's endian.
 * we assign the correct newLine feed variables (of the constants in the beginning), depending on the given
 * operating system's flags - we use different variables to READ from a file and to WRITE
 * to a file (because of potential endian swap).
 *
 * after the preparation we start to read each 2 bytes from the source file, into the buffer, in a loop:
 * if there is a match between the buffer and the old OS newLine feed:
 * ## if the old OS is windows, its newLine feed is 4 bytes, so we read the next 2 bytes to the buffer,
 *    and check if there is a full match.
 *    if there is a full match, we write the new OS newLine feed into the destination file.
 *    if there isn't, we write the first half of the old OS newLine feed (which we found to match), then we
 *          write the buffer.
 * ## if the old OS is not windows, its newLine feed is 2 bytes, so the match we found is a full match.
 *    and we write the new OS newLine feed into the destination file.
 *
 * also if the use endian swap (if needed) before each writing.
 * the function repeats this loop until the end of the source file.
 *
 * @param src the source file to read from.
 * @param dst the destination file to write to.
 * @param systemOld flag identifier of the source file's system encoding.
 * @param systemNew flag identifier of the destination file's system encoding.
 * @param status flag identifier (keep/swap) if to swap endianness.
 */

void endianConvert(const char *src, const char *dst, const char *systemOld, const char *systemNew,
                   const char *status) {

    // open the files.
    FILE *srcFile = fopen(src, "rb");
    FILE *dstFile = fopen(dst, "wb");

    // check if opened correctly.
    if (srcFile == NULL || dstFile == NULL) {
        fclose(srcFile);
        fclose(dstFile);
        return;
    }

    // init reading buffer.
    unsigned char buffer[TWO_BYTES];

    // assign if we need to swap endian to "toSwap".
    int toSwap = (strcmp(status, "-keep") == 0) ? FALSE : TRUE;

    // check the source's file endian
    int isLE = isLittleEndian(srcFile, dstFile, toSwap);

    // assign the newLine feed variables we will use to *READ* in the files.
    const unsigned char *oldLineEncoding = mapEncoding(systemOld, isLE);
    const unsigned char *newLineEncoding = mapEncoding(systemNew, isLE);

    // declare the newLine feed variables we will use to *WRITE* to the files.
    const unsigned char *oldLineWrite;
    const unsigned char *newLineWrite;

    // swap the endian representation of the *WRITING* newLine feed variables if needed.
    if (toSwap) {
        oldLineWrite = mapEncoding(systemOld, !isLE);
        newLineWrite = mapEncoding(systemNew, !isLE);
    } else {
        oldLineWrite = oldLineEncoding;
        newLineWrite = newLineEncoding;
    }

    // determine how many bytes we will need to write, depending on the given OS flags.
    int writeSize = (strcmp(systemNew, "-win") == 0) ? FOUR_BYTES : TWO_BYTES;

    // read 2 bytes each time to the buffer.
    while (fread(buffer, ONE_BYTE, sizeof(buffer), srcFile)) {

        // check if buffer is equal to the newLine feed of the old OS, if so we will need to rewrite it
        // to match the newLine feed of the new OS.
        if (areEqual(oldLineEncoding, buffer)) {

            // if the old OS is windows, oldLineEncoding is 4 bytes
            // and we only checked its first 2 bytes
            // so we will also need to check if the next 2 bytes are equal.
            if (strcmp(systemOld, "-win") == 0) {

                // reading the next 2 bytes into buffer
                // and checking if read correctly.
                if (!fread(buffer, ONE_BYTE, sizeof(buffer), srcFile))
                    break;
                // checking if the 2 last bytes of oldLineEncoding are equal to the new read 2 bytes in buffer.
                if (areEqual(oldLineEncoding + TWO_BYTES, buffer))

                    // if equal we write the new OS newLine feed into the destination file.
                    fwrite(newLineWrite, ONE_BYTE, writeSize, dstFile);
                else {
                    // if not equal, we write only the first 2 bytes of oldLineEncoding.
                    // (which were previously in the buffer).
                    fwrite(oldLineWrite, ONE_BYTE, TWO_BYTES, dstFile);

                    // then we write the 2 new bytes of the buffer.
                    bufferSwap(buffer, sizeof(buffer), toSwap);
                    fwrite(buffer, ONE_BYTE, sizeof(buffer), dstFile);
                }
            } else { // if there is a match and old OS is not windows
                // we write the new OS line feed.
                fwrite(newLineWrite, ONE_BYTE, writeSize, dstFile);
            }
        } else { // there isn't a match, so we write whats inside the buffer as it is.
            bufferSwap(buffer, sizeof(buffer), toSwap);
            fwrite(buffer, ONE_BYTE, sizeof(buffer), dstFile);
        }
    }

    // closing the files.
    fclose(srcFile);
    fclose(dstFile);
}

/**
 * the "second" requested function.
 *
 * we call endianConvert with the flag -keep, so we dont swap endianness.
 *
 * @param src given file to read from.
 * @param dst given file to write to.
 * @param systemOld flag identifier of the source file's system encoding.
 * @param systemNew flag identifier of the destination file's system encoding.
 */

void systemConvert(const char *src, const char *dst, const char *systemOld, const char *systemNew) {
    endianConvert(src, dst, systemOld, systemNew, "-keep");
}

/**
 * the main.
 *
 * @param argc number of argument.
 * @param argv arguments array.
 * @return 0 if argument dont match.
 */
int main(int argc, char *argv[]) {
    if (argc == FIRST_OPTION)
        fileCopy(argv[1], argv[2]);
    if (argc == SECOND_OPTION)
        systemConvert(argv[1], argv[2], argv[3], argv[4]);
    if (argc == THIRD_OPTION)
        endianConvert(argv[1], argv[2], argv[3], argv[4], argv[5]);
    return 0;
}