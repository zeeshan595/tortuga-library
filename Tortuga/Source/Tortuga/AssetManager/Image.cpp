#include "Image.h"

namespace Tortuga
{
namespace AssetManager
{
Image::Image(std::string filePath)
{
    SDL_Surface *imageSurface;
    SDL_RWops *io = SDL_RWFromFile(filePath.c_str(), "rb");
    if (io == NULL)
    {
        Console::Error("Could not load image file!");
        return;
    }
    if (IMG_isPNG(io))
    {
        imageSurface = IMG_LoadPNG_RW(io);
        _imageFileFormat = ImageFormat::PNG;
    }
    else if (IMG_isJPG(io))
    {
        imageSurface = IMG_LoadJPG_RW(io);
        _imageFileFormat = ImageFormat::JPG;
    }
    else if (IMG_isBMP(io))
    {
        imageSurface = IMG_LoadBMP_RW(io);
        _imageFileFormat = ImageFormat::BMP;
    }
    else if (IMG_isGIF(io))
    {
        imageSurface = IMG_LoadGIF_RW(io);
        _imageFileFormat = ImageFormat::GIF;
    }
    else if (IMG_isICO(io))
    {
        imageSurface = IMG_LoadICO_RW(io);
        _imageFileFormat = ImageFormat::ICO;
    }
    else if (IMG_isCUR(io))
    {
        imageSurface = IMG_LoadCUR_RW(io);
        _imageFileFormat = ImageFormat::CUR;
    }
    else if (IMG_isLBM(io))
    {
        imageSurface = IMG_LoadLBM_RW(io);
        _imageFileFormat = ImageFormat::LBM;
    }
    else if (IMG_isPCX(io))
    {
        imageSurface = IMG_LoadPCX_RW(io);
        _imageFileFormat = ImageFormat::PCX;
    }
    else if (IMG_isPNM(io))
    {
        imageSurface = IMG_LoadPNM_RW(io);
        _imageFileFormat = ImageFormat::PNM;
    }
    else if (IMG_isTIF(io))
    {
        imageSurface = IMG_LoadTIF_RW(io);
        _imageFileFormat = ImageFormat::TIF;
    }
    else if (IMG_isWEBP(io))
    {
        imageSurface = IMG_LoadWEBP_RW(io);
        _imageFileFormat = ImageFormat::WEBP;
    }
    else if (IMG_isXCF(io))
    {
        imageSurface = IMG_LoadXCF_RW(io);
        _imageFileFormat = ImageFormat::XCF;
    }
    else if (IMG_isXV(io))
    {
        imageSurface = IMG_LoadXV_RW(io);
        _imageFileFormat = ImageFormat::XV;
    }
    else
    {
        Console::Error("Unknown Image format!");
        _imageFileFormat = ImageFormat::UNKNOWN;
    }

    SDL_RWclose(io);
    
    //Copy image width and height from SDL surface
    _width = imageSurface->w;
    _height = imageSurface->h;

    //Copy pixels from SDL surface
    for (uint32_t i = 0; i < _width * _height; i++)
    {
        const Uint32 *in = (Uint32 *)((uint8_t *)imageSurface->pixels + i * imageSurface->format->BytesPerPixel);
        SDL_Color color;
        SDL_GetRGBA(*in, imageSurface->format, &color.r, &color.g, &color.b, &color.a);
        _pixels.push_back({color.r, color.g, color.b, color.a});
    }

    SDL_FreeSurface(imageSurface);
}
Image::Image(uint32_t width, uint32_t height)
{
    Console::Fatal("Not implimented!");
}
Image::~Image()
{
}
}; // namespace AssetManager
}; // namespace Tortuga