#include "Image.h"

namespace Tortuga
{
namespace AssetManager
{
Image::Image(std::string filePath)
{
    SDL_RWops *io = SDL_RWFromFile(filePath.c_str(), "rb");
    if (io == NULL)
    {
        Console::Error("Could not load image file!");
        return;
    }
    if (IMG_isPNG(io))
    {
        _imageSurface = IMG_LoadPNG_RW(io);
        _imageFormat = ImageFormat::PNG;
    }
    else if (IMG_isJPG(io))
    {
        _imageSurface = IMG_LoadJPG_RW(io);
        _imageFormat = ImageFormat::JPG;
    }
    else if (IMG_isBMP(io))
    {
        _imageSurface = IMG_LoadBMP_RW(io);
        _imageFormat = ImageFormat::BMP;
    }
    else if (IMG_isGIF(io))
    {
        _imageSurface = IMG_LoadGIF_RW(io);
        _imageFormat = ImageFormat::GIF;
    }
    else if (IMG_isICO(io))
    {
        _imageSurface = IMG_LoadICO_RW(io);
        _imageFormat = ImageFormat::ICO;
    }
    else if (IMG_isCUR(io))
    {
        _imageSurface = IMG_LoadCUR_RW(io);
        _imageFormat = ImageFormat::CUR;
    }
    else if (IMG_isLBM(io))
    {
        _imageSurface = IMG_LoadLBM_RW(io);
        _imageFormat = ImageFormat::LBM;
    }
    else if (IMG_isPCX(io))
    {
        _imageSurface = IMG_LoadPCX_RW(io);
        _imageFormat = ImageFormat::PCX;
    }
    else if (IMG_isPNM(io))
    {
        _imageSurface = IMG_LoadPNM_RW(io);
        _imageFormat = ImageFormat::PNM;
    }
    else if (IMG_isTIF(io))
    {
        _imageSurface = IMG_LoadTIF_RW(io);
        _imageFormat = ImageFormat::TIF;
    }
    else if (IMG_isWEBP(io))
    {
        _imageSurface = IMG_LoadWEBP_RW(io);
        _imageFormat = ImageFormat::WEBP;
    }
    else if (IMG_isXCF(io))
    {
        _imageSurface = IMG_LoadXCF_RW(io);
        _imageFormat = ImageFormat::XCF;
    }
    else if (IMG_isXV(io))
    {
        _imageSurface = IMG_LoadXV_RW(io);
        _imageFormat = ImageFormat::XV;
    }
    else
    {
        Console::Error("Unknown Image format!");
        _imageFormat = ImageFormat::UNKNOWN;
    }

    SDL_RWclose(io);
}
Image::Image(uint32_t width, uint32_t height)
{
    Console::Fatal("Not implimented!");
}
Image::~Image()
{
    SDL_FreeSurface(_imageSurface);
}
}; // namespace AssetManager
}; // namespace Tortuga