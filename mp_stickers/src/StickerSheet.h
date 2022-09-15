/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include "Image.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <vector>

struct StickerStruct {
    Image sticker;
    int x;
    int y;
};

class StickerSheet {
    private:
        Image myPicture;
        std::vector<StickerStruct> stickers;
        unsigned maxStickers;
    public:
        std::vector<StickerStruct> getStickers() const;
        StickerSheet(const Image &picture, unsigned max);
        ~StickerSheet();
        StickerSheet(StickerSheet &other);
        const StickerSheet &operator= (const StickerSheet &other);
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, unsigned x, unsigned y);
        bool translate(unsigned index, unsigned x, unsigned y);
        void removeSticker(unsigned index);
        Image* getSticker(unsigned index);
        Image render() const;
};