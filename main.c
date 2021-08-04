/*
 *
 * Generate a png image from scratch
 *
 * Author: Christopher Anderson
 * Date: July 29, 2021
 *
*/

#include <stdio.h>

int main() {

    // ----------------------------PNG File Info----------------------------------
    // Png files are split into chunks, each chunk is either critical or ancillary.
    // critical chunks are required and ancillary are optional
    // 
    // A chunk consists of four parts: 
    // - length (4 bytes, big-endian)
    // - chunk type/name (4 bytes) (converting the 4 hex bytes to ascii gives the chunk name)
    // - chunk data (length bytes)
    // - CRC (cyclic redundancy code/checksum; 4 bytes)
    //   The CRC is a network-byte-order CRC-32 computed over the chunk type 
    //   and chunk data, but not the length.
    // 
    // Chunks:
    // IHDR - Contains basic info about the image
    //        must be the first chunk!
    //        it contains (in this order):
    //         - the image's width (4 bytes)
    //         - height (4 bytes)
    //         - bit depth (1 byte, values 1, 2, 4, 8, or 16)
    //         - color type (1 byte, values 0, 2, 3, 4, or 6)
    //         - compression method (1 byte, value 0)
    //         - filter method (1 byte, value 0)
    //         - and interlace method (1 byte, values 0 "no interlace" or 1 "Adam7 interlace") 
    //        (13 data bytes total)
    //
    // PLTE (Optional, in some cases) - Contains the pallete, a list of colors
    //        The PLTE chunk is essential for color type 3 (indexed color)
    //        It is optional for color types 2 and 6 (truecolor and truecolor with alpha)
    //        and it must not appear for color types 0 and 4 (grayscale and grayscale with alpha)
    //
    // IDAT - contains the image, which may be split among multiple IDAT chunks
    //        Such splitting increases filesize slightly
    //        but makes it possible to generate a PNG in a streaming manner
    //        The IDAT chunk contains the actual image data
    //        which is the output stream of the compression algorithm.
    //
    // IEND - marks the image end
    //        the data field of the IEND chunk has 0 bytes/is empty
    //
    // Shamelessly plagiarized from
    // https://en.wikipedia.org/wiki/Portable_Network_Graphics
    //
    // ---------------------------------------------------------------------------

    // Hexadeciaml data to write to file (image.png)
    unsigned char data[69] = { 

        // The 8 byte signature, this basically says "I am a png file"
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
        
        /************* IHDR chunk *************/
        0x00, 0x00, 0x00, 0x0D, // Length
        0x49, 0x48, 0x44, 0x52, // Type/Name

        // ----- Chunk Data -----
        0x00, 0x00, 0x00, 0x01, // Width
        0x00, 0x00, 0x00, 0x01, // Height
        0x08, // Bit Depth
        0x02, // Color Type
        0x00, // Compression Method
        0x00, // Filter Method
        0x00, // Interlace Method
        // ----------------------

        0x90, 0x77, 0x53, 0xDE, // CRC/Checksum
        /**************************************/


        /************* IDAT chunk *************/
        0x00, 0x00, 0x00, 0x0C, // Length
        0x49, 0x44, 0x41, 0x54, // Type/Name

        // ----- Chunk Data -----
        0x08, 0xD7, 0x63, 0xF8,
        0xCF, 0xC0, 0x00, 0x00,
        0x03, 0x01, 0x01, 0x00,
        // ----------------------

        0x18, 0xDD, 0x8D, 0xB0, // CRC/Checksum
        /**************************************/


        /************* IEND chunk *************/
        0x00, 0x00, 0x00, 0x00, // Length
        0x49, 0x45, 0x4E, 0x44, // Type/Name
        // ----- Chunk Data -----
        // No Data since length is 0
        // ----------------------
        0xAE, 0x42, 0x60, 0x82 // CRC/Checksum
        /**************************************/

    };  // The result is a png file containing one red pixel

    // Writes the above data to a file called image.png
    FILE *write_ptr;
    write_ptr = fopen("image.png","wb");
    fwrite(data, sizeof(data), 1, write_ptr);
    fclose(write_ptr);

}

// Other resources:
// // https://pyokagan.name/blog/2019-10-14-png/