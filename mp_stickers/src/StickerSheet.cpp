#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image &picture, unsigned max) {
        myPicture = Image(picture);
        maxStickers = max;
}
StickerSheet::~StickerSheet() {

}
StickerSheet::StickerSheet(StickerSheet &other) {
    myPicture = other.myPicture;
    stickers = other.stickers;
    maxStickers = other.maxStickers;
}
const StickerSheet& StickerSheet::operator= (const StickerSheet &other) {
    if (this == &other)
        return *this;
    myPicture = other.myPicture;
    stickers = other.stickers;
    maxStickers = other.maxStickers;
    return *this;
}
void StickerSheet::changeMaxStickers(unsigned max) {
    while (!(max >= stickers.size())) {
        stickers.pop_back();
    }
    maxStickers = max;
}
int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
    if (stickers.size() == maxStickers)
        return -1;
    StickerStruct temp;
    temp.sticker = sticker;
    temp.x = x;
    temp.y = y;
    stickers.push_back(temp);
    return stickers.size() - 1;
} //NEED TO WORK OUT HOW LAYERS WORK;

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if (index >= maxStickers || index >= stickers.size())
        return false;
    stickers[index].x = x;
    stickers[index].y = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    stickers.erase(stickers.begin() + index);
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= maxStickers || index >= stickers.size())
        return NULL;
    Image* image_ptr = &stickers[index].sticker;
    return image_ptr;
}


Image StickerSheet::render() const {
    Image ren_image(myPicture);
    for (StickerStruct curr_sticker : stickers) {
        unsigned temp_width = curr_sticker.x + curr_sticker.sticker.width();
        unsigned temp_height = curr_sticker.y + curr_sticker.sticker.height();
        if (temp_width > ren_image.width()) {
            ren_image.resize(temp_width, ren_image.height());
        }
        if (temp_height > ren_image.height()) {
            ren_image.resize(ren_image.width(), temp_height);
        }
    }
    for (StickerStruct curr_sticker : stickers) {
        unsigned height = curr_sticker.sticker.height();
        unsigned width = curr_sticker.sticker.width();
        for (unsigned i = 0; i < height; i++) {
            for (unsigned j = 0; j < width; j++) {
                if (curr_sticker.sticker.getPixel(j, i).a != 0) {
                    ren_image.getPixel(curr_sticker.x + j, curr_sticker.y + i) = curr_sticker.sticker.getPixel(j, i);
                }
            }
        }
    }
    return ren_image;
}

std::vector<StickerStruct> StickerSheet::getStickers() const {
    return stickers;
}