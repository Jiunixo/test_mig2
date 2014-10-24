/**
 * \file test_OGeometrie.cpp
 * \test Testing of OGeometrie libs used by lot of TYMPAN features
 *
 *  Created on: april 17, 2013
 *      Author: Denis THOMASSON <denis.thomasson@edf.fr>
 */

#include <cstdlib>

#include "gtest/gtest.h"
#include "Tympan/models/common/3d.h"

using std::cout;
using std::cerr;
using std::endl;

TEST(TestGeometrie, normale)
{
    // Cr�ation du tableau de points
    OPoint3D pts[3];
    pts[0] = OPoint3D(0.0, 0.0, 0.0);
    pts[1] = OPoint3D(1.0, 0.0, 0.0);
    pts[2] = OPoint3D(0.0, 1.0, 0.0);

    // D�claration du vecteur resultat
    OVector3D normale;

    // Calcul de la normale
    OGeometrie::computeNormal(pts, 3, normale);

    ASSERT_TRUE(normale._z == 1);
}

TEST(TestGeometrie, sym_pt_droite)
{
    // Cr�ation des points
    OPoint3D ptA(-20.0, 0.0, 0.0), ptB(20.0, 0.0, 0.0), ptP(-20.0, 0.0, 2.0), ptI;

    // Calcul du point sym�trique � PtP par rapport � la droite PtA-ptB
    double k = OGeometrie::symPointDroite(ptA, ptB, ptP, ptI);

    ASSERT_TRUE(ptI == OPoint3D(-20.0, 0.0, -2.0));
    ASSERT_TRUE(k == 0.0);

}

TEST(TestGeometrie, matrix_point_multiplication)
{
    OMatrix matrix;
    matrix.setRotationOz(M_PI/2); // quarter turn around Z axis, counter-clock wise
    // Translation
    matrix._m[0][3] = 2;
    matrix._m[1][3] = 1;
    matrix._m[2][3] = 0;
    OPoint3D point (2, 1, 0);
    point = matrix * point;
    EXPECT_DOUBLE_EQ(1, point._x);
    EXPECT_DOUBLE_EQ(3, point._y);
    EXPECT_DOUBLE_EQ(0, point._z);
}

TEST(TestGeometrie, matrix_vector_multiplication)
{
    OMatrix matrix;
    matrix.setRotationOz(M_PI/2); // quarter turn around Z axis, counter-clock wise
    // Translation
    matrix._m[0][3] = 2;
    matrix._m[1][3] = 1;
    matrix._m[2][3] = 0;
    matrix._m[3][3] = 0;
    OVector3D vec (2, -1, 0);
    vec = matrix * vec;
    EXPECT_DOUBLE_EQ(1, vec._x);
    EXPECT_DOUBLE_EQ(2, vec._y);
    EXPECT_DOUBLE_EQ(0, vec._z);
}

TEST(TestGeometrie, inters_demi_segment_avec_segment)
{
    // Cr�ation des points du segment et du point ptS
    OPoint3D ptA(-20.0, -10.0, 5.0), ptB(20.0, 12.0, 5.0), ptS(-20.0, 3.0, 5.0);

    // Test positif (le segment coupe l'horizontale)
    bool resu = OGeometrie::intersDemiSegmentAvecSegment(ptS, ptA, ptB);
    ASSERT_TRUE(resu == true);

    // Test n�gatif (le segment ne coupe pas l'horizontale)
    ptS._z = -3;
    resu = OGeometrie::intersDemiSegmentAvecSegment(ptS, ptA, ptB);
    ASSERT_FALSE(resu == false);
}

TEST(TestGeometrie, inters_droites_points)
{
    // Cr�ation des points des 2 segments
    OPoint3D ptA(-10.0, 23.0, 0.0), ptB(10.0, -17.0, 0.0), ptC(-10.0, -32.0, 0.0), ptD(10.0, 28.0, 0.0), ptI;

    // Test positif (les 2 lignes se croisent)
    int resu = OGeometrie::intersDroitesPoints(ptA, ptB, ptC, ptD, ptI);
    ASSERT_TRUE(resu == 1);
    ASSERT_TRUE(ptI == OPoint3D(1.0, 1.0, 0.0));

    // Test n�gatif (les 2 lignes sont parall�les)
    ptA._y = -1030, ptB._y = -970;
    resu = OGeometrie::intersDroitesPoints(ptA, ptB, ptC, ptD, ptI);
    ASSERT_TRUE(resu == 0);
}

TEST(TestGeometrie, point_in_polygon_angle_sum)
{
    // D�finition du tableau de points
    OPoint3D pts[6];
    pts[0] = OPoint3D(1.0, 1.0, 0.0);
    pts[1] = OPoint3D(5.0, 6.0, 0.0);
    pts[2] = OPoint3D(-1.0, 10.0, 0.0);
    pts[3] = OPoint3D(-10.0, -2.0, 0.0);
    pts[4] = OPoint3D(1.0, -8.0, 0.0);
    pts[5] = OPoint3D(6.0, -3.0, 0.0);

    // D�finition du point � tester
    OPoint3D ptP(-3.0, 3.0, 0.0);
    bool resu = OGeometrie::pointInPolygonAngleSum(ptP, pts, 6);

    // Test positif, le point est dans le polygone
    ASSERT_TRUE(resu == true);

    // Test n�gatif, les point n'est pas dans le polygone
    ptP._x = 4.0;
    resu = OGeometrie::pointInPolygonAngleSum(ptP, pts, 6);
    ASSERT_FALSE(resu == true);
}

