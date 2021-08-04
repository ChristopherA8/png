#include <stdio.h>

int main()
{

    /*
     * This program takes in a png image file and parses the chunk data
    */

    // image.png size: 69
    // hi.png size: 3907
    size_t filesize = 3907;
    unsigned char data[filesize];

    FILE *read_ptr;
    read_ptr = fopen("hi.png", "rb");
    fread(data, filesize, 1, read_ptr);
    fclose(read_ptr);

    /***** Print hex of whole image *****/
    // for (int i = 0; i < sizeof(data); i++) {
    //     printf("0x%x ", data[i]);
    // }

    // for (int i = 0; i < sizeof(data); i++) {
    //     printf("%x ", data[i]);
    // }
    /*************************************/

    // for (int i = 0; i < sizeof(data); i++) {
    //     printf("%c", data[i]);
    // }

    // File Signature
    // printf("PNG Signature: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%x ", data[i]);
    // }
    // printf("\n");

    // see png wikipedia for what each chunk means
    // hi.png chunks
    // qty | name
    //  1  | IHDR
    //  2  | IDAT
    //  1  | eXIf // Metadata looks like weird xml
    //  1  | iCCP
    //  1  | iTXt // See iTXt for contents of this chunk's data
    //  1  | pHYs
    //  1  | iDOT ???? I don't have any documentation of what this chunk is for

    // Count and print out chunk data
    // This is broken after for the first two chunks, in the hi.png file. No idea why. I'll have to hand parse the file and see where it breaks woe
    int i = 8; // starts at 8 instead of 0 since there is an 8-byte file signature before the chunks start
    // I feel like I'm counting wrong
    // who thought this was a good use of time
    while (i < sizeof(data))
    {

        printf("\n");
        int length = 0;
        int checksum = 0;
        unsigned char name[5];

        int z = 0;
        do
        {
            length += data[i + z];
            z++;
        } while (z < 4);

        int p = 4 + 4 + length;
        do
        {
            checksum += data[i + p];
            p++;
        } while (p < 4 + 4 + length + 4);

        int h = 4;
        do
        {
            name[h - 4] = data[i + h];
            h++;
        } while (h < 8);

        // Check for IEND chunk (last chunk)
        if ((data[i + 4] == 0 && data[i + 5] == 0 && data[i + 6] == 0 && data[i + 7] == 0) || (data[i + 4] == 0x49 && data[i + 5] == 0x45 && data[i + 6] == 0x4e && data[i + 7] == 0x44))
        {
            printf("***** IEND Chunk *****\n");
            return 0;
        }

        unsigned char chunkData[length];
        int x = 8;
        do
        {
            chunkData[x - 8] = data[i + x];
            // printf("%x\n", data[i + x]);
            x++;
        } while ((x - 8) < length);

        printf("***** %s Chunk *****\n", name);

        printf("Length: %d\n", length);
        printf("Type/Name: %s\n", name);
        printf("Data: [ ");
        for (int g = 0; g < length; g++)
            printf("0x%x ", chunkData[g]);
        printf("]\n");
        printf("Checksum: %d\n", checksum);

        printf("**********************\n");

        i += 4 + 4 + length + 4;
    }
}