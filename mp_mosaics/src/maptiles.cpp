/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas* ret = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    //Map <AverageColorPoint, TileImage>
    std::map<Point<3>, TileImage*> tileMap;
    std::vector<Point<3>> points;

    // Initialize tileMap and points;
    for (auto& tile : theTiles) {
        LUVAPixel avgColor = tile.getAverageColor();
        Point<3> avgPoint(avgColor.l, avgColor.u, avgColor.v);
        tileMap[avgPoint] = &tile;
        points.push_back(Point<3>(avgColor.l, avgColor.u, avgColor.v));
    }

    //kD-Tree of LUVPixel points
    KDTree<3> tileTree(points);

    std::cout << "COLUMNS: " << ret->getColumns() << std::endl;
    std::cout << "ROWS: " << ret->getRows() << std::endl;
    // Make Tiles
    for (int i = 0; i < ret->getRows(); i++) {
        for (int j = 0; j < ret->getColumns(); j++) {
            LUVAPixel color = theSource.getRegionColor(i, j);
            Point<3> point = tileTree.findNearestNeighbor(Point<3>(color.l, color.u, color.v));
            ret->setTile(i, j, tileMap[point]);
        }
    }

    return ret;
}