TEST(TestGeometrie, point_in_polygon_ray_casting)
{
    // D�finition du tableau de points
    OPoint3D pts[6];
    pts[0] = OPoint3D(1.0, 1.0, 0.0);
    pts[1] = OPoint3D(5.0, 6.0, 0.0);
    pts[2] = OPoint3D(-1.0, 10.0, 0.0);
    pts[3] = OPoint3D(-10.0, -2.0, 0.0);
    pts[4] = OPoint3D(1.0, -8.0, 0.0);
    pts[5] = OPoint3D(6.0, -3.0, 0.0);

    // Calcul du vecteur normal � la face
    OVector3D normale;
    OGeometrie::computeNormal(pts, 6, normale);

    // D�finition de la bounding box
    OBox box(OCoord3D(-10.0, -8.0, 0.0), OCoord3D(6.0, 10.0, 0.0));

    // D�finition du point � tester
    OPoint3D ptP(-3.0, 3.0, 0.0);
    bool resu = OGeometrie::pointInPolygonRayCasting(ptP, pts, 6);

    // Test positif, le point est dans le polygone
    ASSERT_TRUE(resu == true);

    // Test n�gatif, les point n'est pas dans le polygone
    ptP._x = 4.0;
    resu = OGeometrie::pointInPolygonRayCasting(ptP, pts, 6);
    ASSERT_FALSE(resu == true);
}

TEST(TestGeometrie, shortest_seg_between_2_lines)
{
    // Cr�ation des points des 2 segments et du segment retourn�
    OPoint3D pt1(-10.0, 23.0, 0.0), pt2(10.0, -17.0, 0.0), pt3(-10.0, -32.0, 3.0), pt4(10.0, 28.0, 3.0), ptA, ptB;

    // D�claration de mua et mub
    double* mua = new double;
    double* mub = new double;

    // Cas 1 : les deux segments se croisent � une certaine distance
    bool resu = OGeometrie::shortestSegBetween2Lines(pt1, pt2, pt3, pt4, ptA, ptB, mua, mub);
    ASSERT_TRUE(resu == true);
    ASSERT_TRUE(ptA == OPoint3D(1.0, 1.0, 0.0));
    ASSERT_TRUE(ptB == OPoint3D(1.0, 1.0, 3.0));

    // Cas 2 : les deux segments ont une intersection
    pt3._z = pt4._z = 0.0;
    resu = OGeometrie::shortestSegBetween2Lines(pt1, pt2, pt3, pt4, ptA, ptB, mua, mub);
    ASSERT_TRUE(resu == true);
    ASSERT_TRUE(ptA == OPoint3D(1.0, 1.0, 0.0));
    ASSERT_TRUE(ptB == OPoint3D(1.0, 1.0, 0.0));
}

TEST(TestGeometrie, bounding_box)
{
    // D�finition du tableau de points
    OPoint3D pts[6];
    pts[0] = OPoint3D(1.0, 1.0, 5.0);
    pts[1] = OPoint3D(5.0, 6.0, 0.0);
    pts[2] = OPoint3D(-1.0, 10.0, 0.0);
    pts[3] = OPoint3D(-10.0, -2.0, 0.0);
    pts[4] = OPoint3D(1.0, -8.0, 0.0);
    pts[5] = OPoint3D(6.0, -3.0, 0.0);

    // D�claration ptMin et ptMax
    OPoint3D ptMin, ptMax;

    // Calcul des points
    OGeometrie::boundingBox(pts, 6, ptMin, ptMax);

    ASSERT_TRUE(ptMin == OPoint3D(-10.0, -8.0, 0.0));
    ASSERT_TRUE(ptMax == OPoint3D(6.0, 10.0, 5.0));
}

TEST(TestGeometrie, inters_droites_point_vecteur)
{
    // D�finition des points
    OPoint3D ptA0(-10.0, 23.0, 0.0), ptA1(10.0, -17.0, 0.0), ptB0(-10.0, -32.0, 0.0), ptB1(10.0, 28.0, 0.0), ptI;

    // Cr�ation des vecteurs
    OVector3D vecA(ptA0, ptA1), vecB(ptB0, ptB1);

    // Test positif (les 2 lignes se croisent en 1,1)
    int resu = OGeometrie::intersDroitesPointVecteur(ptA0, vecA, ptB0, vecB, ptI);
    ASSERT_TRUE(resu == 1);
    ASSERT_TRUE(ptI == OPoint3D(1.0, 1.0, 0.0));

    // Test n�gatif (les 2 lignes sont parall�les)
    ptA0._y = -1030, ptA1._y = -970;
    vecA = vecB;
    resu = OGeometrie::intersDroitesPointVecteur(ptA0, vecA, ptB0, vecB, ptI);
    ASSERT_TRUE(resu == 0);
}
