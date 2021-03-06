#ifndef LIBGEODECOMP_IO_IMAGEPAINTER_H
#define LIBGEODECOMP_IO_IMAGEPAINTER_H

#include <libgeodecomp/misc/color.h>
#include <libgeodecomp/storage/image.h>

namespace LibGeoDecomp {

class ImagePainter
{
public:
    explicit ImagePainter(Image *image) :
        image(image)
    {}

    void moveTo(const Coord<2>& coord)
    {
        position = coord;
    }

    void fillRect(int originX, int originY, int dimX, int dimY, Color color)
    {
        image->fillBox(position + Coord<2>(originX, originY), dimX, dimY, color);
    }

private:
    Image *image;
    Coord<2> position;
};

}

#endif
