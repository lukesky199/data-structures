#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  
  Image alma;     alma.readFromFile("../alma.png");
  Image i;        i.readFromFile("../i.png");
  Image sticker1; sticker1.readFromFile("../sticker-1.png");
  Image sticker2; sticker2.readFromFile("../sticker-2.png");
  Image sticker3; sticker3.readFromFile("../sticker-3.png");

  i.scale(0.5);
  StickerSheet mySheet(alma, 5);
  mySheet.addSticker(sticker1, 0, 0);
  mySheet.addSticker(sticker2, 550, 0);
  mySheet.addSticker(i, 750, 400);
  mySheet.addSticker(sticker3, 390, 0);
  mySheet.render().writeToFile("../myImage.png");

  return 0;
}
