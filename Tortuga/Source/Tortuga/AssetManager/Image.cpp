#include "Image.h"

namespace Tortuga
{
namespace Assets
{
namespace Image
{
ImageData LoadImage(std::string file)
{
    ImageData data;

    SDL_Surface *imageSurface;
    SDL_RWops *io = SDL_RWFromFile(file.c_str(), "rb");
    if (io == NULL)
    {
        Console::Error("Could not load image file: {0}", Console::Arguments() << file);
    }
    if (IMG_isPNG(io))
    {
        imageSurface = IMG_LoadPNG_RW(io);
        data.FileFormat = ImageFileFormat::PNG;
    }
    else if (IMG_isJPG(io))
    {
        imageSurface = IMG_LoadJPG_RW(io);
        data.FileFormat = ImageFileFormat::JPG;
    }
    else if (IMG_isBMP(io))
    {
        imageSurface = IMG_LoadBMP_RW(io);
        data.FileFormat = ImageFileFormat::BMP;
    }
    else if (IMG_isGIF(io))
    {
        imageSurface = IMG_LoadGIF_RW(io);
        data.FileFormat = ImageFileFormat::GIF;
    }
    else if (IMG_isICO(io))
    {
        imageSurface = IMG_LoadICO_RW(io);
        data.FileFormat = ImageFileFormat::ICO;
    }
    else if (IMG_isTIF(io))
    {
        imageSurface = IMG_LoadTIF_RW(io);
        data.FileFormat = ImageFileFormat::TIF;
    }
    else if (IMG_isCUR(io))
    {
        imageSurface = IMG_LoadCUR_RW(io);
        data.FileFormat = ImageFileFormat::CUR;
    }
    else if (IMG_isLBM(io))
    {
        imageSurface = IMG_LoadLBM_RW(io);
        data.FileFormat = ImageFileFormat::LBM;
    }
    else if (IMG_isPCX(io))
    {
        imageSurface = IMG_LoadPCX_RW(io);
        data.FileFormat = ImageFileFormat::PCX;
    }
    else if (IMG_isPNM(io))
    {
        imageSurface = IMG_LoadPNM_RW(io);
        data.FileFormat = ImageFileFormat::PNM;
    }
    else if (IMG_isWEBP(io))
    {
        imageSurface = IMG_LoadWEBP_RW(io);
        data.FileFormat = ImageFileFormat::WEBP;
    }
    else if (IMG_isXCF(io))
    {
        imageSurface = IMG_LoadXCF_RW(io);
        data.FileFormat = ImageFileFormat::XCF;
    }
    else if (IMG_isXV(io))
    {
        imageSurface = IMG_LoadXV_RW(io);
        data.FileFormat = ImageFileFormat::XV;
    }
    else
    {
        //Console::Error("Unknown Image format!");
        data.FileFormat = ImageFileFormat::UNKNOWN;
    }

    SDL_RWclose(io);

    //Copy image width and height from SDL surface
    data.Width = imageSurface->w;
    data.Height = imageSurface->h;

    //Copy pixels from SDL surface
    for (uint32_t i = 0; i < data.Width * data.Height; i++)
    {
        const Uint32 *in = (Uint32 *)((uint8_t *)imageSurface->pixels + i * imageSurface->format->BytesPerPixel);
        SDL_Color color;
        SDL_GetRGBA(*in, imageSurface->format, &color.r, &color.g, &color.b, &color.a);
        data.Pixels.push_back({color.r, color.g, color.b, color.a});
    }

    SDL_FreeSurface(imageSurface);

    return data;
}
}; // namespace Image
}; // namespace Assets
}; // namespace Tortuga