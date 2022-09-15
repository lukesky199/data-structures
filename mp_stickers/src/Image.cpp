#include "Image.h"

void Image::lighten() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).l + 0.1 > 1) {
                getPixel(j, i).l = 1;
            } else if (getPixel(j, i).l + 0.1 < 0) {
                getPixel(j, i).l = 0;
            } else {
                getPixel(j, i).l += 0.1;
            }
        }
    }
}
void Image::lighten(double amount) {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).l + amount > 1) {
                getPixel(j, i).l = 1;
            } else if (getPixel(j, i).l + amount < 0) {
                getPixel(j, i).l = 0;
            } else {
                getPixel(j, i).l += amount;
            }
        }
    }
}
void Image::darken() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).l - 0.1 > 1) {
                getPixel(j, i).l = 1;
            } else if (getPixel(j, i).l - 0.1 < 0) {
                getPixel(j, i).l = 0;
            } else {
                getPixel(j, i).l = getPixel(j, i).l - 0.1;
            }
        }
    }
}
void Image::darken(double amount) {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).l - amount > 1) {
                getPixel(j, i).l = 1;
            } else if (getPixel(j, i).l - amount < 0) {
                getPixel(j, i).l = 0;
            } else {
                getPixel(j, i).l = getPixel(j, i).l - amount;
            }
        }
    }
}
void Image::saturate() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).s + 0.1 > 1) {
                getPixel(j, i).s = 1;
            } else if (getPixel(j, i).s + 0.1 < 0) {
                getPixel(j, i).s = 0;
            } else {
                getPixel(j, i).s = getPixel(j, i).s + 0.1;
            }
        }
    }
}
void Image::saturate(double amount) {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).s + amount > 1) {
                getPixel(j, i).s = 1;
            } else if (getPixel(j, i).s + amount < 0) {
                getPixel(j, i).s = 0;
            } else {
                getPixel(j, i).s = getPixel(j, i).s + amount;
            }
        }
    }
}
void Image::desaturate() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).s - 0.1 > 1) {
                getPixel(j, i).s = 1;
            } else if (getPixel(j, i).s - 0.1 < 0) {
                getPixel(j, i).s = 0;
            } else {
                getPixel(j, i).s = getPixel(j, i).s - 0.1;
            }
        }
    }
}
void Image::desaturate(double amount) {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if (getPixel(j, i).s - amount > 1) {
                getPixel(j, i).s = 1;
            } else if (getPixel(j, i).s - amount < 0) {
                getPixel(j, i).s = 0;
            } else {
                getPixel(j, i).s = getPixel(j, i).s - amount;
            }
        }
    }
}
void Image::greyscale() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            getPixel(j, i).s = 0;
        }
    }
}
void Image::rotateColor(double degrees) {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            getPixel(j, i).h += degrees;
            if(getPixel(j, i).h >= 360) {
                getPixel(j, i).h -= 360;
            }
            if(getPixel(j, i).h < 0) {
                getPixel(j, i).h += 360;
            }
        }
    }
}
void Image::illinify() {
    for (unsigned i = 0; i < height(); i++) {
        for (unsigned j = 0; j < width(); j++) {
            if(getPixel(j, i).h < 113.5 || getPixel(j, i).h >= 293.5) {
                getPixel(j, i).h = 11;
            } else {
                getPixel(j, i).h = 216;
            }
        }
    }
}
void Image::scale(double factor) {
    if (factor == 1) {
        return;
    }
    if (factor < 1) {
        int inverse = (int)(1/factor);
        for (unsigned i = 0; i < height()/2; i++) {
            for (unsigned j = 0; j < width()/2; j++) {
                getPixel(j, i) = getPixel(j*inverse,i*inverse);
            }
        }
        resize(width() * factor, height() * factor);
    } else {
        int oldWidth = width();
        int oldHeight = height();
        resize(width()*factor, height() * factor);
        for (int i = oldHeight - 1; i >= 0; i--) {
            for (int j = oldWidth - 1; j >= 0; j--) {
                for (int k = 0; k < factor; k++) {
                    for (int l = 0; l < factor; l++) {
                        getPixel(j*factor + k, i*factor + l) = getPixel(j,i);
                    }
                }
            }
        }
    }
}
void Image::scale(unsigned w, unsigned h) {
    double wFactor = (double)w / (double)width();
    double hFactor = (double)h / (double)height();
    double factor = (wFactor <=  hFactor) ? wFactor : hFactor;
    scale(factor);
}