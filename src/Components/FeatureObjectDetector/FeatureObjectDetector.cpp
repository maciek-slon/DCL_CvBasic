/*!
 * \file
 * \brief
 * \author Maciej Stefanczyk
 */

#include <memory>
#include <string>

#include "FeatureObjectDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

using namespace cv;

namespace Processors {
namespace FeatureObjectDetector {

FeatureObjectDetector::FeatureObjectDetector(const std::string & name) :
		Base::Component(name) , 
		object_name("object_name", std::string("")),
		threshold("threshold", 50, "range") {
	registerProperty(object_name);
	
	threshold.addConstraint("0");
	threshold.addConstraint("100");
	registerProperty(threshold);

}

FeatureObjectDetector::~FeatureObjectDetector() {
}

void FeatureObjectDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("in_roi", &in_roi);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&FeatureObjectDetector::onNewImage, this));
	addDependency("onNewImage", &in_img);
	registerHandler("clearObjects", boost::bind(&FeatureObjectDetector::clearObjects, this));
	registerHandler("updateObject", boost::bind(&FeatureObjectDetector::updateObject, this));

}

bool FeatureObjectDetector::onInit() {

	return true;
}

bool FeatureObjectDetector::onFinish() {
	return true;
}

bool FeatureObjectDetector::onStop() {
	return true;
}

bool FeatureObjectDetector::onStart() {
	return true;
}

void FeatureObjectDetector::onNewImage() {
	Mat img_scene = in_img.read();
	Mat img_object;
	
	int roi_w = 150;
	int roi_h = 200;

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	auto surf = xfeatures2d::SURF::create(  );

	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	Mat descriptors_object, descriptors_scene;

	surf->detectAndCompute( img_scene, cv::Mat(), keypoints_scene, descriptors_scene );

	FlannBasedMatcher matcher;
	
	Mat img_matches = cv::Mat::zeros(cv::Size(img_scene.size().width + 2*roi_w, std::max(img_scene.size().height, 2*roi_h)), CV_8UC3);
	img_scene.copyTo(img_matches.colRange(roi_w, roi_w+img_scene.size().width).rowRange(0, img_scene.size().height));
	
	int i = 0;
	cv::Scalar color;
	for (const auto & kv : known_objects) {
		if (i > 3) i = 0;
		std::lock_guard<std::mutex> lock(map_mutex);
		cv::Mat img_object = kv.second;
		
		cv::Point lt;
		switch(i) {
			case 0: lt = cv::Point(0, 0); color = cv::Scalar(0, 255, 0); break;
			case 1: lt = cv::Point(0, roi_h); color = cv::Scalar(0, 255, 255); break;
			case 2: lt = cv::Point(roi_w + img_scene.size().width, 0); color = cv::Scalar(255, 255, 0); break;
			case 3: lt = cv::Point(roi_w + img_scene.size().width, roi_h); color = cv::Scalar(0, 0, 255); break;
		}
		
		kv.second.copyTo(img_matches.colRange(lt.x, lt.x + roi_w).rowRange(lt.y, lt.y + roi_h));
		
		surf->detectAndCompute( img_object, cv::Mat(), keypoints_object, descriptors_object );

		std::vector< DMatch > matches;
		//      match(        query,          train, ...)
		matcher.match( descriptors_object, descriptors_scene, matches );

		double max_dist = 0; double min_dist = 100;

		//-- Quick calculation of max and min distances between keypoints
		for( int i = 0; i < descriptors_object.rows; i++ )
		{ double dist = matches[i].distance;
			if( dist < min_dist ) min_dist = dist;
			if( dist > max_dist ) max_dist = dist;
		}

		printf("-- Max dist : %f \n", max_dist );
		printf("-- Min dist : %f \n", min_dist );

		//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
		std::vector< DMatch > good_matches;

		for( int i = 0; i < descriptors_object.rows; i++ )
		{ if( matches[i].distance < 0.01 * threshold )
			{ good_matches.push_back( matches[i]); }
		}

		for (const auto & m : good_matches) {
			line( img_matches, keypoints_scene[m.trainIdx].pt + cv::Point2f(roi_w, 0), keypoints_object[m.queryIdx].pt + cv::Point2f(lt.x, lt.y), color, 1 );
		}
		/*drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
								 good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
								 std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );*/

		//-- Localize the object
		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		if (good_matches.size() > 5) {

			for( int i = 0; i < good_matches.size(); i++ )
			{
				//-- Get the keypoints from the good matches
				obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
				scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
			}

			Mat H = findHomography( obj, scene, CV_RANSAC );
			
			if (!H.empty()) {

				//-- Get the corners from the image_1 ( the object to be "detected" )
				std::vector<Point2f> obj_corners(4);
				obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
				obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
				std::vector<Point2f> scene_corners(4);

				perspectiveTransform( obj_corners, scene_corners, H);

				//-- Draw lines between the corners (the mapped object in the scene - image_2 )
				line( img_matches, scene_corners[0] + Point2f( roi_w, 0), scene_corners[1] + Point2f( roi_w, 0), color, 2 );
				line( img_matches, scene_corners[1] + Point2f( roi_w, 0), scene_corners[2] + Point2f( roi_w, 0), color, 2 );
				line( img_matches, scene_corners[2] + Point2f( roi_w, 0), scene_corners[3] + Point2f( roi_w, 0), color, 2 );
				line( img_matches, scene_corners[3] + Point2f( roi_w, 0), scene_corners[0] + Point2f( roi_w, 0), color, 2 );
			}
		}
		
		i++;
	}
	out_img.write(img_matches);
}

void FeatureObjectDetector::clearObjects() {
	std::lock_guard<std::mutex> lock(map_mutex);
	known_objects.clear();
}

void FeatureObjectDetector::updateObject() {
	std::lock_guard<std::mutex> lock(map_mutex);
	
	if (known_objects.count(object_name) < 1 && known_objects.size() >= 4) return;
	
	if (!in_roi.empty())
		known_objects[object_name] = in_roi.read().clone();
}



} //: namespace FeatureObjectDetector
} //: namespace Processors
