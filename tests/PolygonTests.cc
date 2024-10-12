#include "PolygonItem.h"
#include <gtest/gtest.h>

TEST(PolygonItemTest, Constructor)
{
    PolygonItem polygonItem;
    EXPECT_EQ(polygonItem.boundingRect(), QRectF());
}

// Test checkLinearOrdering() function
TEST(PolygonItemTest, CheckLinearOrdering)
{
    PolygonItem polygonItem;
    polygonItem.addVertex(QPointF(0, 0));
    polygonItem.addVertex(QPointF(0, 1));
    polygonItem.addVertex(QPointF(1, 1));
    polygonItem.addVertex(QPointF(1, 0));
    EXPECT_TRUE(polygonItem.checkLinearOrdering());

    polygonItem.vertices.swapItemsAt(1, 2);
    EXPECT_FALSE(polygonItem.checkLinearOrdering());

    polygonItem.vertices.swapItemsAt(1, 2);
    EXPECT_TRUE(polygonItem.checkLinearOrdering());
}