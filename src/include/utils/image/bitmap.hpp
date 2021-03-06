//                     _   
//   ___ _ __ __ _ ___| |_ 
//  / __| '__/ _` / __| __|
//  \__ \ | | (_| \__ \ |_ 
//  |___/_|  \__,_|___/\__|
//  (c) Lucas Alvarenga    
//      (lukeathedev)      

//  Bitmap File I/O, capable of writing
//  24-bit uncompressed images.

#pragma once

#include <utils/types.hpp>
using namespace types;

#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

// https://en.wikipedia.org/wiki/BMP_file_format

namespace bitmap
{
    class file
    {
      private:
        #pragma pack(push, 1) // Avoids compiler padding;

        typedef struct _bmp_header
        {
            uint16_t file_type   { 0x4D42 };
            uint32_t file_size   { 0 };
            uint16_t reserved1   { 0 };
            uint16_t reserved2   { 0 };
            uint32_t data_offset { 0 };
        } BMP_HEADER;

        typedef struct _inf_header
        {
            uint32_t header_size  { 0 };
            uint32_t image_width  { 0 };
            uint32_t image_height { 0 };
            uint16_t planes       { 1 };
            uint16_t bit_depth    { 0 };
            uint32_t compression  { 0 };
            uint32_t image_size   { 0 }; // Leave at 0 if uncompressed;
            uint32_t x_px_per_m   { 0 };
            uint32_t y_px_per_m   { 0 };
            uint32_t colors_used  { 0 };
            uint32_t colors_sigf  { 0 };
        } INF_HEADER;

        #pragma pack(pop)

        BMP_HEADER bmp_header;
        INF_HEADER inf_header;

      public:
        std::vector<vec3<int>> data;

        file(int width, int height, int bit_depth)
        {
            bmp_header.file_size = width * height * (bit_depth / 8);
            bmp_header.file_size += sizeof(bmp_header) + sizeof(inf_header);

            inf_header.image_width  = width;
            inf_header.image_height = height;
            inf_header.bit_depth    = bit_depth;
            inf_header.header_size  = sizeof(inf_header);
        }

        void write(std::string filename)
        {
            std::ofstream image { filename, std::ios::binary };

            image.write((char *)&bmp_header, sizeof(bmp_header));
            image.write((char *)&inf_header, sizeof(inf_header));
            
            for (unsigned int i = 0; i < data.size(); ++i)
            {
                image.write((char *)&data[i][0], 1);
                image.write((char *)&data[i][1], 1);
                image.write((char *)&data[i][2], 1);
            }

            return;
        }
    };
} // namespace bitmap
