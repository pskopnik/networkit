/*
 * GeometricGTest.cpp
 *
 *  Created on: 29.07.2014
 *      Author: moritzl
 */

#include "GeometricGTest.h"

namespace NetworKit {

GeometricGTest::GeometricGTest() {
	// TODO Auto-generated constructor stub

}

GeometricGTest::~GeometricGTest() {
	// TODO Auto-generated destructor stub
}

/**
 * test conversion of polar coordinates to cartesian coordinates and back
 */
TEST_F(GeometricGTest, testConversion) {
	Point2D<double> a(6,7);
	double epsilon = 10E-6;
	double angle, radius;
	HyperbolicSpace::cartesianToPolar(a, angle, radius);
	Point2D<double> back = HyperbolicSpace::polarToCartesian(angle, radius);
	EXPECT_NEAR(a[0], back[0], epsilon);
	EXPECT_NEAR(a[1], back[1], epsilon);
	count n = 1000;
	vector<double> angles(n);
	vector<double> radii(n);
	HyperbolicSpace::fillPoints(angles, radii, 1, 1);
	for (index i = 0; i < n; i++) {
		Point2D<double> point = HyperbolicSpace::polarToCartesian(angles[i], radii[i]);
		double phi, r;
		HyperbolicSpace::cartesianToPolar(point, phi,r);
		EXPECT_GE(phi, 0) << "Point (" << point[0] << "," << point[1] << ") was not converted correctly";
		EXPECT_GE(r, 0);
		EXPECT_LE(abs(phi - angles[i]), epsilon);
		EXPECT_LE(abs(r - radii[i]), epsilon);
	}
}

/**
 * Test numeric stability of Euclidean Circle conversion
 */
TEST_F(GeometricGTest, testEuclideanCircleConsistency) {
	count n = 1000000;
	vector<double> angles(n);
	vector<double> radii(n);
	double stretch = 2;
	double alpha = 3;
	double epsilon = 10E-6;
	double R = stretch*HyperbolicSpace::hyperbolicAreaToRadius(n);
	HyperbolicSpace::fillPoints(angles, radii, stretch, alpha);

	for (index i = 0; i < n; i++) {
		Point2D<double> cartesianPoint = HyperbolicSpace::polarToCartesian(angles[i], radii[i]);
		double r_e, euRadius;
		HyperbolicSpace::getEuclideanCircle(radii[i], R, r_e, euRadius);
		double mirrorangle = fmod(angles[i] + M_PI, 2*M_PI);
		double mirrorradiusInside = abs(r_e - euRadius)-epsilon;
		Point2D<double> counterPointInside = HyperbolicSpace::polarToCartesian(mirrorangle, mirrorradiusInside);
		EXPECT_LE(HyperbolicSpace::poincareMetric(cartesianPoint, counterPointInside), R) << "(" << cartesianPoint.getX() << ", " << cartesianPoint.getY() << ")"
				<< " and (" << counterPointInside.getX() << ", " << counterPointInside.getY() << ")" << " are " << HyperbolicSpace::poincareMetric(cartesianPoint, counterPointInside) << " apart from each other, which is more than " << R << ".";

		double mirrorradiusOutside = abs(r_e - euRadius)+epsilon;
		Point2D<double> counterPointOutside = HyperbolicSpace::polarToCartesian(mirrorangle, mirrorradiusOutside);
		EXPECT_GE(HyperbolicSpace::poincareMetric(cartesianPoint, counterPointOutside), R);

	}

}

} /* namespace NetworKit */